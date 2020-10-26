#include "Common.h"

class Lexer{

    public:
        Lexer(const std::string&);
        Token next();
        Token peekNext();

    private:
        const std::string& data;

};
