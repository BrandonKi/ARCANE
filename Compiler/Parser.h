#include "Lexer.h"

class Parser{

    private:
        std::unordered_map<std::string, Type> symbol_table;
        std::vector<Token> tokens;
        uint currentTokenPointer;


    public:
        Parser(std::vector<Token>*, std::unordered_map<std::string, Type>);


    private:

        void parseExpr();
        void nextToken();
        Token* currentToken();

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