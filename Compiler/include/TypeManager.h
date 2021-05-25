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
//      astring name;
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
    astring name;
    u64 size;
    // TODO add aggregate support
};

enum PrimitiveTypeHandles : int {
    TYPE_UNKNOWN,
    TYPE_I8,
    TYPE_I16,
    TYPE_I32,
    TYPE_I64,
    TYPE_U8,
    TYPE_U16,
    TYPE_U32,
    TYPE_U64,
    TYPE_F32,
    TYPE_F64,
};

class TypeManager {

public:
    TypeManager();

    type_handle add_type(const astring&);
    type_handle get_type_handle(const astring&);
    Type get_type(type_handle);

    void add_alias(const astring&, const astring&);

    void define_operator();

private:
    // initialized with language primitives
    std::vector<Type, arena_allocator<Type>> type_table = {
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

};