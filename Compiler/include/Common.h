#include <string>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>

#include <pLog.h>

typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

inline struct ARGS{
    std::string filepath;
    std::string output_filepath; 
    bool optimize;
}args;

enum TokenKind {    // It's named TokenKind instead of TokenType because windows.h stole TokenType from me :(
    /* TODO fill this out */

    NONE,

    /* Types */

    ARC_TRUE,
    ARC_FALSE,
    ARC_INT_LIT,
    ARC_FLOAT_LIT,

    ARC_I8,
    ARC_I16,
    ARC_I32,
    ARC_I64,

    ARC_U8,
    ARC_U16,
    ARC_U32,
    ARC_U64,

    ARC_F32,
    ARC_F64,

    ARC_STRUCT,
    ARC_STR,
    ARC_ARR,

    /* Keywords */

    ARC_FN,
    ARC_RET,
    ARC_IF,
    ARC_ELIF,
    ARC_ELSE,
    ARC_WHILE,
    ARC_FOR,
    ARC_DO,
    ARC_SWITCH,
    ARC_BREAK,
    ARC_CASE,
    ARC_DEFAULT,


};

struct info{
    std::string val;
};

struct Token{
    TokenKind kind;
    u32 srcLine;
    u32 srcChar;
    struct info;
};

std::unordered_map<std::string, TokenKind> keywords( { 

    {"i8", ARC_I8}, {"i16", ARC_I16}, {"i32", ARC_I32}, {"i64", ARC_I64}, 
    {"u8", ARC_U8}, {"u16", ARC_U16}, {"u32", ARC_U32}, {"u64", ARC_U64}, 
    {"f32", ARC_F32}, {"f64", ARC_F64}, {"str", ARC_STR}, {"arr", ARC_ARR}, 
    {"struct", ARC_STRUCT}, {"char", ARC_I8}, {"int", ARC_I32}, {"float", ARC_F32},

    {"fn", ARC_FN}, {"ret", ARC_RET}, {"if", ARC_IF}, {"elif", ARC_ELIF},
    {"else", ARC_ELSE}, {"while", ARC_WHILE}, {"for", ARC_FOR}, {"do", ARC_DO},
    {"switch", ARC_SWITCH}, {"break", ARC_BREAK}, {"case", ARC_CASE}, {"default", ARC_DEFAULT},
    {"true", ARC_TRUE}, {"false", ARC_FALSE}
    
});


// fn
// ret  if    elif  else
// switch   break   case
// default


