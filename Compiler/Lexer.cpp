#include "Lexer.h"

void Lexer::start(){
    // for(int i = 0; i < m_length; i++)
    //     std::cout << m_buf[i];
    std::string temp;
    for(unsigned int i = 0; i < m_length; i++){        
        switch(m_buf[i]){
            CASE_VALID_NAME_CHARACTERS
                i = handleName(i);
                break;
            CASE_DIGITS
                i = handleDigit(i);
                break;
            case '!':
                temp = "!";
                tokens.emplace_back(Token{TokenType(NOT), temp});
                break;
            case '\"':
                i = handleDoubleQuoteString(i);
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
                i = handleSingleQuoteString(i);
                break;
            case '(':
                temp = "(";
                tokens.emplace_back(Token{TokenType(LPAREN), temp});
                break;
            case ')':
                temp = ")";
                tokens.emplace_back(Token{TokenType(RPAREN), temp});
                break;
            case '*':
                temp = "*";
                tokens.emplace_back(Token{TokenType(OPERATOR), temp});
                break;
            case '+':
                temp = "+";
                tokens.emplace_back(Token{TokenType(OPERATOR), temp});
                break;
            case ',':
                temp = ",";
                tokens.emplace_back(Token{TokenType(COMMA), temp});
                break;
            case '-':
                temp = "-";
                tokens.emplace_back(Token{TokenType(OPERATOR), temp});
                break;
            case '.':
                temp = ".";
                tokens.emplace_back(Token{TokenType(DOT), temp});
                break;
            case '/':
                i = handleComment(i);
                break;
            case ':':
                break;
            case ';':
                break;
            case '<':
                temp = "<";
                tokens.emplace_back(Token{TokenType(COMPARISON), temp});
                break;
            case '=':
                i = handleEquals(i);
                break;
            case '>':
                temp = ">";
                tokens.emplace_back(Token{TokenType(COMPARISON), temp});
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
            default:
                break;
        }
    }
}

unsigned int Lexer::handleDigit(unsigned int i){
    while(i < m_length && isdigit(m_buf[i])){
        currentToken.emplace_back(m_buf[i]);
        i++;
    }
    currentToken.emplace_back('\0');
    i--;
    std::string temp = currentToken.data();
    tokens.emplace_back(Token{TokenType(NUMBER), temp});
    currentToken.clear();
    return i;
}

unsigned int Lexer::handleDoubleQuoteString(unsigned int i){
    i++;
    while(i < m_length && m_buf[i] != '\"'){
        if(m_buf[i] == '\\'){
            currentToken.emplace_back(m_buf[i]);
            i++;
        }
        currentToken.emplace_back(m_buf[i]);
        i++;
    }
    currentToken.emplace_back('\0');
    std::string temp = currentToken.data();
    tokens.emplace_back(Token{TokenType(STRING), temp});
    currentToken.clear();
    return i;
}

unsigned int Lexer::handleSingleQuoteString(unsigned int i){
    i++;
    while(i < m_length && m_buf[i] != '\''){
        if(m_buf[i] == '\\'){
            currentToken.emplace_back(m_buf[i]);
            i++;
        }
        currentToken.emplace_back(m_buf[i]);
        i++;
    }
    currentToken.emplace_back('\0');
    std::string temp = currentToken.data();
    tokens.emplace_back(Token{TokenType(STRING), temp});
    currentToken.clear();
    return i;
}

unsigned int Lexer::handleName(unsigned int i){
    while(i < m_length && (isalnum(m_buf[i]) || m_buf[i] == '_')){
        currentToken.emplace_back(m_buf[i]);
        i++;
    }
    currentToken.emplace_back('\0');
    i--;
    std::string temp = currentToken.data();
    tokens.emplace_back(Token{TokenType(NAME), temp});
    currentToken.clear();
    return i;
}

unsigned int Lexer::handleEquals(unsigned int i){
    std::string temp;
    if(i+1 < m_length && m_buf[i+1] == '='){
        temp = "==";
        tokens.emplace_back(Token{TokenType(COMPARISON), temp});
        i++;
    }else{
        temp = "=";
        tokens.emplace_back(Token{TokenType(OPERATOR), temp});
    }
    return i;
}                

unsigned int Lexer::handleComment(unsigned int i){
    if(i+1 < m_length && m_buf[i+1] != '/'){
        std::string temp = "/";
        tokens.emplace_back(Token{TokenType(OPERATOR), temp});
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