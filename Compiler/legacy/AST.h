#include "Lexer.h"

class AST{

    private:
        void* m_memory;
        void* m_memory_ptr;
        unsigned int m_size;

    public:
        AST();
        void init();
        void runTests();
        void print(Expr*);

        Expr* newIntegerExpr(uint);
        Expr* newFloatExpr(double);
        Expr* newStringExpr(std::string);
        Expr* newNameExpr(std::string);
        Expr* newUnaryExpr(Expr*, char);
        Expr* newBinaryExpr(Expr*, Expr*, char);
        Expr* newTernaryExpr(Expr*, Expr*, Expr*);
        Expr* newParenExpr(Expr*);

        Decl* newDecl(DeclType, std::string, Expr*);

    private:
        
        Expr* alloc_expr();
        Decl* alloc_decl();
};