#include "Parser.h"

Parser::Parser(std::vector<RawFile>& projectFiles) {  //TODO good candidate for multithreading
    PROFILE();
    // std::thread worker([](){});
    // lex all the files
    for(RawFile rf: projectFiles){
        std::cout << rf.filepath << '\n';
        Lexer lexer(rf.filedata);
        data.push_back(LexedFile{rf.filepath, lexer.lex()});
    } 
}

Project* Parser::parse() {
    PROFILE();
    Project* project = parseProject();
	
    BytecodeGen gen(project);
    auto x = gen.genCode();

    std::ofstream bin("bin.txt", std::ios::out | std::ios::binary);
    bin.write(reinterpret_cast<char*> (x.data()), x.size());
    bin.close();

    return project;
}

Project* Parser::parseProject() {    //TODO another good candidate for multithreading
    PROFILE();
    std::vector<File*, arena_allocator<File*>> files;
    for(LexedFile& lf : data) {
        // update/reset member variables for each file
        tokens = lf.filedata;
        files.push_back(parseFile());
    }
    return ast.newProjectNode(SourcePos{}, files);
}

File* Parser::parseFile() {
    PROFILE();
    // reset the class wide index variable to zero
    index = 0;
    SourcePos startPos = currentToken()->pos;
    std::vector<Import*, arena_allocator<Import*>> imports;
    std::vector<Decl*, arena_allocator<Decl*>> decls;
    std::vector<Function*, arena_allocator<Function*>> functions;
    for(; index < tokens.size(); index++) {  
        switch(currentToken()->kind) {
            case ARC_IMPORT:
                println("import statement");
                imports.push_back(parseImport());
                break;
            case ARC_FN:
                println("function start");
                std::cout.flush();
                functions.push_back(parseFunction());
                println("function end");
                break;
            case ARC_ID:
                println("decl start");
                decls.push_back(parseDecl());
                break;
            case ARC_EOF:   // placed at the end of each file by the lexer
                return ast.newFileNode(startPos, imports, decls, functions, true);
                break;
        }
    }
    // should never reach this point but return just in case
    return ast.newFileNode(startPos, imports, decls, functions, true);
}

Import* Parser::parseImport() {
    PROFILE();
    return new Import{};
}


Function* Parser::parseFunction() {
    PROFILE();
    SourcePos startPos = currentToken()->pos;
    std::vector<Type, arena_allocator<Type>> fn_args;
    expectToken(ARC_FN);
    astring id;
    if(checkToken(ARC_ID))
        id = *(currentToken()->data);
    nextToken();
    expectToken(ARC_OPEN_PAREN);
    // parse func args
    expectToken(ARC_CLOSE_PAREN);
    expectToken(ARC_COLON);
    Type ret = tokenKind2Type(currentToken()->kind);
    if(ret == -1)
        errorLog.push(ErrorMessage{FATAL, currentToken(), args.path, "Unknown return type"});
    nextToken();
    s_table.addFunction(id, fn_args, FUNCTION, ret);
    Block* block = parseBlock();
    return ast.newFunctionNode(startPos, fn_args, ret, block, true);
}

/**
 *  expect first token to be opening brace '{' 
 */
Block* Parser::parseBlock() {
    PROFILE();
    s_table.pushScope();
    SourcePos startPos = currentToken()->pos;
    std::vector<Statement*, arena_allocator<Statement*>> statements;
    // cases
    // statement
    checkToken(ARC_OPEN_BRACE);
    while(nextToken()->kind != ARC_CLOSE_BRACE)
        statements.push_back(parseStatement());
    // nextToken(); // go past the closed brace
    return new Block{startPos, statements};
}

Statement* Parser::parseStatement() {
    PROFILE();
    SourcePos startPos = currentToken()->pos;
    // cases
    // while loop
    // for loop
    // if
    // ret
    // decl
    // expr
    switch (currentToken()->kind) {
        case ARC_WHILE:
        {
            expectToken(ARC_WHILE);
            auto expr = parseExpr();
            auto block = parseBlock();
            auto result = ast.newStatementNode_while(startPos, ast.newWhileNode(startPos, expr, block));
            return result;
            break;
        }
        case ARC_FOR:
        {
            expectToken(ARC_FOR);
            expectToken(ARC_OPEN_PAREN);
            auto decl = parseDecl();
            expectToken(ARC_SEMICOLON);
            auto expr1 = parseExpr();
            expectToken(ARC_SEMICOLON);
            auto expr2 = parseExpr();
            expectToken(ARC_SEMICOLON);
            expectToken(ARC_CLOSE_PAREN);
            auto block = parseBlock();
            auto result = ast.newStatementNode_for(startPos, ast.newForNode(startPos, decl, expr1, expr2, block));
            return result;
            break;
        }
        case ARC_IF:
        {
            expectToken(ARC_IF);
            auto expr = parseExpr();
            auto block = parseBlock();
            auto result = ast.newStatementNode_if(startPos, ast.newIfNode(startPos, expr, block));
            return result;
            break;
        }    
        case ARC_RET:
        {
            expectToken(ARC_RET);
            auto result = ast.newStatementNode_ret(startPos, ast.newRetNode(startPos, parseExpr()));
            if(currentToken()->kind != ARC_SEMICOLON)
                errorLog.push(ErrorMessage{FATAL, currentToken(), args.path, "Expected semicolon"});
            return result;
            break;
        }
        case ARC_ID:
            // can be decl or expr
            if(peekNextToken()->kind == ARC_COLON || peekNextToken()->kind == ARC_INFER)
                ast.newStatementNode_decl(startPos, parseDecl());
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

Decl* Parser::parseDecl() {
    PROFILE();
    SourcePos startPos = currentToken()->pos;
    astring& id = *(currentToken()->data);
    if(peekNextToken()->kind == ARC_INFER) {
        s_table.addSymbol(id, VARIABLE, TYPE_INFER);
    }
    else if(nextToken()->kind == ARC_COLON) {
        nextToken();
        Type type = tokenKind2Type(currentToken()->kind);
        switch(currentToken()->kind) {
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
                if(peekNextToken()->kind == ARC_SEMICOLON) {
                    //TODO definition but no declaration
                    s_table.addSymbol(id, VARIABLE, tokenKind2Type(currentToken()->kind));
                }
                else if(peekNextToken()->kind == ARC_ASSIGN) {
                    nextToken();
                    nextToken();
                    auto result = ast.newDeclNode(startPos, id, type, parseExpr());
                    // add symbol to table after parsing to avoid situations like this
                    // where a variable is used in it's own decl
                    // var := var + 1;
                    s_table.addSymbol(id, VARIABLE, tokenKind2Type(currentToken()->kind));
                    return result;
                }
                else {
                    //TODO syntax error here
                    errorLog.push(ErrorMessage{FATAL, currentToken(), args.path, "INVALID TOKEN"});
                }
                break;
            default:
                //TODO syntax error here
                errorLog.push(ErrorMessage{FATAL, currentToken(), args.path, "INVALID TOKEN"});
                break;
        }
    }
    else {
        //TODO syntax error here
        errorLog.push(ErrorMessage{FATAL, currentToken(), args.path, "INVALID TOKEN"});
    }
    return new Decl{};
}

/**
 * expects the current token to be the first token in the expression
 */
Expr* Parser::parseExpr() {
    PROFILE();
    std::vector<Token*, arena_allocator<Token*>> result;
    std::vector<Token*, arena_allocator<Token*>> stack;

    /**
     * shunting yard algorithm
     */
    while(currentToken()->kind != ARC_SEMICOLON && currentToken()->kind != ARC_OPEN_BRACE) {
        if(currentToken()->kind == ARC_INT_LIT) {
            result.push_back(currentToken());
        }
        else if(currentToken()->kind == ARC_ID) {    //TODO differentiate between functions and variables
            if(!s_table.has(*(currentToken()->data)))
                errorLog.push(ErrorMessage{FATAL, currentToken(), args.path, "Unknown identifier"});
            result.push_back(currentToken());
        }
    else if(isOperator(currentToken()->kind)) {  //TODO support unary operators also
            while ((!stack.empty()) && (isOperator(stack.back()->kind)) && 
                        ((precedence(stack.back()->kind) > precedence(currentToken()->kind)) || 
                            (precedence(stack.back()->kind) == precedence(currentToken()->kind))) && 
                                (stack.back()->kind != ARC_OPEN_PAREN)) {
                result.push_back(stack.back());
                stack.pop_back();
            }
            stack.push_back(currentToken());
        }
        else if(currentToken()->kind == ARC_OPEN_PAREN) {
            stack.push_back(currentToken());
        }
        else if(currentToken()->kind == ARC_CLOSE_PAREN) {
            while(stack.back()->kind != ARC_OPEN_PAREN) {
                result.push_back(stack.back());
                stack.pop_back();
                if(stack.empty()){
                    //TODO emit error message for mismatched parentheses
                }
            }
            stack.pop_back();
        }
        nextToken();
    }

    while(!stack.empty()) {
        result.push_back(stack.back());
        stack.pop_back();
    }

    /**
     * convert the vector of Tokens into a Expr tree
     */
    std::vector<Expr*, arena_allocator<Expr*>> conversionStack;
    for(int i = 0; i < result.size(); i++) {
        Token* token = result[i];
        if(isOperator(token->kind)) {
            if(isUnaryOperator(token->kind)) {
                conversionStack.push_back(ast.newExprNode_unaryExpr(token->pos, token->kind, conversionStack.back()));
                conversionStack.pop_back();
            }
            else {
                Expr* operand1 = conversionStack.back();
                conversionStack.pop_back();
                Expr* operand2 = conversionStack.back();
                conversionStack.pop_back();
                conversionStack.push_back(ast.newExprNode_binExpr(token->pos, token->kind, operand1, operand2));
            }
        }
        else {
            switch(result[i]->kind) {
                case ARC_INT_LIT:
                    conversionStack.push_back(ast.newExprNode_intLiteral(token->pos, astoll(*(token->data))));
                    break;
                case ARC_ID:
                    conversionStack.push_back(ast.newExprNode_variable(token->pos, token->data));
                    break;
            }
            
        }
        printTokenln(result[i]);
    }
    return conversionStack.back();
}

inline Token* Parser::currentToken() {
    PROFILE();
    return &tokens[index];
}

inline Token* Parser::nextToken() {
    PROFILE();
    if(index + 1 >= tokens.size()) {
        errorLog.push(ErrorMessage{FATAL, currentToken(), args.path, "Reached EOF while parsing"});
        errorLog.flush();
    }

    return &tokens[++index];
}

inline Token* Parser::peekNextToken() {
    PROFILE();
    if(index + 1 >= tokens.size()) {
        errorLog.push(ErrorMessage{FATAL, currentToken(), args.path, "Reached EOF while parsing"});
        errorLog.flush();
    }
    return &tokens[index + 1];
}

// check if current token is something
// does not increment
inline bool Parser::checkToken(TokenKind kind) {
    [[likely]] if(currentToken()->kind == kind)
        return true;
    errorLog.push(ErrorMessage{FATAL, currentToken(), args.path, "Unexpected token"});
    errorLog.flush();
    return false;    
}

// TODO since this function doesn't exit
// errors will propogate throughout the whole program
// unless the return value is handled
// maybe have a function that just exits when a unexpected token is found
// or turn off syntax errors for the current block and try to recover
//
// expect the current token to be something otherwise syntax error
//
inline bool Parser::expectToken(TokenKind kind) {
    [[likely]] if(checkToken(kind)) {
        nextToken();
        return true;
    }
    errorLog.push(ErrorMessage{FATAL, currentToken(), args.path, "Unexpected token"});
    errorLog.flush();
    nextToken();
    return false;
}

inline Type Parser::tokenKind2Type(TokenKind tkn) {
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
    return (Type)-1;
}

bool Parser::isOperator(TokenKind kind) {
    PROFILE();
    return kind >= 49;  //FIXME this needs to change if the TokenKind enum changes
}

bool Parser::isUnaryOperator(TokenKind kind) {
    PROFILE();
    return 
        kind == ARC_NEGATE || kind == ARC_NOT || kind == ARC_PRE_INCREMENT || 
        kind == ARC_POST_INCREMENT || kind == ARC_PRE_DECREMENT || kind == ARC_POST_DECREMENT;
}

u8 Parser::precedence(TokenKind kind) {
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
            return -1; // this should never happen
    }
}