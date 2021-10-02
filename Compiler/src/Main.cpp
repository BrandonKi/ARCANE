#include <string>
#include <vector>

#include "Compiler.h"

#include <Arcvm.h>

void parse_args(const int, const char* argv[]);

int main(const int argc, const char* argv[]) {
    PROFILE();
    parse_args(argc, argv);

    Compiler compiler;
    auto vm = compiler.compile();

    std::cout << "Exited with code: " << vm.run() << '\n';
}

void parse_args(const int argc, const char* argv[]) {         //TODO implement the rest of the available flags
    PROFILE();
    std::vector<std::string_view> arg_list(argv, argv + argc);    //TODO delete this temporary parser and write a good cmd arg parser
    for(size_t i = 0; i < arg_list.size(); ++i) {
        const auto& str = arg_list[i];
        if(str == "--lex-out")
            args.lex_out = true;
        else if(str == "--lex-out-v") {
            args.lex_out = true;
            args.verbose_lex_out = true;
        }
        else if(str == "-O")
            args.optimize = true;
        else if(str == "-p")
            args.project = true;
        else if(str == "-out") {
            if(i + 1 >= arg_list.size()) {
                // TODO invalid command line arguments
            }
            args.output_path = arg_list[++i];
        }
    }
    if(args.output_path.empty())
        args.output_path = "out.arcb";
    args.path = arg_list.back();
}
