#ifndef PROCESS_MANAGER_HPP
#define PROCESS_MANAGER_HPP

#include<vector>

#include"Process.hpp"

#include"Logger.hpp"
#include"NulLogger.hpp"

/**
 * @brief Class used to manage Processes
 * 
 */
class ProcessManager
{
    private:
    /// Vector of pointers to attached processes. Attach processes automatically by `createProcess()`
    std::vector<Process*> m_processes;

    ILogger* m_pLogger = nullptr;

    public:

    ProcessManager(ILogger* _p_logger = NulLogger::getInstance());

    /**
     * @brief Destroy the Process Manager object
     * 
     */
    ~ProcessManager();

    /**
     * @brief Creates a process and attaches it to the processes vector.
     * 
     * Process is NOT started until `initiateAll()` is called.
     * 
     * @param pathname Pathname to executable to be executed in a new process
     */
    Process* createProcess(const std::string& pathname, const std::vector<char*>& arguments); // TODO docs
    Process* createProcess(const std::string& pathname);

    // TODO Move semantics   
    /**
     * @brief Creates a process and attaches it to the processes vector. (WIP)
     * 
     * Using move semantics \n
     * Process is NOT started until `initiateAll()` is called. \n
     * 
     * @param pathname Pathname to executable to be executed in a new process
     */
    Process* createProcess(std::string&& pathname); // TODO docs
    //void attachProcess(Process* p_process);

    int initiateAll();

    /// Kill all attached processes. (soft kill - sends )
    void killAll();

    /// Kill all attached processes immediately
    void forceKillAll();

    /// Kill and restart process with PID only if it is owned by this ProcessManager
    void resetProcess(unsigned int PID);
};

#endif