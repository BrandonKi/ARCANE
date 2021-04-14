#ifndef PARSER_H
#define PARSER_H

#include "SymbolTable.h"

class Parser{

    public:
        explicit Parser(std::vector<RawFile, arena_allocator<RawFile>>&);
        Project* parse();

    private:
        std::vector<LexedFile> data_;
        std::vector<Token, arena_allocator<Token>> tokens_;
        u64 index_;
        AST ast_;
        SymbolTable s_table_;

        Project* parse_project();
        File* parse_file();
        Import* parse_import();
        Function* parse_function();
        Block* parse_block();
        Statement* parse_statement();
        Decl* parse_decl();
        Expr* parse_expr();

        [[nodiscard]] Token* current_token();
        [[nodiscard]] Token* next_token();
        void next_token_noreturn();
        [[nodiscard]] Token* peek_next_token();
        bool check_token(const TokenKind);
        bool expect_token(const TokenKind);

        u8 precedence(const TokenKind);

        Type token_kind_to_type(const TokenKind);
};

#endif