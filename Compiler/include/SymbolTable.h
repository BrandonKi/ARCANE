#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "BytecodeGen.h"

enum SymbolType {
    NONE,
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
        void add_symbol(const astring&, const SymbolType, const Type);
        void add_function(const astring&, const std::vector<Type, arena_allocator<Type>>, const SymbolType, const Type); 
        bool has(const astring&);
        bool scope_has(const astring&);
        SymbolType get_kind(const astring&);
        bool is_function(const astring&);
        bool is_variable(const astring&);
        void push_scope();
        void pop_scope();

    private:

        hash_map global_;
        std::vector<hash_map, arena_allocator<hash_map>> table_;
        
        pair get(astring&);
        hash_map& current_scope();
};

#endif