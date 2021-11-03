// keeping this around as a reference for now
/*
//TODO remove me in the future

#include "BytecodeLinker.h"

extern ErrorHandler error_log;
extern TypeManager type_manager;

using vm = Arcvm;

BytecodeLinker::BytecodeLinker() {
    PROFILE();
}

// FIXME honestly this whole thing is a mess
// TODO rewrite the whole linker and possibly move it into it's own project
code_block BytecodeLinker::link_file(std::vector<linkable_function>& functions) {
    PROFILE();
    update_function_table(functions);
    auto code = code_block{};
    for(auto& function : functions) {
        for(u32 i = 0; i < function.code.size(); ++i) {
            // TODO this is_letter check has a high chance of breaking
            if(function.code[i] == vm::call_short && is_letter(reinterpret<char>(function.code[i+1]))) {
                char *raw_fn_name = reinterpret_cast<char*>(&(function.code.data()[i+1]));
                auto fn_name = std::string(raw_fn_name);
                auto a = function_table_.at(fn_name);

                // only one character function names work at the moment
                // this is because the address of every function changes if I have extra characters in the bytecode

                function.code[i+1] = static_cast<u8>(function.args.size());       // number of parameters
                function.code[i+2] = static_cast<u8>(a - 3);     // address
            }
            else if(function.code[i] == vm::push_string) {
                // move past the string literal part
                // in case someone put the vm::call_short byte in their string
            }
        }
    }
    // FIXME temporary
    return code;
}

void BytecodeLinker::update_function_table(std::vector<linkable_function>& functions) {
    PROFILE();
    u64 location = 0;
    for(auto& function : functions) {
        function_table_.insert_or_assign(*function.name, location);
        location += function.code.size();
    }
}
*/