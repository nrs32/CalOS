#include "CommandLine.h"
#include "Path.h"
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
	void handleCommand(Path path);
	void childExecuteCommand(CommandLine CL, Path path);
	void handleCd(CommandLine CL);

	public:
		SNShell() {};
		void run();
};