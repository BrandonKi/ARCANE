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

        Project* parse_project();
        File* parse_file();
        Import* parse_import();
        Function* parse_function();
        Block* parse_block();
        Statement* parse_statement();
        Decl* parse_decl();
        Expr* parse_expr();

        Token* current_token();
        Token* next_token();
        Token* peek_next_token();
        bool check_token(TokenKind);
        bool expect_token(TokenKind);

        bool is_operator(TokenKind);
        bool is_unary_operator(TokenKind);
        u8 precedence(TokenKind);

        Type token_kind_to_type(TokenKind);
};