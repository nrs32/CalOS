/**
 * Class dealing with the command line argument 
 * 
 * @author Sebrina Zeleke 
 * 3 March 2020
 */


#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
using namespace std;

#ifndef COMMANDLINE_H_
#define COMMANDLINE_H_

class CommandLine {

public:
    CommandLine(istream& in);
    ~CommandLine();
    char* getCommand() const;
    int getArgCount() const;
    char** getArgVector() const;
    char* getArgVector(int i) const;
    bool noAmpersand() const;

private:
    char** argv;
    int argc;
    int size;
    bool hasAmpersand = false;
    char* tempStorage; // Use this to allocate memory for argv and deallocate it
    char** split(char* str, char* delimeter);

};

#endif /* COMMANDLINE_H_ */

