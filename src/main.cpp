#include <windows.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <getopt.h>
#include <typeinfo>
#include "Output.h"
#include "Services.h"
/**
    @Author = thehawk as camael24
    @Github = https://github.com/camael24/Prompt
    @License = http://creativecommons.org/licenses/by-nc-sa/3.0/fr/
*/
void usage()
{
    std::cout << "Usage of Prompt: " << std::endl;
    std::cout << "    -p  <path>      PathProgram (Path to php-cgi , if php-cgi are already in the global path do nothing ;) )" << std::endl;
    std::cout << "    -n  <name>      NameService" << std::endl;
    std::cout << "    -s  <signal>    Signal (install | uninstall | start | stop | restart)" << std::endl;
    std::cout << "    -h              This Help" << std::endl;
    std::cout << "    -v              Version" << std::endl;
}
int main (int argc, char **argv)
{

    char *path = "php-cgi -b 127.0.0.1:9000";
    char *name = "PromptServices";
    char *signal = "help";

    int index;
    int c;

    opterr = 0;

    while ((c = getopt (argc, argv, "hvp:n:s:")) != -1)
        /**
            -p  <path>      PathProgram
            -n  <name>      NameService
            -s  <signal>    Signal (install | uninstall | start | stop | restart)
            -h              Help
            -v              Version
        */
        switch (c)
        {
        case 'p':
            path = optarg;
            break;
        case 'n':
            name = optarg;
            break;
        case 's':
            signal = optarg;
            break;
        case 'h':
            usage();
            return 0;
            break;

        case '?':
            if (optopt == 'p' || optopt == 's' || optopt == 'n')
                fprintf (stderr, "Option -%c requires an argument.\n", optopt);
            else if (isprint (optopt))
                fprintf (stderr, "Unknown option `-%c'.\n", optopt);
            else
                fprintf (stderr,
                         "Unknown option character `\\x%x'.\n",
                         optopt);
            return 1;
        default:
            return 1;
        }


    for (index = optind; index < argc; index++)
        printf ("Non-option argument %s\n", argv[index]);

    printf ("Path = %s, Name = %s, Signal = %s\n",path, name, signal);

    Output out;

    if (strcmp(signal , "install") == 0)
    {
        Services services(out , name , path);
    }
    else if (strcmp(signal , "uninstall") == 0)
    {
        Services services(out , name , path);
        services.Stop();
        services.UnRegister();
    }
    else if (strcmp(signal , "start") == 0)
    {
        Services services(out , name , path);
        services.Start();

    }
    else if (strcmp(signal , "stop") == 0)
    {
        Services services(out , name , path);
        services.Stop();
    }
    else if (strcmp(signal , "restart") == 0)
    {
        Services services(out , name , path);
        services.Stop();
        services.Start();
    }
    else if (strcmp(signal , "help") == 0)
    {
        usage();
    }
    else
    {
        fprintf (stderr,"Unknown option signal %s.\n",signal);
    }

    return 0;
}
