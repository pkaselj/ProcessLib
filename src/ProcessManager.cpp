#include "ProcessManager.hpp"
#include "Kernel.hpp"

#include<sys/wait.h>

#include<algorithm>

ProcessManager::ProcessManager(ILogger* _p_logger)
{
    m_pLogger = _p_logger;
    if(m_pLogger == nullptr)
        m_pLogger = NulLogger::getInstance();

    *m_pLogger << "ProcessManager created!";
}

ProcessManager::~ProcessManager()
{
    /*
        for (auto& process : m_processes)
            delete process;
    */
    while (m_processes.empty() == false)
    {
        delete m_processes.back();
        m_processes.pop_back();
    }

    *m_pLogger << "ProcessManager destroyed!";
    
}

Process* ProcessManager::createProcess(const std::string& pathname)
{
    return createProcess(pathname, {});
}

Process* ProcessManager::createProcess(const std::string& pathname, const std::vector<char*>& arguments)
{
    Process* p_process = new Process(pathname, arguments);
    m_processes.push_back(p_process);

    if (p_process == nullptr)
    {
        *m_pLogger << "ProcessManager - error while trying to allocate process pointer for: " + pathname;
        Kernel::Fatal_Error("ProcessManager - error while trying to allocate process pointer for: " + pathname);
    }

    *m_pLogger << "Created process with name: " + pathname;

    return p_process;
}

Process* ProcessManager::createProcess(std::string&& pathname)
{
    Process* p_process = new Process(pathname);
    m_processes.push_back(p_process);

    if (p_process == nullptr)
    {
        *m_pLogger << "ProcessManager - error while trying to allocate process pointer for: " + pathname;
        Kernel::Fatal_Error("ProcessManager - error while trying to allocate process pointer for: " + pathname);
    }

    *m_pLogger << "Created process with name: " + pathname;

    return p_process;
}

int ProcessManager::initiateAll()
{
    int pid = -1;

    for(const auto& process : m_processes)
    {
        pid = process->initiate();
        if(pid == 0)
        {
            exit(0);
            break;
        }

        *m_pLogger << "Initiated process: \"" + process->getName() + "\" with PID: " + std::to_string(pid);
        Kernel::Trace("Initiated process: \"" + process->getName() + "\" with PID: " + std::to_string(pid));
    }

    return pid;
        
}

void ProcessManager::killAll()
{ 
    *m_pLogger << "Kill all!";
    for (auto& process : m_processes)
    {
        *m_pLogger << "Process: " + process->getName();

        *m_pLogger << "\tChecking if it already exited.";
        int childExited = waitpid(process->getPID(), nullptr, WNOHANG);
        if (childExited == 0) // Child didn't exit
        {
            *m_pLogger << "\tProcess still running!";
            process->killProcess();

            waitpid(process->getPID(), nullptr, 0);
        }

        *m_pLogger << "Child with PID: " + std::to_string(process->getPID()) + " killed!";
        Kernel::Trace("Child with PID: " + std::to_string(process->getPID()) + " killed!");
        
    }

    // sleep(5); // TODO define waiting time

    // forceKillAll(); // accidental kills?
}

void ProcessManager::forceKillAll()
{
    *m_pLogger << "Force kill all!";
    for (auto& process : m_processes)
    {
        *m_pLogger << "Process: " + process->getName();

        *m_pLogger << "\tChecking if it already exited.";
        int childExited = waitpid(process->getPID(), nullptr, WNOHANG);
        if (childExited == 0) // Child didn't exit
        {
            *m_pLogger << "\tProcess still running!";
            process->forceKillProcess();

            waitpid(process->getPID(), nullptr, 0);
        }

        *m_pLogger << "Child with PID: " + std::to_string(process->getPID()) + " forcefully killed!";
        Kernel::Trace("Child with PID: " + std::to_string(process->getPID()) + " forcefully killed!");
        
        delete process;
    }
        
    /*
        int pid_done = -1;
        while ((pid_done = wait(NULL)) > 0)
        {
            *m_pLogger << "Child with PID: " + std::to_string(pid_done) + " exited!";
            Kernel::Trace("Child with PID: " + std::to_string(pid_done) + " exited!");
        }
    */

        

    *m_pLogger << "All children exited!";
    Kernel::Trace("All children exited!");
}

void ProcessManager::resetProcess(unsigned int PID)
{
    auto matchesPID = [=](Process* pProcess) {return pProcess->getPID() == PID;};
    auto position = std::find_if(m_processes.begin(), m_processes.end(), matchesPID);

    if (position == m_processes.end())
    {
        *m_pLogger << "ProcessManager: Trying to reset process out of process managers jurisdiction - PID: " + std::to_string(PID);
        Kernel::Warning("ProcessManager: Trying to reset process out of process managers jurisdiction - PID: " + std::to_string(PID));
        return;
    }

    (*position)->restart();

    *m_pLogger << "Restarted process: \"" + (*position)->getName() + "\" with PID: " + std::to_string((*position)->getPID());
    Kernel::Trace("Restarted process: \"" + (*position)->getName() + "\" with PID: " + std::to_string((*position)->getPID()) );

}