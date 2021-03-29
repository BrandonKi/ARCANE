#include <string>
#include <vector>

#include "Compiler.h"

void parseArgs(int, char* argv[]);

int main(int argc, char* argv[]) {
    PROFILE();

    init_pLog();        // init pLog lib

    parseArgs(argc, argv);

    Compiler compiler;
    compiler.compile();
}

void parseArgs(int argc, char* argv[]) {         //TODO implement the rest of the availible flags
    PROFILE();
    std::vector<astring> argList(argv, argv + argc);    //TODO delete this temporary parser and write a good cmd arg parser
    for(astring& str : argList){
        if(str == "--lex-out")
            args.lexOut = true;
        else if(str == "--lex-out-v"){
            args.lexOut = true;
            args.verboseLexOut = true;
        }
        else if(str == "-O")
            args.optimize = true;
        else if(str == "-p")
            args.project = true;
        //if(str == "-out")
    }
    args.output_path = "out.exe";
    args.path = argList.back();
}
