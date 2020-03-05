/**
 * Implementation for Path class representing directories to search for executables. 
 * 
 * @author Nikita Sietsema
 * 3 March 2020
 **/

#include <iostream>
#include <unistd.h> // For Linux
#include <cstdlib>  // for access to getenv
#include <vector>
#include "Path.h"
#include "Utils.h"  // For split
using namespace std;

// Path Constructor
Path::Path() { 
    // Get the value of PATH and split the paths at the ';'
    char* envVarValue = getenv("PATH");

    // Handle case if PATH does not exist (could this happen?)
    if (envVarValue == NULL) {
        cout << "Error: PATH does not exist.\n";
        exit(-1);
    }

    char delimeter[2] = ";";

    // Use Utils split function
    Utils utils = Utils();
    directories = utils.split(envVarValue, delimeter);
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
        // Check for the program name in the directory name
        // TODO: this is probably bad, re-write it
        if (directories[i].find(program) != -1) {
            return i;
        }
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
    }

    return directories[i];
}