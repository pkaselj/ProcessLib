#ifndef PROCESS_HPP
#define PROCESS_HPP

#include<string>

class Process
{
    private:
    std::string pathname = "";

    public:
    int pid = -1;

    Process() = delete;
    Process(const std::string& _pathname);
    ~Process() = default; // for shared memory
    int initiate();
    void forceKill();
};

#endif

