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

inline struct ARGS {
    std::string filepath;
    std::string output_filepath; 
    bool optimize;
}args;

enum TokenKind {    // It's named TokenKind instead of TokenType because windows.h stole TokenType from me :(
    /* TODO fill this out */

    NONE,

    /* Types */

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

    ARC_STRUCT,     // TODO this is just filler for now. It needs type information to be useful
    ARC_STR,
    ARC_ARR,       // TODO this is just filler for now. It needs type information to be useful

    ARC_ID,

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

    ARC_TRUE,
    ARC_FALSE,
    
    /* Symbols */

    ARC_OPEN_BRACE,
    ARC_CLOSE_BRACE,
    ARC_OPEN_BRACKET,
    ARC_CLOSE_BRACKET,
    ARC_OPEN_PAREN,
    ARC_CLOSE_PAREN,
    ARC_DOT,
    ARC_COMMA,
    ARC_TERNARY,
    ARC_SEMICOLON,
    ARC_COLON,
    ARC_AT,     //TODO implement AT, HASH, and DOLLAR
    ARC_HASH,   //
    ARC_DOLLAR, //

    /* Operators */

    ARC_ADD_EQUAL,
    ARC_SUB_EQUAL,
    ARC_DIV_EQUAL,
    ARC_MUL_EQUAL,
    ARC_MOD_EQUAL,
    ARC_OR_EQUAL,
    ARC_AND_EQUAL,
    ARC_NOT_EQUAL,
    ARC_LEFT_SHIFT_EQUAL,
    ARC_RIGHT_SHIFT_EQUAL,
    ARC_XOR_EQUAL,
    ARC_LESS_EQUAL,
    ARC_GREATER_EQUAL,
    ARC_EQUAL,
    ARC_ASSIGN,
    ARC_INFER_ASSIGN,
    ARC_ADD,
    ARC_SUB,
    ARC_DIV,
    ARC_MUL,
    ARC_MOD,
    ARC_BIN_OR,
    ARC_BIN_AND,
    ARC_NOT,
    ARC_LEFT_SHIFT,
    ARC_RIGHT_SHIFT,
    ARC_XOR,
    ARC_LESS,
    ARC_GREATER,
    ARC_NEGATE,
    ARC_LOGICAL_OR,
    ARC_LOGICAL_AND,
    ARC_PRE_INCREMENT,
    ARC_POST_INCREMENT,
    ARC_PRE_DECREMENT,
    ARC_POST_DECREMENT,
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

const static std::unordered_map<std::string, TokenKind> keywords( { 

    {"i8", ARC_I8}, {"i16", ARC_I16}, {"i32", ARC_I32}, {"i64", ARC_I64}, 
    {"u8", ARC_U8}, {"u16", ARC_U16}, {"u32", ARC_U32}, {"u64", ARC_U64}, 
    {"f32", ARC_F32}, {"f64", ARC_F64}, {"str", ARC_STR}, {"arr", ARC_ARR}, 
    {"struct", ARC_STRUCT}, {"char", ARC_I8}, {"int", ARC_I32}, {"float", ARC_F32},

    {"fn", ARC_FN}, {"ret", ARC_RET}, {"if", ARC_IF}, {"elif", ARC_ELIF},
    {"else", ARC_ELSE}, {"while", ARC_WHILE}, {"for", ARC_FOR}, {"do", ARC_DO},
    {"switch", ARC_SWITCH}, {"break", ARC_BREAK}, {"case", ARC_CASE}, {"default", ARC_DEFAULT},
    {"true", ARC_TRUE}, {"false", ARC_FALSE}
    
});

