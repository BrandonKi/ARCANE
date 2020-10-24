#include <string>
#include <vector>

#include "Compiler.h"

using namespace pLog;

void parseArgs(int, char* argv[]);

/**
 * @brief The main function and entry point of the compiler front-end
 * parses cmd args and calls compile()
 * 
 * @return [int] exit code
 */
int main(int argc, char* argv[]){

    init_pLog();        // init pLog lib

    parseArgs(argc, argv);

    Compiler compiler;
    compiler.compile();
}

/**
 * @brief parses command line arguments and updates the global args struct accordingly
 * 
 * @param argc number of args
 * @param argv array of args
 */
void parseArgs(int argc, char* argv[]){         //TODO implement the rest of the availible flags
    std::vector<std::string> argList(argv, argv + argc);
    args.filepath = argList[1];
}

