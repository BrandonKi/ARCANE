#include "SymbolTable.h"


void SymbolTable::addSymbol(Token* token, TokenType type, std::vector<Token*> pf_expr){
    m_symbol_table[token->val] = new ST_entry{token->val, T_type_to_ST_type(type), token->l_pos, token->c_pos, true};
    logn(token->type << " ::: " << T_type_to_ST_type(token->type));
    m_symbol_table[token->val]->pf_expr = pf_expr;
}

void SymbolTable::addSymbol(Token* token, TokenType type, std::vector<ST_entry*> subtable){
    m_symbol_table[token->val] = new ST_entry{token->val, T_type_to_ST_type(type), token->l_pos, token->c_pos, true};
    logn(token->type << " ::: " << T_type_to_ST_type(token->type));
    m_symbol_table[token->val]->subtable = subtable; // if symbol is a function. Does not currently work.
}

void SymbolTable::addSymbol(Token* token, TokenType type){
    m_symbol_table[token->val] = new ST_entry{token->val, T_type_to_ST_type(type), token->l_pos, token->c_pos, true};
    logn(token->type << " ::: " << T_type_to_ST_type(token->type));
    m_symbol_table[token->val]->declared = false;    // if a variable is defined but not given a value 
    
}

bool SymbolTable::contains(std::string name){
    return m_symbol_table.find(name) != m_symbol_table.end();
}

void SymbolTable::printSymbolTable(){
    for (std::pair<std::string, ST_entry*> element : m_symbol_table)
        printf("%s :|: %s :|: {%u, %u} :|: %s\n", element.first.c_str(), ST_type_to_string(element.second->type).c_str(), element.second->l_pos, element.second->c_pos, token_vec_to_string(element.second->pf_expr).c_str());
}

std::string SymbolTable::token_vec_to_string(std::vector<Token*> vec){
    std::string result;
    result.reserve(vec.size());
    for(Token* t: vec){
        result += t->val + std::string(" ");
    }
    return result;
}

std::string SymbolTable::ST_type_to_string(ST_TYPE type){
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
        case ST_LONG:
            return std::string("ST_LONG");
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

ST_TYPE SymbolTable::T_type_to_ST_type(TokenType type){
    switch(type){
        case T_INT:
            return ST_INT;
        case T_UINT:
            return ST_UINT;
        case T_CHAR:
            return ST_CHAR;
        case T_UCHAR:
            return ST_UCHAR;
        case T_FLOAT:
            return ST_FLOAT;
        case T_DOUBLE:
            return ST_DOUBLE;
        case T_LONG:
            return ST_LONG;
        case T_STRING:
            return ST_STRING;
        case T_ARRAY:
            return ST_ARRAY;
        case T_FUNC:
            return ST_FUNC;
        default:
            return ST_INVALID;
    }
}