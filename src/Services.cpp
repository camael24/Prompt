#include "Services.h"
/**
    @Author = thehawk as camael24
    @Github = https://github.com/camael24/Prompt
    @License = http://creativecommons.org/licenses/by-nc-sa/3.0/fr/
*/
Services::Services(Output _debug , std::string _serviceName , std::string sPath)
{

    debug = _debug;
    serviceName = _serviceName;
    if(this->isAvailable())
    {
        schSCManager = OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);  // full access rights
        if (NULL == schSCManager)
        {
            printf("OpenSCManager failed (%d)\n", GetLastError());
        }

        // Get a handle to the service.

        schService = OpenService(schSCManager,TEXT(serviceName.c_str()),SERVICE_ALL_ACCESS);  // full access
        if (schService == NULL)
        {
            printf("OpenService failed (%d)\n", GetLastError());
        }
    }
    else
    {
        printf("Service isn't installed yet");
        this->getKeyParameter(sPath);
        this->RunAnProgrammAsServices();
    }
}
bool Services::isAvailable()
{
    Registry r(HKEY_LOCAL_MACHINE);
    return r.ExistsKey("SYSTEM\\CurrentControlSet\\services\\"+serviceName);
}
bool Services::isLaunch()
{
    if (!QueryServiceStatusEx(
                schService,                     // handle to service
                SC_STATUS_PROCESS_INFO,         // information level
                (LPBYTE) &ssStatus,             // address of structure
                sizeof(SERVICE_STATUS_PROCESS), // size of structure
                &dwBytesNeeded ) )              // size needed if buffer is too small
    {
        printf("QueryServiceStatusEx failed (%d)\n", GetLastError());
        return "";
    }
    if(ssStatus.dwCurrentState != SERVICE_STOPPED && ssStatus.dwCurrentState != SERVICE_STOP_PENDING)
    {
        return TRUE;
    }
    return FALSE;

}
void Services::waitUntilItStart()
{
    if (!QueryServiceStatusEx(
                schService,                     // handle to service
                SC_STATUS_PROCESS_INFO,         // info level
                (LPBYTE) &ssStatus,             // address of structure
                sizeof(SERVICE_STATUS_PROCESS), // size of structure
                &dwBytesNeeded ) )              // if buffer too small
    {
        printf("QueryServiceStatusEx failed (%d)\n", GetLastError());
    }

    // Save the tick count and initial checkpoint.

    dwStartTickCount = GetTickCount();
    dwOldCheckPoint = ssStatus.dwCheckPoint;

    while (ssStatus.dwCurrentState == SERVICE_START_PENDING)
    {
        // Do not wait longer than the wait hint. A good interval is
        // one-tenth the wait hint, but no less than 1 second and no
        // more than 10 seconds.

        dwWaitTime = ssStatus.dwWaitHint / 10;

        if( dwWaitTime < 1000 )
            dwWaitTime = 1000;
        else if ( dwWaitTime > 10000 )
            dwWaitTime = 10000;

        Sleep( dwWaitTime );

        // Check the status again.

        if (!QueryServiceStatusEx(
                    schService,             // handle to service
                    SC_STATUS_PROCESS_INFO, // info level
                    (LPBYTE) &ssStatus,             // address of structure
                    sizeof(SERVICE_STATUS_PROCESS), // size of structure
                    &dwBytesNeeded ) )              // if buffer too small
        {
            printf("QueryServiceStatusEx failed (%d)\n", GetLastError());
            break;
        }

        if ( ssStatus.dwCheckPoint > dwOldCheckPoint )
        {
            // Continue to wait and check.

            dwStartTickCount = GetTickCount();
            dwOldCheckPoint = ssStatus.dwCheckPoint;
        }
        else
        {
            if(GetTickCount()-dwStartTickCount > ssStatus.dwWaitHint)
            {
                // No progress made within the wait hint.
                break;
            }
        }
    }
}
void Services::waitUntilItStop()
{
    if (!QueryServiceStatusEx(
                schService,                     // handle to service
                SC_STATUS_PROCESS_INFO,         // info level
                (LPBYTE) &ssStatus,             // address of structure
                sizeof(SERVICE_STATUS_PROCESS), // size of structure
                &dwBytesNeeded ) )              // if buffer too small
    {
        printf("QueryServiceStatusEx failed (%d)\n", GetLastError());
    }
    dwStartTickCount = GetTickCount();
    dwOldCheckPoint = ssStatus.dwCheckPoint;

    // Wait for the service to stop before attempting to start it.

    while (ssStatus.dwCurrentState == SERVICE_STOP_PENDING)
    {
        // Do not wait longer than the wait hint. A good interval is
        // one-tenth of the wait hint but not less than 1 second
        // and not more than 10 seconds.

        dwWaitTime = ssStatus.dwWaitHint / 10;

        if( dwWaitTime < 1000 )
            dwWaitTime = 1000;
        else if ( dwWaitTime > 10000 )
            dwWaitTime = 10000;

        Sleep( dwWaitTime );

        // Check the status until the service is no longer stop pending.

        if (!QueryServiceStatusEx(
                    schService,                     // handle to service
                    SC_STATUS_PROCESS_INFO,         // information level
                    (LPBYTE) &ssStatus,             // address of structure
                    sizeof(SERVICE_STATUS_PROCESS), // size of structure
                    &dwBytesNeeded ) )              // size needed if buffer is too small
        {
            printf("QueryServiceStatusEx failed (%d)\n", GetLastError());
        }

        if ( ssStatus.dwCheckPoint > dwOldCheckPoint )
        {
            // Continue to wait and check.

            dwStartTickCount = GetTickCount();
            dwOldCheckPoint = ssStatus.dwCheckPoint;
        }
        else
        {
            if(GetTickCount()-dwStartTickCount > ssStatus.dwWaitHint)
            {
                printf("Timeout waiting for service to stop\n");
            }
        }
    }
}
void Services::Start()
{
    this->Start(false);
}
void Services::Start(bool forceStart)
{
    if(this->isLaunch())
    {
        debug.echo("The Service "+ serviceName+" is ever launch");
        // TODO (Julien#1#): Use here an Stop Command and wait !
        if(forceStart)
        {
            this->Stop();
        }
        else
        {
            return;
        }

    }
    // Attempt to start the service.

    if (!StartService(schService,0,NULL))
    {
        printf("StartService failed (%d)\n", GetLastError());
    }
    this->waitUntilItStart();

    if (ssStatus.dwCurrentState == SERVICE_RUNNING)
    {
        debug.echo("Service "+serviceName+" started successfully.");
    }
    else
    {
        printf("Service not started. \n");
        printf("  Current State: %d\n", ssStatus.dwCurrentState);
        printf("  Exit Code: %d\n", ssStatus.dwWin32ExitCode);
        printf("  Check Point: %d\n", ssStatus.dwCheckPoint);
        printf("  Wait Hint: %d\n", ssStatus.dwWaitHint);
    }
}
void Services::Stop()
{
    SERVICE_STATUS_PROCESS ssp;
    DWORD dwStartTime = GetTickCount();
    DWORD dwBytesNeeded;
    DWORD dwTimeout = 30000; // 30-second time-out
    DWORD dwWaitTime;

    if ( !QueryServiceStatusEx(
                schService,
                SC_STATUS_PROCESS_INFO,
                (LPBYTE)&ssp,
                sizeof(SERVICE_STATUS_PROCESS),
                &dwBytesNeeded ) )
    {
        printf("QueryServiceStatusEx failed (%d)\n", GetLastError());
    }

    if ( ssp.dwCurrentState == SERVICE_STOPPED )
    {
        printf("Service is already stopped.\n");
    }
    this->waitUntilItStop();
    if ( !ControlService(
                schService,
                SERVICE_CONTROL_STOP,
                (LPSERVICE_STATUS) &ssp ) )
    {
        printf( "ControlService failed (%d)\n", GetLastError() );
    }

    // Wait for the service to stop.

    while ( ssp.dwCurrentState != SERVICE_STOPPED )
    {
        Sleep( ssp.dwWaitHint );
        if ( !QueryServiceStatusEx(
                    schService,
                    SC_STATUS_PROCESS_INFO,
                    (LPBYTE)&ssp,
                    sizeof(SERVICE_STATUS_PROCESS),
                    &dwBytesNeeded ) )
        {
            printf( "QueryServiceStatusEx failed (%d)\n", GetLastError() );
        }

        if ( ssp.dwCurrentState == SERVICE_STOPPED )
            break;

        if ( GetTickCount() - dwStartTime > dwTimeout )
        {
            printf( "Wait timed out\n" );
        }
    }
    debug.echo("Service "+serviceName+" stopped successfully");

}
void Services::Register(std::string sPath)
{
    if(this->isAvailable() == FALSE)
    {
        SC_HANDLE schSCManager;
        SC_HANDLE schService;
        /*TCHAR szPath[MAX_PATH] = sPath.c_str();
        if( !GetModuleFileName( NULL, szPath, MAX_PATH ) )
        {
            printf("Cannot install service (%d)\n", GetLastError());
            debug.WhatError();
            return;
        }*/

        // Get a handle to the SCM database.
        schSCManager = OpenSCManager(
                           NULL,                    // local computer
                           NULL,                    // ServicesActive database
                           SC_MANAGER_ALL_ACCESS);  // full access rights

        if (NULL == schSCManager)
        {
            printf("OpenSCManager failed (%d)\n", GetLastError());
            return;
        }

        // Create the service

        schService = CreateService(
                         schSCManager,              // SCM database
                         TEXT(serviceName.c_str()),                   // name of service
                         TEXT(serviceName.c_str()),                   // service name to display
                         SERVICE_ALL_ACCESS,        // desired access
                         SERVICE_WIN32_OWN_PROCESS, // service type
                         SERVICE_DEMAND_START,      // start type
                         SERVICE_ERROR_NORMAL,      // error control type
                         sPath.c_str(),                     // path to service's binary
                         NULL,                      // no load ordering group
                         NULL,                      // no tag identifier
                         NULL,                      // no dependencies
                         NULL,                      // LocalSystem account
                         NULL);                     // no password

        if (schService == NULL)
        {
            printf("CreateService failed (%d)\n", GetLastError());
            return;
        }
        else printf("Service installed successfully\n");
    }
}
void Services::UnRegister()
{
    if(DeleteService(schService))
    {
        debug.echo("[OK] Suppression "+serviceName);
    }
    else
    {
        printf("Delete Service failed (%d)\n", GetLastError());
        debug.echo("[FAIL] Suppression "+serviceName);
    }
}
bool Services::wellInstalled()
{

    Registry r(HKEY_LOCAL_MACHINE);
    if(r.ExistsKey("SYSTEM\\CurrentControlSet\\services\\"+serviceName))
    {
        std::string key = r.ReadKey("SYSTEM\\CurrentControlSet\\services\\"+serviceName +"\\Parameters" , "Application");
        std::cout << "Verify key registry" << std::endl;
        std::cout << "REGEDIT : " << key << std::endl;
        std::cout << "REGEDIT : " << uriPath << std::endl;
        if(key == uriPath)
        {
            return true;
        }
        return false;
    }
    return false;

}
void Services::getKeyParameter(std::string _uriPath)
{
    uriPath = _uriPath;
}
void Services::RunAnProgrammAsServices()
{
    this->Register("C:\\Windows\\System32\\srvany.exe");
    Registry r(HKEY_LOCAL_MACHINE);
    r.CreateKey("SYSTEM\\CurrentControlSet\\services\\"+serviceName , "Parameters");
    r.NewValueFromKey("SYSTEM\\CurrentControlSet\\services\\"+serviceName+"\\Parameters" , "Application" , uriPath);

}
void Services::Close()
{
    if(this->isAvailable())
    {
        CloseServiceHandle(schService);
        CloseServiceHandle(schSCManager);
    }

}
Services::~Services()
{


}
