#include "TypeManager.h"

extern ErrorHandler error_log;
TypeManager type_manager;

// forward declare Function
struct Function;

TypeManager::TypeManager() {
    PROFILE();
}

type_handle TypeManager::add_type(const std::string& name) {
    PROFILE();
    // TODO actually support adding more types
    // this is just placeholder
    type_table.push_back({name, 8});
    return static_cast<type_handle>(type_table.size() - 1);
}

type_handle TypeManager::get_type_handle(const std::string& name) {
    PROFILE();
    for(size_t i = 0; i < type_table.size(); ++i) //FIXME if this is a common operation then consider a hash table
        if(type_table[i].name == name)
            return static_cast<type_handle>(i);
    return -1;
}

Type TypeManager::get_type(const type_handle handle) {
    PROFILE();
    return type_table[static_cast<size_t>(handle)];
}

void TypeManager::add_alias(const std::string& alias, const std::string& original) {
    PROFILE();
    static_cast<void>(alias);
    static_cast<void>(original);
}

void TypeManager::define_operator(TokenKind op, type_handle lhs, type_handle rhs, Function* impl) {
    PROFILE();
    static_cast<void>(op);
    static_cast<void>(lhs);
    static_cast<void>(rhs);
    static_cast<void>(impl);
    // the Function type needs to be forward declared
    // otherwise we run into the circular dependency mess :(
}

bool TypeManager::operator_exists(TokenKind op, type_handle lhs, type_handle rhs) {
    PROFILE();
    auto sub_table = operator_table[static_cast<size_t>(lhs)];
    auto result = std::find_if(sub_table.cbegin(), sub_table.cend(), [=](const auto& element) {
        return element.op == op && element.rhs == rhs;
    });
    if(result == sub_table.end())
        return false;
    return true;
}

Function* TypeManager::get_operator_impl(TokenKind op, type_handle lhs, type_handle rhs) {
    PROFILE();
    static_cast<void>(op);
    static_cast<void>(lhs);
    static_cast<void>(rhs);
    return nullptr;    // TODO actually implement this
}

type_handle TypeManager::get_operator_result_type(TokenKind op, type_handle lhs, type_handle rhs) {
    PROFILE();
    static_cast<void>(op);
    static_cast<void>(lhs);
    static_cast<void>(rhs);
    return TYPE_I64;    // TODO actually implement this
}

void TypeManager::define_conversion(type_handle, type_handle, Function*) {
    PROFILE();
}

bool TypeManager::conversion_exists(type_handle lhs, type_handle rhs) {
    PROFILE();
    auto sub_table = conversion_table[static_cast<size_t>(lhs)];
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


arcvm::Type TypeManager::to_ir_type(type_handle type) {
    switch(type) {
        case TYPE_UNKNOWN:
            return arcvm::Type::none;
        case TYPE_I8:
            return arcvm::Type::ir_i8;
        case TYPE_I16:
            return arcvm::Type::ir_i16;
        case TYPE_I32:
            return arcvm::Type::ir_i32;
        case TYPE_I64:
            return arcvm::Type::ir_i64;
        case TYPE_U8:
            return arcvm::Type::ir_u8;
        case TYPE_U16:
            return arcvm::Type::ir_u16;
        case TYPE_U32:
            return arcvm::Type::ir_u32;
        case TYPE_U64:
            return arcvm::Type::ir_u64;
        //case TYPE_BOOL:
            //return arcvm::Type::ir_b1;
        //case TYPE_BOOL:
            //return arcvm::Type::ir_b8;
        // TODO returning aggregate types etc. etc.
        default:
            // FIXME this is just for debugging
            return static_cast<arcvm::Type>(-1);
    }
}