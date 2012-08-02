#ifndef PROCESSOR_H
#define PROCESSOR_H
#define _WIN32_WINNT 0x0501
#include <windows.h>
#include <winbase.h>
#include <process.h>
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <string>
#include <iostream>
#include <fstream>
#include "Output.h"
/**
    @Author = thehawk as camael24
    @Github = https://github.com/camael24/Prompt
    @License = http://creativecommons.org/licenses/by-nc-sa/3.0/fr/
*/
class Processor
{
public:
    Processor(const char* directory , Output debug);
    HANDLE Launcher(std::string uri);
    bool isEverLaunch();
    void registerPid(int pid);
    bool kill();
    bool kill(HANDLE h);
    virtual ~Processor();
    int _pid;
protected:
private:
    PROCESS_INFORMATION pi;
    std::string filePid;
    const char* _directory;
    Output _out;
    HANDLE _h;
};

#endif // PROCESSOR_H
