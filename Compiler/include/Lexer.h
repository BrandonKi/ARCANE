#include "ErrorHandler.h"

class Lexer{

    public:
        Lexer(const std::string&);
        void lex();
        char currentChar();
        char nextChar();
        char peekNextChar();

    private:
        const std::string& data;
        std::vector<Token*> tokens;
        u32 index;
        u32 lineNum;
        u32 charNum;

        Token* number_lit();

        constexpr static inline bool isDigit(char c) noexcept {
            return c >= 48 && c <= 57;
        }

        constexpr static inline bool isAlpha(char c) noexcept {
            return (c >= 65 && c <= 90) || (c >= 97 && c <= 122) || c == 95;
        }

};
