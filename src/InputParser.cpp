
#include "InputParser.hpp"

namespace InputParser {

string toLowerCase(string str) {
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

map<string, string> parseArguments(stringstream& ss) {
    map<string, string> args;
    string key, value;
    while (ss >> key) {
        char nextChar = ss.peek();
        while (nextChar == ' ') {
            ss.get();
            nextChar = ss.peek();
        }

        if (nextChar == '"') {
            ss.get();
            getline(ss, value, '"');
        } else {
            ss >> value;
        }
        args[key] = value;
    }
    return args;
}

vector<string> splitString(const string& str, char delimiter) {
    vector<string> tokens;
    string token;
    stringstream ss(str);
    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

}
