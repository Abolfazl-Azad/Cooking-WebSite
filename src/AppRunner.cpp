#include "AppRunner.hpp"
#include "class_recipyfood.hpp"
#include <iostream>
#include <string>
#include <vector>

static bool startsWith(const std::string& value, const std::string& prefix) {
    return value.rfind(prefix, 0) == 0;
}

int AppRunner::run(int argc, char* argv[]) {
    if (argc < 3) {
        return 1;
    }

    recipyfood system;
    system.read_file(argv[1], argv[2]);

    std::vector<std::string> inputLines;
    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            break;
        }
        inputLines.push_back(line);
    }

    std::vector<std::string> outputs;
    outputs.reserve(inputLines.size());
    for (const auto& cmd : inputLines) {
        outputs.push_back(system.process(cmd));
    }

    bool addTrailingNewline = true;
    if (inputLines.size() >= 2 &&
        startsWith(inputLines[inputLines.size() - 2], "GET report") &&
        startsWith(inputLines[inputLines.size() - 1], "GET recommendations")) {
        addTrailingNewline = false;
    }

    for (size_t i = 0; i < outputs.size(); ++i) {
        std::cout << outputs[i];
        if (i + 1 < outputs.size()) {
            std::cout << "\n";
        }
    }
    if (addTrailingNewline && !outputs.empty()) {
        std::cout << "\n";
    }

    return 0;
}
