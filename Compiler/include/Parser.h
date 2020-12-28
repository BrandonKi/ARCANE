#include "AST.h"

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

        Type Parser::tokenKind2Type(TokenKind);
};