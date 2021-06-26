#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "AST.h"

enum SymbolType {
    NONE,
    VARIABLE,
    FUNCTION
};

struct Symbol {
    SymbolType type; 
    type_handle result_type;
    std::vector<Arg, arena_allocator<Arg>> args;
};



class SymbolTable {
    public:

        using pair = std::pair<std::string, Symbol>;
        using hash_map =
            std::unordered_map<
                std::string,
                Symbol,
                std::hash<std::string>,
                std::equal_to<std::string>,
                arena_allocator<
                    std::pair<
                        const std::string,
                        Symbol
                    >
                >
            >;
        
        SymbolTable();
        void add_symbol(const std::string, const SymbolType, const type_handle);
        void add_function(const std::string, const std::vector<Arg, arena_allocator<Arg>>, const SymbolType, const type_handle); 
        bool has(const std::string&);
        bool scope_has(const std::string&);
        SymbolType get_kind(const std::string&);
        type_handle get_type(const std::string&);
        u32 get_num_args(const std::string&);
        bool is_function(const std::string&);
        bool is_variable(const std::string&);
        void push_scope();
        void pop_scope();

    private:

        hash_map global_;
        std::vector<hash_map, arena_allocator<hash_map>> table_;
        
        pair get(std::string&);
        hash_map& current_scope();
};

#endif