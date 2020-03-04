#include <iostream>
#include <unistd.h>
#include <cstdlib> // for access to getenv
#include <vector>
#include <sstream> // for easy string splitting
using namespace std;

/**
 * Class representing directories to search for executables. 
 * 
 * @author Nikita Sietsema
 * 3 March 2020
 **/
class Path {
	vector<string> directories;

	/**
	 * Split a char* into tokens using a delimeter
	 * @param str is a char* to be split into vector<string>
	 * @param delimeter is the char* to split the string at
	 * @return a vector containing the string tokens from str 
	 * */
	vector<string> split(char* str, char* delimeter) {
		char* token;
		vector<string> result;

		// call strtok repeatedly to split the string at every delimeter
		// add each token to result vector<string>
		token = strtok(str, delimeter);
		while (token != NULL)
		{
			result.push_back(token);
			token = strtok(NULL, delimeter);
		}

		return result;
	}

	public:
		Path() { // <-- constructor
			// Get the value of PATH and split the paths at the ';'
			// TODO: check for null return from getenv
			char* envVarValue = getenv("PATH");
			char delimeter[2] = ";";
			directories = split(envVarValue, delimeter);
		}

		/**
		 * Look for a program in the PATH and return the index of the directory for that program 
		 * and otherwise return -1 for not found
		 * @param program a string of the program name
		 * @return the int index of the program if found, and otherwise -1
		 * */
		int find(const string& program) const {			
			// look for program and return the index when found
			for (int i = 0; i < directories.size(); i++) {
				// Check for the program name in the directory name
				// TODO: this is probably bad, re-write it
				if (directories[i].find(program) != -1) {
					return i;
				}
			}

			// We did not find the program, return -1
			return -1;
		}

		/**
		 * Find the directory at a specified index in directories vector
		 * @param i int of program location in directories vector
		 * @return a string name of the directory at the given location
		 * */
		string getDirectory(int i) const {
			// TODO: handle invalid index
			return directories[i];
		}

		// TODO: comment out -- used for testing
		// vector<string> get() { return directories; }
};

// TODO: comment out -- used for testing
// int main() {
// 	Path newPath = Path();
	
// 	///////////////////////////////////////////
// 	vector<string> result = newPath.get();
// 	for (int i = 0; i < result.size(); i++) {
// 		cout << result[i] << "\n";
// 	}

// 	cout << "\npath of IntelliJ: " << newPath.getDirectory(newPath.find("IntelliJ")) << "\n";
// 	///////////////////////////////////////////

// 	return 0;
// };