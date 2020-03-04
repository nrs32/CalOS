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
		Prompt() { // <-- constructor
			// getcwd system call returns a string of the current working directory
			char* cwd = getcwd(buff, FILENAME_MAX);

			// Check for errors from getcwd
			if (cwd == NULL) {
				perror("Error: ");
				exit(-1);
			} else {
				currentPrompt = cwd;
			}
		}

		// Return prompt with shell name in magenta and directory in cyan
		string get() { return setMagenta + "SNShell " + setCyan + currentPrompt + setDefault + "\n$ "; }
};

// TODO: delete -- used for testing
// int main() {
// 	Prompt newPrompt = Prompt();
// 	cout << newPrompt.get() << "\n";
// 	return 0;
// };