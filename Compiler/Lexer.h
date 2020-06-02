#include "commonInclude.h"

// #define DEBUG_BUILD
#define RELEASE_BUILD

class Lexer{

    private:
        unsigned int m_length;
        const char* m_buf;
        std::vector<Token> tokens;
        std::vector<char> currentToken;

    public:
        Lexer(const char* buf, unsigned int length):m_buf(buf),m_length(length){tokens.reserve(300);currentToken.reserve(50);start();}
        ~Lexer(){delete[] m_buf;}                                     // IMPORTANT! delete m_buf if not needed later
        std::vector<Token> getTokens(){return tokens;}

    private:
        void start();
        unsigned int handleDigit(unsigned int);
        unsigned int handleString(unsigned int);
        unsigned int handleName(unsigned int);
        unsigned int handleEquals(unsigned int);
        unsigned int handleComment(unsigned int);
        void printTokens();
};