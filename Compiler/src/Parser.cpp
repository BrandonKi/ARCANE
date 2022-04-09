#include "Parser.h"

extern ErrorHandler error_log;
extern TypeManager type_manager;

Parser::Parser(std::vector<RawFile>& project_files) {
    //TODO good candidate for multithreading
    PROFILE();
    // std::thread worker([](){});
    // lex all the files
    for(const auto& rf: project_files){
        Lexer lexer(rf.filename, rf.filedata.c_str(), rf.filedata.size());
        data_.push_back(LexedFile{ rf.filepath, rf.filename, lexer.lex() });
    }
}

Project* Parser::parse() {
    PROFILE();
    Project* project = parse_project();

    return project;
}

Project* Parser::parse_project() {    //TODO another good candidate for multithreading
    PROFILE();
    std::vector<File*> files;
    for(const auto& lf : data_) {
        // update/reset member variables for each file
        tokens_ = lf.filedata;
        current_filename_ = lf.filename;
        files.push_back(parse_file());
    }
    return ast_.new_project_node(SourcePos{}, files);
}

File* Parser::parse_file() {
    PROFILE();
    const SourcePos start_pos = current_token()->pos;
    std::vector<Import*> imports;
    std::vector<Decl*> decls;
    std::vector<Function*> functions;
    std::vector<PolyFunction*> poly_functions;
    for(index_ = 0; index_ < tokens_.size(); ++index_) {
        switch(current_token()->kind) {
            case ARC_IMPORT:
                println("import statement");
                imports.push_back(parse_import());
                break;
            case ARC_POLY_START:
                poly_functions.push_back(parse_poly_function());
                break;
            case ARC_FN:
                functions.push_back(parse_function());
                break;
            case ARC_ID:
                decls.push_back(parse_decl());
                break;
            case ARC_EOF:   // placed at the end of each file by the lexer
                return ast_.new_file_node(start_pos, current_filename_, imports, decls, poly_functions, functions, true);
                break;
            default:
                break;
        }
    }
    // should never reach this point but return just in case
    return ast_.new_file_node(start_pos, current_filename_, imports, decls, poly_functions, functions, true);
}

Import* Parser::parse_import() {
    PROFILE();
    return new Import{};
}

PolyFunction* Parser::parse_poly_function() {
    PROFILE();
    const SourcePos start_pos = current_token()->pos;
    auto poly_args = parse_poly_fn_args();
    auto fn = parse_function_base();

    return ast_.new_poly_function_node(start_pos, fn.id, poly_args, fn.args, fn.return_type, fn.body);
}

std::vector<Arg> Parser::parse_poly_fn_args() {
    PROFILE();
    expect_token(ARC_POLY_START);
    std::vector<Arg> result;
    for(int i = 0; current_token()->kind == ARC_ID; ++i) {
        auto id = std::string(current_token()->data);
        auto pos = current_token()->pos;
        next_token_noreturn();
        expect_token(ARC_COLON);
        result.push_back(Arg{{pos}, id, token_kind_to_type(current_token()->kind)});
        s_table_.add_symbol(id, VARIABLE, result.back().type);
        next_token_noreturn();
        if(check_token(ARC_COMMA))
           next_token_noreturn();
    }
    expect_token(ARC_GREATER);
    return result;
}

Function* Parser::parse_function() {
    PROFILE();
    auto fn = parse_function_base();

    if(fn.id == "main" && fn.return_type == TYPE_i32)
        return ast_.new_function_node(fn.pos, fn.id, fn.args, fn.return_type, fn.body, true);
    return ast_.new_function_node(fn.pos, fn.id, fn.args, fn.return_type, fn.body, false);
}

// TODO improve syntax error messages for this function
std::vector<Arg> Parser::parse_fn_args() {
    PROFILE();
    expect_token(ARC_OPEN_PAREN);
    std::vector<Arg> result;
    for(int i = 0; current_token()->kind == ARC_ID; ++i) {
        auto id = std::string(current_token()->data);
        auto pos = current_token()->pos;
        next_token_noreturn();
        expect_token(ARC_COLON);
        result.push_back(Arg{{pos}, id, token_kind_to_type(current_token()->kind)});
        s_table_.add_symbol(id, VARIABLE, result.back().type);
        next_token_noreturn();
        if(check_token(ARC_COMMA))
           next_token_noreturn();
    }
    expect_token(ARC_CLOSE_PAREN);
    return result;
}

Function Parser::parse_function_base() {
    PROFILE();
    const SourcePos start_pos = current_token()->pos;
    expect_token(ARC_FN);
    verify_token(ARC_ID);
    auto id = std::string(current_token()->data);
    next_token_noreturn();
    s_table_.push_scope();
    auto fn_args = parse_fn_args();
    expect_token(ARC_COLON);
    type_handle ret = token_kind_to_type(current_token()->kind);
    if(ret == -1)
        error_log.exit(ErrorMessage{FATAL, current_token()->pos, current_filename_, "Unknown return type"});
    next_token_noreturn();
    s_table_.add_function_to_parent_scope(id, fn_args, FUNCTION, ret);

    verify_token(ARC_OPEN_BRACE);
    Block* block = parse_bare_block(); // doesn't use parse_block because it controls its scope

    s_table_.pop_scope();
    return Function{start_pos, id, fn_args, ret, block, false};
}

/**
 *  expect first token to be opening brace '{'
 */
Block* Parser::parse_block() {
    PROFILE();
    s_table_.push_scope();
    // cases
    // statement
    verify_token(ARC_OPEN_BRACE);
    // nextToken(); // go past the closed brace
    auto* result = parse_bare_block();
    s_table_.pop_scope();
    return result;
}

Block* Parser::parse_bare_block() {
    const SourcePos start_pos = current_token()->pos;
    std::vector<Statement*> statements;
    while(peek_next_token()->kind != ARC_CLOSE_BRACE) {
        next_token_noreturn();
        statements.push_back(parse_statement());
    }
    next_token_noreturn();
    //expect_token(ARC_CLOSE_BRACE);
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
            verify_token(ARC_ID);
            auto id = std::string(current_token()->data);
            if(s_table_.has(id))
                error_log.exit(ErrorMessage{FATAL, current_token()->pos, current_filename_, "redeclared identifier"});
            next_token_noreturn();
            expect_token(ARC_COLON);
            auto *decl_expr = parse_expr();
            expect_token(ARC_COMMA);
            auto *expr = parse_expr();
            // TODO infer type here
            auto *decl = ast_.new_decl_node(start_pos, id, TYPE_i32, decl_expr);
            s_table_.add_symbol(id, VARIABLE, token_kind_to_type(current_token()->kind));

            auto *block = parse_block();
            auto *result = ast_.new_statement_node_for(start_pos, ast_.new_for_node(start_pos, decl, expr, block));
            return result;
        }
        case ARC_IF:      // FIXME must have an if before an elif statement
        case ARC_ELIF:    // probably need a to maintain a stack to verify this, also seperate these switch cases
        {
            //expect_token(ARC_IF);
            next_token_noreturn();
            auto *expr = parse_expr();
            auto *block = parse_block();
            auto *else_stmnt = parse_else();
            auto *result = ast_.new_statement_node_if(start_pos, ast_.new_if_node(start_pos, expr, block, else_stmnt));
            return result;
        }
        case ARC_RET:
        {
            expect_token(ARC_RET);
            auto *result = ast_.new_statement_node_ret(start_pos, ast_.new_ret_node(start_pos, parse_expr()));
            if(current_token()->kind != ARC_SEMICOLON)    // FIXME this error is never reached
                error_log.exit(ErrorMessage{FATAL, current_token()->pos, current_filename_, "Expected semicolon"});
            return result;

        }
        case ARC_ID:
            // can be decl or expr
            if(peek_next_token()->kind == ARC_COLON || peek_next_token()->kind == ARC_INFER)
                return ast_.new_statement_node_decl(start_pos, parse_decl());
            else
                return ast_.new_statement_node_expr(start_pos, parse_expr());
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

Block* Parser::parse_else() {
    if(peek_next_token()->kind == ARC_ELSE) {
        next_token_noreturn();
        next_token_noreturn();
        return parse_block();
    }
    else if(peek_next_token()->kind == ARC_ELIF) {
        next_token_noreturn();
        return parse_bare_block();
    }
    else {
        // this breaks everything
        // I don't remember what this was for
        // next_token_noreturn();
        return nullptr;
    }
}

Decl* Parser::parse_decl() {
    PROFILE();
    const SourcePos start_pos = current_token()->pos;
    std::string id = std::string(current_token()->data);
    if(s_table_.has(id)) {
        error_log.exit(ErrorMessage{FATAL, current_token()->pos, current_filename_, "redeclared identifier"});
    }
    if(peek_next_token()->kind == ARC_INFER) {
        next_token_noreturn();
        next_token_noreturn();
        auto *result = ast_.new_decl_node(start_pos, id, TYPE_unknown, parse_expr());
        s_table_.add_symbol(id, VARIABLE, TYPE_unknown);
        return result;
    }
    else if(next_token()->kind == ARC_COLON) {
        next_token_noreturn();
        const type_handle type = token_kind_to_type(current_token()->kind);
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
                    // where a variable is used in it's own declaration
                    // var := var + 1;
                    s_table_.add_symbol(id, VARIABLE, token_kind_to_type(current_token()->kind));
                    return result;
                }
                else {
                    //TODO syntax error here
                    error_log.exit(ErrorMessage{FATAL, current_token()->pos, current_filename_, "INVALID TOKEN"});
                }
                break;
            default:
                //TODO syntax error here
                error_log.exit(ErrorMessage{FATAL, current_token()->pos, current_filename_, "INVALID TOKEN"});
                break;
        }
    }
    else {
        //TODO syntax error here
         error_log.exit(ErrorMessage{FATAL, current_token()->pos, current_filename_, "INVALID TOKEN"});
    }
    return new Decl{};
}

/**
 * expects the current token to be the first token in the expression
 */
Expr* Parser::parse_expr(bool stop_at_paren) {
    auto result = parse_expr_0(stop_at_paren);
    return parse_expr_1(result);
}

std::vector<Token*> Parser::parse_expr_0(bool stop_at_paren) {
    PROFILE();
    std::vector<Token*> result;
    std::vector<Token*> stack;

    /**
     * shunting yard algorithm
     */
    auto paren_count = 0;
    while(current_token()->kind != ARC_SEMICOLON &&
            current_token()->kind != ARC_OPEN_BRACE &&
            current_token()->kind != ARC_COMMA) {
        if(current_token()->kind == ARC_TRUE ||
           current_token()->kind == ARC_FALSE ||
           current_token()->kind == ARC_INT_LIT ||
           current_token()->kind == ARC_FLOAT_LIT) {
            result.push_back(current_token());
        }
        else if(current_token()->kind == ARC_ID) {
            auto id = std::string(current_token()->data);

            if(!s_table_.has(id)) {
                error_log.exit(ErrorMessage{FATAL, current_token()->pos, current_filename_, "Unknown identifier"});
            }

            // if not function call
            if(peek_next_token()->kind != ARC_OPEN_PAREN &&
               peek_next_token()->kind != ARC_DOT &&
               s_table_.get_kind(id) == VARIABLE)
                result.push_back(current_token());
            else {
                if(peek_next_token()->kind == ARC_DOT) {
                    result.push_back(current_token());
                    next_token_noreturn();
                    expect_token(ARC_DOT);
                }
                auto* fn_call = current_token();
                next_token_noreturn();
                expect_token(ARC_OPEN_PAREN);
                while(current_token()->kind != ARC_CLOSE_PAREN) {
                    auto arg_expr = parse_expr_0(true);
                    result.insert(result.end(), arg_expr.begin(), arg_expr.end());
                    next_token_noreturn();
                }
                verify_token(ARC_CLOSE_PAREN);
                result.push_back(fn_call);
            }
        }
        else if(is_operator(current_token()->kind)) {  //TODO support unary operators also
            if(!is_unary_operator(current_token()->kind)) {
                while ((!stack.empty()) && (is_operator(stack.back()->kind)) &&
                        ((precedence(stack.back()->kind) > precedence(current_token()->kind)) ||
                        (precedence(stack.back()->kind) == precedence(current_token()->kind))) &&
                        (stack.back()->kind != ARC_OPEN_PAREN)) {
                    result.push_back(stack.back());
                    stack.pop_back();
                }
            }
            stack.push_back(current_token());
        }
        else if(current_token()->kind == ARC_OPEN_PAREN) {
            ++paren_count;
            stack.push_back(current_token());
        }
        else if(current_token()->kind == ARC_CLOSE_PAREN) {
            // if we reach a close paren without a open paren while parsing a function call
            // stop parsing the expr and continue
            if(stop_at_paren && --paren_count == -1) {
                prev_token_noreturn();
                break;
            }
            while(stack.back()->kind != ARC_OPEN_PAREN) {
                result.push_back(stack.back());
                stack.pop_back();
                if(stack.empty()) {
                    error_log.exit(ErrorMessage{FATAL, current_token()->pos, current_filename_, "Extra closing parentheses"});
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
    return result;
}

/**
 * convert the vector of Tokens into a Expr tree
 */
Expr* Parser::parse_expr_1(std::vector<Token*> result) {
    std::vector<Expr*> conversion_stack;
    for(auto *tkn : result) {
        if(is_operator(tkn->kind)) {
            if(is_unary_operator(tkn->kind)) {
                Expr* operand = conversion_stack.back();
                conversion_stack.pop_back();
                conversion_stack.push_back(ast_.new_expr_node_unary_expr(tkn->pos, tkn->kind, operand));
            }
            else {
                Expr* operand1 = conversion_stack.back();
                conversion_stack.pop_back();
                Expr* operand2 = conversion_stack.back();
                conversion_stack.pop_back();
                conversion_stack.push_back(ast_.new_expr_node_bin_expr(tkn->pos, tkn->kind, operand2, operand1));
            }
        }
        else {
            auto tkn_data = std::string(tkn->data);
            switch(tkn->kind) {
                case ARC_TRUE:
                    conversion_stack.push_back(ast_.new_expr_node_int_literal(tkn->pos, 1)); // TODO make bool literal node or something
                    break;
                case ARC_FALSE:
                    conversion_stack.push_back(ast_.new_expr_node_int_literal(tkn->pos, 0)); // TODO make bool literal node or something
                    break;
                case ARC_INT_LIT:
                    conversion_stack.push_back(ast_.new_expr_node_int_literal(tkn->pos, std::stoll(tkn_data)));
                    break;
                case ARC_FLOAT_LIT:
                    conversion_stack.push_back(ast_.new_expr_node_float_literal(tkn->pos, std::stod(tkn_data)));
                    break;
                case ARC_ID:
                    if(s_table_.is_function(tkn_data)) {
                        auto ret_type = s_table_.get_type(tkn_data);
                        auto num_args = s_table_.get_num_args(tkn_data);
                        auto fn_args = new Expr*[num_args];
                        for(u32 i = 0; i < num_args; ++i) {
                            fn_args[num_args - i - 1] = conversion_stack.back();
                            conversion_stack.pop_back();
                        }
                        auto node = ast_.new_expr_node_fn_call(tkn->pos, tkn_data, num_args, fn_args, ret_type);
                        conversion_stack.push_back(node);
                    }
                    else {
                        auto node = ast_.new_expr_node_variable(tkn->pos, tkn_data, s_table_.get_type(tkn_data));
                        conversion_stack.push_back(node);
                    }
                    break;
            }

        }
    }
    return conversion_stack.back();
}

inline Token* Parser::current_token() {
    PROFILE();
    return &tokens_[static_cast<u32>(index_)];
}

inline Token* Parser::next_token() {
    PROFILE();
    if(index_ + 1 >= tokens_.size()) {
        error_log.exit(ErrorMessage{FATAL, current_token()->pos, current_filename_, "Reached EOF while parsing"});
    }

    return &tokens_[static_cast<u32>(++index_)];
}

inline void Parser::next_token_noreturn() {
    PROFILE();
    if (index_ + 1 >= tokens_.size()) {
        error_log.exit(ErrorMessage{FATAL, current_token()->pos, current_filename_, "Reached EOF while parsing" });
    }
    ++index_;
}

inline void Parser::prev_token_noreturn() {
    PROFILE();
    if (index_ + 1 >= tokens_.size()) {
        error_log.exit(ErrorMessage{FATAL, current_token()->pos, current_filename_, "Reached EOF while parsing" });
    }
    --index_;
}

inline Token* Parser::peek_next_token() {
    PROFILE();
    if(index_ + 1 >= tokens_.size()) {
        error_log.exit(ErrorMessage{FATAL, current_token()->pos, current_filename_, "Reached EOF while parsing"});
    }
    return &tokens_[static_cast<u32>(index_ + 1)];
}

// check if current token is something
// does not increment
inline bool Parser::check_token(const TokenKind kind) {
    PROFILE();
    [[likely]] if(current_token()->kind == kind)
        return true;
    return false;
}

// check if current token is something
// does not increment
inline void Parser::verify_token(const TokenKind kind) {
    PROFILE();
    [[likely]] if(current_token()->kind == kind)
        return;
    error_log.exit(ErrorMessage{FATAL, current_token()->pos, current_filename_, "Unexpected token"});
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
    PROFILE();
    [[likely]] if(check_token(kind)) {
        next_token_noreturn();
        return true;
    }
    error_log.exit(ErrorMessage{FATAL, current_token()->pos, current_filename_, "Unexpected token"});
    next_token_noreturn();
    return false;
}

inline type_handle Parser::token_kind_to_type(const TokenKind tkn) {
    PROFILE();
    switch(tkn){
        case ARC_INFER:
            return TYPE_unknown;
        case ARC_TYPE:
            return TYPE_type;
        case ARC_I8:
            return TYPE_i8;
        case ARC_I16:
            return TYPE_i16;
        case ARC_I32:
            return TYPE_i32;
        case ARC_I64:
            return TYPE_i64;
        case ARC_U8:
            return TYPE_u8;
        case ARC_U16:
            return TYPE_u16;
        case ARC_U32:
            return TYPE_u32;
        case ARC_U64:
            return TYPE_u64;
        case ARC_F32:
            return TYPE_f32;
        case ARC_F64:
            return TYPE_f64;
    }
    return static_cast<type_handle>(-1);   // invalid value
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