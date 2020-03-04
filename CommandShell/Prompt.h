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
	string currentPrompt;
  	char buff[FILENAME_MAX]; // define buffer of FILENAME_MAX size

	public:
		Prompt() { // <-- constructor
			// getcwd system call returns a string of the current working directory
			// TODO: check for null returned by getcwd which happens on error
			currentPrompt =  getcwd(buff, FILENAME_MAX);
		}

		string get() { return currentPrompt + "$ "; }
};

// TODO: comment out -- used for testing
// int main() {
// 	Prompt newPrompt = Prompt();
// 	cout << newPrompt.get() << "\n";
// 	return 0;
// };