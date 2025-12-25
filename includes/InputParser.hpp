// InputParser.hpp
#ifndef INPUT_PARSER_HPP
#define INPUT_PARSER_HPP

#include "global.hpp"

class InputParser {
public:
    static map<string, string> parseArguments(stringstream& ss);
    static vector<string> splitString(const string& str, char delimiter);
    static string toLowerCase(string str);
};

#endif