#include "ProgramRunner.hpp"

#include "AppRunner.hpp"
#include "WebRunner.hpp"

int ProgramRunner::run(int argc, char* argv[]) {
    if (argc >= 4) {
        WebRunner runner;
        return runner.run(argc, argv);
    }
    if (argc >= 3) {
        AppRunner runner;
        return runner.run(argc, argv);
    }
    return 1;
}
