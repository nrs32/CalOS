// Give us access to our other classes
#include "Path.h"
#include "Prompt.h"

// Other imports
#include <iostream>
#include <unistd.h>

/**
 * SNShell is a command shell that reads a user command from the keyboard 
 * and performs that command on behalf of the user
 * 
 * supported commands are: ls, ps, pwd, cd and pwd
 * 
 * @author Nikita Sietsema and Sebrina Zeleke
 * 4 March 2020
 * */
class SNShell {

	public:
        // Constructor 
		SNShell() { }

        // Continuously prompt for and handle commands
		void run() { 
            // ANSI strings to set text color
            string setCyan = "\033[36m";
            string setDefault = "\033[0m";

            // Infinite loop
            while (1) {
                // Print out prompt (in cyan) and wait for user command
                Prompt newPrompt = Prompt();
                cout << setCyan + newPrompt.get() + setDefault;
                string command;
                cin >> command; // TODO: must handle spaces correctly

                // handle command
                // TODO: make private functions used to handle each command
                // and handleCommand function to parse and find the correct command?
                cout << "Got command: " << command << "\n";
            }
        }
};