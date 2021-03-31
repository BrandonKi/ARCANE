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
    pushScope();
}

void SymbolTable::addSymbol(astring& id, SymbolType kind, Type arc_type) {
    currentScope().insert(pair(id, Symbol{kind, arc_type}));
}

void SymbolTable::addFunction(astring& id, std::vector<Type, arena_allocator<Type>> args, SymbolType kind, Type arc_type) {
    currentScope().insert(pair(id, Symbol{kind, arc_type, args}));
}


bool SymbolTable::has(astring& id) {
    for(hash_map& map : table)
        if(map.contains(id))
            return true;
    return false;
}

// check if element exists before calling function
SymbolType SymbolTable::getKind(astring& id) {
    for(hash_map& map : table) {
        auto result = map.find(id);
        if(result != map.end())
            return result->second.type;
    }
    return VARIABLE;    // never get here
}

bool SymbolTable::scopeHas(astring& id) {
    return currentScope().contains(id);
}

bool SymbolTable::isFunction(astring& id) {
    // FIXME this searches for the symbol twice
    return has(id) && getKind(id) == FUNCTION;
}

bool SymbolTable::isVariable(astring& id) {
    // FIXME this searches for the symbol twice
    return has(id) && getKind(id) == VARIABLE;
}

hash_map& SymbolTable::currentScope() {
    return table.back();
}

void SymbolTable::pushScope() {
    table.push_back(hash_map{});
}

void SymbolTable::popScope() {
    table.pop_back();
}