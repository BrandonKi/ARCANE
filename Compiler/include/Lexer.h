#include "MemoryPool.h"

class Lexer{

    public:
        Lexer(const std::string&);
        void lex();
        Token* next();
        Token* peekNext();

    private:
        const std::string& data;
        MemoryPool<Token> memPool;
        std::vector<Token*> tokens;
        u32 index;
        u32 lineNum;
        u32 charNum;

        Token* number_lit();

        Token* allocToken(Token);

        constexpr static inline bool isDigit(char c){
            return c >= 48 && c <= 57;
        }

        constexpr static inline bool isAlpha(char c){
            return (c >= 65 && c <= 90) || (c >= 97 && c <= 122) || c == 95;
        }


};
