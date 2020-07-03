#include "Parser.h"

void Parser::start(){    
    for(pos_ptr = 0; pos_ptr < m_tokens.size(); pos_ptr++){
        parseStatement();
    }
    std::cin.get();
    symbol_table.printSymbolTable();
}

void Parser::parseStatement(){
    if(currentToken()->type == T_ID){
        if(peekNextToken()->type == T_INFER){
            parseInferDecl();
        }
        else if(peekNextToken()->type == T_COLON){
            parseExplicitDecl();
        }
    }
    switch(currentToken()->type){
        case T_FOR:
            break;
        case T_WHILE:
            break;
        case T_RETURN:
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
    }

}

void Parser::parseInferDecl(){
    nextToken();
    if(symbol_table.contains(currentToken()->val)){    // Check for redeclaration
        printError(ERR_REDECL);
        WAIT_AND_EXIT(-1);
    }
    nextToken();
    parseExpr(T_SEMICOLON);
}

void Parser::parseExplicitDecl(){
    //if(m_symbol_table.contains(currentToken()->val))

    if(symbol_table.contains(currentToken()->val)){    // Check for redeclaration
        printError(ERR_REDECL);
        WAIT_AND_EXIT(-1);
    }
    Token* name = currentToken();
    pos_ptr += 2;  // go forward two tokens
    TokenType type = currentToken()->type;
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
                symbol_table.addSymbol(name, type, expr);
            }
            else{
                symbol_table.addSymbol(name, type);
            }
            break;
        default:
            log("ERROR");
    }
}

std::vector<Token*> Parser::parseExpr(TokenType T_type){
    std::vector<Token*> expr;
    expr.reserve(30);
    while(true){
        DEBUG_PRINT_INFIX(std::cout << "{" << currentToken()->type << ", " << currentToken()->val << "} ");
        if(currentToken()->type == T_type){
            while(!m_stack.empty()){
                expr.push_back(m_stack.top());
                m_stack.pop();
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
                        REPORT_ERROR("Extra closing parenthesis at " << currentToken()->l_pos << ":" << currentToken()->c_pos);
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
            if(symbol_table.contains(currentToken()->val)){
                printError(ERR_UNDEFINED_ID);
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

void Parser::printError(ErrorType type){
    Token* token = currentToken();
    log('\n');
    switch(type){
        case ERR_REDECL:
            std::cout << "[" << token->l_pos << ", " << token->c_pos << "] ERROR: Redeclaration of '" 
            << token->val << "'\n\t" << token->l_pos << " | " << token->val << " " << nextToken()->val 
            << " " << nextToken()->val <<  " " << peekNextToken()->val << " ...";
            break;
        case ERR_UNDEFINED_ID:
            std::cout << "[" << token->l_pos << ", " << token->c_pos << "] ERROR: Undefined identifier '" 
            << token->val << "'\n\t" << token->l_pos << " | " << token->val << " " << nextToken()->val 
            << " " << nextToken()->val <<  " " << peekNextToken()->val << " ...";
            break;
    }
}