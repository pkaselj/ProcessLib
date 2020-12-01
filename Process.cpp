#include"Process.hpp"

#include<unistd.h>
#include<signal.h>

Process::Process(const std::string& _pathname)
    : pathname(_pathname)
{}

int Process::initiate()
{ 
    pid = fork();
    if(pid == 0)
        execl(pathname.c_str(), pathname.c_str(), (void *)NULL);
    else
        return pid;
    return -1;
}

void Process::forceKill()
{
    kill(pid, SIGKILL);
}