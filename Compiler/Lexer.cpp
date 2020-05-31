#include "Lexer.h"

void Lexer::start(){
    std::cin.get();
    // for(int i = 0; i < m_length; i++)
        // std::cout << m_buf[i];
    for(unsigned int i = 0; i < m_length; i++){
        DEBUG_PRINT_TOKENS;
        switch(m_buf[i]){
            CASE_VALID_NAME_CHARACTERS
                i = handleName(i);
                break;
            CASE_DIGITS
                i = handleDigit(i);
                break;
            case '!': 
                break;
            case '\"':
                i = handleString(i);
                break;
            case '#': 
                break;
            case '$': 
                break;
            case '%': 
                break;
            case '&': 
                break;
            case '\'': 
                break;
            case '(':
                tokens.emplace_back(Token{TokenType(PAREN), "("});
                break;
            case ')':
                tokens.emplace_back(Token{TokenType(PAREN), ")"});
                break;
            case '*':
                tokens.emplace_back(Token{TokenType(OPERATOR), "*"});
                break;
            case '+':
                tokens.emplace_back(Token{TokenType(OPERATOR), "+"});
                break;
            case ',':
                tokens.emplace_back(Token{TokenType(COMMA), ","});
                break;
            case '-':
                tokens.emplace_back(Token{TokenType(OPERATOR), "-"});
                break;
            case '.':
                break;
            case '/':
                i = handleComment(i);
                break;
            case ':':
                break;
            case ';':
                break;
            case '<':
                tokens.emplace_back(Token{TokenType(COMPARISON), "<"});
                break;
            case '=':
                i = handleEquals(i);
                break;
            case '>':
                tokens.emplace_back(Token{TokenType(COMPARISON), ">"});
                break;
            case '?':
                break;
            case '@':
                break;
            case '[':
                break;
            case '\\':
                break;
            case ']':
                break;
            case '^':
                break;
            case '`':
                break;
            case '{':
                break;
            case '|':
                break;
            case '}':
                break;
            case '~':
                break;
        }
    }
}

unsigned int Lexer::handleDigit(unsigned int i){
    while(i < m_length && isdigit(m_buf[i])){
        currentToken.push_back(m_buf[i]);
        i++;
    }
    currentToken.push_back('\0');
    i--;
    tokens.emplace_back(Token{TokenType(NUMBER), currentToken.data()});
    currentToken.clear();
    return i;
}

unsigned int Lexer::handleString(unsigned int i){
    i++;
    while(i < m_length && m_buf[i] != '\"'){
        if(m_buf[i] == '\\'){
            currentToken.push_back(m_buf[i]);
            i++;
        }
        currentToken.push_back(m_buf[i]);
        i++;
    }
    currentToken.push_back('\0');
    tokens.emplace_back(Token{TokenType(STRING), currentToken.data()});
    currentToken.clear();
    return i;
}

unsigned int Lexer::handleName(unsigned int i){
    while(i < m_length && (isalnum(m_buf[i]) || m_buf[i] == '_')){
        currentToken.push_back(m_buf[i]);
        i++;
    }
    currentToken.push_back('\0');
    i--;
    tokens.emplace_back(Token{TokenType(NAME), currentToken.data()});
    currentToken.clear();
    return i;
}

unsigned int Lexer::handleEquals(unsigned int i){
    if(i+1 < m_length && m_buf[i+1] == '='){
        tokens.emplace_back(Token{TokenType(COMPARISON), "=="});
        i++;
    }else
        tokens.emplace_back(Token{TokenType(OPERATOR), "="});
    return i;
}                

unsigned int Lexer::handleComment(unsigned int i){
    if(i+1 < m_length && m_buf[i+1] != '/'){
        tokens.emplace_back(Token{TokenType(OPERATOR), "/"});
    }
    else
        for(; i < m_length && m_buf[i] != '\n'; i++){}
    i--;
    return i;
}

void Lexer::printTokens(){
    std::cout << "\n";
    for (auto& t : tokens) {
        std:: cout << " {" << t.type << ", " << t.data << "} ";
    }
    std::cout << "\n";
}