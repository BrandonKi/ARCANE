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
            case '{':
                tokens.push_back(createToken(std::string(1, currentChar()), ARC_OPEN_BRACE));
                break;
            case '}':
                tokens.push_back(createToken(std::string(1, currentChar()), ARC_CLOSE_BRACE));
                break;
            case '[':
                tokens.push_back(createToken(std::string(1, currentChar()), ARC_OPEN_BRACKET));
                break;
            case ']':
                tokens.push_back(createToken(std::string(1, currentChar()), ARC_CLOSE_BRACKET));
                break;
            case '(':
                tokens.push_back(createToken(std::string(1, currentChar()), ARC_OPEN_PAREN));
                break;
            case ')':
                tokens.push_back(createToken(std::string(1, currentChar()), ARC_CLOSE_PAREN));
                break;
            case '.':
                tokens.push_back(createToken(std::string(1, currentChar()), ARC_DOT));
                break;
            case ',':
                tokens.push_back(createToken(std::string(1, currentChar()), ARC_COMMA));
                break;
            case '?':
                tokens.push_back(createToken(std::string(1, currentChar()), ARC_TERNARY));
                break;
            case ';':
                tokens.push_back(createToken(std::string(1, currentChar()), ARC_SEMICOLON));
                break;
            case ':':   //TODO ARC_COLON is not trivial to tokenize
                tokens.push_back(createToken(std::string(1, currentChar()), ARC_COLON));
                break;
            case '@':
                tokens.push_back(createToken(std::string(1, currentChar()), ARC_AT));
                break;
            case '#':
                tokens.push_back(createToken(std::string(1, currentChar()), ARC_HASH));
                break;
            case '$':
                tokens.push_back(createToken(std::string(1, currentChar()), ARC_DOLLAR));
                break;
            case '+':   //TODO ARC_ADD is not trivial to tokenize
                tokens.push_back(createToken(std::string(1, currentChar()), ARC_ADD));
                break;
            case '-':   //TODO ARC_SUB is not trivial to tokenize
                tokens.push_back(createToken(std::string(1, currentChar()), ARC_SUB));
                break;
            case '/':   //TODO ARC_DIV is not trivial to tokenize
                tokens.push_back(createToken(std::string(1, currentChar()), ARC_DIV));
                break;
            case '*':   //TODO ARC_MUL is not trivial to tokenize
                tokens.push_back(createToken(std::string(1, currentChar()), ARC_MUL));
                break;
            case '%':   //TODO ARC_MOD is not trivial to tokenize
                tokens.push_back(createToken(std::string(1, currentChar()), ARC_MOD));
                break;
            case '|':   //TODO ARC_OR is not trivial to tokenize
                tokens.push_back(createToken(std::string(1, currentChar()), ARC_BIN_OR));
                break;
            case '&':   //TODO ARC_AND is not trivial to tokenize
                tokens.push_back(createToken(std::string(1, currentChar()), ARC_BIN_AND));
                break;
            case '!':   //TODO ARC_NOT is not trivial to tokenize
                tokens.push_back(createToken(std::string(1, currentChar()), ARC_NOT));
                break;
            case '<':   //TODO ARC_LESS is not trivial to tokenize
                tokens.push_back(createToken(std::string(1, currentChar()), ARC_LESS));
                break;
            case '>':   //TODO ARC_GREATER is not trivial to tokenize
                tokens.push_back(createToken(std::string(1, currentChar()), ARC_GREATER));
                break;
            case '^':   //TODO ARC_XOR is not trivial to tokenize
                tokens.push_back(createToken(std::string(1, currentChar()), ARC_XOR));
                break;
            case '=':   //TODO ARC_EQUAL is not trivial to tokenize
                tokens.push_back(createToken(std::string(1, currentChar()), ARC_ASSIGN));
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

inline Token* Lexer::createToken(std::string& id, TokenKind kind){
    return new Token {id, kind, line, col};
}

inline Token* Lexer::createToken(std::string&& id, TokenKind kind){
    return new Token {id, kind, line, col};
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