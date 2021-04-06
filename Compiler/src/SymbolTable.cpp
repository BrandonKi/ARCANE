#include "SymbolTable.h"

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

SymbolTable::SymbolTable() {
    push_scope();
}

void SymbolTable::add_symbol(astring& id, SymbolType kind, Type arc_type) {
    current_scope().insert(pair(id, Symbol{kind, arc_type}));
}

void SymbolTable::add_function(astring& id, std::vector<Type, arena_allocator<Type>> args, SymbolType kind, Type arc_type) {
    current_scope().insert(pair(id, Symbol{kind, arc_type, args}));
}


bool SymbolTable::has(astring& id) {
    for(hash_map& map : table)
        if(map.contains(id))
            return true;
    return false;
}

// check if element exists before calling function
SymbolType SymbolTable::get_kind(astring& id) {
    for(hash_map& map : table) {
        auto result = map.find(id);
        if(result != map.end())
            return result->second.type;
    }
    return VARIABLE;    // never get here
}

bool SymbolTable::scope_has(astring& id) {
    return current_scope().contains(id);
}

bool SymbolTable::is_function(astring& id) {
    // FIXME this searches for the symbol twice
    return has(id) && get_kind(id) == FUNCTION;
}

bool SymbolTable::is_variable(astring& id) {
    // FIXME this searches for the symbol twice
    return has(id) && get_kind(id) == VARIABLE;
}

hash_map& SymbolTable::current_scope() {
    return table.back();
}

void SymbolTable::push_scope() {
    table.push_back(hash_map{});
}

void SymbolTable::pop_scope() {
    table.pop_back();
}