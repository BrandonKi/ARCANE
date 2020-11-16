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
typedef float_t f32;
typedef double_t f64;

inline struct ARGS {
    std::string filepath;
    std::string output_filepath;
    bool LexOut = false;
    bool verboseLexOut = false;
    bool optimize = false;
}args;

enum TokenKind {    // It's named TokenKind instead of TokenType because windows.h stole TokenType from me :(
    // NOTE Any changes to this enum must be changed below in the str() method as well
    /* TODO fill this out */

    ARC_NONE,

    /* Types */

    ARC_INT_LIT,
    ARC_FLOAT_LIT,
    ARC_STRUCT_LIT,
    ARC_STRING_LIT,
    ARC_ARR_LIT,

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
    ARC_LESSER_EQUAL,
    ARC_GREATER_EQUAL,
    ARC_EQUAL,
    ARC_ASSIGN,
    ARC_INFER,
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
    ARC_LESSER,
    ARC_GREATER,
    ARC_NEGATE,
    ARC_LOGICAL_OR,
    ARC_LOGICAL_AND,
    ARC_PRE_INCREMENT,
    ARC_POST_INCREMENT,
    ARC_PRE_DECREMENT,
    ARC_POST_DECREMENT,
};

struct Token{
    TokenKind kind;
    u32 srcLine;
    u32 srcChar;
    u32 startPos;
    u32 endPos;
    std::string data;
};

const static std::unordered_map<std::string, TokenKind> keywords( { 

    {"i8", ARC_I8}, {"i16", ARC_I16}, {"i32", ARC_I32}, {"i64", ARC_I64}, 
    {"u8", ARC_U8}, {"u16", ARC_U16}, {"u32", ARC_U32}, {"u64", ARC_U64}, 
    {"f32", ARC_F32}, {"f64", ARC_F64}, {"string", ARC_STR}, {"arr", ARC_ARR}, 
    {"struct", ARC_STRUCT}, {"char", ARC_I8}, {"int", ARC_I32}, {"float", ARC_F32},

    {"fn", ARC_FN}, {"ret", ARC_RET}, {"if", ARC_IF}, {"elif", ARC_ELIF},
    {"else", ARC_ELSE}, {"while", ARC_WHILE}, {"for", ARC_FOR}, {"do", ARC_DO},
    {"switch", ARC_SWITCH}, {"break", ARC_BREAK}, {"case", ARC_CASE}, {"default", ARC_DEFAULT},
    {"true", ARC_TRUE}, {"false", ARC_FALSE}
    
});

inline void printToken(Token* t){
    print("'" + std::string(t->data) + "' " + std::to_string(static_cast<i32>(t->kind)) + " [" + std::to_string(t->srcLine) + ", " + std::to_string(t->srcChar) + "] " + "[" + std::to_string(t->startPos) + ", " + std::to_string(t->endPos) + "]");
}

inline void printTokenln(Token* t){
    println("'" + std::string(t->data) + "' " + std::to_string(static_cast<i32>(t->kind)) + " [" + std::to_string(t->srcLine) + ", " + std::to_string(t->srcChar) + "] " + "[" + std::to_string(t->startPos) + ", " + std::to_string(t->endPos) + "]");
}

inline std::string str(TokenKind kind){
    
    switch(kind){

    case ARC_NONE:
        return "NONE";
        
    case ARC_INT_LIT:
        return "INT_LIT";
        
    case ARC_FLOAT_LIT:
        return "FLOAT_LIT";
        
    case ARC_STRUCT_LIT:
        return "STRUCT_LIT";

    case ARC_STRING_LIT:
        return "STRING_LIT";

    case ARC_ARR_LIT:
        return "ARR_LIT";

    case ARC_I8:
        return "I8";
        
    case ARC_I16:
        return "I16";
        
    case ARC_I32:
        return "I32";
        
    case ARC_I64:
        return "I64";
        

    case ARC_U8:
        return "U8";
        
    case ARC_U16:
        return "U16";
        
    case ARC_U32:
        return "U32";
        
    case ARC_U64:
        return "U64";
        

    case ARC_F32:
        return "F32";
        
    case ARC_F64:
        return "F64";
        

    case ARC_STRUCT:
        return "STRUCT";
        
    case ARC_STR:
        return "STR";
        
    case ARC_ARR:
        return "ARR";
        

    case ARC_ID:
        return "ID";
        

    /* Keywords */

    case ARC_FN:
        return "FN";
        
    case ARC_RET:
        return "RET";
        
    case ARC_IF:
        return "IF";
        
    case ARC_ELIF:
        return "ELIF";
        
    case ARC_ELSE:
        return "ELSE";
        
    case ARC_WHILE:
        return "WHILE";
        
    case ARC_FOR:
        return "FOR";
        
    case ARC_DO:
        return "DO";
        
    case ARC_SWITCH:
        return "SWITCH";
        
    case ARC_BREAK:
        return "BREAK";
        
    case ARC_CASE:
        return "CASE";
        
    case ARC_DEFAULT:
        return "DEFAULT";
        

    case ARC_TRUE:
        return "TRUE";
        
    case ARC_FALSE:
        return "FALSE";
        

    /* Symbols */

    case ARC_OPEN_BRACE:
        return "OPEN_BRACE";
        
    case ARC_CLOSE_BRACE:
        return "CLOSE_BRACE";
        
    case ARC_OPEN_BRACKET:
        return "OPEN_BRACKET";
        
    case ARC_CLOSE_BRACKET:
        return "CLOSE_BRACKET";
        
    case ARC_OPEN_PAREN:
        return "OPEN_PAREN";
        
    case ARC_CLOSE_PAREN:
        return "CLOSE_PAREN";
        
    case ARC_DOT:
        return "DOT";
        
    case ARC_COMMA:
        return "COMMA";
        
    case ARC_TERNARY:
        return "TERNARY";
        
    case ARC_SEMICOLON:
        return "SEMICOLON";
        
    case ARC_COLON:
        return "COLON";
        
    case ARC_AT:
        return "AT";
        
    case ARC_HASH:
        return "HASH";
        
    case ARC_DOLLAR:
        return "DOLLAR";
        

    /* Operators */

    case ARC_ADD_EQUAL:
        return "ADD_EQUAL";
        
    case ARC_SUB_EQUAL:
        return "SUB_EQUAL";
        
    case ARC_DIV_EQUAL:
        return "DIV_EQUAL";
        
    case ARC_MUL_EQUAL:
        return "MUL_EQUAL";
        
    case ARC_MOD_EQUAL:
        return "MOD_EQUAL";
        
    case ARC_OR_EQUAL:
        return "OR_EQUAL";
        
    case ARC_AND_EQUAL:
        return "AND_EQUAL";
        
    case ARC_NOT_EQUAL:
        return "NOT_EQUAL";
        
    case ARC_LEFT_SHIFT_EQUAL:
        return "LEFT_SHIFT_EQUAL";
        
    case ARC_RIGHT_SHIFT_EQUAL:
        return "RIGHT_SHIFT_EQUAL";
        
    case ARC_XOR_EQUAL:
        return "XOR_EQUAL";
        
    case ARC_LESSER_EQUAL:
        return "LESSER_EQUAL";
        
    case ARC_GREATER_EQUAL:
        return "GREATER_EQUAL";
        
    case ARC_EQUAL:
        return "EQUAL";
        
    case ARC_ASSIGN:
        return "ASSIGN";
        
    case ARC_INFER:
        return "INFER";
        
    case ARC_ADD:
        return "ADD";
        
    case ARC_SUB:
        return "SUB";
        
    case ARC_DIV:
        return "DIV";
        
    case ARC_MUL:
        return "MUL";
        
    case ARC_MOD:
        return "MOD";
        
    case ARC_BIN_OR:
        return "BIN_OR";
        
    case ARC_BIN_AND:
        return "BIN_AND";
        
    case ARC_NOT:
        return "NOT";
        
    case ARC_LEFT_SHIFT:
        return "LEFT_SHIFT";
        
    case ARC_RIGHT_SHIFT:
        return "RIGHT_SHIFT";
        
    case ARC_XOR:
        return "XOR";
        
    case ARC_LESSER:
        return "LESSER";
        
    case ARC_GREATER:
        return "GREATER";
        
    case ARC_NEGATE:
        return "NEGATE";
        
    case ARC_LOGICAL_OR:
        return "LOGICAL_OR";
        
    case ARC_LOGICAL_AND:
        return "LOGICAL_AND";
        
    case ARC_PRE_INCREMENT:
        return "PRE_INCREMENT";
        
    case ARC_POST_INCREMENT:
        return "POST_INCREMENT";
        
    case ARC_PRE_DECREMENT:
        return "PRE_DECREMENT";
        
    case ARC_POST_DECREMENT:
        return "POST_DECREMENT";
        
    }
}

inline std::string getStringRep(TokenKind kind){
    
    switch(kind){

    case ARC_NONE:
        return "NONE";
        
    case ARC_INT_LIT:
        return "";
        
    case ARC_FLOAT_LIT:
        return "";
        
    case ARC_STRUCT_LIT:
        return "{}";
    
    case ARC_STRING_LIT:
        return "";

    case ARC_ARR_LIT:
        return "[]";

    case ARC_I8:
        return "i8";
        
    case ARC_I16:
        return "i16";
        
    case ARC_I32:
        return "i32";
        
    case ARC_I64:
        return "i64";
        

    case ARC_U8:
        return "u8";
        
    case ARC_U16:
        return "u16";
        
    case ARC_U32:
        return "u32";
        
    case ARC_U64:
        return "u64";
        

    case ARC_F32:
        return "f32";
        
    case ARC_F64:
        return "f64";
        

    case ARC_STRUCT:
        return "struct";
        
    case ARC_STR:
        return "string";
        
    case ARC_ARR:
        return "arr";
        

    case ARC_ID:
        return "";
        

    /* Keywords */

    case ARC_FN:
        return "fn";
        
    case ARC_RET:
        return "ret";
        
    case ARC_IF:
        return "if";
        
    case ARC_ELIF:
        return "elif";
        
    case ARC_ELSE:
        return "else";
        
    case ARC_WHILE:
        return "while";
        
    case ARC_FOR:
        return "for";
        
    case ARC_DO:
        return "do";
        
    case ARC_SWITCH:
        return "switch";
        
    case ARC_BREAK:
        return "break";
        
    case ARC_CASE:
        return "case";
        
    case ARC_DEFAULT:
        return "default";
        

    case ARC_TRUE:
        return "true";
        
    case ARC_FALSE:
        return "false";
        

    /* Symbols */

    case ARC_OPEN_BRACE:
        return "{";
        
    case ARC_CLOSE_BRACE:
        return "}";
        
    case ARC_OPEN_BRACKET:
        return "[";
        
    case ARC_CLOSE_BRACKET:
        return "]";
        
    case ARC_OPEN_PAREN:
        return "(";
        
    case ARC_CLOSE_PAREN:
        return ")";
        
    case ARC_DOT:
        return ".";
        
    case ARC_COMMA:
        return ",";
        
    case ARC_TERNARY:
        return "?";
        
    case ARC_SEMICOLON:
        return ";";
        
    case ARC_COLON:
        return ":";
        
    case ARC_AT:
        return "@";
        
    case ARC_HASH:
        return "#";
        
    case ARC_DOLLAR:
        return "$";
        

    /* Operators */

    case ARC_ADD_EQUAL:
        return "+=";
        
    case ARC_SUB_EQUAL:
        return "-=";
        
    case ARC_DIV_EQUAL:
        return "/=";
        
    case ARC_MUL_EQUAL:
        return "*=";
        
    case ARC_MOD_EQUAL:
        return "%=";
        
    case ARC_OR_EQUAL:
        return "|=";
        
    case ARC_AND_EQUAL:
        return "&=";
        
    case ARC_NOT_EQUAL:
        return "!=";
        
    case ARC_LEFT_SHIFT_EQUAL:
        return "<<=";
        
    case ARC_RIGHT_SHIFT_EQUAL:
        return ">>=";
        
    case ARC_XOR_EQUAL:
        return "^=";
        
    case ARC_LESSER_EQUAL:
        return "<=";
        
    case ARC_GREATER_EQUAL:
        return ">=";
        
    case ARC_EQUAL:
        return "==";
        
    case ARC_ASSIGN:
        return "=";
        
    case ARC_INFER:
        return ":=";
        
    case ARC_ADD:
        return "+";
        
    case ARC_SUB:
        return "-";
        
    case ARC_DIV:
        return "/";
        
    case ARC_MUL:
        return "*";
        
    case ARC_MOD:
        return "%";
        
    case ARC_BIN_OR:
        return "|";
        
    case ARC_BIN_AND:
        return "&";
        
    case ARC_NOT:
        return "!";
        
    case ARC_LEFT_SHIFT:
        return "<<";
        
    case ARC_RIGHT_SHIFT:
        return ">>";
        
    case ARC_XOR:
        return "^";
        
    case ARC_LESSER:
        return "<";
        
    case ARC_GREATER:
        return ">";
        
    case ARC_NEGATE:
        return "-x";
        
    case ARC_LOGICAL_OR:
        return "||";
        
    case ARC_LOGICAL_AND:
        return "&&";
        
    case ARC_PRE_INCREMENT:
        return "++x";
        
    case ARC_POST_INCREMENT:
        return "x++";
        
    case ARC_PRE_DECREMENT:
        return "--x";
        
    case ARC_POST_DECREMENT:
        return "x--";
        
    }
}