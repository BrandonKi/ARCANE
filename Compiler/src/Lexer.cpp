#include "Lexer.h"

Lexer::Lexer(const astring& filedata):
    data(filedata), tokens(arena_allocator<Token>{}), index(0), line(1), col(1)
{
    PROFILE();
    // errorLog.push(ErrorMessage{FATAL, new Token{astring("100"), (TokenKind)0, 0, 0}, astring("filename.txt"), astring("invalid token")});
    // errorLog.flush();
    tokens.reserve(100);
}

std::vector<Token, arena_allocator<Token>> Lexer::lex() {
    PROFILE();

    while(index < data.size()) {

        switch(currentChar()) {
            CASE_DIGIT:
                tokens.push_back(lexNumberLit());
                break;
            CASE_ID:
                tokens.push_back(lexIdentifier());
                break;
            case '\'':
            case '"': 
                tokens.push_back(lexString());
                break;
            case '`':   //TODO interpolated string literal is not trivial to tokenize
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
    if(args.lexOut)
        printTokens(args.verboseLexOut);  // true for verbose and false for succint
    tokens.push_back(createToken(ARC_EOF, col, index));
    return std::move(tokens);   // not a big deal but there is no reason to make an extra copy
}


void Lexer::consumeComment() {
    PROFILE();

    if(nextChar() == '*') {
        nextChar_noreturn();
        while(index < data.size() && !(currentChar() == '*' && peekNextChar() == '/')){
            if(currentChar() == '\n')
                line++;
            nextChar_noreturn();
        }
        nextChar_noreturn();
    }
    else{
        while(index < data.size() &&  currentChar() != '\n'){
            nextChar_noreturn();
        }
    }
}

Token Lexer::lexNumberLit() {
    PROFILE();
    astring& num = *new astring;
    u32 startPos = index;
    u32 currentCol = col;
    bool isFloat = false;
    bool isInt = true;
    if(peekNextChar() == 'x') {  //TODO implement hex literal lexing
        nextChar_noreturn();
        isInt = false;
        isFloat = false;
    }
    while(isDigit(currentChar())) {      //TODO refactor
        num.push_back(currentChar());

        if(peekNextChar() == '.') {
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
    else {
        //TODO convert hex literal to int or float literal
        return createToken(ARC_INT_LIT, currentCol, startPos, num);
    }
}

Token Lexer::lexIdentifier() {
    PROFILE();

    astring& id = *new astring;
    u32 startPos = index;
    u32 currentCol = col;

    while(isLetter(currentChar())  || isDigit(currentChar())) {
        id.push_back(currentChar());
        nextChar_noreturn();
    }

    prevChar_noreturn();    // the above loop goes one char too far so decrement here

    if(keywords.find(id) != keywords.end())
        return createToken(keywords.find(id)->second, currentCol, startPos);
    return createToken(ARC_ID, currentCol, startPos, id);
}

Token Lexer::lexString() {  //TODO escape sequences
    PROFILE();

    u32 startPos = index;

    astring& id = *new astring;
        
    char end = currentChar();

    while(nextChar() != end) {
        id.push_back(currentChar());
    }

    return createToken(ARC_STRING_LIT, startPos, id);
}

Token Lexer::lexInterpolatedString() {  //TODO implement interpolated strings
    // errorLog.push(ErrorMessage{FATAL, lexString(), args.path, astring("interpolated strings are not implemented yet stop trying to use them >.>")});
    // errorLog.flush();
    std::exit(-1);
    // return new Token{};
}

Token Lexer::lexColon() {
    PROFILE();

    u32 startPos = index;
    u32 startCol = col;
    
    if(peekNextChar() == '=') {
        nextChar_noreturn();
        return createToken(ARC_INFER, startCol, startPos);
    }
    else
        return createToken(ARC_COLON, startPos);

}

inline Token Lexer::lexAdd() {
    PROFILE();

    u32 startPos = index;
    u32 startCol = col;

    if(peekNextChar() == '=') {
        nextChar_noreturn();
        return createToken(ARC_ADD_EQUAL, startCol, startPos);
    }
    else if(peekNextChar() == '+') {
        nextChar_noreturn();
        if(tokens.back().kind == ARC_ID)  // FIXME incorrectly lexes some cases for ex. "4++", "*++4", etc.
            return createToken(ARC_POST_INCREMENT, startCol, startPos);
        return createToken(ARC_PRE_INCREMENT, startCol, startPos);
    }
    else
        return createToken(ARC_ADD, startPos);
}

Token Lexer::lexSub() {
    PROFILE();
    
    u32 startPos = index;
    u32 startCol = col;

    if(peekNextChar() == '=') {
        nextChar_noreturn();
        return createToken(ARC_SUB_EQUAL, startCol, startPos);
    }
    else if(peekNextChar() == '-') {
        nextChar_noreturn();
        if(tokens.back().kind == ARC_ID)   // FIXME incorrectly lexes some cases for ex. "4--", "*--4", etc.
            return createToken(ARC_POST_DECREMENT, startCol, startPos);
        return createToken(ARC_PRE_DECREMENT, startCol, startPos);
    }
    else
        return createToken(ARC_SUB, startPos);  // FIXME doesn't even attempt to parse a unary sub
}

Token Lexer::lexDiv() {
    PROFILE();
    
    u32 startPos = index;
    u32 startCol = col;

    if(peekNextChar() == '=') {
        nextChar_noreturn();
        return createToken(ARC_DIV_EQUAL, startCol, startPos);
    }
    else
        return createToken(ARC_DIV, startPos);
}

Token Lexer::lexMul() {
    PROFILE();
    
    u32 startPos = index;
    u32 startCol = col;

    if(peekNextChar() == '=') {
        nextChar_noreturn();
        return createToken(ARC_MUL_EQUAL, startCol, startPos);
    }
    else
        return createToken(ARC_MUL, startPos);
}

Token Lexer::lexMod() {
    PROFILE();
    
    u32 startPos = index;
    u32 startCol = col;

    if(peekNextChar() == '=') {
        nextChar_noreturn();
        return createToken(ARC_MOD_EQUAL, startCol, startPos);
    }
    else
        return createToken(ARC_MOD, startPos);
}

Token Lexer::lexOr() {
    PROFILE();
    
    u32 startPos = index;
    u32 startCol = col;

    if(peekNextChar() == '=') {
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

Token Lexer::lexAnd() {
    PROFILE();
    
    u32 startPos = index;
    u32 startCol = col;

    if(peekNextChar() == '=') {
        nextChar_noreturn();
        return createToken(ARC_AND_EQUAL, startCol, startPos);
    }
    else if(peekNextChar() == '&') {
        nextChar_noreturn();
        return createToken(ARC_LOGICAL_AND, startCol, startPos);
    }
    else
        return createToken(ARC_BIN_AND, startPos);
}

Token Lexer::lexNot() {
    PROFILE();
    
    u32 startPos = index;
    u32 startCol = col;

    if(peekNextChar() == '=') {
        nextChar_noreturn();
        return createToken(ARC_NOT_EQUAL, startCol, startPos);
    }
    else
        return createToken(ARC_NOT, startPos);
}

Token Lexer::lexXor(){
    PROFILE();

    u32 startPos = index;
    u32 startCol = col;

    if(peekNextChar() == '=') {
        nextChar_noreturn();
        return createToken(ARC_XOR_EQUAL, startCol, startPos);
    }
    else
        return createToken(ARC_XOR, startPos);
}

Token Lexer::lexLesser() {
    PROFILE();
    
    u32 startPos = index;
    u32 startCol = col;

    if(peekNextChar() == '=') {
        nextChar_noreturn();
        return createToken(ARC_LESSER_EQUAL, startCol, startPos);
    }
    else
        return createToken(ARC_LESSER, startPos);
}

Token Lexer::lexGreater() {
    PROFILE();
    
    u32 startPos = index;
    u32 startCol = col;

    if(peekNextChar() == '=') {
        nextChar_noreturn();
        return createToken(ARC_GREATER_EQUAL, startCol, startPos);
    }
    else
        return createToken(ARC_GREATER, startPos);
}

Token Lexer::lexEqual() {
    PROFILE();
    
    u32 startPos = index;
    u32 startCol = col;

    if(peekNextChar() == '=') {
        nextChar_noreturn();
        return createToken(ARC_EQUAL, startCol, startPos);
    }
    else
        return createToken(ARC_ASSIGN, startPos);
}

inline char Lexer::currentChar() {
    PROFILE();
    return data[index];
}

inline char Lexer::nextChar() {
    PROFILE();
    col++;
    return data[++index];
}

inline char Lexer::prevChar() {
    PROFILE();
    col--;
    return data[--index];
}

inline void Lexer::nextChar_noreturn() {
    PROFILE();
    col++;
    index++;
}

inline void Lexer::prevChar_noreturn() {
    PROFILE();
    col--;
    index--;
}

inline char Lexer::peekNextChar() {
    PROFILE();
    return data[index+1];
}

inline Token Lexer::createToken(TokenKind kind, u32 startPos) {
    PROFILE();
    // Token* tkn = allocator.alloc<Token>();
    // *tkn = Token {kind, SourcePos{line, col, startPos, index}, nullptr};
    return Token {kind, SourcePos{line, col, startPos, index}, nullptr};
}
 
inline Token Lexer::createToken(TokenKind kind, u32 currentCol, u32 startPos) {
    PROFILE();
    // Token* tkn = allocator.alloc<Token>();
    // *tkn = Token {kind, SourcePos{line, currentCol, startPos, index}, nullptr};
    return Token {kind, SourcePos{line, currentCol, startPos, index}, nullptr};
}

inline Token Lexer::createToken(TokenKind kind, u32 startPos, astring& val) {
    PROFILE();
    return createToken(kind, col, startPos, val);
}

inline Token Lexer::createToken(TokenKind kind, u32 startPos, astring&& val) {
    PROFILE();
    return createToken(kind, col, startPos, val);
}


inline Token Lexer::createToken(TokenKind kind, u32 currentCol, u32 startPos, astring& val) {   //FIXME no reason to pass a string here
    PROFILE();
    return Token {kind, SourcePos{line, currentCol, startPos, index}, &val};
}

inline Token Lexer::createToken(TokenKind kind, u32 currentCol, u32 startPos, astring&& val) {   //FIXME no reason to pass a string here
    PROFILE();
    astring* val2 = new astring(val);   //FIXME temporary
    return Token {kind, SourcePos{line, currentCol, startPos, index}, val2};
}

inline void Lexer::printTokens(bool verbose) {
    PROFILE();
    if(verbose)
        for(Token tkn : tokens)
            printTokenln(&tkn);
    else{
        for(Token tkn : tokens)
            if(tkn.data == nullptr)
                println(astrtostr(str(tkn.kind) + ": " + getStringRep(tkn.kind)));
            else
                println(astrtostr(str(tkn.kind) + ": " + *(tkn.data)));
    }
}