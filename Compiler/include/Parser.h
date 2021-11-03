#ifndef PARSER_H
#define PARSER_H
#include <string>
#include <iostream>

#include "IRGen.h"

using namespace std::string_literals;

class Parser {
    public:
        explicit Parser(std::vector<RawFile>&);
        Project* parse();

    private:
        std::vector<LexedFile> data_;
        std::vector<Token> tokens_;
        u64 index_ = 0;
        AST ast_;
        SymbolTable s_table_;
        std::string current_filename_;

        Project* parse_project();
        File* parse_file();
        Import* parse_import();
        Function* parse_function();
        std::vector<Arg> parse_fn_args();
        Block* parse_block();
        Block* parse_bare_block();
        Statement* parse_statement();
        std::vector<IfStmnt*> parse_elif_stmnts();
        Block* parse_else();
        Decl* parse_decl();
        Expr* parse_expr(bool stop_at_paren = false);
        std::vector<Token*> parse_expr_0(bool stop_at_paren = false);
        Expr* parse_expr_1(std::vector<Token*>);


        [[nodiscard]] Token* current_token();
        [[nodiscard]] Token* next_token();
        void next_token_noreturn();
        void prev_token_noreturn();
        [[nodiscard]] Token* peek_next_token();
        bool check_token(const TokenKind);
        void verify_token(const TokenKind);
        bool expect_token(const TokenKind);

        u8 precedence(const TokenKind);

        type_handle token_kind_to_type(const TokenKind);
};

#endif