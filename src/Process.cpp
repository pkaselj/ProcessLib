#include"Process.hpp"

#include<unistd.h>
#include<signal.h>
#include<sys/wait.h>

Process::Process(const std::string& pathname)
    : m_pathname(pathname), m_arguments({nullptr}), m_status(enuProcessStatus::NOT_STARTED)
{}

Process::Process(const std::string& pathname, const std::vector<char*>& arguments)
    : m_pathname(pathname), m_arguments(arguments), m_status(enuProcessStatus::NOT_STARTED)
{
    m_arguments.push_back(nullptr);
}

int Process::initiate()
{ 
    m_pid = fork();
    if(m_pid == 0)
    {
        m_status = RUNNING;
        if (m_arguments.size() > 0)
        {
            m_arguments.insert(m_arguments.begin(), const_cast<char*>(m_pathname.c_str()) );
            execv(m_pathname.c_str(), &m_arguments[0]);
        }
        else
        {
            execl(m_pathname.c_str(), m_pathname.c_str(), (void *)NULL);
        }
        
        m_status = STOPPED;

        exit(0);
    }
        
    return m_pid;
}

void Process::forceKillProcess()
{
    kill(m_pid, SIGKILL);
}

void Process::killProcess()
{
    kill(m_pid, SIGABRT);
}

std::string Process::getName() const
{
    return m_pathname;
}

void Process::restart()
{

    int processExited = waitpid(m_pid, nullptr, WNOHANG);
    if (processExited == 0)
    {
        killProcess();
        waitpid(m_pid, nullptr, 0);
    }

    

    /*
    if (m_status == RUNNING)
    {
        // killProcess();
        forceKillProcess();

        // sleep(waitTime);
    }

    waitpid(m_pid, nullptr, 0);
    */
    
    

    initiate();
}