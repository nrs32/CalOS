#include "CommandLine.h"
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
class SNShell {
	void handleCommand();
	void childExecuteCommand(CommandLine CL);
	void handleCd(CommandLine CL);

	public:
		SNShell() {};
		void run();
};