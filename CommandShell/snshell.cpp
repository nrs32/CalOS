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

// Other imports
#include <iostream>
#include <thread>
#include <unistd.h>    // For Linux
#include <string.h>    // For strcmp
#include <sys/types.h> // for waitpid
#include <sys/wait.h>  // for waitpid

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

                            // cout << "test invalid index: " << testPath.getDirectory(40) << "\n";

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

    // Create a child to handle the command
    pid_t pid = fork();
    
    // Check for error
    if (pid == -1){ 
        cout << "Could not fork."; 
        exit(1); 

    // This is the parent thread
    } else if (pid > 0) {

        if (CL.noAmpersand()) { waitpid(pid, NULL, 0); }

    // This is the child thread
    } else if (pid == 0) {
        char* command = CL.getCommand(); // <-- TODO!!! getCommand should ignore apersands

        Prompt newPrompt = Prompt();
        if (!strcmp(command, "cd")) {
            // int argc = CL.getArgCount();
            cout << "handle cd here" << endl;

        } else if (!strcmp(command, "pwd")) {
            // Like the linux terminal
            // Just ignore unnecessary args and print the working directory
            cout << newPrompt.get_pwd() << endl;

        } else if (!strcmp(command, "exit")) {
            cout << "handle exit here" << endl;

        // Use execve to handle built in commands and their errors
        // (commands like: ls and mkdir)
        } else {
            Path path = Path();
            string commandPath = path.getDirectory(path.find(command)) + "/" + command;
            execve(commandPath.c_str(), CL.getArgVector(), NULL);
            perror("Error: ");
        }
    }
}