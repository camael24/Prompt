#ifndef SERVICES_H
#define SERVICES_H
#include <windows.h>
#include <string>
#include "Processor.h"
#include "Registry.h"
#include "Output.h"
/**
    @Author = thehawk as camael24
    @Github = https://github.com/camael24/Prompt
    @License = http://creativecommons.org/licenses/by-nc-sa/3.0/fr/
*/
class Services
{
public:
    Services(Output debug , std::string serviceName , std::string sPath);
    void Start();
    void Start(bool forceStart);
    void getKeyParameter(std::string uriPath);
    bool wellInstalled();
    void Stop();
    bool isLaunch();
    bool isAvailable();
    void waitUntilItStart();
    void waitUntilItStop();
    void Register(std::string sPath);
    void UnRegister();
    void RunAnProgrammAsServices();
    void Close();
    virtual ~Services();
protected:
private:
    SERVICE_STATUS_PROCESS ssStatus;
    SC_HANDLE schSCManager;
    SC_HANDLE schService;
    Output debug;
    std::string uriPath;
    std::string serviceName;
    DWORD dwBytesNeeded;
    DWORD dwOldCheckPoint;
    DWORD dwStartTickCount;
    DWORD dwWaitTime;

};

#endif // SERVICES_H
