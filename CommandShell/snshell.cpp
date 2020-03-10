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
    Path path = Path();

    // Infinite loop
    while (1) {
        // Print out prompt
        Prompt newPrompt = Prompt();
        cout << newPrompt.get();

        handleCommand(path);
    }
}

// Determine what command we got and call the handler
void SNShell::handleCommand(Path path) {
    CommandLine CL = CommandLine(cin);
    char* command = CL.getCommand();

    if (CL.getArgCount() != 0) {
        // Let parent handle exit and do not create child.
        if (!strcmp(command, "exit")) { 
            exit(0); 

        // Parent should also handle cd
        } else if (!strcmp(command, "cd")) {
            handleCd(CL);

        // Create a child to handle the command
        } else {
            pid_t pid = fork();
            
            // Check for error making child
            if (pid == -1) { 
                cout << "Could not fork to execute command.\n"; 

            // This is the parent thread
            } else if (pid > 0) {
                if (CL.noAmpersand()) { waitpid(pid, NULL, 0); }

            // This is the child thread
            } else if (pid == 0) {
                childExecuteCommand(CL, path);
            }
        }
    }
}

/**
 * Allow parent to change directory to given path, or default to home
 * @param CL the current commandLine
 * */
void SNShell::handleCd(CommandLine &CL) {
    int numArgs = CL.getArgCount();

    if (numArgs > 2) {
        cout << "Too many arguments" << endl;

    } else {
        // Change to given directory or change to Home directory
        int success = chdir(numArgs == 1 ? getenv("HOME") : CL.getArgVector()[1]);

        // Check for chdir error
        Utils utils = Utils();
        if (success == -1) { utils.handleError("Error changing directory", false); }
    }
}

/**
 * Function for the child thread to execute the given command
 * @param CL the current commandLine being used
 * */
void SNShell::childExecuteCommand(CommandLine CL, Path path) {
    char* command = CL.getCommand();
    Utils utils = Utils(); // used for errors

    if (!strcmp(command, "pwd")) {
        // Like the linux terminal
        // Just ignore unnecessary args and print the working directory
        Prompt newPrompt = Prompt();
        cout << newPrompt.get_pwd() << endl;
    
    } else if (!strcmp(command, "hi") || !strcmp(command, "Hi")) {
        cout << "Hello" << endl;

    } else if (!strcmp(command, "hello") || !strcmp(command, "Hello")) {
        cout << "Hi" << endl;

    // Use execve to handle built in commands and their errors
    // (commands like: ls,cat, ps, mkdir, etc.)
    } else {
        string commandPath = path.getDirectory(path.find(command)) + "/" + command;
        execve(commandPath.c_str(), CL.getArgVector(), NULL);
        utils.handleError("Error", true);
    }

    exit(0); // End child process
}