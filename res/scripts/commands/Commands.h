#pragma once

#include <iostream>
#include <map>
#include <string>

using std::cout;
using std::endl;
using std::map;
using std::string;

enum Names{
        ABOUT,
        ADD,
        RM,
        LS,
        PYTHON,
        HELP,
        ALL //Used to iterate through the enum to list all commands.
};
    void getHelp(int command);
    void listCommands();