#include "ByteCodeGen.h"

enum SymbolType {
    VARIABLE,
    FUNCTION
};

struct Symbol {
    SymbolType type; 
    Type result_type;
    std::vector<Type, arena_allocator<Type>> args;
};



class SymbolTable {
    public:

        using pair = std::pair<astring, Symbol>;
        using hash_map =
            std::unordered_map<
                astring,
                Symbol,
                std::hash<astring>,
                std::equal_to<astring>,
                arena_allocator<
                    std::pair<
                        const astring,
                        Symbol
                    >
                >
            >;
        SymbolTable();
        void add_symbol(astring&, SymbolType, Type);
        void add_function(astring&, std::vector<Type, arena_allocator<Type>>, SymbolType, Type); 
        bool has(astring&);
        bool scope_has(astring&);
        SymbolType get_kind(astring&);
        bool is_function(astring&);
        bool is_variable(astring&);
        void push_scope();
        void pop_scope();

    private:

        hash_map global;
        std::vector<hash_map, arena_allocator<hash_map>> table;
                
        pair get(astring&);
        hash_map& current_scope();

};