// type table
// essentially just an array of strings that represent each type
// example
//  [0] = "u8"
//  [1] = "u16"
//  ....
//
//  this way new types can be added easily and type aliases are trivial
//
// or alternatively we can hold more information about each type
// Type {
//      std::string name;
//      u64 size;
//      // TODO add aggregate support
//  }
//
//
//  pick one....
//  add_alias("byte", "u8")
//  add_alias("byte", handle1)

//  add_type("u8")
//  handle1 = get_type_handle("u8")
//  add_type("u16")
//  handle2 = get_type_handle("u16")
//  add_alias("byte", "u8")
//  Function* fn = ...
//  define_operator(ARC_ADD, handle1, handle2, fn)
//
//
#include "ErrorHandler.h"

using type_handle = i32;

struct Type {
    std::string name;
    u64 size;
    // TODO add aggregate support
};

enum PrimitiveTypeHandles : int {
    TYPE_unknown,
    TYPE_type,
    TYPE_i8,
    TYPE_i16,
    TYPE_i32,
    TYPE_i64,
    TYPE_u8,
    TYPE_u16,
    TYPE_u32,
    TYPE_u64,
    TYPE_f32,
    TYPE_f64,
};

struct Function;

class TypeManager {


public:
    TypeManager();

    type_handle add_type(const std::string&);
    type_handle get_type_handle(const std::string&);
    Type get_type(type_handle);


    void add_alias(const std::string&, const std::string&);

    // TODO operator functions
    void define_operator(TokenKind, type_handle, type_handle, Function*);
    bool operator_exists(TokenKind, type_handle, type_handle);
    Function* get_operator_impl(TokenKind, type_handle, type_handle);
    type_handle get_operator_result_type(TokenKind, type_handle, type_handle);

    // TODO conversion functions
    void define_conversion(type_handle, type_handle, Function*);
    bool conversion_exists(type_handle, type_handle);
    Function* get_conversion_impl(type_handle, type_handle);
    type_handle get_conversion_result_type(type_handle, type_handle);

    arcvm::Type to_ir_type(type_handle);

private:

    bool is_primitive(type_handle);
    bool is_float(type_handle);
    bool is_int(type_handle);

    // the following has been simplified and now just needs to be implemented
    // now primitive types are builtin instead of being provided the same way as non-primitive types
    // because of this 90% of the complexity is gone :)

    // initialized with language primitives
    std::vector<Type> type_table = {
        {"__UNKNOWN__", 0},
        {"i8", 1},
        {"i16", 2},
        {"i32", 4},
        {"i64", 8},
        {"u8", 1},
        {"u16", 2},
        {"u32", 4},
        {"u64", 8},
        {"f32", 4},
        {"f64", 8},
    };

    struct Operator {
        TokenKind op;
        type_handle rhs;
        Function* operator_impl = nullptr;
    };

    std::vector<std::vector<Operator>> operator_table = {};

    struct Conversion {
        type_handle target_type;
        Function* conversion_impl = nullptr;
    };

    std::vector<std::vector<Conversion>> conversion_table = {};

};