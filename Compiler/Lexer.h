#include "Token.h"

#define CASE_VALID_ID   case 'A':case 'B':case 'C':case 'D':case 'E':case 'F':case 'G':case 'H':case 'I': \
                        case 'J':case 'K':case 'L':case 'M':case 'N':case 'O':case 'P':case 'Q':case 'R': \
                        case 'S':case 'T':case 'U':case 'V':case 'W':case 'X':case 'Y':case 'Z':case 'a': \
                        case 'b':case 'c':case 'd':case 'e':case 'f':case 'g':case 'h':case 'i':case 'j': \
                        case 'k':case 'l':case 'm':case 'n':case 'o':case 'p':case 'q':case 'r':case 's': \
                        case 't':case 'u':case 'v':case 'w':case 'x':case 'y':case 'z':case '_'
#define CASE_DIGITS     case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9'

class Lexer{        
    private:
        unsigned int m_line;
        unsigned int m_char;
        const char* m_filedata;
        unsigned int m_length;
        std::unordered_map<std::string, int> m_keywords;
        std::vector<Token*> m_tokens;
        std::vector<char> m_ctoken;

    public:
        Lexer(const char*, unsigned int);
        std::vector<Token*> getTokens(){return m_tokens;}
        std::unordered_map<std::string, int> getKeywords(){return m_keywords;}
        
    private:
        void start();
        unsigned int handleID(unsigned int);
        unsigned int handleDigit(unsigned int);
        unsigned int handleDoubleQuoteString(unsigned int);
        unsigned int handleSingleQuoteString(unsigned int);

        unsigned int handleEquals(unsigned int);
        unsigned int handleLessThan(unsigned int);
        unsigned int handleGreaterThan(unsigned int);
        unsigned int handleSub(unsigned int);
        unsigned int handleAdd(unsigned int);
        unsigned int handleDiv(unsigned int);
        unsigned int handleMul(unsigned int);
        unsigned int handleMod(unsigned int);
        unsigned int handleBinAnd(unsigned int);
        unsigned int handleBinOr(unsigned int);
        unsigned int handleComment(unsigned int);

        void printTokens();

        void initKeywords();
        bool isKeyword(std::string&);
        Keyword getVal(std::string&);
}; 