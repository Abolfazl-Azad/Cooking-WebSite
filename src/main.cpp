#include "class_recipyfood.hpp"
#include <iostream>

using namespace std;

string getInput() {
    string line;
    if (!getline(cin, line)) return "";
    return line;
}

void printOutput(const string& result) {
    cout << result << endl;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        return 1;
    }
    recipyfood system;
    system.read_file(argv[1], argv[2]);
    while (true) {
        string line = getInput();
        if (line == "") break;
        
        string result = system.process(line);
        printOutput(result);
    }


    return 0;
}