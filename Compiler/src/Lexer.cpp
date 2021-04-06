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

        switch(current_char()) {
            CASE_DIGIT:
                tokens.push_back(lex_number_lit());
                break;
            CASE_ID:
                tokens.push_back(lex_identifier());
                break;
            case '\'':
            case '"': 
                tokens.push_back(lex_string());
                break;
            case '`':   //TODO interpolated string literal is not trivial to tokenize
                tokens.push_back(lex_interpolated_string());
                break;
            case '{':
                tokens.push_back(create_token(ARC_OPEN_BRACE, index));
                break;
            case '}':
                tokens.push_back(create_token(ARC_CLOSE_BRACE, index));
                break;
            case '[':
                tokens.push_back(create_token(ARC_OPEN_BRACKET, index));
                break;
            case ']':
                tokens.push_back(create_token(ARC_CLOSE_BRACKET, index));
                break;
            case '(':
                tokens.push_back(create_token(ARC_OPEN_PAREN, index));
                break;
            case ')':
                tokens.push_back(create_token(ARC_CLOSE_PAREN, index));
                break;
            case '.':
                tokens.push_back(create_token(ARC_DOT, index));
                break;
            case ',':
                tokens.push_back(create_token(ARC_COMMA, index));
                break;
            case '?':
                tokens.push_back(create_token(ARC_TERNARY, index));
                break;
            case ';':
                tokens.push_back(create_token(ARC_SEMICOLON, index));
                break;
            case ':':
                tokens.push_back(lex_colon());
                break;
            case '@':
                tokens.push_back(create_token(ARC_AT, index));
                break;
            case '#':
                tokens.push_back(create_token(ARC_HASH, index));
                break;
            case '$':
                tokens.push_back(create_token(ARC_DOLLAR, index));
                break;
            case '+':
                tokens.push_back(lex_add());
                break;
            case '-':
                tokens.push_back(lex_sub());
                break;
            case '/':
                if(peek_next_char() == '/' || peek_next_char() == '*')
                    consume_comment();
                else
                    tokens.push_back(lex_div());
                break;
            case '*':
                tokens.push_back(lex_mul());
                break;
            case '%':
                tokens.push_back(lex_mod());
                break;
            case '|':
                tokens.push_back(lex_or());
                break;
            case '&':
                tokens.push_back(lex_and());
                break;
            case '!':
                tokens.push_back(lex_not());
                break;
            case '^':
                tokens.push_back(lex_xor());
                break;
            case '<':
                tokens.push_back(lex_lesser());
                break;
            case '>':
                tokens.push_back(lex_greater());
                break;
            case '=':
                tokens.push_back(lex_equal());
                break;
            case '\n':
                line++;
                col = 0;
                break;
            default:
                break;
        }
            next_char_noreturn();
    }
    if(args.lexOut)
        print_tokens(args.verboseLexOut);  // true for verbose and false for succint
    tokens.push_back(create_token(ARC_EOF, col, index));
    return std::move(tokens);   // not a big deal but there is no reason to make an extra copy
}


void Lexer::consume_comment() {
    PROFILE();

    if(next_char() == '*') {
        next_char_noreturn();
        while(index < data.size() && !(current_char() == '*' && peek_next_char() == '/')){
            if(current_char() == '\n')
                line++;
            next_char_noreturn();
        }
        next_char_noreturn();
    }
    else{
        while(index < data.size() &&  current_char() != '\n'){
            next_char_noreturn();
        }
    }
}

Token Lexer::lex_number_lit() {
    PROFILE();
    astring& num = *new astring;
    u32 startPos = index;
    u32 currentCol = col;
    bool isFloat = false;
    bool isInt = true;
    if(peek_next_char() == 'x') {  //TODO implement hex literal lexing
        next_char_noreturn();
        isInt = false;
        isFloat = false;
    }
    while(is_digit(current_char())) {      //TODO refactor
        num.push_back(current_char());

        if(peek_next_char() == '.') {
            // if(isFloat) //TODO print error message because two "." are present in number literal
            isFloat = true;
            isInt = false;

            num.push_back(next_char());
        }

        next_char_noreturn();
    }

    prev_char_noreturn();    // the above loop goes one char too far so decrement here

    if(isInt)
        return create_token(ARC_INT_LIT, currentCol, startPos, num);
    else if(isFloat)
        return create_token(ARC_FLOAT_LIT, currentCol, startPos, num);
    else {
        //TODO convert hex literal to int or float literal
        return create_token(ARC_INT_LIT, currentCol, startPos, num);
    }
}

Token Lexer::lex_identifier() {
    PROFILE();

    astring& id = *new astring;
    u32 startPos = index;
    u32 currentCol = col;

    while(is_letter(current_char())  || is_digit(current_char())) {
        id.push_back(current_char());
        next_char_noreturn();
    }

    prev_char_noreturn();    // the above loop goes one char too far so decrement here

    if(keywords.find(id) != keywords.end())
        return create_token(keywords.find(id)->second, currentCol, startPos);
    return create_token(ARC_ID, currentCol, startPos, id);
}

Token Lexer::lex_string() {  //TODO escape sequences
    PROFILE();

    u32 startPos = index;

    astring& id = *new astring;
        
    char end = current_char();

    while(next_char() != end) {
        id.push_back(current_char());
    }

    return create_token(ARC_STRING_LIT, startPos, id);
}

Token Lexer::lex_interpolated_string() {  //TODO implement interpolated strings
    // errorLog.push(ErrorMessage{FATAL, lexString(), args.path, astring("interpolated strings are not implemented yet stop trying to use them >.>")});
    // errorLog.flush();
    std::exit(-1);
    // return new Token{};
}

Token Lexer::lex_colon() {
    PROFILE();

    u32 startPos = index;
    u32 startCol = col;
    
    if(peek_next_char() == '=') {
        next_char_noreturn();
        return create_token(ARC_INFER, startCol, startPos);
    }
    else
        return create_token(ARC_COLON, startPos);

}

inline Token Lexer::lex_add() {
    PROFILE();

    u32 startPos = index;
    u32 startCol = col;

    if(peek_next_char() == '=') {
        next_char_noreturn();
        return create_token(ARC_ADD_EQUAL, startCol, startPos);
    }
    else if(peek_next_char() == '+') {
        next_char_noreturn();
        if(tokens.back().kind == ARC_ID)  // FIXME incorrectly lexes some cases for ex. "4++", "*++4", etc.
            return create_token(ARC_POST_INCREMENT, startCol, startPos);
        return create_token(ARC_PRE_INCREMENT, startCol, startPos);
    }
    else
        return create_token(ARC_ADD, startPos);
}

Token Lexer::lex_sub() {
    PROFILE();
    
    u32 startPos = index;
    u32 startCol = col;

    if(peek_next_char() == '=') {
        next_char_noreturn();
        return create_token(ARC_SUB_EQUAL, startCol, startPos);
    }
    else if(peek_next_char() == '-') {
        next_char_noreturn();
        if(tokens.back().kind == ARC_ID)   // FIXME incorrectly lexes some cases for ex. "4--", "*--4", etc.
            return create_token(ARC_POST_DECREMENT, startCol, startPos);
        return create_token(ARC_PRE_DECREMENT, startCol, startPos);
    }
    else
        return create_token(ARC_SUB, startPos);  // FIXME doesn't even attempt to parse a unary sub
}

Token Lexer::lex_div() {
    PROFILE();
    
    u32 startPos = index;
    u32 startCol = col;

    if(peek_next_char() == '=') {
        next_char_noreturn();
        return create_token(ARC_DIV_EQUAL, startCol, startPos);
    }
    else
        return create_token(ARC_DIV, startPos);
}

Token Lexer::lex_mul() {
    PROFILE();
    
    u32 startPos = index;
    u32 startCol = col;

    if(peek_next_char() == '=') {
        next_char_noreturn();
        return create_token(ARC_MUL_EQUAL, startCol, startPos);
    }
    else
        return create_token(ARC_MUL, startPos);
}

Token Lexer::lex_mod() {
    PROFILE();
    
    u32 startPos = index;
    u32 startCol = col;

    if(peek_next_char() == '=') {
        next_char_noreturn();
        return create_token(ARC_MOD_EQUAL, startCol, startPos);
    }
    else
        return create_token(ARC_MOD, startPos);
}

Token Lexer::lex_or() {
    PROFILE();
    
    u32 startPos = index;
    u32 startCol = col;

    if(peek_next_char() == '=') {
        next_char_noreturn();
        return create_token(ARC_OR_EQUAL, startCol, startPos);
    }
    else if(peek_next_char() == '|'){
        next_char_noreturn();
        return create_token(ARC_LOGICAL_OR, startCol, startPos);
    }
    else
        return create_token(ARC_BIN_OR, startPos);
}

Token Lexer::lex_and() {
    PROFILE();
    
    u32 startPos = index;
    u32 startCol = col;

    if(peek_next_char() == '=') {
        next_char_noreturn();
        return create_token(ARC_AND_EQUAL, startCol, startPos);
    }
    else if(peek_next_char() == '&') {
        next_char_noreturn();
        return create_token(ARC_LOGICAL_AND, startCol, startPos);
    }
    else
        return create_token(ARC_BIN_AND, startPos);
}

Token Lexer::lex_not() {
    PROFILE();
    
    u32 startPos = index;
    u32 startCol = col;

    if(peek_next_char() == '=') {
        next_char_noreturn();
        return create_token(ARC_NOT_EQUAL, startCol, startPos);
    }
    else
        return create_token(ARC_NOT, startPos);
}

Token Lexer::lex_xor(){
    PROFILE();

    u32 startPos = index;
    u32 startCol = col;

    if(peek_next_char() == '=') {
        next_char_noreturn();
        return create_token(ARC_XOR_EQUAL, startCol, startPos);
    }
    else
        return create_token(ARC_XOR, startPos);
}

Token Lexer::lex_lesser() {
    PROFILE();
    
    u32 startPos = index;
    u32 startCol = col;

    if(peek_next_char() == '=') {
        next_char_noreturn();
        return create_token(ARC_LESSER_EQUAL, startCol, startPos);
    }
    else
        return create_token(ARC_LESSER, startPos);
}

Token Lexer::lex_greater() {
    PROFILE();
    
    u32 startPos = index;
    u32 startCol = col;

    if(peek_next_char() == '=') {
        next_char_noreturn();
        return create_token(ARC_GREATER_EQUAL, startCol, startPos);
    }
    else
        return create_token(ARC_GREATER, startPos);
}

Token Lexer::lex_equal() {
    PROFILE();
    
    u32 startPos = index;
    u32 startCol = col;

    if(peek_next_char() == '=') {
        next_char_noreturn();
        return create_token(ARC_EQUAL, startCol, startPos);
    }
    else
        return create_token(ARC_ASSIGN, startPos);
}

inline char Lexer::current_char() {
    PROFILE();
    return data[index];
}

inline char Lexer::next_char() {
    PROFILE();
    col++;
    return data[++index];
}

inline char Lexer::prev_char() {
    PROFILE();
    col--;
    return data[--index];
}

inline void Lexer::next_char_noreturn() {
    PROFILE();
    col++;
    index++;
}

inline void Lexer::prev_char_noreturn() {
    PROFILE();
    col--;
    index--;
}

inline char Lexer::peek_next_char() {
    PROFILE();
    return data[index+1];
}

inline Token Lexer::create_token(TokenKind kind, u32 startPos) {
    PROFILE();
    // Token* tkn = allocator.alloc<Token>();
    // *tkn = Token {kind, SourcePos{line, col, startPos, index}, nullptr};
    return Token {kind, SourcePos{line, col, startPos, index}, nullptr};
}
 
inline Token Lexer::create_token(TokenKind kind, u32 currentCol, u32 startPos) {
    PROFILE();
    // Token* tkn = allocator.alloc<Token>();
    // *tkn = Token {kind, SourcePos{line, currentCol, startPos, index}, nullptr};
    return Token {kind, SourcePos{line, currentCol, startPos, index}, nullptr};
}

inline Token Lexer::create_token(TokenKind kind, u32 startPos, astring& val) {
    PROFILE();
    return create_token(kind, col, startPos, val);
}

inline Token Lexer::create_token(TokenKind kind, u32 startPos, astring&& val) {
    PROFILE();
    return create_token(kind, col, startPos, val);
}


inline Token Lexer::create_token(TokenKind kind, u32 currentCol, u32 startPos, astring& val) {   //FIXME no reason to pass a string here
    PROFILE();
    return Token {kind, SourcePos{line, currentCol, startPos, index}, &val};
}

inline Token Lexer::create_token(TokenKind kind, u32 currentCol, u32 startPos, astring&& val) {   //FIXME no reason to pass a string here
    PROFILE();
    astring* val2 = new astring(val);   //FIXME temporary
    return Token {kind, SourcePos{line, currentCol, startPos, index}, val2};
}

inline void Lexer::print_tokens(bool verbose) {
    PROFILE();
    if(verbose)
        for(Token tkn : tokens)
            println_token(&tkn);
    else{
        for(Token tkn : tokens)
            if(tkn.data == nullptr)
                println(astrtostr(str(tkn.kind) + ": " + get_string_rep(tkn.kind)));
            else
                println(astrtostr(str(tkn.kind) + ": " + *(tkn.data)));
    }
}