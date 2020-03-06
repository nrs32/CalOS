#include "CommandLine.h"
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
	void handleCommand();
	char** findCommand(CommandLine currentCL);

	public:
		SNShell() {};
		void run();
};