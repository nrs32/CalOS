#include <iostream>
#include <vector>
using namespace std;

/**
 * Class to be used for general actions
 * 
 * @author Nikita Sietsema
 * 4 March 2020
 */
class Utils {
	public:
		Utils() { };
		vector<string> split(char* str, char* delimeter);
};