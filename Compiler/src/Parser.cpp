#include "Parser.h"

Parser::Parser(std::vector<RawFile, arena_allocator<RawFile>>& project_files):
	index_(0)
{  //TODO good candidate for multithreading
    PROFILE();
    // std::thread worker([](){});
    // lex all the files
    for(const auto& rf: project_files){
        std::cout << rf.filepath << '\n';
        Lexer lexer(rf.filedata);
        data_.push_back(LexedFile{rf.filepath, lexer.lex()});
    } 
}

Project* Parser::parse() {
    PROFILE();
    Project* project = parse_project();

    return project;
}

Project* Parser::parse_project() {    //TODO another good candidate for multithreading
    PROFILE();
    std::vector<File*, arena_allocator<File*>> files;
    for(const auto& lf : data_) {
        // update/reset member variables for each file
        tokens_ = lf.filedata;
        files.push_back(parse_file());
    }
    return ast_.new_project_node(SourcePos{}, files);
}

File* Parser::parse_file() {
    PROFILE();
    // reset the class wide index variable to zero
    index_ = 0;
    const SourcePos start_pos = current_token()->pos;
    std::vector<Import*, arena_allocator<Import*>> imports;
    std::vector<Decl*, arena_allocator<Decl*>> decls;
    std::vector<Function*, arena_allocator<Function*>> functions;
    for(; index_ < tokens_.size(); index_++) {  
        switch(current_token()->kind) {
            case ARC_IMPORT:
                println("import statement");
                imports.push_back(parse_import());
                break;
            case ARC_FN:
                println("function start");
                std::cout.flush();
                functions.push_back(parse_function());
                println("function end");
                break;
            case ARC_ID:
                println("decl start");
                decls.push_back(parse_decl());
                break;
            case ARC_EOF:   // placed at the end of each file by the lexer
                return ast_.new_file_node(start_pos, imports, decls, functions, true);
                break;
            default:
                break;
        }
    }
    // should never reach this point but return just in case
    return ast_.new_file_node(start_pos, imports, decls, functions, true);
}

Import* Parser::parse_import() {
    PROFILE();
    return new Import{};
}


Function* Parser::parse_function() {
    PROFILE();
    const SourcePos start_pos = current_token()->pos;
    std::vector<Type, arena_allocator<Type>> fn_args;
    expect_token(ARC_FN);
    astring id;
    if(check_token(ARC_ID))
        id = *(current_token()->data);
    next_token_noreturn();
    expect_token(ARC_OPEN_PAREN);
    // parse func args
    expect_token(ARC_CLOSE_PAREN);
    expect_token(ARC_COLON);
    Type ret = token_kind_to_type(current_token()->kind);
    if(ret == -1)
        errorLog.push(ErrorMessage{FATAL, current_token(), args.path, "Unknown return type"});
    next_token_noreturn();
    s_table_.add_function(id, fn_args, FUNCTION, ret);
    Block* block = parse_block();
    return ast_.new_function_node(start_pos, id, fn_args, ret, block, true);
}

/**
 *  expect first token to be opening brace '{' 
 */
Block* Parser::parse_block() {
    PROFILE();
    s_table_.push_scope();
    const SourcePos start_pos = current_token()->pos;
    std::vector<Statement*, arena_allocator<Statement*>> statements;
    // cases
    // statement
    check_token(ARC_OPEN_BRACE);
    while(next_token()->kind != ARC_CLOSE_BRACE)
        statements.push_back(parse_statement());
    // nextToken(); // go past the closed brace
    return new Block{ {start_pos}, statements };
}

Statement* Parser::parse_statement() {
    PROFILE();
    const SourcePos start_pos = current_token()->pos;
    // cases
    // while loop
    // for loop
    // if
    // ret
    // decl
    // expr
    switch (current_token()->kind) {
        case ARC_WHILE:
        {
            expect_token(ARC_WHILE);
            auto *expr = parse_expr();
            auto *block = parse_block();
            auto *result = ast_.new_statement_node_while(start_pos, ast_.new_while_node(start_pos, expr, block));
            return result;
            break;
        }
        case ARC_FOR:
        {
            expect_token(ARC_FOR);
            expect_token(ARC_OPEN_PAREN);
            auto *decl = parse_decl();
            expect_token(ARC_SEMICOLON);
            auto *expr1 = parse_expr();
            expect_token(ARC_SEMICOLON);
            auto *expr2 = parse_expr();
            expect_token(ARC_SEMICOLON);
            expect_token(ARC_CLOSE_PAREN);
            auto *block = parse_block();
            auto *result = ast_.new_statement_node_for(start_pos, ast_.new_for_node(start_pos, decl, expr1, expr2, block));
            return result;
            break;
        }
        case ARC_IF:
        {
            expect_token(ARC_IF);
            auto *expr = parse_expr();
            auto *block = parse_block();
            auto *result = ast_.new_statement_node_if(start_pos, ast_.new_if_node(start_pos, expr, block));
            return result;
            break;
        }    
        case ARC_RET:
        {
            expect_token(ARC_RET);
            auto *result = ast_.new_statement_node_ret(start_pos, ast_.new_ret_node(start_pos, parse_expr()));
            if(current_token()->kind != ARC_SEMICOLON)
                errorLog.push(ErrorMessage{FATAL, current_token(), args.path, "Expected semicolon"});
            return result;
            break;
        }
        case ARC_ID:
            // can be decl or expr
            if(peek_next_token()->kind == ARC_COLON || peek_next_token()->kind == ARC_INFER)
                ast_.new_statement_node_decl(start_pos, parse_decl());
            break;
        case ARC_INT_LIT:
        case ARC_FLOAT_LIT:
        case ARC_STRING_LIT:
        // this must be an expr
        // can also start with a unary operator
        break;
        default:
            break;
    }
    return new Statement{};
}

Decl* Parser::parse_decl() {
    PROFILE();
    const SourcePos start_pos = current_token()->pos;
    astring& id = *(current_token()->data);
    if(peek_next_token()->kind == ARC_INFER) {
        s_table_.add_symbol(id, VARIABLE, TYPE_INFER);
    }
    else if(next_token()->kind == ARC_COLON) {
        next_token_noreturn();
        const Type type = token_kind_to_type(current_token()->kind);
        switch(current_token()->kind) {
            case ARC_I8:
            case ARC_I16:
            case ARC_I32:
            case ARC_I64:
            case ARC_U8:
            case ARC_U16:
            case ARC_U32:
            case ARC_U64:
            case ARC_F32:
            case ARC_F64:
            case ARC_STRUCT:
            case ARC_STR:
            case ARC_ARR:
                if(peek_next_token()->kind == ARC_SEMICOLON) {
                    //TODO definition but no declaration
                    s_table_.add_symbol(id, VARIABLE, token_kind_to_type(current_token()->kind));
                }
                else if(peek_next_token()->kind == ARC_ASSIGN) {
                    next_token_noreturn();
                    next_token_noreturn();
                    auto *result = ast_.new_decl_node(start_pos, id, type, parse_expr());
                    // add symbol to table after parsing to avoid situations like this
                    // where a variable is used in it's own decl
                    // var := var + 1;
                    s_table_.add_symbol(id, VARIABLE, token_kind_to_type(current_token()->kind));
                    return result;
                }
                else {
                    //TODO syntax error here
                    errorLog.push(ErrorMessage{FATAL, current_token(), args.path, "INVALID TOKEN"});
                }
                break;
            default:
                //TODO syntax error here
                errorLog.push(ErrorMessage{FATAL, current_token(), args.path, "INVALID TOKEN"});
                break;
        }
    }
    else {
        //TODO syntax error here
        errorLog.push(ErrorMessage{FATAL, current_token(), args.path, "INVALID TOKEN"});
    }
    return new Decl{};
}

/**
 * expects the current token to be the first token in the expression
 */
Expr* Parser::parse_expr() {
    PROFILE();
    std::vector<Token*, arena_allocator<Token*>> result;
    std::vector<Token*, arena_allocator<Token*>> stack;

    /**
     * shunting yard algorithm
     */
    while(current_token()->kind != ARC_SEMICOLON &&
            current_token()->kind != ARC_OPEN_BRACE &&
                current_token()->kind != ARC_COMMA) {
        if(current_token()->kind == ARC_INT_LIT) {
            result.push_back(current_token());
        }
        else if(current_token()->kind == ARC_ID) {    //TODO differentiate between functions and variables
            if(!s_table_.has(*(current_token()->data)))
                errorLog.push(ErrorMessage{FATAL, current_token(), args.path, "Unknown identifier"});
            // if not function call
            if(peek_next_token()->kind != ARC_OPEN_PAREN)
                result.push_back(current_token());
            else {
                //TODO add support for function args
                // right now just skip over the parens
                next_token_noreturn();
                next_token_noreturn();
            }
        }
        else if(is_operator(current_token()->kind)) {  //TODO support unary operators also
                while ((!stack.empty()) && (is_operator(stack.back()->kind)) && 
                            ((precedence(stack.back()->kind) > precedence(current_token()->kind)) || 
                                (precedence(stack.back()->kind) == precedence(current_token()->kind))) && 
                                    (stack.back()->kind != ARC_OPEN_PAREN)) {
                    result.push_back(stack.back());
                    stack.pop_back();
                }
                stack.push_back(current_token());
            }
        else if(current_token()->kind == ARC_OPEN_PAREN) {
            stack.push_back(current_token());
        }
        else if(current_token()->kind == ARC_CLOSE_PAREN) {
            while(stack.back()->kind != ARC_OPEN_PAREN) {
                result.push_back(stack.back());
                stack.pop_back();
                if(stack.empty()){
                    errorLog.push(ErrorMessage{FATAL, current_token(), args.path, "Extra closing parentheses"});
                }
            }
            stack.pop_back();
        }
        next_token_noreturn();
    }

    while(!stack.empty()) {
        result.push_back(stack.back());
        stack.pop_back();
    }

    /**
     * convert the vector of Tokens into a Expr tree
     */
    std::vector<Expr*, arena_allocator<Expr*>> conversion_stack;
    for(auto *tkn : result) {
        Token* token = tkn;
        if(is_operator(token->kind)) {
            if(is_unary_operator(token->kind)) {
                conversion_stack.push_back(ast_.new_expr_node_unary_expr(token->pos, token->kind, conversion_stack.back()));
                conversion_stack.pop_back();
            }
            else {
                Expr* operand1 = conversion_stack.back();
                conversion_stack.pop_back();
                Expr* operand2 = conversion_stack.back();
                conversion_stack.pop_back();
                conversion_stack.push_back(ast_.new_expr_node_bin_expr(token->pos, token->kind, operand2, operand1));
            }
        }
        else {
            switch(tkn->kind) {
                case ARC_INT_LIT:
                    conversion_stack.push_back(ast_.new_expr_node_int_literal(token->pos, astoll(*(token->data))));
                    break;
                case ARC_ID:
                    conversion_stack.push_back(ast_.new_expr_node_variable(token->pos, *(token->data)));
                    break;
            }
            
        }
        println_token(tkn);
    }
    return conversion_stack.back();
}

inline Token* Parser::current_token() {
    PROFILE();
    return &tokens_[index_];
}

inline Token* Parser::next_token() {
    PROFILE();
    if(index_ + 1 >= tokens_.size()) {
        errorLog.push(ErrorMessage{FATAL, current_token(), args.path, "Reached EOF while parsing"});
        errorLog.flush();
    }

    return &tokens_[++index_];
}

inline void Parser::next_token_noreturn() {
    PROFILE();
    if (index_ + 1 >= tokens_.size()) {
        errorLog.push(ErrorMessage{ FATAL, current_token(), args.path, "Reached EOF while parsing" });
        errorLog.flush();
    }
    ++index_;
}

inline Token* Parser::peek_next_token() {
    PROFILE();
    if(index_ + 1 >= tokens_.size()) {
        errorLog.push(ErrorMessage{FATAL, current_token(), args.path, "Reached EOF while parsing"});
        errorLog.flush();
    }
    return &tokens_[index_ + 1];
}

// check if current token is something
// does not increment
inline bool Parser::check_token(const TokenKind kind) {
    [[likely]] if(current_token()->kind == kind)
        return true;
    errorLog.push(ErrorMessage{FATAL, current_token(), args.path, "Unexpected token"});
    errorLog.flush();
    return false;    
}

// TODO since this function doesn't exit
// errors will propagate throughout the whole program
// unless the return value is handled
// maybe have a function that just exits when a unexpected token is found
// or turn off syntax errors for the current block and try to recover
//
// expect the current token to be something otherwise syntax error
//
inline bool Parser::expect_token(const TokenKind kind) {
    [[likely]] if(check_token(kind)) {
        next_token_noreturn();
        return true;
    }
    errorLog.push(ErrorMessage{FATAL, current_token(), args.path, "Unexpected token"});
    errorLog.flush();
    next_token_noreturn();
    return false;
}

inline Type Parser::token_kind_to_type(const TokenKind tkn) {
    PROFILE();
    switch(tkn){
        case ARC_INFER:
            return TYPE_INFER;
        case ARC_I8:
            return TYPE_I8;
        case ARC_I16:
            return TYPE_I16;
        case ARC_I32:
            return TYPE_I32;
        case ARC_I64:
            return TYPE_I64;
        case ARC_U8:
            return TYPE_U8;
        case ARC_U16:
            return TYPE_U16;
        case ARC_U32:
            return TYPE_U32;
        case ARC_U64:
            return TYPE_U64;
        case ARC_F32:
            return TYPE_F32;
        case ARC_F64:
            return TYPE_F64;
        case ARC_STRUCT:
            return TYPE_STRUCT;
        case ARC_STR:
            return TYPE_STRING;
        case ARC_ARR:
            return TYPE_ARR;
    }
    return static_cast<Type>(-1);   // invalid value
}

u8 Parser::precedence(const TokenKind kind) {
    PROFILE();

    switch(kind){
        case ARC_PRE_INCREMENT:
        case ARC_POST_INCREMENT:
        case ARC_PRE_DECREMENT:
        case ARC_POST_DECREMENT:
        case ARC_NOT:
        case ARC_NEGATE:
            return 7;
        case ARC_DIV:
        case ARC_MUL:
        case ARC_MOD:
            return 6;
        case ARC_ADD:
        case ARC_SUB:
            return 5;
        case ARC_BIN_OR:
        case ARC_BIN_AND:
        case ARC_LEFT_SHIFT:
        case ARC_RIGHT_SHIFT:
        case ARC_XOR:
            return 4;
        case ARC_LOGICAL_OR:
        case ARC_LOGICAL_AND:
            return 3;
        case ARC_LESSER_EQUAL:
        case ARC_GREATER_EQUAL:
        case ARC_EQUAL:
        case ARC_LESSER:
        case ARC_GREATER:
            return 2;
        case ARC_ADD_EQUAL:
        case ARC_SUB_EQUAL:
        case ARC_DIV_EQUAL:
        case ARC_MUL_EQUAL:
        case ARC_MOD_EQUAL:
        case ARC_OR_EQUAL:
        case ARC_AND_EQUAL:
        case ARC_NOT_EQUAL:
        case ARC_LEFT_SHIFT_EQUAL:
        case ARC_RIGHT_SHIFT_EQUAL:
        case ARC_XOR_EQUAL:
            return 1;
        case ARC_ASSIGN:
        case ARC_INFER:
            return 0;
        default:
            return 0xff; // invalid value
    }
}