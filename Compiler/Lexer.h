#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>

// #define DEBUG_BUILD
#define RELEASE_BUILD

#define CASE_VALID_NAME_CHARACTERS case 'A':case 'B':case 'C':case 'D':case 'E':case 'F':case 'G':case 'H':case 'I': \
                                   case 'J':case 'K':case 'L':case 'M':case 'N':case 'O':case 'P':case 'Q':case 'R': \
                                   case 'S':case 'T':case 'U':case 'V':case 'W':case 'X':case 'Y':case 'Z':case 'a': \
                                   case 'b':case 'c':case 'd':case 'e':case 'f':case 'g':case 'h':case 'i':case 'j': \
                                   case 'k':case 'l':case 'm':case 'n':case 'o':case 'p':case 'q':case 'r':case 's': \
                                   case 't':case 'u':case 'v':case 'w':case 'x':case 'y':case 'z':case '_':
#define CASE_DIGITS case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':

#ifdef DEBUG_BUILD
    #define DEBUG_PRINT_TOKENS std::cout << "\n"; printTokens()
#else
    #define DEBUG_PRINT_TOKENS
#endif

enum TokenType{OPERATOR, NUMBER, STRING, PAREN, COMMA, NAME, COMPARISON};
    
struct Token{
    TokenType type;
    std::string data;
};

class Lexer{

    private:
        unsigned int m_length;
        const char* m_buf;
        std::vector<Token> tokens;
        std::vector<char> currentToken;

    public:
        Lexer(const char* buf, unsigned int length):m_buf(buf),m_length(length){tokens.reserve(300);currentToken.reserve(50);};
        ~Lexer(){delete[] m_buf;}                                     // IMPORTANT! delete m_buf if not needed later
        void start();
        std::vector<Token> getTokens(){return tokens;}

    private:
        unsigned int handleDigit(unsigned int);
        unsigned int handleString(unsigned int);
        unsigned int handleName(unsigned int);
        unsigned int handleEquals(unsigned int);
        unsigned int handleComment(unsigned int);
        void printTokens();
};