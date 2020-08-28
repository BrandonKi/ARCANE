#include "Parser.h"

bool inFunction = false;

void Parser::start(){    
    initSymbolTableList();
    parseFile();
    std::cin.get();
    STPrint();
}

void Parser::parseFile(){
    for(; pos_ptr < m_tokens.size(); pos_ptr++){
        parseStatement();
    }
}

void Parser::parseStatementBlock(){
    for(; pos_ptr < m_tokens.size(); pos_ptr++){
        parseStatement();
        logn("[[" << brace_count << "]]");
        if(brace_count == 0){
            return;
        }
    }
}

void Parser::parseStatement(){
    logn(currentToken()->val);
    if(currentToken()->type == T_ID){
        T_Type nextTokenType = peekNextToken()->type;
        if(nextTokenType == T_INFER){
            parseInferDecl();
        } 
        else if(nextTokenType == T_COLON){
            parseExplicitDecl();
        }
    }
    switch(currentToken()->type){
        case T_FOR:
            break;
        case T_WHILE:
            break;
        case T_RET:
            parseRet();
            break;
        case T_IF:
            break;
        case T_ELIF:
            break;
        case T_ELSE:
            break;
        case T_SWITCH:
            break;
        case T_CASE:
            break;
        case T_BREAK:
            break;
        case T_FN:
            parseFnDecl();
            break;
        case T_LBRACE:
            brace_count++;
            break;
        case T_RBRACE:
            brace_count--;
            break;
        default:
            break;
    }

}

void Parser::parseInferDecl(){
    nextToken();
    if(ID_isDefined(currentToken()->val)){    // Check for redeclaration
        ErrorHandler::printError(ERR_REDECL, m_tokens, pos_ptr);
        WAIT_AND_EXIT(-1);
    }
    nextToken();
    parseExpr(T_SEMICOLON);
}

void Parser::parseExplicitDecl(){

    if(ID_isDefined(currentToken()->val)){    // Check for redeclaration
        ErrorHandler::printError(ERR_REDECL, m_tokens, pos_ptr);
        WAIT_AND_EXIT(-1);
    }
    Token* name = currentToken();
    pos_ptr += 2;  // go forward two tokens
    T_Type type = currentToken()->type;
    switch(type){
        case T_CHAR:
        case T_BYTE:
        case T_UCHAR:
        case T_UBYTE:
        case T_INT:
        case T_UINT:
        case T_LONG:
        case T_FLOAT:
        case T_DOUBLE:
        case T_STRING:
            nextToken();
            if(currentToken()->type != T_SEMICOLON){
                nextToken();
                std::vector<Token*> expr = parseExpr(T_SEMICOLON);
                symbol_table_list.back().addSymbol(name, type, expr);
                IR_gen.TAC_genVarDecl(name->val, &expr);
            }
            else{
                symbol_table_list.back().addSymbol(name, type, false);
                IR_gen.TAC_genVarDecl(name->val, nullptr);
            }
            
            break;
        default:
            log("ERROR");
    }
}

void Parser::parseFnDecl(){

    if(inFunction && currentToken()->type == T_FN){
        ErrorHandler::printError(ERR_INVALID_FN_DECL, m_tokens, pos_ptr);
        std::exit(-1);
    }
    Token* fn_name = nextToken();
    std::vector<Token*> data;
    data.push_back(0);  // save space for return type
    newScope();         // start of new scope. Appends a new symbol table to end
    if(nextToken()->type == T_LPAREN && peekNextToken()->type != T_RPAREN){
        while(peekNextToken()->type == T_COMMA || peekNextToken()->type == T_ID){
            if(peekNextToken()->type == T_COMMA)
                nextToken();
            Token* temp = nextToken();        // identifier
            nextToken();                      // colon
            data.push_back(nextToken());      // type
            symbol_table_list.back().addSymbol(temp, currentToken()->type, true);
        }
    }
    nextToken();                // right paren
    nextToken();                // colon
    data[0] = nextToken();      // return type
    nextToken();
    symbol_table_list[symbol_table_list.size()-2].addSymbol(fn_name, T_FN, data);
    IR_gen.TAC_genLabel(fn_name->val);
    IR_gen.TAC_genStartFn();
    inFunction = true;
    parseFnBody();
    inFunction = false;
    IR_gen.TAC_genEndFn();
    STPrint();
    symbol_table_list.pop_back();              //  clear sub symbol table after parsing
    #ifdef DEBUG_BUILD
        IR_gen.printTable();
    #endif
}

void Parser::parseFnBody(){
    parseStatementBlock();
}

void Parser::parseRet(){
    nextToken();
    std::vector<Token*> expr = parseExpr(T_SEMICOLON);
    if(expr.size() == 0)                //@TODO check if this returns the correct type later
        IR_gen.TAC_genRet(true);
    else{
        IR_gen.TAC_genExpr(&expr);
        IR_gen.TAC_genRet(false);
    }
}

std::vector<Token*> Parser::parseExpr(T_Type T_type){
    std::vector<Token*> expr;
    expr.reserve(30);
    while(true){
        DEBUG_PRINT_INFIX(std::cout << "{" << currentToken()->type << ", " << currentToken()->val << "} ");
        if(currentToken()->type == T_type){
            while(!m_stack.empty()){
                expr.push_back(m_stack.top());
                m_stack.pop();
                if(expr.back()->type == T_LPAREN){
                    ErrorHandler::printError(ERR_EXTRA_OPEN_PAREN, m_tokens, pos_ptr);
                    WAIT_AND_EXIT(-1);
                }
            }
            break;
        }
        else if(currentToken()->type == T_LPAREN || currentToken()->type == T_RPAREN){
            if(currentToken()->type == T_LPAREN){
                m_stack.push(currentToken());
            }
            else{
                while(m_stack.top()->type != T_LPAREN){
                    expr.push_back(m_stack.top());
                    m_stack.pop();
                    if(m_stack.empty()){
                        ErrorHandler::printError(ERR_EXTRA_CLOSE_PAREN, m_tokens, pos_ptr);
                        WAIT_AND_EXIT(-1);
                    }
                }
                if(!m_stack.empty())
                    m_stack.pop();
                    
            }
        }
        else if(currentToken()->type == T_OPERATOR){
            if(m_stack.empty() || precedence(currentToken()->op_info) > precedence(m_stack.top()->op_info)){
                m_stack.push(currentToken());
            }
            else if(precedence(currentToken()->op_info) == precedence(m_stack.top()->op_info)){  // needs to change. right now only works with left to right
                if(currentToken()->op_info > 6){  // Checks if it is not a unary operator
                    expr.push_back(m_stack.top());
                    m_stack.pop();
                }
                m_stack.push(currentToken());
            }
            else{
                while(!m_stack.empty() && m_stack.top()->type != T_LPAREN && precedence(currentToken()->op_info) < precedence(m_stack.top()->op_info)){
                    expr.push_back(m_stack.top());
                    m_stack.pop();
                }
                m_stack.push(currentToken());
            }
        }
        else if(currentToken()->type == T_FLOAT_LIT || currentToken()->type == T_NUMBER_LIT || currentToken()->type == T_CHAR_LIT || currentToken()->type == T_STR_LIT)
            expr.push_back(currentToken());
        else if(currentToken()->type == T_ID ){
            if(!ID_isDefined(currentToken()->val)){
                ErrorHandler::printError(ERR_UNDEFINED_ID, m_tokens, pos_ptr);
                WAIT_AND_EXIT(-1);
            }
            expr.push_back(currentToken());
        }
        nextToken();
    }
    DEBUG_PRINT_POSTFIX(
        std::cout << "\n";
        for(Token* t : expr)
            std::cout << t->val << " ";
        std::cout << "\n";
    );
    return expr;
}

unsigned int Parser::precedence(OperatorDescriptor op){       // this is not the final precedence table
    switch(op){
        case OP_LOG_NOT:
        case OP_UNARY_PLUS:
        case OP_UNARY_SUB:
        case OP_UNARY_PRE_DEC:
        case OP_UNARY_PRE_INC:
        case OP_UNARY_POST_DEC:
        case OP_UNARY_POST_INC:
            return 8;

        case OP_MUL:
        case OP_DIV:
        case OP_MOD:
            return 7;
        
        case OP_ADD:
        case OP_SUB:
            return 6;
        
        case OP_BIN_OR:
        case OP_BIN_AND:
        case OP_LSHIFT:
        case OP_RSHIFT:
            return 5;

        case OP_EQUAL:
        case OP_NOT_EQUAL:
        case OP_LESS_EQUAL:
        case OP_GREAT_EQUAL:
        case OP_GREAT:
        case OP_LESS:
            return 4;

        case OP_LOG_AND:
        case OP_LOG_OR:
            return 3;
        
        case OP_ADD_EQUAL:
        case OP_SUB_EQUAL:
        case OP_MUL_EQUAL:
        case OP_DIV_EQUAL:
        case OP_MOD_EQUAL:
        case OP_LSHIFT_EQUAL:
        case OP_RSHIFT_EQUAL:
        case OP_AND_EQUAL:
        case OP_OR_EQUAL:
        case OP_XOR_EQUAL:
            return 2;
        
        case OP_ASSIGN:
            return 1;
        
        case OP_TERNARY:   // This doesn't do anything
        default:
            return 0;
    };
    return 0;
}


Token* Parser::nextToken(){
    return m_tokens[++pos_ptr];
}

Token* Parser::peekNextToken(){
    return m_tokens[pos_ptr + 1];
}

Token* Parser::peekTwoTokens(){
    return m_tokens[pos_ptr + 2];
}

Token* Parser::currentToken(){
    return m_tokens[pos_ptr];
}

void Parser::initSymbolTableList(){
    newScope();
}

void Parser::newScope(){
    symbol_table_list.emplace_back(SymbolTable());
}

bool Parser::ID_isDefined(std::string ID){
    for(SymbolTable st : symbol_table_list)
        if(st.contains(ID))
            return true;
    return false;
}

void Parser::STPrint(){
    for(SymbolTable st: symbol_table_list){
        std::cout << "\n";
        st.printSymbolTable();
        std::cout << "|\n";
    }
}