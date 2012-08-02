#ifndef OUTPUT_H
#define OUTPUT_H
#include <stdio.h>
#include <string>
#include <iostream>
#include <windows.h>
/**
    @Author = thehawk as camael24
    @Github = https://github.com/camael24/Prompt
    @License = http://creativecommons.org/licenses/by-nc-sa/3.0/fr/
*/

class Output
{
    public:
        Output();
        void static WhatError();
        void echo(std::string line);
        virtual ~Output();

    protected:
    private:
};

#endif // OUTPUT_H
