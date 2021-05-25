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

SymbolTable::SymbolTable():
    global_(), table_()
{
    push_scope();
}

void SymbolTable::add_symbol(const astring& id, const SymbolType kind, const type_handle arc_type) {
    current_scope().insert(pair(id, Symbol{kind, arc_type}));
}

void SymbolTable::add_function(const astring& id, const std::vector<type_handle, arena_allocator<type_handle>> fn_args, const SymbolType kind, const type_handle arc_type) {
    current_scope().insert(pair(id, Symbol{kind, arc_type, fn_args}));
}

bool SymbolTable::has(const astring& id) {
    return std::ranges::any_of(table_.cbegin(), table_.cend(), 
        [&](const auto& map) {return map.contains(id);}
    );
}

// return NONE if element does not exist
SymbolType SymbolTable::get_kind(const astring& id) {
    for(const auto& map : table_) {
        auto result = map.find(id);
        if(result != map.end())
            return result->second.type;
    }
    return NONE;
}

bool SymbolTable::scope_has(const astring& id) {
    return current_scope().contains(id);
}

bool SymbolTable::is_function(const astring& id) {
    return get_kind(id) == FUNCTION;
}

bool SymbolTable::is_variable(const astring& id) {
    return get_kind(id) == VARIABLE;
}

hash_map& SymbolTable::current_scope() {
    return table_.back();
}

void SymbolTable::push_scope() {
    table_.emplace_back(hash_map{});
}

void SymbolTable::pop_scope() {
    table_.pop_back();
}