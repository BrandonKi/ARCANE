// keeping this around as a reference for now
/*
//TODO remove me in the future

#include "TypeInference.h"

// include to get access to instructions
#include "Arcvm.h"

using code_block = std::vector<u8>;

struct linkable_function {
    const std::string* name;
    std::vector<Arg> args;
    code_block code;
};

struct linkable_file {
    const std::string* name;
    code_block code;
};

class BytecodeLinker final {
    public:
        BytecodeLinker();
        code_block link(std::vector<linkable_file>&);
        code_block link_file(std::vector<linkable_function>&);
        void update_function_table(std::vector<linkable_function>&);

    private:
        std::unordered_map<std::string, u64> function_table_;
};
*/