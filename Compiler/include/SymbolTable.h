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
    type_handle result_type;
    std::vector<type_handle, arena_allocator<type_handle>> args;
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
        void add_symbol(const astring&, const SymbolType, const type_handle);
        void add_function(const astring&, const std::vector<type_handle, arena_allocator<type_handle>>, const SymbolType, const type_handle); 
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