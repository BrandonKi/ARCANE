#include "Token.h"

#define CASE_VALID_NAME_CHARACTERS case 'A':case 'B':case 'C':case 'D':case 'E':case 'F':case 'G':case 'H':case 'I': \
                                   case 'J':case 'K':case 'L':case 'M':case 'N':case 'O':case 'P':case 'Q':case 'R': \
                                   case 'S':case 'T':case 'U':case 'V':case 'W':case 'X':case 'Y':case 'Z':case 'a': \
                                   case 'b':case 'c':case 'd':case 'e':case 'f':case 'g':case 'h':case 'i':case 'j': \
                                   case 'k':case 'l':case 'm':case 'n':case 'o':case 'p':case 'q':case 'r':case 's': \
                                   case 't':case 'u':case 'v':case 'w':case 'x':case 'y':case 'z':case '_':

class Lexer{        
    private:
        std::vector<Token*> tokens;

    public:
        Lexer(const char*, unsigned int);
        std::vector<Token*> getTokens(){return tokens;}
        
        
}; 