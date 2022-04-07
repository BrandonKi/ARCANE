#ifndef LEXER_H
#define LEXER_H

#include "TypeManager.h"

class Lexer {

    #define CASE_DIGIT case '0': case '1': case '2': case '3': case '4': case '5': \
                       case '6': case '7': case '8': case '9'

    #define CASE_ID case 'A':case 'B':case 'C':case 'D':case 'E':case 'F':case 'G':case 'H':case 'I': \
                    case 'J':case 'K':case 'L':case 'M':case 'N':case 'O':case 'P':case 'Q':case 'R': \
                    case 'S':case 'T':case 'U':case 'V':case 'W':case 'X':case 'Y':case 'Z':case 'a': \
                    case 'b':case 'c':case 'd':case 'e':case 'f':case 'g':case 'h':case 'i':case 'j': \
                    case 'k':case 'l':case 'm':case 'n':case 'o':case 'p':case 'q':case 'r':case 's': \
                    case 't':case 'u':case 'v':case 'w':case 'x':case 'y':case 'z':case '_'

    public:
        Lexer(const std::string&, const char*, size_t);
        [[nodiscard]] std::vector<Token> lex();
        [[nodiscard]] char current_char() const;
        [[nodiscard]] char next_char();
        [[nodiscard]] char prev_char();
        void next_char_noreturn();
        void prev_char_noreturn();
        [[nodiscard]] char peek_next_char() const;
        [[nodiscard]] char peek_prev_char() const;

    private:
        const std::string filename_;
        const char * data_;
        size_t data_size;
        std::vector<Token> tokens_;
        u32 index_;
        u32 line_;
        u32 col_;

        void consume_comment();

        [[nodiscard]] Token lex_number_lit();
        [[nodiscard]] Token lex_identifier();
        [[nodiscard]] Token lex_string();
        [[nodiscard]] Token lex_interpolated_string();
        [[nodiscard]] Token lex_hash();
        [[nodiscard]] Token lex_colon();
        [[nodiscard]] Token lex_add();
        [[nodiscard]] Token lex_sub();
        [[nodiscard]] Token lex_div();
        [[nodiscard]] Token lex_mul();
        [[nodiscard]] Token lex_mod();
        [[nodiscard]] Token lex_or();
        [[nodiscard]] Token lex_and();
        [[nodiscard]] Token lex_not();
        [[nodiscard]] Token lex_xor();
        [[nodiscard]] Token lex_lesser();
        [[nodiscard]] Token lex_greater();
        [[nodiscard]] Token lex_equal();

        [[nodiscard]] Token create_token(const TokenKind, const u32) const;
        [[nodiscard]] Token create_token(const TokenKind, const u32, const u32) const;
        [[nodiscard]] Token create_token(const TokenKind, const u32, std::string_view) const;
        [[nodiscard]] Token create_token(const TokenKind, const u32, const u32, std::string_view) const;

        void print_tokens(const bool) const;

};

#endif