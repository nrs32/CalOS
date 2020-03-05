/**
 * Class representing a shell prompt with a getter for the current working directory.
 * 
 * @author Nikita Sietsema
 * 3 March 2020
 */

#include <iostream>
#include <unistd.h>
#include <stdio.h>  // for access to FILENAME_MAX 
#include <direct.h> // Used for Windows :D
#include "Prompt.h"
using namespace std;

Prompt::Prompt() { // <-- constructor
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