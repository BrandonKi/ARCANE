#include "Lexer.h"

Lexer::Lexer(const std::string& filedata):
    data(filedata), tokens(), index(0), line(1), col(1), errorLog(filedata)
{
    // errorLog.push(ErrorMessage{FATAL, new Token{std::string("100"), (TokenKind)0, 0, 0}, std::string("filename.txt"), std::string("invalid token")});
    // errorLog.flush();
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
                tokens.push_back(createToken(ARC_OPEN_BRACE, index));
                break;
            case '}':
                tokens.push_back(createToken(ARC_CLOSE_BRACE, index));
                break;
            case '[':
                tokens.push_back(createToken(ARC_OPEN_BRACKET, index));
                break;
            case ']':
                tokens.push_back(createToken(ARC_CLOSE_BRACKET, index));
                break;
            case '(':
                tokens.push_back(createToken(ARC_OPEN_PAREN, index));
                break;
            case ')':
                tokens.push_back(createToken(ARC_CLOSE_PAREN, index));
                break;
            case '.':
                tokens.push_back(createToken(ARC_DOT, index));
                break;
            case ',':
                tokens.push_back(createToken(ARC_COMMA, index));
                break;
            case '?':
                tokens.push_back(createToken(ARC_TERNARY, index));
                break;
            case ';':
                tokens.push_back(createToken(ARC_SEMICOLON, index));
                break;
            case ':':
                tokens.push_back(lexColon());
                break;
            case '@':
                tokens.push_back(createToken(ARC_AT, index));
                break;
            case '#':
                tokens.push_back(createToken(ARC_HASH, index));
                break;
            case '$':
                tokens.push_back(createToken(ARC_DOLLAR, index));
                break;
            case '+':
                tokens.push_back(lexAdd());
                break;
            case '-':
                tokens.push_back(lexSub());
                break;
            case '/':
                if(peekNextChar() == '/' || peekNextChar() == '*')
                    consumeComment();
                else
                    tokens.push_back(lexDiv());
                break;
            case '*':
                tokens.push_back(lexMul());
                break;
            case '%':
                tokens.push_back(lexMod());
                break;
            case '|':
                tokens.push_back(lexOr());
                break;
            case '&':
                tokens.push_back(lexAnd());
                break;
            case '!':
                tokens.push_back(lexNot());
                break;
            case '^':
                tokens.push_back(lexXor());
                break;
            case '<':
                tokens.push_back(lexLesser());
                break;
            case '>':
                tokens.push_back(lexGreater());
                break;
            case '=':
                tokens.push_back(lexEqual());
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
    if(args.LexOut)
        printTokens(args.verboseLexOut);  // true for verbose false for succint
}


void Lexer::consumeComment(){

    if(nextChar() == '*'){
        nextChar_noreturn();
        while(!(currentChar() == '*' && peekNextChar() == '/')){    // TODO possible to run out of the bounds of the file here
            if(currentChar() == '\n')
                line++;
            nextChar_noreturn();
        }
        nextChar_noreturn();
    }
    else{
        while(currentChar() != '\n'){
            nextChar_noreturn();
        }
    }
}

Token* Lexer::lexNumberLit(){
    std::string num;
    u32 startPos = index;
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
        return createToken(ARC_INT_LIT, currentCol, startPos, num);
    else if(isFloat)
        return createToken(ARC_FLOAT_LIT, currentCol, startPos, num);
    else{
        //TODO convert hex literal to int or float literal
        return createToken(ARC_INT_LIT, currentCol, startPos, num);
    }
}

Token* Lexer::lexIdentifier(){

    std::string id;
    u32 startPos = index;
    u32 currentCol = col;

    while(isAlpha(currentChar())  || isDigit(currentChar())){
        id.push_back(currentChar());
        nextChar_noreturn();
    }

    prevChar_noreturn();    // the above loop goes one char too far so decrement here

    if(keywords.find(id) != keywords.end())
        return createToken(keywords.find(id)->second, currentCol, startPos);
    return createToken(ARC_ID, currentCol, startPos, id);
}

Token* Lexer::lexString(){  //TODO escape sequences

    u32 startPos = index;

    std::string id;
        
    char end = currentChar();

    while(nextChar() != end){
        id.push_back(currentChar());
    }

    return createToken(ARC_STRING_LIT, startPos, id);
}

Token* Lexer::lexInterpolatedString(){  //TODO implement interpolated strings
    errorLog.push(ErrorMessage{FATAL, lexString(), args.filepath, std::string("interpolated strings are not implemented yet stop trying to use them >.>")});
    errorLog.flush();
    std::exit(-1);
    // return new Token{};
}

Token* Lexer::lexColon(){

    u32 startPos = index;
    u32 startCol = col;
    
    if(peekNextChar() == '='){
        nextChar_noreturn();
        return createToken(ARC_INFER, startCol, startPos);
    }
    else
        return createToken(ARC_COLON, startPos);

}

inline Token* Lexer::lexAdd(){

    u32 startPos = index;
    u32 startCol = col;

    if(peekNextChar() == '='){
        nextChar_noreturn();
        return createToken(ARC_ADD_EQUAL, startCol, startPos);
    }
    else if(peekNextChar() == '+'){
        nextChar_noreturn();
        if(tokens.back()->kind == ARC_ID)  // FIXME incorrectly lexes some cases for ex. "4++", "*++4", etc.
            return createToken(ARC_POST_INCREMENT, startCol, startPos);
        return createToken(ARC_PRE_INCREMENT, startCol, startPos);
    }
    else
        return createToken(ARC_ADD, startPos);
}

Token* Lexer::lexSub(){
    
    u32 startPos = index;
    u32 startCol = col;

    if(peekNextChar() == '='){
        nextChar_noreturn();
        return createToken(ARC_SUB_EQUAL, startCol, startPos);
    }
    else if(peekNextChar() == '-'){
        nextChar_noreturn();
        if(tokens.back()->kind == ARC_ID)   // FIXME incorrectly lexes some cases for ex. "4--", "*--4", etc.
            return createToken(ARC_POST_DECREMENT, startCol, startPos);
        return createToken(ARC_PRE_DECREMENT, startCol, startPos);
    }
    else
        return createToken(ARC_ADD, startPos);  // FIXME doesn't even attempt to parse a unary sub
}

Token* Lexer::lexDiv(){
    
    u32 startPos = index;
    u32 startCol = col;

    if(peekNextChar() == '='){
        nextChar_noreturn();
        return createToken(ARC_DIV_EQUAL, startCol, startPos);
    }
    else
        return createToken(ARC_DIV, startPos);
}

Token* Lexer::lexMul(){
    
    u32 startPos = index;
    u32 startCol = col;

    if(peekNextChar() == '='){
        nextChar_noreturn();
        return createToken(ARC_MUL_EQUAL, startCol, startPos);
    }
    else
        return createToken(ARC_MUL, startPos);
}

Token* Lexer::lexMod(){
    
    u32 startPos = index;
    u32 startCol = col;

    if(peekNextChar() == '='){
        nextChar_noreturn();
        return createToken(ARC_MOD_EQUAL, startCol, startPos);
    }
    else
        return createToken(ARC_MOD, startPos);
}

Token* Lexer::lexOr(){
    
    u32 startPos = index;
    u32 startCol = col;

    if(peekNextChar() == '='){
        nextChar_noreturn();
        return createToken(ARC_OR_EQUAL, startCol, startPos);
    }
    else if(peekNextChar() == '|'){
        nextChar_noreturn();
        return createToken(ARC_LOGICAL_OR, startCol, startPos);
    }
    else
        return createToken(ARC_BIN_OR, startPos);
}

Token* Lexer::lexAnd(){
    
    u32 startPos = index;
    u32 startCol = col;

    if(peekNextChar() == '='){
        nextChar_noreturn();
        return createToken(ARC_AND_EQUAL, startCol, startPos);
    }
    else if(peekNextChar() == '&'){
        nextChar_noreturn();
        return createToken(ARC_LOGICAL_AND, startCol, startPos);
    }
    else
        return createToken(ARC_BIN_AND, startPos);
}

Token* Lexer::lexNot(){
    
    u32 startPos = index;
    u32 startCol = col;

    if(peekNextChar() == '='){
        nextChar_noreturn();
        return createToken(ARC_NOT_EQUAL, startCol, startPos);
    }
    else
        return createToken(ARC_NOT, startPos);
}

Token* Lexer::lexXor(){
    
    u32 startPos = index;
    u32 startCol = col;

    if(peekNextChar() == '='){
        nextChar_noreturn();
        return createToken(ARC_XOR_EQUAL, startCol, startPos);
    }
    else
        return createToken(ARC_XOR, startPos);
}

Token* Lexer::lexLesser(){
    
    u32 startPos = index;
    u32 startCol = col;

    if(peekNextChar() == '='){
        nextChar_noreturn();
        return createToken(ARC_LESSER_EQUAL, startCol, startPos);
    }
    else
        return createToken(ARC_LESSER, startPos);
}

Token* Lexer::lexGreater(){
    
    u32 startPos = index;
    u32 startCol = col;

    if(peekNextChar() == '='){
        nextChar_noreturn();
        return createToken(ARC_GREATER_EQUAL, startCol, startPos);
    }
    else
        return createToken(ARC_GREATER, startPos);
}

Token* Lexer::lexEqual(){
    
    u32 startPos = index;
    u32 startCol = col;

    if(peekNextChar() == '='){
        nextChar_noreturn();
        return createToken(ARC_EQUAL, startCol, startPos);
    }
    else
        return createToken(ARC_ASSIGN, startPos);
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

inline Token* Lexer::createToken(TokenKind kind, u32 startPos){
    Token* tkn = new Token {kind, line, col, startPos, index, std::string()};
    return tkn;
}

inline Token* Lexer::createToken(TokenKind kind, u32 currentCol, u32 startPos){
    Token* tkn = new Token {kind, line, currentCol, startPos, index, std::string()};
    return tkn;
}

inline Token* Lexer::createToken(TokenKind kind, u32 startPos, std::string& val){
    return createToken(kind, col, startPos, val);
}

inline Token* Lexer::createToken(TokenKind kind, u32 startPos, std::string&& val){
    return createToken(kind, col, startPos, val);
}


inline Token* Lexer::createToken(TokenKind kind, u32 currentCol, u32 startPos, std::string& val){
    Token* tkn = new Token {kind, line, currentCol, startPos, index, val};
    return tkn;
}

inline Token* Lexer::createToken(TokenKind kind, u32 currentCol, u32 startPos, std::string&& val){
    Token* tkn = new Token {kind, line, currentCol, startPos, index, val};
    return tkn;
}

inline void Lexer::printTokens(bool verbose){
    if(verbose)
        for(Token* tkn : tokens)
            printTokenln(tkn);
    else{
        for(Token* tkn : tokens)
            if(tkn->data == std::string(""))
                println(str(tkn->kind) + ": " + getStringRep(tkn->kind));
            else
                println(str(tkn->kind) + ": " + tkn->data);
    }
}