#include "Parser.h"

class AST{

    private:
        void* m_memory;
        void* m_memory_ptr;
        unsigned int m_size;

    public:
        AST();
        void init();
        void runTests();

    private:
        
        void print(Expr*);
        Expr* alloc_expr();
        Expr* newIntegerExpr(uint);
        Expr* newStringExpr(std::string);
        Expr* newUnaryExpr(Expr*, char);
        Expr* newBinaryExpr(Expr*, Expr*, char);
        Expr* newTernaryExpr(Expr*, Expr*, Expr*);
};