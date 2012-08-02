#ifndef REGISTRY_H
#define REGISTRY_H
#include <windows.h>
#include <stdio.h>
#include <process.h>
#include <tchar.h>
#include <iostream>
#include <string>
#include "Output.h"
/**
    @Author = thehawk as camael24
    @Github = https://github.com/camael24/Prompt
    @License = http://creativecommons.org/licenses/by-nc-sa/3.0/fr/
*/
class Registry
{
public:
    Registry(HKEY key);
    // KEY
    std::string ReadKey(std::string subKey , std::string value);
    bool ExistsKey(std::string subKey);
    std::string DeleteKey(std::string subKey);
    std::string CreateKey(std::string subKey , std::string keyToCreate);

    // VALUES
    std::string ModifyValueFormKey(std::string subKey , std::string keyName , std::string keyValue);
    std::string NewValueFromKey(std::string subkey , std::string keyName , std::string keyValue);

    virtual ~Registry();
protected:
    void setResult(TCHAR result);
    void setStatus(LONG status);
private:
    BOOL RegDelnode (std::string lpSubKey);
    BOOL RegDelnodeRecurse (HKEY hKeyRoot, std::string lpSubKey);
    HKEY _masterKey;
};

#endif // REGISTRY_H
