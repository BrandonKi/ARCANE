#include "Lexer.h"

Lexer::Lexer(const std::string& data):
    data(data), memPool(100), tokens(), index(0), lineNum(0), charNum(0)
{
    tokens.reserve(100);
}

void Lexer::lex(){
    while(index < data.size()){
        switch(data[index]){
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                number_lit();
            // default:
            //     peekNext();
        }
    }
}


inline Token* Lexer::number_lit(){
    std::string num("");
    bool isFloat = false;
    bool isHex = false;
    bool isInt = true;
    if(index+1 == 'x')
        isHex = true;
    while(isDigit(data[index])){
        index++;
        charNum++;
        switch(data[index]){
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                num.push_back(data[index]);
                break;
            case '.':
                if(isFloat)
                    continue;   //TODO print error message because two "." are present in number literal 
                isFloat = true;
                isHex = false;
                isInt = false;
                break;
            default:
                break;
        }
    }
    if(isInt)
        return allocToken(std::move(Token()));
    else if(isFloat)
        return allocToken(std::move(Token()));
    else
        return allocToken(std::move(Token()));
}

Token* Lexer::allocToken(Token tkn){
    Token* t = memPool.allocate();
    *t = tkn;
    return t;
}

inline Token* Lexer::next(){
    return nullptr;
}

inline Token* Lexer::peekNext(){   
    return nullptr;
}