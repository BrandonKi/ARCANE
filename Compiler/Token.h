#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <unordered_map>
#include <future>

// #define DP_KEYWORDS
// #define DP_TOKEN_STEP
// #define DP_TOKEN_RESULT
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

enum TokenType{ T_KEYWORD, T_CHAR, T_NUMBER, T_FLOAT, T_STR, T_ID, 
                T_OPERATOR, T_COMMA, T_DOT, T_LPAREN, T_RPAREN, T_NOT, 
                T_LBRACKET, T_RBRACKET, T_TERNARY, T_COLON, T_SEMICOLON
              };


enum Keyword{   K_AUTO, K_CHAR, K_UCHAR, K_BYTE, K_UBYTE, K_INT, K_UINT, 
                K_FLOAT, K_DOUBLE, K_LONG, K_STRING, K_FOR, K_WHILE, K_RETURN, 
                K_IF, K_ELIF, K_ELSE, K_SWITCH, K_CASE, K_BREAK
            };

enum OperatorDescriptor{    OP_LOG_NOT, OP_UNARY_POST_INC, OP_UNARY_PRE_INC, OP_UNARY_POST_DEC, 
                            OP_UNARY_PRE_DEC, OP_UNARY_PLUS, OP_UNARY_SUB, OP_LSHIFT, OP_RSHIFT, 
                            OP_BIN_AND, OP_BIN_OR, OP_BIN_XOR, OP_MUL, OP_DIV, OP_MOD, OP_ADD, 
                            OP_SUB, OP_EQUAL, OP_NOT_EQUAL, OP_LESS_EQUAL, OP_GREAT_EQUAL, 
                            OP_GREAT, OP_LESS, OP_LOG_AND, OP_LOG_OR, OP_ASSIGN, OP_ADD_EQUAL, OP_SUB_EQUAL, 
                            OP_MUL_EQUAL, OP_DIV_EQUAL, OP_MOD_EQUAL, OP_RSHIFT_EQUAL, OP_LSHIFT_EQUAL, 
                            OP_AND_EQUAL, OP_OR_EQUAL, OP_XOR_EQUAL
                        };

struct Token{
    TokenType type;
    std::string val;
    unsigned int l_pos;
    unsigned int c_pos;
    union{
        OperatorDescriptor op_info;
        Keyword key;
    };
};