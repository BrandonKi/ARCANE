#include "Lexer.h"

Lexer::Lexer(const std::string& data):
    data(data), memPool(100), tokens(), index(0), lineNum(1), charNum(1)
{
    tokens.reserve(100);
}

void Lexer::lex() {     // TODO return Token* instead
    while(index < data.size()){
        switch(currentChar()){
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
                printTokenln(*number_lit());
                break;
            case '\n':
                lineNum++;
                charNum = 0;
                break;
            default:
                break;
        }
            charNum++;
            index++;
    }
}


Token* Lexer::number_lit(){
    std::string num;
    num.reserve(10);
    u32 charPos = charNum;
    bool isFloat = false;
    bool isInt = true;
    if(peekNextChar() == 'x'){
        index++;
        isInt = false;
        isFloat = false;
    }
    while(isDigit(currentChar())) {
        if(isDigit(currentChar())){
            num.push_back(currentChar());
        }
        else if(currentChar() == '.'){
            if(isFloat)
                continue;   //TODO print error message because two "." are present in number literal 
            isFloat = true;
            isInt = false;
            num.push_back(currentChar());
        }
        else{
            //TODO error here. Invalid character in literal
        }
        charNum++;
        index++;
    }

    index--;

    if(isInt)
        return allocToken(std::move(Token{num.c_str(), ARC_INT_LIT, lineNum, charPos}));
    else if(isFloat)
        return allocToken(std::move(Token{num.c_str(), ARC_FLOAT_LIT, lineNum, charPos}));
    else{
        //TODO convert hex literal to int or float literal
        return allocToken(std::move(Token{num.c_str(), ARC_INT_LIT, lineNum, charPos}));
    }
}

Token* Lexer::allocToken(Token tkn){
    Token* t = memPool.allocate();
    *t = std::move(tkn);
    return t;
}

inline char Lexer::currentChar(){
    return data[index];
}

inline char Lexer::nextChar(){
    return data[++index];
}

inline char Lexer::peekNextChar(){   
    return data[index+1];
}