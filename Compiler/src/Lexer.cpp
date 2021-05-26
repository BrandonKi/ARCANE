#include "Lexer.h"

extern ErrorHandler error_log;
extern TypeManager type_manager;

Lexer::Lexer(const astring& filename, const astring& filedata):
    filename_(filename), data_(filedata), tokens_(arena_allocator<Token>{}),
    index_(0), line_(1), col_(1)
{
    PROFILE();
    tokens_.reserve(100);
}

std::vector<Token, arena_allocator<Token>> Lexer::lex() {
    PROFILE();

    while(index_ < data_.size()) {

        switch(current_char()) {
            CASE_DIGIT:
                tokens_.push_back(lex_number_lit());
                break;
            CASE_ID:
                tokens_.push_back(lex_identifier());
                break;
            case '\'':
            case '"': 
                tokens_.push_back(lex_string());
                break;
            case '`':   //TODO interpolated string literal is not trivial to tokenize
                tokens_.push_back(lex_interpolated_string());
                break;
            case '{':
                tokens_.push_back(create_token(ARC_OPEN_BRACE, index_));
                break;
            case '}':
                tokens_.push_back(create_token(ARC_CLOSE_BRACE, index_));
                break;
            case '[':
                tokens_.push_back(create_token(ARC_OPEN_BRACKET, index_));
                break;
            case ']':
                tokens_.push_back(create_token(ARC_CLOSE_BRACKET, index_));
                break;
            case '(':
                tokens_.push_back(create_token(ARC_OPEN_PAREN, index_));
                break;
            case ')':
                tokens_.push_back(create_token(ARC_CLOSE_PAREN, index_));
                break;
            case '.':
                tokens_.push_back(create_token(ARC_DOT, index_));
                break;
            case ',':
                tokens_.push_back(create_token(ARC_COMMA, index_));
                break;
            case '?':
                tokens_.push_back(create_token(ARC_TERNARY, index_));
                break;
            case ';':
                tokens_.push_back(create_token(ARC_SEMICOLON, index_));
                break;
            case ':':
                tokens_.push_back(lex_colon());
                break;
            case '@':
                tokens_.push_back(create_token(ARC_AT, index_));
                break;
            case '#':
                tokens_.push_back(create_token(ARC_HASH, index_));
                break;
            case '$':
                tokens_.push_back(create_token(ARC_DOLLAR, index_));
                break;
            case '+':
                tokens_.push_back(lex_add());
                break;
            case '-':
                tokens_.push_back(lex_sub());
                break;
            case '/':
                if(peek_next_char() == '/' || peek_next_char() == '*')
                    consume_comment();
                else
                    tokens_.push_back(lex_div());
                break;
            case '*':
                tokens_.push_back(lex_mul());
                break;
            case '%':
                tokens_.push_back(lex_mod());
                break;
            case '|':
                tokens_.push_back(lex_or());
                break;
            case '&':
                tokens_.push_back(lex_and());
                break;
            case '!':
                tokens_.push_back(lex_not());
                break;
            case '^':
                tokens_.push_back(lex_xor());
                break;
            case '<':
                tokens_.push_back(lex_lesser());
                break;
            case '>':
                tokens_.push_back(lex_greater());
                break;
            case '=':
                tokens_.push_back(lex_equal());
                break;
            case '\n':
                line_++;
                col_ = 0;
                break;
            default:
                break;
        }
            next_char_noreturn();
    }
    if(args.lex_out)
        print_tokens(args.verbose_lex_out);  // true for verbose and false for succinct
    tokens_.push_back(create_token(ARC_EOF, col_, index_));
    return std::move(tokens_);   // not a big deal but there is no reason to make an extra copy
}


void Lexer::consume_comment() {
    PROFILE();

    if(next_char() == '*') {
        next_char_noreturn();
        while(index_ < data_.size() && !(current_char() == '*' && peek_next_char() == '/')){
            if(current_char() == '\n')
                line_++;
            next_char_noreturn();
        }
        next_char_noreturn();
    }
    else{
        while(index_ < data_.size() &&  current_char() != '\n'){
            next_char_noreturn();
        }
    }
}

Token Lexer::lex_number_lit() {
    PROFILE();
    auto& num = *new astring;    //FIXME use arena alloc
    const auto start_pos = index_;
    const auto current_col = col_;
    auto is_float = false;
    auto is_int = true;
    if(peek_next_char() == 'x') {  //TODO implement hex literal lexing
        next_char_noreturn();
        is_int = false;
        is_float = false;
    }
    while(is_digit(current_char())) {      //TODO refactor
        num.push_back(current_char());

        if(peek_next_char() == '.') {
            // if(isFloat) //TODO print error message because two "." are present in number literal
            is_float = true;
            is_int = false;

            num.push_back(next_char());
        }
        next_char_noreturn();
    }
    prev_char_noreturn();    // the above loop goes one char too far so decrement here
    // if the literal is negative figure that out here
    if(tokens_.back().kind == ARC_NEGATE) {
        tokens_.pop_back();
        num = "-" + num;
    }
    if(is_int)
        return create_token(ARC_INT_LIT, current_col, start_pos, num);
    else if(is_float)
        return create_token(ARC_FLOAT_LIT, current_col, start_pos, num);
    else {
        //TODO convert hex literal to int or float literal
        return create_token(ARC_INT_LIT, current_col, start_pos, num);
    }
}

Token Lexer::lex_identifier() {
    PROFILE();

    auto& id = *new astring; //FIXME use arena alloc
    const auto start_pos = index_;
    const auto current_col = col_;

    while(is_letter(current_char())  || is_digit(current_char())) {
        id.push_back(current_char());
        next_char_noreturn();
    }

    prev_char_noreturn();    // the above loop goes one char too far so decrement here

    if(keywords.find(id) != keywords.end())
        return create_token(keywords.find(id)->second, current_col, start_pos);
    return create_token(ARC_ID, current_col, start_pos, id);
}

Token Lexer::lex_string() {  //TODO escape sequences
    PROFILE();

    const auto start_pos = index_;

    auto& id = *new astring; //FIXME use arena alloc

    // current_char is either " or '
    const auto end = current_char();

    while(next_char() != end) {
        id.push_back(current_char());
    }

    return create_token(ARC_STRING_LIT, start_pos, id);
}

Token Lexer::lex_interpolated_string() {  //TODO implement interpolated strings
    PROFILE();
    // FIXME
    const auto start_pos = index_;
    const auto start_col = col_;

    while(next_char() != '`');
    auto tkn = create_token(ARC_STRING_LIT, start_col, start_pos);
    error_log.exit(ErrorMessage{FATAL, &tkn, filename_, astring("interpolated strings are not implemented yet")});
    return Token{};
}

Token Lexer::lex_colon() {
    PROFILE();

    const auto start_pos = index_;
    const auto start_col = col_;
    
    if(peek_next_char() == '=') {
        next_char_noreturn();
        return create_token(ARC_INFER, start_col, start_pos);
    }
    else
        return create_token(ARC_COLON, start_pos);

}

inline Token Lexer::lex_add() {
    PROFILE();

    const auto start_pos = index_;
    const auto start_col = col_;

    if(peek_next_char() == '=') {
        next_char_noreturn();
        return create_token(ARC_ADD_EQUAL, start_col, start_pos);
    }
    else if(peek_next_char() == '+') {
        next_char_noreturn();
        if(tokens_.back().kind == ARC_ID)  // FIXME incorrectly lexes some cases for ex. "4++", "*++4", etc.
            return create_token(ARC_POST_INCREMENT, start_col, start_pos);
        return create_token(ARC_PRE_INCREMENT, start_col, start_pos);
    }
    else
        return create_token(ARC_ADD, start_pos);
}

Token Lexer::lex_sub() {
    PROFILE();
    
    const auto start_pos = index_;
    const auto start_col = col_;

    if(peek_next_char() == '=') {
        next_char_noreturn();
        return create_token(ARC_SUB_EQUAL, start_col, start_pos);
    }
    else if(peek_next_char() == '-') {
        next_char_noreturn();
        if(!tokens_.empty() && tokens_.back().kind == ARC_ID)   // FIXME incorrectly lexes some cases for ex. "4--", "*--4", etc.
            return create_token(ARC_POST_DECREMENT, start_col, start_pos);
        return create_token(ARC_PRE_DECREMENT, start_col, start_pos);
    }
    else {
        if(tokens_.empty() || is_operator(tokens_.back().kind) || is_keyword(tokens_.back().kind))
            return create_token(ARC_NEGATE, start_pos);
        return create_token(ARC_SUB, start_pos);  // FIXME doesn't even attempt to parse a unary sub
    }
}

Token Lexer::lex_div() {
    PROFILE();
    
    const auto start_pos = index_;
    const auto start_col = col_;

    if(peek_next_char() == '=') {
        next_char_noreturn();
        return create_token(ARC_DIV_EQUAL, start_col, start_pos);
    }
    else
        return create_token(ARC_DIV, start_pos);
}

Token Lexer::lex_mul() {
    PROFILE();
    
    const auto start_pos = index_;
    const auto start_col = col_;

    if(peek_next_char() == '=') {
        next_char_noreturn();
        return create_token(ARC_MUL_EQUAL, start_col, start_pos);
    }
    else
        return create_token(ARC_MUL, start_pos);
}

Token Lexer::lex_mod() {
    PROFILE();
    
    const auto start_pos = index_;
    const auto start_col = col_;

    if(peek_next_char() == '=') {
        next_char_noreturn();
        return create_token(ARC_MOD_EQUAL, start_col, start_pos);
    }
    else
        return create_token(ARC_MOD, start_pos);
}

Token Lexer::lex_or() {
    PROFILE();
    
    const auto start_pos = index_;
    const auto start_col = col_;

    if(peek_next_char() == '=') {
        next_char_noreturn();
        return create_token(ARC_OR_EQUAL, start_col, start_pos);
    }
    else if(peek_next_char() == '|'){
        next_char_noreturn();
        return create_token(ARC_LOGICAL_OR, start_col, start_pos);
    }
    else
        return create_token(ARC_BIN_OR, start_pos);
}

Token Lexer::lex_and() {
    PROFILE();
    
    const auto start_pos = index_;
    const auto start_col = col_;

    if(peek_next_char() == '=') {
        next_char_noreturn();
        return create_token(ARC_AND_EQUAL, start_col, start_pos);
    }
    else if(peek_next_char() == '&') {
        next_char_noreturn();
        return create_token(ARC_LOGICAL_AND, start_col, start_pos);
    }
    else
        return create_token(ARC_BIN_AND, start_pos);
}

Token Lexer::lex_not() {
    PROFILE();
    
    const auto start_pos = index_;
    const auto start_col = col_;

    if(peek_next_char() == '=') {
        next_char_noreturn();
        return create_token(ARC_NOT_EQUAL, start_col, start_pos);
    }
    else
        return create_token(ARC_NOT, start_pos);
}

Token Lexer::lex_xor(){
    PROFILE();

    const auto start_pos = index_;
    const auto start_col = col_;

    if(peek_next_char() == '=') {
        next_char_noreturn();
        return create_token(ARC_XOR_EQUAL, start_col, start_pos);
    }
    else
        return create_token(ARC_XOR, start_pos);
}

Token Lexer::lex_lesser() {
    PROFILE();
    
    const auto start_pos = index_;
    const auto start_col = col_;

    if(peek_next_char() == '=') {
        next_char_noreturn();
        return create_token(ARC_LESSER_EQUAL, start_col, start_pos);
    }
    else
        return create_token(ARC_LESSER, start_pos);
}

Token Lexer::lex_greater() {
    PROFILE();
    
    const auto start_pos = index_;
    const auto start_col = col_;

    if(peek_next_char() == '=') {
        next_char_noreturn();
        return create_token(ARC_GREATER_EQUAL, start_col, start_pos);
    }
    else
        return create_token(ARC_GREATER, start_pos);
}

Token Lexer::lex_equal() {
    PROFILE();
    
    const auto start_pos = index_;
    const auto start_col = col_;
    
    if(peek_next_char() == '=') {
        next_char_noreturn();
        return create_token(ARC_EQUAL, start_col, start_pos);
    }
    else
        return create_token(ARC_ASSIGN, start_pos);
}

inline char Lexer::current_char() const {
    return data_[index_];
}

inline char Lexer::next_char() {
    col_++;
    return data_[++index_];
}

inline char Lexer::prev_char() {
    col_--;
    return data_[--index_];
}

inline void Lexer::next_char_noreturn() {
    col_++;
    index_++;
}

inline void Lexer::prev_char_noreturn() {
    col_--;
    index_--;
}

inline char Lexer::peek_next_char() const {
    return data_[index_+1];
}

inline char Lexer::peek_prev_char() const {
    return data_[index_ - 1];
}

inline Token Lexer::create_token(const TokenKind kind, const u32 start_pos) const {
    PROFILE();
    return Token {kind, SourcePos{line_, col_, start_pos, index_}, nullptr};
}
 
inline Token Lexer::create_token(const TokenKind kind, const u32 current_col, const u32 start_pos) const {
    PROFILE();
    return Token {kind, SourcePos{line_, current_col, start_pos, index_}, nullptr};
}

inline Token Lexer::create_token(const TokenKind kind, const u32 start_pos, astring& val) const {
    PROFILE();
    return create_token(kind, col_, start_pos, val);
}

inline Token Lexer::create_token(const TokenKind kind, const u32 start_pos, astring&& val) const {
    PROFILE();
    return create_token(kind, col_, start_pos, val);
}

inline Token Lexer::create_token(const TokenKind kind, const u32 current_col, const u32 start_pos, astring& val) const  {   //FIXME no reason to pass a string here
    PROFILE();
    return Token {kind, SourcePos{line_, current_col, start_pos, index_}, &val};
}

inline Token Lexer::create_token(const TokenKind kind, const u32 current_col, const u32 start_pos, astring&& val) const  {   //FIXME no reason to pass a string here
    PROFILE();
    auto* val2 = new astring(val);   //FIXME temporary
    return Token {kind, SourcePos{line_, current_col, start_pos, index_}, val2};
}

inline void Lexer::print_tokens(const bool verbose) const {
    PROFILE();
    if(verbose)
        for(const auto& tkn : tokens_)
            println_token(&tkn);
    else{
        for(const auto& tkn : tokens_)
            if(tkn.data == nullptr)
                println(astrtostr(str(tkn.kind) + ": " + get_string_rep(tkn.kind)));
            else
                println(astrtostr(str(tkn.kind) + ": " + *(tkn.data)));
    }
}