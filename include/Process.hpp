#ifndef PROCESS_HPP
#define PROCESS_HPP

#include<string>
#include<vector>

/**
 * @brief Wrapper class for processes
 * 
 */
class Process
{
public:

    typedef enum
    {
        NOT_STARTED,
        RUNNING,
        STOPPED
    } enuProcessStatus;

    enuProcessStatus getStatus() const { return m_status; }

    /// Default constructor is deleted since Process without executable makes no sense
    Process() = delete;

    /**
     * @brief Construct a new Process object
     *
     * @param _pathname Pathname to a process executable
     */
    Process(const std::string& pathname);
    Process(const std::string& pathname, const std::vector<char*>& arguments);

    /**
     * @brief Destroy the Process object
     *
     */
    ~Process() {}; // for shared memory

    /**
     * @brief Executes the process.
     *
     * Process executable pathname is initialized in constructor, \n
     * and stored in `pathname` memeber variable. \n
     *
     * @return int returns PID of process
     */
    int initiate();

    /// Kills the process referenced by current Process object -- hard kill
    void forceKillProcess();

    /// Kills the process referenced by current Process object -- soft kill
    void killProcess();

    std::string getName() const;

    unsigned int getPID() const { return m_pid; }

    void restart();

    private:
    /// Pathname to a process executable
    std::string m_pathname = "";
    std::vector<char*> m_arguments;

    // TODO : Create getter.
    /// Process PID, set to -1 by default. Set by `initiate()`.
    int m_pid = -1;

    enuProcessStatus m_status;


};

#endif

