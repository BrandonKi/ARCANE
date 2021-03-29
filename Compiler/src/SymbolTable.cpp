#include "SymbolTable.h"

void SymbolTable::addSymbol(astring& id, SymbolType type, Type arc_type) {
    table.insert(pair(id, Symbol{type, arc_type}));
}

bool SymbolTable::has(astring& id) {
    return table.contains(id);
}

bool SymbolTable::isFunction(astring& id) {
    return table.contains(id) && table[id].type == FUNCTION;
}

bool SymbolTable::isVariable(astring& id) {
    return table.contains(id) && table[id].type == VARIABLE;
}