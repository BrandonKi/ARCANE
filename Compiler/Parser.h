#include "AST.h"

class Parser{

    private:
        std::vector<Token> tokens;
        uint currentTokenPointer;
        AST ast;

    public:
        Parser(std::vector<Token>);


    private:

        void parseExpr();
        void parseDecl();

        Token* nextToken();
        Token* currentToken();
        void expectToken(TokenType);


        Expr* parseExpr_10();
        Expr* parseExpr_9();
        Expr* parseExpr_8();
        Expr* parseExpr_7();
        Expr* parseExpr_6();
        Expr* parseExpr_5();
        Expr* parseExpr_4();
        Expr* parseExpr_3();
        Expr* parseExpr_2();
        Expr* parseExpr_1();
        Expr* parseExpr_0();
        

};