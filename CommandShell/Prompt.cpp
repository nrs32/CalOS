/**
 * Class representing a shell prompt with a getter for the current working directory.
 * 
 * @author Nikita Sietsema
 * 3 March 2020
 */
#include "Prompt.h"

#include <iostream>
#include <unistd.h>
#include <stdio.h>  // for access to FILENAME_MAX 
using namespace std;

// Constructor responsible for initializing 
// the prompt from the current working directory
Prompt::Prompt() {
    // getcwd system call returns a string of the current working directory
    char* cwd = getcwd(buff, FILENAME_MAX);

    // Check for errors from getcwd
    if (cwd == NULL) {
        perror("Error: ");
        exit(-1);
    } else {
        currentPrompt = cwd;
    }
};