#include <iostream>
#include <vector>
using namespace std;

/**
 * Class representing directories to search for executables. 
 * 
 * @author Nikita Sietsema
 * 3 March 2020
 **/
class Path {
	vector<string> directories;

	public:
		Path();

		int find(const string& program) const;
		string getDirectory(int i) const;
};