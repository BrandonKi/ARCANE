#include "Lexer.h"

Lexer::Lexer(const std::string& data):
    data(data), index(0), lineNum(0), charNum(0)
{

}

Token Lexer::next(){
    Token temp = std::move(peekNext());
    index++;
    return std::move(temp);
}

inline Token Lexer::peekNext(){
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
            return number_lit();
        // default:
        //     peekNext();
    }
    return nullptr;
}

inline Token Lexer::number(){
    std::string num;
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
                num.append(data[index]);
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
        return Token();
    else if(isFloat)
        return Token();
    else
        return Token();
}

