#include "TypeManager.h"

extern ErrorHandler error_log;
TypeManager type_manager;

// forward declare Function
struct Function;

TypeManager::TypeManager() {
    PROFILE();
}

type_handle TypeManager::add_type(const astring& name) {
    PROFILE();
    // TODO actually support adding more types
    // this is just placeholder
    type_table.push_back({name, 8});
    return static_cast<type_handle>(type_table.size() - 1);
}

type_handle TypeManager::get_type_handle(const astring& name) {
    PROFILE();
    for(auto i = 0; i < type_table.size(); ++i) //FIXME if this is a common operation then consider a hash table
        if(type_table[i].name == name)
            return i;
    return -1;
}

Type TypeManager::get_type(const type_handle handle) {
    PROFILE();
    return type_table[handle];
}

void TypeManager::add_alias(const astring& alias, const astring& original) {
    PROFILE();
}

void TypeManager::define_operator(TokenKind op, type_handle lhs, type_handle rhs, Function* impl) {
    PROFILE();
    // the Function type needs to be forward declared
    // otherwise we run into the circular dependency mess :(
}

bool TypeManager::operator_exists(TokenKind op, type_handle lhs, type_handle rhs) {
    PROFILE();
    auto sub_table = operator_table[lhs];
    auto result = std::find_if(sub_table.cbegin(), sub_table.cend(), [=](const auto& element) {
        return element.op == op && element.rhs == rhs;
    });
    if(result == sub_table.end())
        return false;
    return true;
}

Function* TypeManager::get_operator_impl(TokenKind op, type_handle lhs, type_handle rhs) {
    PROFILE();
    return nullptr;    // TODO actually implement this
}

type_handle TypeManager::get_operator_result_type(TokenKind op, type_handle lhs, type_handle rhs) {
    PROFILE();
    return TYPE_I64;    // TODO actually implement this
}

void TypeManager::define_conversion(type_handle, type_handle, Function*) {
    PROFILE();
}

bool TypeManager::conversion_exists(type_handle lhs, type_handle rhs) {
    PROFILE();
    auto sub_table = conversion_table[lhs];
    auto result = std::find_if(sub_table.cbegin(), sub_table.cend(), [=](const auto& element) {
        return element.target_type == rhs;
    });
    if(result == sub_table.end())
        return false;
    return true;
}

Function* get_conversion_impl(type_handle, type_handle) {
    return nullptr;
}

type_handle TypeManager::get_conversion_result_type(type_handle, type_handle) {
    PROFILE();
    return TYPE_I64;
}