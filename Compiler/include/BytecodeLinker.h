#include "TypeInference.h"

// include to get access to instructions
#include "Arcvm.h"

using code_block = std::vector<u8, arena_allocator<u8>>;

struct linkable_function {
    const astring* name;
    std::vector<Arg, arena_allocator<Arg>> args;
    code_block code;
};

struct linkable_file {
    const astring* name;
    code_block code;
};

class BytecodeLinker final {
    public:
        BytecodeLinker();
        code_block link(std::vector<linkable_file, arena_allocator<linkable_file>>&);
        code_block link_file(std::vector<linkable_function, arena_allocator<linkable_function>>&);
        void update_function_table(std::vector<linkable_function, arena_allocator<linkable_function>>&);

    private:
        std::unordered_map<astring, u64> function_table_;
};