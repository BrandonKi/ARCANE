#include "Lexer.h"

enum DeclType{D_SBYTE, D_UBYTE, D_SINT, D_UINT, D_FLOAT, D_DOUBLE, D_STRING, D_ARRAY};
enum ExprType{E_UNARY, E_BINARY, E_TERNARY, E_INTEGER, E_FLOAT, E_STRING, E_INDEX, E_FIELD};
enum Type{T_INT, T_FLOAT, T_STR, T_ARR};

struct Decl{
    DeclType type;
    std::string name;
    
    
};

struct Expr {
    ExprType type;
    union{
        struct {
            unsigned long long int val;
        } e_integer;
        struct {
            double val;
        } e_float;
        struct {
            std::string val;
        } e_string;
        struct {
            Type type;
            Expr* index;
        } e_index;
        struct {
            Expr* left;
            Expr* right;
            char op;
        } e_binary;
        struct {
            Expr* expr;
            char op;
        } e_unary;
    };
};

typedef uint_fast64_t uint;

class AST{

    private:
        void* m_memory;
        void* m_memory_ptr;
        unsigned int m_size;

    public:
        AST();
        void start();

    private:
        void init();
        void print(Expr*);
        Expr* alloc_expr();
        Expr* newIntegerExpr(uint);
        Expr* newStringExpr(std::string);
        Expr* newUnaryExpr(Expr*, char);
        Expr* newBinaryExpr(Expr*, Expr*, char);
};