#include "Lexer.h"

Lexer::Lexer(const std::string& filedata):
    data(filedata), tokens(), index(0), line(1), col(1), error(filedata)
{
    // error.log(ErrorMessage{FATAL, new Token{std::string("100"), (TokenKind)0, 0, 0}, std::string("filename.txt"), std::string("invalid token")});
    // error.flush();
    tokens.reserve(100);
}

void Lexer::lex() {     // TODO return Token* instead

    while(index < data.size()){

        switch(currentChar()){
            CASE_DIGIT:
                tokens.push_back(number_lit());
                break;
            CASE_ID:
                tokens.push_back(identifier());
                break;
            case '\n':
                line++;
                col = 0;
                break;
            default:
                break;
        }
            col++;
            index++;
    }

    printTokens(false);
}


Token* Lexer::number_lit(){
    std::string num;
    u32 currentCol = col;
    bool isFloat = false;
    bool isInt = true;
    if(peekNextChar() == 'x'){  //TODO implement hex literal lexing
        index++;
        col++;
        isInt = false;
        isFloat = false;
    }
    while(isDigit(currentChar())) {
        num.push_back(currentChar());

        if(peekNextChar() == '.'){
            // if(isFloat) //TODO print error message because two "." are present in number literal
            isFloat = true;
            isInt = false;

            num.push_back(nextChar());
        }

        col++;
        index++;
    }

    col--;
    index--;

    if(isInt)
        return createToken(num, ARC_INT_LIT, currentCol);
    else if(isFloat)
        return createToken(num, ARC_FLOAT_LIT, currentCol);
    else{
        //TODO convert hex literal to int or float literal
        return createToken(num, ARC_INT_LIT, currentCol);
    }
}

Token* Lexer::identifier(){

    std::string id;

    u32 currentCol = col;

    while(isAlpha(currentChar())  || isDigit(currentChar())){
        id.push_back(currentChar());
        col++;
        index++;
    }

    col--;
    index--;

    if(keywords.find(id) != keywords.end())
        return createToken(id, keywords.find(id)->second, currentCol);
    return createToken(id, ARC_ID, currentCol);
}

inline char Lexer::currentChar(){
    return data[index];
}

inline char Lexer::nextChar(){
    col++;
    return data[++index];
}

inline char Lexer::peekNextChar(){   
    return data[index+1];
}

inline Token* Lexer::createToken(std::string& id, TokenKind kind, u32 currentCol){
    return new Token {id, kind, line, currentCol};
}

void Lexer::printTokens(bool verbose){
    if(verbose)
        for(Token*& tkn : tokens)
            printTokenln(*tkn);
    else{
        for(Token*& tkn : tokens)
            println(str(tkn->kind) + ": " + tkn->id);
    }
}