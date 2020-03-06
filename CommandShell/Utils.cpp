/**
 * Class to be used for general actions
 * 
 * @author Nikita Sietsema
 * 4 March 2020
 */
#include "Utils.h"

#include <iostream>
#include <vector>
#include <unistd.h> // For Linux
#include <cstdlib>  // For access to getenv
#include <string.h> // For strtok
using namespace std;

/**
 * Split a char* into tokens using a delimeter
 * @param str is a char* to be split into vector<string>
 * @param delimeter is the char* to split the string at
 * @return a vector containing the string tokens from str 
 * */
vector<string> Utils::split(char* str, char* delimeter) {
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
};