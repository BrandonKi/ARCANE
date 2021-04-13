#include <string>
#include <vector>

#include "Compiler.h"

void parseArgs(int, char* argv[]);

int main(int argc, char* argv[]) {
    PROFILE();

    init_pLog();        // init pLog lib

    parseArgs(argc, argv);

    Compiler compiler;
    auto code = compiler.compile();

    std::ofstream bin("test.arcb", std::ios::out | std::ios::binary);
    bin.write(reinterpret_cast<char*> (code.data()), code.size());
    bin.close();

    Arcvm VM;
    if(VM.load_program(reinterpret_cast<char*>(code.data()), code.size()) == false)
        return -1;
    std::cout << "Exited with code: " << VM.run();
}

void parseArgs(int argc, char* argv[]) {         //TODO implement the rest of the available flags
    PROFILE();
    std::vector<astring> argList(argv, argv + argc);    //TODO delete this temporary parser and write a good cmd arg parser
    for(astring& str : argList){
        if(str == "--lex-out")
            args.lex_out = true;
        else if(str == "--lex-out-v"){
            args.lex_out = true;
            args.verbose_lex_out = true;
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
