#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <unordered_map>
#include <future>
#include <cstring>

#include "OS.h"

// #define DP_KEYWORDS
// #define DP_TOKEN_STEP
#define DP_TOKEN_RESULT
#define DP_INFIX_EXPR
#define DP_POSTFIX_EXPR

#ifdef DP_KEYWORDS
#define DEBUG_PRINT_KEYWORDS(x) x
#else
#define DEBUG_PRINT_KEYWORDS(x)
#endif

#ifdef DP_TOKEN_RESULT
#define DEBUG_PRINT_TOKENS(x) x
#else
#define DEBUG_PRINT_TOKENS(x)
#endif

#ifdef DP_TOKEN_STEP
#define DEBUG_PRINT_STEP(x) x
#else
#define DEBUG_PRINT_STEP(x)
#endif

#ifdef DP_INFIX_EXPR
#define DEBUG_PRINT_INFIX(x) x
#else
#define DEBUG_PRINT_INFIX(x)
#endif

#ifdef DP_POSTFIX_EXPR
#define DEBUG_PRINT_POSTFIX(x) x
#else
#define DEBUG_PRINT_POSTFIX(x)
#endif



#define log(x) std::cout << x
#define logn(x) std::cout << x << "\n"
#define REPORT_ERROR(x) std::cout << x << "\n"; std::cin.get(); std::exit(-1)
#define WAIT_AND_EXIT(x) std::cin.get(); std::exit(x)

enum ErrorType{ 
                ERR_REDECL, ERR_UNDEFINED_ID, ERR_CLOSE_EXTRA_PAREN, ERR_OPEN_EXTRA_PAREN
              };

enum T_Type{ 
                T_CHAR_LIT, T_NUMBER_LIT, T_LONG_LIT, T_FLOAT_LIT, T_STR_LIT, T_ID, T_INFER,
                T_OPERATOR, T_COMMA, T_DOT, T_LPAREN, T_RPAREN, T_NOT, 
                T_LBRACKET, T_RBRACKET, T_TERNARY, T_COLON, T_SEMICOLON,

                T_AUTO, T_CHAR, T_UCHAR, T_BYTE, T_UBYTE, T_INT, T_UINT, 
                T_LONG, T_FLOAT, T_DOUBLE, T_STRING, T_FOR, T_WHILE, T_RET, 
                T_IF, T_ELIF, T_ELSE, T_SWITCH, T_CASE, T_BREAK, T_FN, T_ARRAY,
                T_INCLUDE, T_IMPORT
              };

enum OperatorDescriptor{    
                            OP_LOG_NOT, OP_UNARY_POST_INC, OP_UNARY_PRE_INC, OP_UNARY_POST_DEC, 
                            OP_UNARY_PRE_DEC, OP_UNARY_PLUS, OP_UNARY_SUB, OP_LSHIFT, OP_RSHIFT, 
                            OP_BIN_AND, OP_BIN_OR, OP_BIN_XOR, OP_MUL, OP_DIV, OP_MOD, OP_ADD, 
                            OP_SUB, OP_EQUAL, OP_NOT_EQUAL, OP_LESS_EQUAL, OP_GREAT_EQUAL, 
                            OP_GREAT, OP_LESS, OP_LOG_AND, OP_LOG_OR, OP_ASSIGN, OP_ADD_EQUAL, OP_SUB_EQUAL, 
                            OP_MUL_EQUAL, OP_DIV_EQUAL, OP_MOD_EQUAL, OP_RSHIFT_EQUAL, OP_LSHIFT_EQUAL, 
                            OP_AND_EQUAL, OP_OR_EQUAL, OP_XOR_EQUAL, OP_TERNARY
                        };

struct Token{
    T_Type type;
    std::string val;
    unsigned int l_pos;
    unsigned int c_pos;
    unsigned int F_c_pos;
    union{
        OperatorDescriptor op_info;
    };
};
