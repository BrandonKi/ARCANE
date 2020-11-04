#include "Lexer.h"

Lexer::Lexer(const std::string& filedata):
    data(filedata), tokens(), index(0), line(1), col(1), error(filedata)
{
    // error.log(ErrorMessage{FATAL, new Token{std::string("100"), (TokenKind)0, 0, 0}, std::string("filename.txt"), std::string("invalid token")});
    // error.flush();
    tokens.reserve(100);
}

void Lexer::lex(){     // TODO return Token* instead

    while(index < data.size()){

        switch(currentChar()){
            CASE_DIGIT:
                tokens.push_back(lexNumberLit());
                break;
            CASE_ID:
                tokens.push_back(lexIdentifier());
                break;
            case '\'':  //TODO ARC_STRING_LIT is not trivial to tokenize
            case '"':   //TODO ARC_STRING_LIT is not trivial to tokenize
                tokens.push_back(lexString());
                break;
            case '`':   //TODO interpolated string literals is not trivial to tokenize
                tokens.push_back(lexInterpolatedString());
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
                tokens.push_back(lexColon());
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
            case '^':   //TODO ARC_XOR is not trivial to tokenize
                tokens.push_back(createToken(std::string(1, currentChar()), ARC_XOR));
                break;
            case '<':   //TODO ARC_LESS is not trivial to tokenize
                tokens.push_back(createToken(std::string(1, currentChar()), ARC_LESSER));
                break;
            case '>':   //TODO ARC_GREATER is not trivial to tokenize
                tokens.push_back(createToken(std::string(1, currentChar()), ARC_GREATER));
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
            nextChar_noreturn();
    }

    printTokens(false);  // true for verbose false for succint
}


Token* Lexer::lexNumberLit(){
    std::string num;
    u32 currentCol = col;
    bool isFloat = false;
    bool isInt = true;
    if(peekNextChar() == 'x'){  //TODO implement hex literal lexing
        nextChar_noreturn();
        isInt = false;
        isFloat = false;
    }
    while(isDigit(currentChar())){      //TODO refactor
        num.push_back(currentChar());

        if(peekNextChar() == '.'){
            // if(isFloat) //TODO print error message because two "." are present in number literal
            isFloat = true;
            isInt = false;

            num.push_back(nextChar());
        }

        nextChar_noreturn();
    }

    prevChar_noreturn();    // the above loop goes one char too far so decrement here

    if(isInt)
        return createToken(num, ARC_INT_LIT, currentCol);
    else if(isFloat)
        return createToken(num, ARC_FLOAT_LIT, currentCol);
    else{
        //TODO convert hex literal to int or float literal
        return createToken(num, ARC_INT_LIT, currentCol);
    }
}

Token* Lexer::lexIdentifier(){

    std::string id;

    u32 currentCol = col;

    while(isAlpha(currentChar())  || isDigit(currentChar())){
        id.push_back(currentChar());
        nextChar_noreturn();
    }

    prevChar_noreturn();    // the above loop goes one char too far so decrement here

    if(keywords.find(id) != keywords.end())
        return createToken(id, keywords.find(id)->second, currentCol);
    return createToken(id, ARC_ID, currentCol);
}

Token* Lexer::lexString(){  //TODO escape sequences 

    std::string id;
        
    char end = currentChar();

    while(nextChar() != end){
        id.push_back(currentChar());
    }

    nextChar_noreturn();

    return createToken(id, ARC_STRING_LIT);
}

Token* Lexer::lexInterpolatedString(){  //TODO implement interpolated strings
    error.log(ErrorMessage{FATAL, lexString(), args.filepath, std::string("interpolated strings are not implemented yet stop trying to use them >.>")});
    error.flush();
    std::exit(-1);
    // return new Token{};
}

Token* Lexer::lexColon(){

    if(peekNextChar() == '='){
        nextChar_noreturn();
        return createToken(std::string(":="), ARC_INFER);
    }
    else
        return createToken(std::string(1, currentChar()), ARC_COLON);

}

Token* Lexer::lexAdd(){
    return new Token{};
}

Token* Lexer::lexSub(){
    return new Token{};
}

Token* Lexer::lexDiv(){
    return new Token{};
}

Token* Lexer::lexMul(){
    return new Token{};
}

Token* Lexer::lexMod(){
    return new Token{};
}

Token* Lexer::lexOr(){
    return new Token{};
}

Token* Lexer::lexAnd(){
    return new Token{};
}

Token* Lexer::lexNot(){
    return new Token{};
}

Token* Lexer::lexXor(){
    return new Token{};
}

Token* Lexer::lexLess(){
    return new Token{};
}

Token* Lexer::lexGreater(){
    return new Token{};
}

Token* Lexer::lexEqual(){
    return new Token{};
}




inline char Lexer::currentChar(){
    return data[index];
}

inline char Lexer::nextChar(){
    col++;
    return data[++index];
}

inline char Lexer::prevChar(){
    col--;
    return data[--index];
}

inline void Lexer::nextChar_noreturn(){
    col++;
    index++;
}

inline void Lexer::prevChar_noreturn(){
    col--;
    index--;
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