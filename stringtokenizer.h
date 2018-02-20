#ifndef STRINGTOKENIZER_H
#define STRINGTOKENIZER_H

#include <cstring>  // header for dealing with C-style strings
#include <string>   // header for the std::string class, with related functions and operators.
#include <vector>
using namespace std;


/********************************************************************************
class to get tokens from a string with separators

Usage
-----

std::string myString  = "one/two/three/four/five/six/seven/eight/nine/ten";
std::string delimiter = "/";
StringTokenizer strTokenizer(myString, delimiter);
vector<std::string> strArray = strTokenizer.GetTokens();

********************************************************************************/

#include "CCommons_dfdllh1.h"

class StringTokenizer
{
    vector<std::string> m_strArray;

public:
    StringTokenizer(char* i_input, const char* i_delimiters)
    {
        Execute(i_input, i_delimiters);
    }

    StringTokenizer(std::string i_input, std::string i_delimiters)
    {

        // i_input -> std::string to char *
        char * input = new char[i_input.length()+1];
        std::strcpy(input, i_input.c_str());

        // i_delimiters -> std::string to char *
        char * delimiters = new char[i_delimiters.length()+1];
        std::strcpy(delimiters, i_delimiters.c_str());

        Execute(input, delimiters);
    }

    vector<std::string>& GetTokens()
    {
        return m_strArray;
    }

private:

    void Execute(char* i_input, const char* i_delimiters)
    {
        for(char* token = strtok(i_input, i_delimiters);
            token != NULL; token = strtok(NULL, i_delimiters))
        {
            std::string	tok = std::string(token);
            m_strArray.push_back(tok);
        }
    }
};

#include "CCommons_dfdllh2.h"

#endif // STRINGTOKENIZER_H
