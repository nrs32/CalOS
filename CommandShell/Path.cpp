/**
 * Implementation for Path class representing directories to search for executables. 
 * 
 * @author Nikita Sietsema
 * 3 March 2020
 **/
#include "Path.h"
#include "Utils.h" // For split

#include <iostream>
#include <vector>
#include <unistd.h>    // For Linux
#include <cstdlib>     // For access to getenv
#include <string.h>    // For strcmp
#include <dirent.h>    // For opendir
#include <sys/types.h> // For opendir
#include <errno.h>     // To handle errors from readdir

// Special for windows :D
#ifdef _WIND32
#define DELIMETER ";" // Windows
#else
#define DELIMETER ":" // Linux
#endif

using namespace std;

// Constructor responsible for setting the available 
// directories found in PATH
Path::Path() {
    // Get the value of PATH
    char* path = getenv("PATH");

    // Handle the case that PATH does not exist (could this ever happen?)
    if (path == NULL) {
        cout << "Error: PATH does not exist.\n";
        exit(-1);
    }

    // Use Utils split function to split the paths at the DELIMETER
    Utils utils = Utils();
    directories = utils.split(path, DELIMETER);
}

/**
 * Print perror using message and exit
 * @param message a specific string to give perror
 * */
void Path::exitOnError(string message) const {
    perror(message.c_str());
    exit(-1);
}

/**
 * Look for a program in the PATH and return the index of the directory for that program 
 * and otherwise return -1 for not found
 * @param program a string of the program name
 * @return the int index of the program if found, and otherwise -1
 * */
int Path::find(const string& program) const {			
    // look for program and return the index when found
    for (int i = 0; i < directories.size(); i++) {
        DIR* currentDirectory = opendir(directories[i].c_str());
        dirent * entry; // An entry in the currentDirectory

        // handle possible error from opendir
        if (currentDirectory == NULL) { exitOnError("Error with opendir: "); }

        // Set errno to 0 so we can see if readdir errored
        errno = 0;

        while ((entry = readdir(currentDirectory)) != NULL) {
            // See if we found the program
            if (!strcmp(entry->d_name, program.c_str())) {
                int success = closedir(currentDirectory);
                if (success == -1) { exitOnError("Error with closedir: "); }
                return i;
            }
        }

        // Make sure we did not break the while loop because readdir error-ed
        if (errno != 0) { exitOnError("Error with readdir: "); }

        int success = closedir(currentDirectory);
        if (success == -1) { exitOnError("Error with closedir: "); }
    }

    // We did not find the program, return -1
    return -1;
}

/**
 * Find the directory at a specified index in directories vector
 * @param i int of program location in directories vector
 * @return a string name of the directory at the given location
 * */
string Path::getDirectory(int i) const {

    // Check for invalid index
    if (directories.size() < (i + 1) || i < 0) {
        cout << "Error: Invalid index - no directory found.\n";
        exit(-1);
    }

    return directories[i];
}