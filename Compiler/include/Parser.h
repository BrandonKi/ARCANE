#include "SymbolTable.h"

class Parser{

    public:
        Parser(std::vector<RawFile>&);
        Project* parse();

    private:
        std::vector<LexedFile> data;
        std::vector<Token*> tokens;
        u64 index;
        AST ast;

        Project* parseProject();
        File* parseFile();
        Decl* Parser::parseDecl();
        Expr* Parser::parseExpr();

        Token* Parser::currentToken();
        Token* Parser::nextToken();
        Token* Parser::peekNextToken();

        bool Parser::isOperator(TokenKind);
        bool Parser::isUnaryOperator(TokenKind kind);
        u8 Parser::precedence(TokenKind kind);

        Type Parser::tokenKind2Type(TokenKind);
};