#ifndef INPUT_PARSER_HPP
#define INPUT_PARSER_HPP

#include "global.hpp"

namespace InputParser {
map<string, string> parseArguments(stringstream& ss);
vector<string> splitString(const string& str, char delimiter);
string toLowerCase(string str);
}

#endif
