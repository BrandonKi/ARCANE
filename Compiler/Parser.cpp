#include "Parser.h"

void Parser::start(){
    
    for(pos_ptr = 0; pos_ptr < m_tokens.size(); pos_ptr++){
        if(currentToken()->type == T_KEYWORD)
            parseKeyword();
        else
            parseExpr();
    }
}

void Parser::parseKeyword(){
    Token* token;
    switch(std::stoi(currentToken()->val)){
        case K_AUTO:
            break;
        case K_CHAR:
            break;
        case K_UCHAR:
            break;
        case K_BYTE:
            break;
        case K_UBYTE:
            break;
        case K_INT:
            token = nextToken();
            if(token->type == T_ID && nextToken()->type == T_ASSIGN){                            // This can be easily optimized
                parseExpr();
            }
            break;
        case K_UINT:
            break;
        case K_FLOAT:
            break;
        case K_DOUBLE:
            break;
        case K_LONG:
            break;
        case K_STRING:
            break;
        case K_FOR:
            break;
        case K_WHILE:
            break;
        case K_RETURN:
            break;
        case K_IF:
            break;
        case K_ELIF:
            break;
        case K_ELSE:
            break;
        case K_SWITCH:
            break;
        case K_CASE:
            break;
        case K_BREAK:
            break;
    };
    
}

void Parser::parseExpr(){
    std::vector<Token*> expr;
    expr.reserve(30);
    while(true){
        DEBUG_PRINT_INFIX(std::cout << currentToken()->val << " ");
        if(currentToken()->type == T_SEMICOLON){
            while(!m_stack.empty()){
                expr.push_back(m_stack.top());
                m_stack.pop();
            }
            break;
        }
        else if(currentToken()->type == T_LPAREN || currentToken()->type == T_RPAREN){
            if(currentToken()->type == T_LPAREN)
                m_stack.push(currentToken());
            else{
                while(m_stack.top()->type != T_LPAREN){
                    expr.push_back(m_stack.top());
                    m_stack.pop();
                }
                m_stack.pop();
            }
        }
        else if(currentToken()->type == T_OPERATOR){
            if(m_stack.empty() || precedence(currentToken()->val) > precedence(m_stack.top()->val))
                m_stack.push(currentToken());
            else if(precedence(currentToken()->val) == precedence(m_stack.top()->val)){  // needs to change. right now only works with left to right
                expr.push_back(m_stack.top());
                m_stack.pop();
                m_stack.push(currentToken());
            }
            else{
                while(!m_stack.empty() && m_stack.top()->type != T_LPAREN && precedence(currentToken()->val) < precedence(m_stack.top()->val)){
                    expr.push_back(m_stack.top());
                    m_stack.pop();
                }
                m_stack.push(currentToken());
            }
        }
        else if(currentToken()->type == T_FLOAT || currentToken()->type == T_NUMBER)
            expr.push_back(currentToken());
        nextToken();
    }

    DEBUG_PRINT_POSTFIX(
        std::cout << "\n";
        for(Token* t : expr)
            std::cout << t->val << " ";
    );
}

unsigned int Parser::precedence(std::string& op){       // this is not the real precedence table
    if(op.size() == 1){
        switch(op[0]){
            case '+':
            case '-':
                return 4;
            case '*':
            case '/':
            case '%':
                return 5;
            case '|':
            case '&':
                return 6;
            default:
                return 0;
        };
    }
    else{
        return 0;
    }
}


Token* Parser::nextToken(){
    return m_tokens[++pos_ptr];
}

Token* Parser::currentToken(){
    return m_tokens[pos_ptr];
}