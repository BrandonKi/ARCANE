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
    // TODO implement this
    assert(false);
}

void TypeManager::define_operator(TokenKind op, type_handle lhs, type_handle rhs, Function* impl) {
    PROFILE();
    // TODO
    assert(false);
}

bool TypeManager::operator_exists(TokenKind op, type_handle lhs, type_handle rhs) {
    PROFILE();
    if(is_primitive(lhs) && is_primitive(rhs))
        return true;
    assert(false);
    return false;
}

Function* TypeManager::get_operator_impl(TokenKind op, type_handle lhs, type_handle rhs) {
    PROFILE();
    assert(false);
    return nullptr;    // TODO implement this
}

type_handle TypeManager::get_operator_result_type(TokenKind op, type_handle lhs, type_handle rhs) {
    PROFILE();
    if(is_float(lhs) || is_float(rhs)) {
        return std::max(lhs, rhs);
    }
    else if(is_int(lhs) && is_int(rhs)) {
        return std::max(lhs, rhs);
    }
    // TODO handle unsigned stuff and bool too I guess
    assert(false);
    return TYPE_i64;
}

void TypeManager::define_conversion(type_handle, type_handle, Function*) {
    PROFILE();
    assert(false);
}

bool TypeManager::conversion_exists(type_handle src, type_handle result) {
    PROFILE();
    if(is_primitive(src) && is_primitive(result))
        return true;
    assert(false);
    return false;
}

Function* TypeManager::get_conversion_impl(type_handle, type_handle) {
    assert(false);
    return nullptr;
}

type_handle TypeManager::get_conversion_result_type(type_handle src, type_handle result) {
    PROFILE();
    if(!is_primitive(src) || !is_primitive(result))
        assert(false);
    else {
        return result;
    }
}

arcvm::Type TypeManager::to_ir_type(type_handle type) {
    switch(type) {
        case TYPE_unknown:
            return arcvm::Type::none;
        case TYPE_type:
            assert(false);
            return arcvm::Type::none;
        case TYPE_i8:
            return arcvm::Type::ir_i8;
        case TYPE_i16:
            return arcvm::Type::ir_i16;
        case TYPE_i32:
            return arcvm::Type::ir_i32;
        case TYPE_i64:
            return arcvm::Type::ir_i64;
        case TYPE_u8:
            return arcvm::Type::ir_u8;
        case TYPE_u16:
            return arcvm::Type::ir_u16;
        case TYPE_u32:
            return arcvm::Type::ir_u32;
        case TYPE_u64:
            return arcvm::Type::ir_u64;
        case TYPE_f32:
            assert(false);
            return arcvm::Type::none;
        case TYPE_f64:
            assert(false);
            return arcvm::Type::none;

        //case TYPE_BOOL:
            //return arcvm::Type::ir_b1;
        //case TYPE_BOOL:
            //return arcvm::Type::ir_b8;
        // TODO returning aggregate types etc. etc.
        default:
            assert(false); // unable to convert to ir type
            return arcvm::Type::none; // just to disable the warning
    }
}

bool TypeManager::is_primitive(type_handle type) {
    if(type > TYPE_unknown && type <= TYPE_f64)
        return true;
    return false;
}

bool TypeManager::is_float(type_handle type) {
    if(type == TYPE_f32 || type == TYPE_f64)
        return true;
    return false;
}

bool TypeManager::is_int(type_handle type) {
    if(type >= TYPE_i8 && type <= TYPE_i64)
        return true;
    return false;
}