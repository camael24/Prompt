#include "Output.h"
/**
    @Author = thehawk as camael24
    @Github = https://github.com/camael24/Prompt
    @License = http://creativecommons.org/licenses/by-nc-sa/3.0/fr/
*/
Output::Output()
{
    //ctor
}
void Output::echo(std::string line)
{
    std::cout << line << std::endl;
}
void Output::WhatError()
{

    LPVOID lpMsgBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        GetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
        (LPTSTR) &lpMsgBuf,
        0,
        NULL
    );
// Process any inserts in lpMsgBuf.
// ...
// Display the string.
    MessageBox( NULL, (LPCTSTR)lpMsgBuf, "Error", MB_OK | MB_ICONINFORMATION );
// Free the buffer.
    LocalFree( lpMsgBuf );
}
Output::~Output()
{
    //dtor
}
