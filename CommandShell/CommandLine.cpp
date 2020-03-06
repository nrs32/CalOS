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
    // string argument;
    
    // getline(in, argument);
    // char space[2] = " ";

    // char* arg[argument.size() + 1];
	// strcpy(arg, argument.c_str());

    // argv = CommandLine::split(arg, space);
    // argc = argv.size();

    string argument;
    char * memory;
    char space[2] = " ";
    getline(in, argument);
    memory = (char*) calloc (argument.size(), sizeof(char));
    strcpy(memory, argument.c_str());
    argv = split(memory, space);
}

char* CommandLine::getCommand() const {
    return argv[0];
}

int CommandLine::getArgCount() const {
    return argc;
}

char** CommandLine::getArgVector() const{
    return argv;
}

char* CommandLine::getArgVector(int i) const{
    return argv[i];
}

bool CommandLine::noAmpersand() const {
    char* ampersand;
    const char* amp = "&";

    for(int i =0; i < argc; i++){
        ampersand = std::strstr(argv[i], amp);
        if(ampersand != NULL){
            return false;
        }
    }

    return true;
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

    argc = index; // it is equal to index beause it increments before it exits
    return result;
}