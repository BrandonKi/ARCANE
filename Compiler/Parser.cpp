#include "Parser.h"

void Parser::start(){
    
    for(pos_ptr = 0; pos_ptr < m_tokens.size(); pos_ptr++){
        if(currentToken()->type == T_KEYWORD)
            parseKeyword();
        else
            parseExpr();
    }
    printSymbolTable();
}

void Parser::parseKeyword(){
    Token* token;
    switch(currentToken()->key){
        case K_AUTO:
            break;
        case K_CHAR:
        case K_BYTE:
            token = nextToken();
            if(token->type == T_ID && peekNextToken()->type == T_OPERATOR && peekNextToken()->op_info == OP_ASSIGN){
                if(m_symbol_table.find(token->val) != m_symbol_table.end()){
                    REPORT_ERROR("[" << token->l_pos << ", " << token->c_pos << "] ERROR: Redeclaration of '" << token->val << "'\n\t" << token->l_pos << " |  char " << token->val << " " << nextToken()->val << " " << peekNextToken()->val << " ...");
                }
                m_symbol_table[token->val] = SymbolTableEntry{token->c_pos, token->l_pos, ST_CHAR};
                parseExpr();
            }
            break;
        case K_UCHAR:
        case K_UBYTE:
            token = nextToken();
            if(token->type == T_ID && peekNextToken()->type == T_OPERATOR && peekNextToken()->op_info == OP_ASSIGN){
                if(m_symbol_table.find(token->val) != m_symbol_table.end()){
                    REPORT_ERROR("[" << token->l_pos << ", " << token->c_pos << "] ERROR: Redeclaration of '" << token->val << "'\n\t" << token->l_pos << " |  uchar " << token->val << " " << nextToken()->val << " " << peekNextToken()->val << " ...");
                }
                m_symbol_table[token->val] = SymbolTableEntry{token->c_pos, token->l_pos, ST_UCHAR};
                parseExpr();
            }
            break;
        case K_INT:
            token = nextToken();
            if(token->type == T_ID && peekNextToken()->type == T_OPERATOR && peekNextToken()->op_info == OP_ASSIGN){
                if(m_symbol_table.find(token->val) != m_symbol_table.end()){
                    REPORT_ERROR("[" << token->l_pos << ", " << token->c_pos << "] ERROR: Redeclaration of '" << token->val << "'\n\t" << token->l_pos << " |  int " << token->val << " " << nextToken()->val << " " << peekNextToken()->val << " ...");
                }
                m_symbol_table[token->val] = SymbolTableEntry{token->c_pos, token->l_pos, ST_INT};
                parseExpr();
            }
            break;
        case K_UINT:
            token = nextToken();
            if(token->type == T_ID && peekNextToken()->type == T_OPERATOR && peekNextToken()->op_info == OP_ASSIGN){
                if(m_symbol_table.find(token->val) != m_symbol_table.end()){
                    REPORT_ERROR("[" << token->l_pos << ", " << token->c_pos << "] ERROR: Redeclaration of '" << token->val << "'\n\t" << token->l_pos << " |  uint " << token->val << " " << nextToken()->val << " " << peekNextToken()->val << " ...");
                }
                m_symbol_table[token->val] = SymbolTableEntry{token->c_pos, token->l_pos, ST_UINT};
                parseExpr();
            }
            break;
        case K_FLOAT:
            token = nextToken();
            if(token->type == T_ID && peekNextToken()->type == T_OPERATOR && peekNextToken()->op_info == OP_ASSIGN){
                if(m_symbol_table.find(token->val) != m_symbol_table.end()){
                    REPORT_ERROR("[" << token->l_pos << ", " << token->c_pos << "] ERROR: Redeclaration of '" << token->val << "'\n\t" << token->l_pos << " |  float " << token->val << " " << nextToken()->val << " " << peekNextToken()->val << " ...");
                }
                m_symbol_table[token->val] = SymbolTableEntry{token->c_pos, token->l_pos, ST_FLOAT};
                parseExpr();
            }
            break;
        case K_DOUBLE:
            token = nextToken();
            if(token->type == T_ID && peekNextToken()->type == T_OPERATOR && peekNextToken()->op_info == OP_ASSIGN){
                if(m_symbol_table.find(token->val) != m_symbol_table.end()){
                    REPORT_ERROR("[" << token->l_pos << ", " << token->c_pos << "] ERROR: Redeclaration of '" << token->val << "'\n\t" << token->l_pos << " |  double " << token->val << " " << nextToken()->val << " " << peekNextToken()->val << " ...");
                }
                m_symbol_table[token->val] = SymbolTableEntry{token->c_pos, token->l_pos, ST_DOUBLE};
                parseExpr();
            }
            break;
        case K_LONG:
            token = nextToken();
            if(token->type == T_ID && peekNextToken()->type == T_OPERATOR && peekNextToken()->op_info == OP_ASSIGN){
                if(m_symbol_table.find(token->val) != m_symbol_table.end()){
                    REPORT_ERROR("[" << token->l_pos << ", " << token->c_pos << "] ERROR: Redeclaration of '" << token->val << "'\n\t" << token->l_pos << " |  long " << token->val << " " << nextToken()->val << " " << peekNextToken()->val << " ...");
                }
                m_symbol_table[token->val] = SymbolTableEntry{token->c_pos, token->l_pos, ST_LONG};
                parseExpr();
            }
            break;
        case K_STRING:
            token = nextToken();
            if(token->type == T_ID && peekNextToken()->type == T_OPERATOR && peekNextToken()->op_info == OP_ASSIGN){
                if(m_symbol_table.find(token->val) != m_symbol_table.end()){
                    REPORT_ERROR("[" << token->l_pos << ", " << token->c_pos << "] ERROR: Redeclaration of '" << token->val << "'\n\t" << token->l_pos << " |  string " << token->val << " " << nextToken()->val << " " << peekNextToken()->val << " ...");
                }
                m_symbol_table[token->val] = SymbolTableEntry{token->c_pos, token->l_pos, ST_STRING};
                parseExpr();
            }
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
    expr.reserve(20);
    while(true){
        DEBUG_PRINT_INFIX(std::cout << "{" << currentToken()->type << ", " << currentToken()->val << "} ");
        if(currentToken()->type == T_SEMICOLON){
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
                expr.push_back(m_stack.top());
                m_stack.pop();
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
        else if(currentToken()->type == T_FLOAT || currentToken()->type == T_NUMBER || currentToken()->type == T_ID)
            expr.push_back(currentToken());
        nextToken();
    }

    DEBUG_PRINT_POSTFIX(
        std::cout << "\n";
        for(Token* t : expr)
            std::cout << t->val << " ";
        std::cout << "\n";
    );
}

unsigned int Parser::precedence(OperatorDescriptor op){       // this is not the final precedence table
    switch(op){
        case OP_LOG_NOT:
        case OP_UNARY_PRE_DEC:
        case OP_UNARY_PRE_INC:
            return 8;

        case OP_BIN_OR:
        case OP_BIN_AND:
        case OP_LSHIFT:
        case OP_RSHIFT:
            return 7;
    
        case OP_MUL:
        case OP_DIV:
        case OP_MOD:
            return 6;
        
        case OP_ADD:
        case OP_SUB:
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

Token* Parser::currentToken(){
    return m_tokens[pos_ptr];
}

void Parser::printSymbolTable(){
    for (std::pair<std::string, SymbolTableEntry> element : m_symbol_table)
        printf("%s :: %s {%u, %u}\n", element.first.c_str(), ST_type_to_string(element.second.type).c_str(), element.second.l_pos, element.second.c_pos);
}

std::string Parser::ST_type_to_string(SymbolTableEntryType type){
    switch(type){
        case ST_INT:
            return std::string("ST_INT");
        case ST_UINT:
            return std::string("ST_UINT");
        case ST_CHAR:
            return std::string("ST_CHAR");
        case ST_UCHAR:
            return std::string("ST_UCHAR");
        case ST_FLOAT:
            return std::string("ST_FLOAT");
        case ST_DOUBLE:
            return std::string("ST_DOUBLE");
        case ST_STRING:
            return std::string("ST_STRING");
        case ST_ARRAY:
            return std::string("ST_ARRAY");
        case ST_FUNC:
            return std::string("ST_FUNC");
        default:
            return std::string("INVALID_TYPE");
    }
}