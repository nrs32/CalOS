/**
 * Class dealing with the command line argument 
 * 
 * @author Sebrina Zeleke 
 * 3 March 2020
 */

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


/**
 * Explicit constructor for CommandLine 
 * @param istream& in
 * */
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
    tempStorage = argument.c_str();
    argv = split(tempStorage, space);
}

/**
 * Get the command from a command line  
 * @param none
 * @return command
 * */
char* CommandLine::getCommand() const {
    return argv[0];
}

/**
 * Get the number of argument   
 * @param none
 * @return argc
 * */
int CommandLine::getArgCount() const {
    return argc;
}

/**
 * Get the argument vector
 * @param none
 * @return argv
 * */
char** CommandLine::getArgVector() const {
    return argv;
}

/**
 * Get the command from a command line of index i 
 * @param int i
 * @return argv[i]
 * */
char* CommandLine::getArgVector(int i) const{
    return argv[i];
}

/**
 * Check if there an ampersand   
 * @param none
 * @return true if there is no ampersand 
 * */
bool CommandLine::noAmpersand() const {
    return !hasAmpersand;
}


/**
 * Split a given argument with delimeter   
 * @param char* str, 
 * @param char* delimeter
 * @return a char** that is the splitted argument 
 * */
char** CommandLine::split(char* str, char* delimeter) {
    char* token;
    char** result;

    // call strtok repeatedly to split the string at every delimeter
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

/**
 * Destructor   
 * */
CommandLine::~CommandLine() {
    // free(tempStorage);
    // argv = NULL;
    // tempStorage = NULL;
}