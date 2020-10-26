#include "Lexer.h"

class Parser{

    public:
        Parser(const std::string& data);
        void parse();   //TODO figure out what IR I'm going to use and replace return type with it

    private:
        const std::string& data;
        
};