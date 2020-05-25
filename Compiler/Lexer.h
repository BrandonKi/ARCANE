#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>

#define CASE_VALID_NAME_CHARACTERS case 'A': case 'B':case 'C':case 'D':case 'E':case 'F':case 'G':case 'H':case 'I':case 'J':case 'K':case 'L':case 'M':case 'N':case 'O':case 'P':case 'Q':case 'R':case 'S':case 'T':case 'U':case 'V':case 'W':case 'X':case 'Y':case 'Z':case 'a':case 'b':case 'c':case 'd':case 'e':case 'f':case 'g':case 'h':case 'i':case 'j':case 'k':case 'l':case 'm':case 'n':case 'o':case 'p':case 'q':case 'r':case 's':case 't':case 'u':case 'v':case 'w':case 'x':case 'y':case 'z':case '_':
#define CASE_DIGITS case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':

class Lexer{

enum TokenType{OPERATOR, NUMBER, STRING, PAREN, COMMA};
    struct Token{
        TokenType type;
        const char* data;
    };
    private:
        unsigned int m_length;
        const char* m_buf;
        std::vector<Token> tokens;
        std::vector<char> currentToken;

    public:
        inline Lexer(const char* buf, unsigned int length):m_buf(buf),m_length(length){tokens.reserve(300);currentToken.reserve(50);};
        // inline ~Lexer();                                     // IMPORTANT! delete m_buf if not needed later
        void start();


    private:
        unsigned int handleDigit(unsigned int);
        unsigned int handleString(unsigned int);
        unsigned int handleName(unsigned int);
        void printTokens();
};