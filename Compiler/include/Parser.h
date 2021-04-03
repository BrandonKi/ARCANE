#include "SymbolTable.h"

class Parser{

    public:
        Parser(std::vector<RawFile, arena_allocator<RawFile>>&);
        Project* parse();

    private:
        std::vector<LexedFile> data;
        std::vector<Token, arena_allocator<Token>> tokens;
        u64 index;
        AST ast;
        SymbolTable s_table;

        Project* parseProject();
        File* parseFile();
        Import* parseImport();
        Function* parseFunction();
        Block* parseBlock();
        Statement* parseStatement();
        Decl* parseDecl();
        Expr* parseExpr();

        Token* currentToken();
        Token* nextToken();
        Token* peekNextToken();
        bool checkToken(TokenKind);
        bool expectToken(TokenKind);

        bool isOperator(TokenKind);
        bool isUnaryOperator(TokenKind);
        u8 precedence(TokenKind);

        Type tokenKind2Type(TokenKind);
};