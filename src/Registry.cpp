#include "Registry.h"
/**
    @Author = thehawk as camael24
    @Github = https://github.com/camael24/Prompt
    @License = http://creativecommons.org/licenses/by-nc-sa/3.0/fr/
*/
Registry::Registry(HKEY masterKey)
{
    _masterKey = masterKey;
}
std::string Registry::ReadKey(std::string subKey , std::string value)
{
    if(this->ExistsKey(subKey))
    {
        std::string baz;
        LONG lResult;
        DWORD dwLen = MAX_PATH;
        TCHAR szName[MAX_PATH];
        HKEY hKey;

        lResult = RegOpenKeyEx(_masterKey,TEXT(subKey.c_str()),0,KEY_READ, &hKey);
        lResult = RegQueryValueEx(hKey, TEXT(value.c_str()), 0, 0, (BYTE *)szName, &dwLen);
        RegCloseKey(hKey);
        baz = TEXT(szName);
        return baz;
    }
}
bool Registry::ExistsKey(std::string subKey)
{
    LONG lResult;
    HKEY hKey;
    //Output d;
    //d.echo("Test of "+subKey);
    lResult = RegOpenKeyEx(_masterKey,TEXT(subKey.c_str()),0,KEY_READ, &hKey);
    RegCloseKey(hKey);
    if (lResult != ERROR_SUCCESS)
    {
       return false;
    }
    else
    {
        return true;
    }
}
std::string Registry::CreateKey(std::string subKey , std::string keyToCreate)
{
    std::string d;
    LONG lResult;
    HKEY hKey;

    d = subKey+"\\"+keyToCreate;
    if(!this->ExistsKey(d))
    {
        LONG l = RegCreateKeyEx(_masterKey,TEXT(d.c_str()),0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&hKey,NULL);
        RegCloseKey(hKey);
        if(l != EXIT_SUCCESS)
        {
            return "[FAIL] you can't use it :s " + d;
        }
        else
        {
            return "[OK] " + d;
        }
    }
    else
    {
        return "The Key " + d + " already exists";
    }
}
BOOL Registry::RegDelnodeRecurse (HKEY hKeyRoot, std::string lpSubKey)
{
    LPTSTR lpEnd;
    LONG lResult;
    DWORD dwSize;
    TCHAR szName[MAX_PATH];
    HKEY hKey;
    FILETIME ftWrite;

    // First, see if we can delete the key without having
    // to recurse.

    lResult = RegDeleteKey(hKeyRoot, TEXT(lpSubKey.c_str()));

    if (lResult == ERROR_SUCCESS)
        return TRUE;

    lResult = RegOpenKeyEx (hKeyRoot, TEXT(lpSubKey.c_str()), 0, KEY_READ, &hKey);

    if (lResult != ERROR_SUCCESS)
    {
        if (lResult == ERROR_FILE_NOT_FOUND)
        {
            printf("Key not found.\n");
            return TRUE;
        }
        else
        {
            printf("Error opening key.\n");
            return FALSE;
        }
    }

    // Check for an ending slash and add one if it is missing.


    // Enumerate the keys

    dwSize = MAX_PATH;
    lResult = RegEnumKeyEx(hKey, 0, szName, &dwSize, NULL,
                           NULL, NULL, &ftWrite);

    if (lResult == ERROR_SUCCESS)
    {
        do
        {

            //StringCchCopy (lpEnd, MAX_PATH*2, szName);

            if (!this->RegDelnodeRecurse(hKeyRoot, TEXT(lpSubKey.c_str())))
            {
                break;
            }

            dwSize = MAX_PATH;

            lResult = RegEnumKeyEx(hKey, 0, szName, &dwSize, NULL,
                                   NULL, NULL, &ftWrite);

        }
        while (lResult == ERROR_SUCCESS);
    }

    lpEnd--;
    *lpEnd = TEXT('\0');

    RegCloseKey (hKey);

    // Try again to delete the key.

    lResult = RegDeleteKey(hKeyRoot, TEXT(lpSubKey.c_str()));

    if (lResult == ERROR_SUCCESS)
        return TRUE;

    return FALSE;
}

BOOL Registry::RegDelnode (std::string lpSubKey)
{
    return this->RegDelnodeRecurse(_masterKey, TEXT(lpSubKey));
}
std::string Registry::DeleteKey(std::string subKey)
{
    if(this->ExistsKey(subKey))
    {
        if(this->RegDelnode(subKey) && !this->ExistsKey(subKey))
        {
            return "[OK] it delete "+subKey;
        }
        else
        {
            return "[FAIL] You don't delete it " + subKey;
        }
    }
    else
    {
        return "[FAIL] it don't exist " + subKey;
    }
}
std::string Registry::NewValueFromKey(std::string subKey , std::string keyName , std::string keyValue)
{
    LONG lResult;
    HKEY hKey;

    lResult = RegOpenKeyEx(_masterKey,TEXT(subKey.c_str()),0,KEY_ALL_ACCESS, &hKey);
    LONG l = RegSetValueEx(hKey, TEXT(keyName.c_str()), 0, REG_SZ, (CONST BYTE*)keyValue.c_str(), keyValue.size() );
    if(l == ERROR_SUCCESS)
    {
        return "[Modify] " + subKey +" , "+keyName+" : " +keyValue;
    }
    else
    {
        return "[Fail fail] " + subKey +" , "+keyName+" : " +keyValue;
    }

    RegCloseKey( hKey );

}
std::string Registry::ModifyValueFormKey(std::string subKey , std::string keyName , std::string keyValue)
{
    LONG lResult;
    HKEY hKey;

    lResult = RegOpenKeyEx(_masterKey,TEXT(subKey.c_str()),0,KEY_ALL_ACCESS, &hKey);
    LONG l = RegSetValueEx(hKey, TEXT(keyName.c_str()), 0, REG_SZ, (CONST BYTE*)keyValue.c_str(), keyValue.size() );
    if(l == ERROR_SUCCESS)
    {
        return "[Create] " + subKey +" , "+keyName+" : " +keyValue;
    }
    else
    {
        return "[Fail] " + subKey +" , "+keyName+" : " +keyValue;
    }

    RegCloseKey( hKey );

}
Registry::~Registry()
{
    //dtor
}
