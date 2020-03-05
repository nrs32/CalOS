#include "commandLine.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <stdio.h> 
#include <string.h>
#include <cstring>

using namespace std;


/*
- class to read a command line
- public methods
    - commandLine
    -getCommand
    -getArgCount
    -getArgVector
    -noAmpersand

commandLine
    read command from in
    parse the argument
    build instance variable 

*/


commandLine::commandLine(istream& in){
    // string argument;
    
    // getline(in, argument);
    // char space[2] = " ";

    // char* arg[argument.size() + 1];
	// strcpy(arg, argument.c_str());

    // argv = commandLine::split(arg, space);
    // argc = argv.size();

    string argument;
    char * memory;
    char space[2] = " ";
    getline(in, argument);
    memory = (char*) calloc (argument.size(), sizeof(char));
    strcpy(memory, argument.c_str());

    argv = commandLine::split(memory, space);

    //where do you free memory?
}

char* commandLine::getCommand() const {
    return argv[0];
}

int commandLine::getArgCount() const {
    return argc;
}

char** commandLine::getArgVector() const{
    return argv;
}

char* commandLine::getArgVector(int i) const{
    return argv[i];
}

bool commandLine::noAmpersand() const {
    char* ampersand;
    const char* amp = "$";

    for(int i =0; i < argc; i++){
        ampersand = std::strstr(argv[i], amp);
        if(ampersand != NULL){
            return false;
        }
    }

    return true;
}


char** commandLine::split(char* str, char* delimeter) {
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
