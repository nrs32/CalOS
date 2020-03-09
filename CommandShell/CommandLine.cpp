#include "CommandLine.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <stdio.h> 
#include <string.h>
#include <cstring>
#include "Utils.h"
using namespace std;


/*
- class to read a command line
- public methods
    - CommandLine
    -getCommand
    -getArgCount
    -getArgVector
    -noAmpersand

CommandLine
    read command from in
    parse the argument
    build instance variable 

*/


CommandLine::CommandLine(istream& in){
    string argument;
    char space[2] = " ";
    getline(in, argument);
    int argSize = argument.size();

    if (argSize == 0) {
        cout << "No command provided." << endl;
        argc = 0;

    } else {
        char lastArgChar[2] = { argument[argSize -1] };

        // Check for an & in the argument. 
        if (!strcmp(lastArgChar, "&")) {
            if (argSize >= 2) {
                // Make sure & follows a space and is not part of fileName
                char secondtoLast[2] = { argument[argSize -2] };
                if (!strcmp(secondtoLast, space)) {
                    // If there is an &, remove it and set hasAmpersand.
                    hasAmpersand = true;
                    argument = argument.substr(0, argSize -1);
                }
            }
        }
    }

    tempStorage = (char*) calloc (argSize, sizeof(char));
    strcpy(tempStorage, argument.c_str());
    argv = split(tempStorage, space);
}

char* CommandLine::getCommand() const {
    return argv[0];
}

int CommandLine::getArgCount() const {
    return argc;
}

char** CommandLine::getArgVector() const {
    return argv;
}

char* CommandLine::getArgVector(int i) const{
    return argv[i];
}

bool CommandLine::noAmpersand() const {
    return !hasAmpersand;
}

char** CommandLine::split(char* str, char* delimeter) {
    char* token;
    char** result;

    // call strtok repeatedly to split the string at every delimeter
    // add each token to result vector<string>
    token = strtok(str, delimeter);
    int index = 0;


    while (token != NULL)
        {
        result[index] = token;
        token = strtok(NULL, delimeter);
        index++;
    }
    
    result[index] = NULL;

    argc = index; // it is equal to index beause it increments before it exits
    return result;
}

CommandLine::~CommandLine() {
    // free(tempStorage);
    // argv = NULL;
    // tempStorage = NULL;
}