#include "Parser.h"

using namespace pLog;

Parser::Parser(const std::string& code):
    data(code)
{
    /* nothing to see here :) */
}

void Parser::parse(){
    print(args.filepath);
    Lexer lexer(data);
    lexer.lex();
}