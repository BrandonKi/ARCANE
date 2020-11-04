#include <string>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>

#include <pLog.h>

using namespace pLog;

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
    // NOTE Any changes to this enum must be changed below in the str() method as well
    /* TODO fill this out */

    ARC_NONE,

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
    std::string id;
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

inline void printToken(Token t){
    print("'" + std::string(t.id) + "' " + std::to_string((int)t.kind) + " [" + std::to_string(t.srcLine) + ", " + std::to_string(t.srcChar) + "]");
}

inline void printTokenln(Token t){
    println("'" + std::string(t.id) + "' " + std::to_string((int)t.kind) + " [" + std::to_string(t.srcLine) + ", " + std::to_string(t.srcChar) + "]");
}

inline std::string str(TokenKind kind){
    
    switch(kind){

    case ARC_NONE:
        return "NONE";
        break;
    case ARC_INT_LIT:
        return "INT_LIT";
        break;
    case ARC_FLOAT_LIT:
        return "FLOAT_LIT";
        break;

    case ARC_I8:
        return "I8";
        break;
    case ARC_I16:
        return "I16";
        break;
    case ARC_I32:
        return "I32";
        break;
    case ARC_I64:
        return "I64";
        break;

    case ARC_U8:
        return "U8";
        break;
    case ARC_U16:
        return "U16";
        break;
    case ARC_U32:
        return "U32";
        break;
    case ARC_U64:
        return "U64";
        break;

    case ARC_F32:
        return "F32";
        break;
    case ARC_F64:
        return "F64";
        break;

    case ARC_STRUCT:
        return "STRUCT";
        break;
    case ARC_STR:
        return "STR";
        break;
    case ARC_ARR:
        return "ARR";
        break;

    case ARC_ID:
        return "ID";
        break;

    /* Keywords */

    case ARC_FN:
        return "FN";
        break;
    case ARC_RET:
        return "RET";
        break;
    case ARC_IF:
        return "IF";
        break;
    case ARC_ELIF:
        return "ELIF";
        break;
    case ARC_ELSE:
        return "ELSE";
        break;
    case ARC_WHILE:
        return "WHILE";
        break;
    case ARC_FOR:
        return "FOR";
        break;
    case ARC_DO:
        return "DO";
        break;
    case ARC_SWITCH:
        return "SWITCH";
        break;
    case ARC_BREAK:
        return "BREAK";
        break;
    case ARC_CASE:
        return "CASE";
        break;
    case ARC_DEFAULT:
        return "DEFAULT";
        break;

    case ARC_TRUE:
        return "TRUE";
        break;
    case ARC_FALSE:
        return "FALSE";
        break;

    /* Symbols */

    case ARC_OPEN_BRACE:
        return "OPEN_BRACE";
        break;
    case ARC_CLOSE_BRACE:
        return "CLOSE_BRACE";
        break;
    case ARC_OPEN_BRACKET:
        return "OPEN_BRACKET";
        break;
    case ARC_CLOSE_BRACKET:
        return "CLOSE_BRACKET";
        break;
    case ARC_OPEN_PAREN:
        return "OPEN_PAREN";
        break;
    case ARC_CLOSE_PAREN:
        return "CLOSE_PAREN";
        break;
    case ARC_DOT:
        return "DOT";
        break;
    case ARC_COMMA:
        return "COMMA";
        break;
    case ARC_TERNARY:
        return "TERNARY";
        break;
    case ARC_SEMICOLON:
        return "SEMICOLON";
        break;
    case ARC_COLON:
        return "COLON";
        break;
    case ARC_AT:
        return "AT";
        break;
    case ARC_HASH:
        return "HASH";
        break;
    case ARC_DOLLAR:
        return "DOLLAR";
        break;

    /* Operators */

    case ARC_ADD_EQUAL:
        return "ADD_EQUAL";
        break;
    case ARC_SUB_EQUAL:
        return "SUB_EQUAL";
        break;
    case ARC_DIV_EQUAL:
        return "DIV_EQUAL";
        break;
    case ARC_MUL_EQUAL:
        return "MUL_EQUAL";
        break;
    case ARC_MOD_EQUAL:
        return "MOD_EQUAL";
        break;
    case ARC_OR_EQUAL:
        return "OR_EQUAL";
        break;
    case ARC_AND_EQUAL:
        return "AND_EQUAL";
        break;
    case ARC_NOT_EQUAL:
        return "NOT_EQUAL";
        break;
    case ARC_LEFT_SHIFT_EQUAL:
        return "LEFT_SHIFT_EQUAL";
        break;
    case ARC_RIGHT_SHIFT_EQUAL:
        return "RIGHT_SHIFT_EQUAL";
        break;
    case ARC_XOR_EQUAL:
        return "XOR_EQUAL";
        break;
    case ARC_LESS_EQUAL:
        return "LESS_EQUAL";
        break;
    case ARC_GREATER_EQUAL:
        return "GREATER_EQUAL";
        break;
    case ARC_EQUAL:
        return "EQUAL";
        break;
    case ARC_ASSIGN:
        return "ASSIGN";
        break;
    case ARC_INFER_ASSIGN:
        return "INFER_ASSIGN";
        break;
    case ARC_ADD:
        return "ADD";
        break;
    case ARC_SUB:
        return "SUB";
        break;
    case ARC_DIV:
        return "DIV";
        break;
    case ARC_MUL:
        return "MUL";
        break;
    case ARC_MOD:
        return "MOD";
        break;
    case ARC_BIN_OR:
        return "BIN_OR";
        break;
    case ARC_BIN_AND:
        return "BIN_AND";
        break;
    case ARC_NOT:
        return "NOT";
        break;
    case ARC_LEFT_SHIFT:
        return "LEFT_SHIFT";
        break;
    case ARC_RIGHT_SHIFT:
        return "RIGHT_SHIFT";
        break;
    case ARC_XOR:
        return "XOR";
        break;
    case ARC_LESS:
        return "LESS";
        break;
    case ARC_GREATER:
        return "GREATER";
        break;
    case ARC_NEGATE:
        return "NEGATE";
        break;
    case ARC_LOGICAL_OR:
        return "LOGICAL_OR";
        break;
    case ARC_LOGICAL_AND:
        return "LOGICAL_AND";
        break;
    case ARC_PRE_INCREMENT:
        return "PRE_INCREMENT";
        break;
    case ARC_POST_INCREMENT:
        return "POST_INCREMENT";
        break;
    case ARC_PRE_DECREMENT:
        return "PRE_DECREMENT";
        break;
    case ARC_POST_DECREMENT:
        return "POST_DECREMENT";
        break;
    }
}