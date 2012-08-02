#include "Processor.h"
/**
    @Author = thehawk as camael24
    @Github = https://github.com/camael24/Prompt
    @License = http://creativecommons.org/licenses/by-nc-sa/3.0/fr/
*/
Processor::Processor(const char* directory , Output debug)
{
    _directory = directory;
    _out = debug;
    filePid = directory;
    filePid += "file.pid";
}
bool Processor::isEverLaunch()
{
    std::cout << filePid << std::endl;
    std::ifstream monFlux(filePid.c_str());
    if(!monFlux.fail()) // Test de la présence du fichier
    {
        std::string fLine;
        getline(monFlux , fLine);

        return true;
    }
    else
    {
        return false;
    }
}
void Processor::registerPid(int pid)
{
    std::ofstream monFlux(filePid.c_str());
    monFlux << pid;
    monFlux.close();

}
HANDLE Processor::Launcher(std::string uri)
{
    STARTUPINFO si;


    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );


    char url[(strlen(_directory)+strlen(uri.c_str()))];
    strcpy(url, _directory);
    strcat(url, uri.c_str());
    std::cout << "Launch Process : " << url << std::endl;
    if(!CreateProcess(NULL, url,NULL,NULL,FALSE,0,NULL,NULL,&si,&pi))
    {
        printf( "CreateProcess failed (%d).\n", GetLastError() );
        _out.WhatError();

    }

    _pid = (int) GetProcessId(pi.hProcess);
    this->registerPid(_pid);
    return pi.hProcess;


}
bool Processor::kill()
{
    return TerminateProcess(pi.hProcess , 0);
}
bool Processor::kill(HANDLE h)
{
    return TerminateProcess(h , 0);
}
Processor::~Processor()
{
    //dtor
}
