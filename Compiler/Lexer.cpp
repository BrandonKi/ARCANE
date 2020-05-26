#include "Lexer.h"

void Lexer::start(){
    std::cin.get();
    for(unsigned int i = 0; i < m_length; i++){
        // std::cout << m_buf[i] << " ";
        printTokens();
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
                tokens.push_back({TokenType(PAREN), "("});
                break;
            case ')':
                tokens.push_back({TokenType(PAREN), "("});
                break;
            case '*':
                tokens.push_back({TokenType(OPERATOR), "*"});
                break;
            case '+':
                tokens.push_back({TokenType(OPERATOR), "+"});
                break;
            case ',':
                tokens.push_back({TokenType(COMMA), ","});
                break;
            case '-':
                tokens.push_back({TokenType(OPERATOR), "-"});
                break;
            case '.':
                break;
            case '/':
                tokens.push_back({TokenType(OPERATOR), "/"});
                break;
            case ':':
                break;
            case ';':
                break;
            case '<':
                break;
            case '=':
                break;
            case '>':
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
    std::cout << "\n";
    printTokens();
}

unsigned int Lexer::handleDigit(unsigned int i){
    while(isdigit(m_buf[i])){
        currentToken.push_back(m_buf[i]);
        i++;
    }
    currentToken.push_back('\0');
    i--;
    char* data = new char[currentToken.size()];             // memory leak
    strcpy(data, currentToken.data());
    tokens.push_back({TokenType(NUMBER), data});
    currentToken.clear();
    return i;
}

unsigned int Lexer::handleString(unsigned int i){
    i++;
    while(m_buf[i] != '\"'){
        if(m_buf[i] == '\\'){
            currentToken.push_back(m_buf[i]);
            i++;
        }
        currentToken.push_back(m_buf[i]);
        i++;
    }
    currentToken.push_back('\0');
    char* data = new char[currentToken.size()];             // memory leak
    strcpy(data, currentToken.data());
    tokens.push_back({TokenType(STRING), data});
    currentToken.clear();
    return i;
}

unsigned int Lexer::handleName(unsigned int i){
    while(isalnum(m_buf[i]) || m_buf[i] == '_'){
        currentToken.push_back(m_buf[i]);
        i++;
    }
    currentToken.push_back('\0');
    i--;
    char* data = new char[currentToken.size()];             // memory leak
    strcpy(data, currentToken.data());
    tokens.push_back({TokenType(STRING), data});
    currentToken.clear();
    return i;
}

void Lexer::printTokens(){
    std::cout << "\n";
    for (auto& t : tokens) {
        std:: cout << " {" << t.type << ", " << t.data << "} ";
    }
    std::cout << "\n";
    unsigned int size = tokens.size();
    for (unsigned int i = 0; i < size; i++) {
        std:: cout << " {" << tokens[i].type << ", " << tokens[i].data << "} ";
    }
    std::cout << "\n";
}