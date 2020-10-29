#include "Parser.h"

Parser::Parser(const std::string& code):
    data(code)
{
    /* nothing to see here :) */
}

void Parser::parse(){
    Lexer lexer(data);
    lexer.lex();
}