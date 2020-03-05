#include <iostream>
#include <unistd.h>
#include <stdio.h>  // for access to FILENAME_MAX 
#include <direct.h> // Used for Windows :D
using namespace std;

/**
 * Class representing a shell prompt with a getter for the current working directory.
 * 
 * @author Nikita Sietsema
 * 3 March 2020
 */
class Prompt {
	// ANSI strings to set text color
	string setMagenta = "\033[35m";
    string setCyan = "\033[36m";
    string setDefault = "\033[0m";

	string currentPrompt;
  	char buff[FILENAME_MAX]; // define buffer of FILENAME_MAX size

	public:
		Prompt();

		// Return prompt with shell name in magenta and directory in cyan
		string get() { return setMagenta + "SNShell " + setCyan + currentPrompt + setDefault + "\n$ "; }
};