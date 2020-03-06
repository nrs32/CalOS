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
	void exitOnError(string message) const;

	public:
		Path();

		int find(const string& program) const;
		string getDirectory(int i) const;

		// TODO: comment out / delete -- used for testing
		vector<string> get() { return directories; }
};