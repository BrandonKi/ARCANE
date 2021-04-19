#include <string>
#include <vector>

#include "Compiler.h"

void parse_args(const int, const char* argv[]);

int main(const int argc, const char* argv[]) {
    PROFILE();

    init_pLog();        // init pLog lib

    parse_args(argc, argv);

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

void parse_args(const int argc, const char* argv[]) {         //TODO implement the rest of the available flags
    PROFILE();
    std::vector<astring> arg_list(argv, argv + argc);    //TODO delete this temporary parser and write a good cmd arg parser
    for(const auto& str : arg_list){
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
    args.path = arg_list.back();
}
