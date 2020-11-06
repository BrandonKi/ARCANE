#include <string>
#include <vector>

#include "Compiler.h"

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
    compiler.compile(compiler.filedata());
}

/**
 * @brief parses command line arguments and updates the global args struct accordingly
 * 
 * @param argc number of args
 * @param argv array of args
 */
void parseArgs(int argc, char* argv[]){         //TODO implement the rest of the availible flags
    std::vector<std::string> argList(argv, argv + argc);    //TODO delete this temporary parser and write a good cmd arg parser
    for(std::string& str : argList){
        if(str == "--lex-out")
            args.LexOut = true;
        else if(str == "--lex-out-v"){
            args.LexOut = true;
            args.verboseLexOut = true;
        }
        else if(str == "-O")
            args.optimize = true;
        //if(str == "-out")
    }
            args.output_filepath = "out.exe";
    args.filepath = argList[1];
}

