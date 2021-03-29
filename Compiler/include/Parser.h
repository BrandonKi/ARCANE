#include "SymbolTable.h"

class Parser{

    public:
        Parser(std::vector<RawFile>&);
        Project* parse();

    private:
        std::vector<LexedFile> data;
        std::vector<Token, arena_allocator<Token>> tokens;
        u64 index;
        AST ast;

        Project* parseProject();
        File* parseFile();
        Import* parseImport();
        Function* parseFunction();
        Decl* parseDecl();
        Expr* parseExpr();

        Token* currentToken();
        Token* nextToken();
        Token* peekNextToken();

        bool isOperator(TokenKind);
        bool isUnaryOperator(TokenKind kind);
        u8 precedence(TokenKind kind);

        Type tokenKind2Type(TokenKind);
};