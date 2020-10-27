#include "Common.h"

class Lexer{

    public:
        Lexer(const std::string&);
        Token* next();
        Token* peekNext();

    private:
        const std::string& data;
        u32 index;
        u32 lineNum;
        u32 charNum;

        Token* number();

        constexpr static inline bool isDigit(char c){
            return c >= 48 && c <= 57;
        }

        constexpr static inline bool isAlpha(char c){
            return (c >= 65 && c <= 90) || (c >= 97 && c <= 122) || c == 95;
        }


};
