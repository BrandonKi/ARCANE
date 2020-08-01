#include "Parser.h"

bool inFunction = false;

void Parser::start(){    
    initSymbolTableList();
    parseStatementBlock();
    std::cin.get();
    STPrint();
}

void Parser::parseStatementBlock(){
    for(; pos_ptr < m_tokens.size(); pos_ptr++){
        parseStatement();
    }
}

void Parser::parseStatement(){
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
    data.push_back(0);
    if(nextToken()->type == T_LPAREN && peekNextToken()->type != T_RPAREN){
        while(peekNextToken()->type == T_COMMA || peekNextToken()->type == T_ID){
            nextToken();
            Token* temp = nextToken();        // identifier
            nextToken();                      // colon
            nextToken();                      // type
            symbol_table_list.back().addSymbol(temp, currentToken()->type, true);
        }
    }
    else
        nextToken();
    nextToken();        // colon
    data[0] = nextToken();
    nextToken();        // RBRACE
    symbol_table_list.back().addSymbol(fn_name, T_FN, data);
    IR_gen.TAC_genLabel(fn_name->val);
    IR_gen.TAC_genStartFn();
    newScope();                 // start of new scope. Appends a new symbol table to end
    inFunction = true;
    parseFnBody();
    inFunction = false;
    IR_gen.TAC_genEndFn();
    STPrint();
    symbol_table_list.pop_back();              //  clear sub symbol table after parsing
}

void Parser::parseFnBody(){
    parseStatementBlock();
}

void Parser::parseRet(){
    nextToken();
    std::vector<Token*> expr = parseExpr(T_SEMICOLON);
    IR_gen.TAC_genExpr(&expr);
    IR_gen.printTable();
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
                    ErrorHandler::printError(ERR_OPEN_EXTRA_PAREN, m_tokens, pos_ptr);
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
                        ErrorHandler::printError(ERR_CLOSE_EXTRA_PAREN, m_tokens, pos_ptr);
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
            std::cout << "\n" << currentToken()->val << "\n";
            if(!ID_isDefined(currentToken()->val)){
                ErrorHandler::printError(ERR_UNDEFINED_ID, m_tokens, pos_ptr);
                WAIT_AND_EXIT(-1);
            }
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