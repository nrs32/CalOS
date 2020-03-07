/**
 * SNShell is a command shell that reads a user command from the keyboard 
 * and performs that command on behalf of the user
 * 
 * Supports usual terminal commands with execve
 * And supports pwd, cd and exit
 * 
 * @author Nikita Sietsema and Sebrina Zeleke
 * 4 March 2020
 * */

// Give us access to our other classes
#include "Path.h"
#include "Prompt.h"
#include "SNShell.h"
#include "CommandLine.h"
#include "Utils.h"

// Other imports
#include <iostream>
#include <thread>
#include <unistd.h>    // For Linux
#include <string.h>    // For strcmp
#include <sys/types.h> // for waitpid
#include <sys/wait.h>  // for waitpid
#include <errno.h>     // To reset errno

// Continuously prompt for and handle commands
void SNShell::run() { 
    // Infinite loop
    while (1) {

                            ///////////////////////////////////////////
                            // // TODO: DELETE THIS!!
                            // PATH TEST
                            // Path testPath = Path();

                            // vector<string> testResult = testPath.get();
                            // for (int i = 0; i < testResult.size() && i < 3; i++) {
                            //     cout << testResult[i] << "\nsize: " << testResult.size() << "\n";
                            // }

                            // cout << "\npath of ls: " << testPath.getDirectory(testPath.find("ls")) << "\n";

                            // cout << "test invalid index: " << 
                            // cout << testPath.getDirectory(40);// << "\n";

                            // // PROMPT TEST
                            // Prompt testPrompt = Prompt();
                            // cout << testPrompt.get() << "\n";

                            // COMMAND LINE TEST
                            // CommandLine testCL = CommandLine(cin);
                            ///////////////////////////////////////////


        // Print out prompt
        Prompt newPrompt = Prompt();
        cout << newPrompt.get();

        handleCommand();
    }
}

// Determine what command we got and call the handler
void SNShell::handleCommand() {
    CommandLine CL = CommandLine(cin);

    if (CL.getArgCount() != 0) {
        // Let parent handle exit and do not create child.
        if (!strcmp(CL.getCommand(), "exit")) { exit(0); }

        // Create a child to handle the command
        pid_t pid = fork();
        
        // Check for error
        if (pid == -1){ 
            cout << "Could not fork to execute command.\n"; 

        // This is the parent thread
        } else if (pid > 0) {
            if (CL.noAmpersand()) { waitpid(pid, NULL, 0); }

        // This is the child thread
        } else if (pid == 0) {
            childExecuteCommand(CL);
        }
    }
}

/**
 * Function for the child thread to execute the given command
 * @param CL the current commandLine being used
 * */
void SNShell::childExecuteCommand(CommandLine CL) {
        char* command = CL.getCommand();
        Utils utils = Utils(); // used for errors

        Prompt newPrompt = Prompt();
        if (!strcmp(command, "cd")) {

            int numArgs = CL.getArgCount();

            if (numArgs > 2) {
                cout << "Too many arguments" << endl;

            } else {
                // Change to given directory or change to Home directory
                int success = chdir(numArgs == 1 ? getenv("HOME") : CL.getArgVector()[1]);

                // Check for chdir error
                if (success == -1) { utils.handleError("Error changing directory: ", true); }
            }

        } else if (!strcmp(command, "pwd")) {
            // Like the linux terminal
            // Just ignore unnecessary args and print the working directory
            cout << newPrompt.get_pwd() << endl;

        } else if (!strcmp(command, "hi") || !strcmp(command, "Hi")) {
            cout << "Hello" << endl;

        } else if (!strcmp(command, "hello") || !strcmp(command, "Hello")) {
            cout << "Hello" << endl;

        // Use execve to handle built in commands and their errors
        // (commands like: ls and mkdir)
        } else {
            Path path = Path();
            string commandPath = path.getDirectory(path.find(command)) + "/" + command;
            execve(commandPath.c_str(), CL.getArgVector(), NULL );
            utils.handleError("Error: ", true);
        }
}