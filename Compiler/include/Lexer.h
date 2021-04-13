#ifndef LEXER_H
#define LEXER_H

#include "ErrorHandler.h"

static ErrorHandler errorLog{};

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
        explicit Lexer(const astring&);
        [[nodiscard]] std::vector<Token, arena_allocator<Token>> lex();
        [[nodiscard]] char current_char() const;
        [[nodiscard]] char next_char();
        [[nodiscard]] char prev_char();
        void next_char_noreturn();
        void prev_char_noreturn();
        [[nodiscard]] char peek_next_char() const;
        [[nodiscard]] char peek_prev_char() const;

    private:
        const astring data_;
        std::vector<Token, arena_allocator<Token>> tokens_;
        u32 index_;
        u32 line_;
        u32 col_;

        void consume_comment();

        Token lex_number_lit();
        Token lex_identifier();
        Token lex_string();
        Token lex_interpolated_string();
        Token lex_colon();
        Token lex_add();
        Token lex_sub();
        Token lex_div();
        Token lex_mul();
        Token lex_mod();
        Token lex_or();
        Token lex_and();
        Token lex_not();
        Token lex_xor();
        Token lex_lesser(); 
        Token lex_greater();
        Token lex_equal();

        [[nodiscard]] Token create_token(const TokenKind, const u32) const;
        [[nodiscard]] Token create_token(const TokenKind, const u32, const u32) const;
        [[nodiscard]] Token create_token(const TokenKind, const u32, astring&) const;
        [[nodiscard]] Token create_token(const TokenKind, const u32, astring&&) const;
        [[nodiscard]] Token create_token(const TokenKind, const u32, const u32, astring&) const;
        [[nodiscard]] Token create_token(const TokenKind, const u32, const u32, astring&&) const;

        void print_tokens(const bool) const;

        constexpr static inline bool is_digit(const char c) noexcept {
            return c >= 48 && c <= 57;
        }

        constexpr static inline bool is_letter(const char c) noexcept {
            return (c >= 65 && c <= 90) || (c >= 97 && c <= 122) || c == 95;
        }

};

#endif