#include "Lexer.h"


Lexer::Lexer(const char* filedata, unsigned int length):
    m_filedata(filedata), m_length(length)
{
    m_tokens.reserve(300);
    m_ctoken.reserve(30);
    initKeywords(); 
    start();
}

void Lexer::start(){
    for(unsigned int i = 0; i < m_length; i++){
        DEBUG_PRINT_STEP(printTokens(););
        Token* token;
        switch(m_filedata[i]){
            CASE_VALID_ID:
                i = handleID(i);
                break;
            CASE_DIGITS:
                i = handleDigit(i);
                break;
            case '!':
                m_tokens.emplace_back(new Token{TokenType(T_NOT), std::string(1, m_filedata[i]), i});
                break;
            case '\"':
                i = handleDoubleQuoteString(i);
                break;
            case '#': 
                break;
            case '$': 
                m_tokens.emplace_back(new Token{TokenType(T_KEYWORD), std::string(1, m_filedata[i]), i}); // this should be changed in the future
                break;                                                                                 // idk what I want to do here atm
            case '%':
                m_tokens.emplace_back(new Token{TokenType(T_OPERATOR), std::string(1, m_filedata[i]), i});
                break;
            case '&': 
                break;
            case '\'': 
                i = handleSingleQuoteString(i);
                break;
            case '(':
                m_tokens.emplace_back(new Token{TokenType(T_LPAREN), std::string(1, m_filedata[i]), i});
                break;
            case ')':
                m_tokens.emplace_back(new Token{TokenType(T_RPAREN), std::string(1, m_filedata[i]), i});
                break;
            case '*':
                m_tokens.emplace_back(new Token{TokenType(T_OPERATOR), std::string(1, m_filedata[i]), i});
                break;
            case '+':
                m_tokens.emplace_back(new Token{TokenType(T_OPERATOR), std::string(1, m_filedata[i]), i});
                break;
            case ',':
                m_tokens.emplace_back(new Token{TokenType(T_COMMA), std::string(1, m_filedata[i]), i});
                break;
            case '-':
                m_tokens.emplace_back(new Token{TokenType(T_OPERATOR), std::string(1, m_filedata[i]), i});
                break;
            case '.':
                m_tokens.emplace_back(new Token{TokenType(T_DOT), std::string(1, m_filedata[i]), i});
                break;
            case '/':
                i = handleComment(i);
                break;
            case ':':
                m_tokens.emplace_back(new Token{TokenType(T_COLON), std::string(1, m_filedata[i]), i});
                break;
            case ';':
                m_tokens.emplace_back(new Token{TokenType(T_SEMICOLON), std::string(1, m_filedata[i]), i});   
                break;
            case '<':
                i = handleLessThan(i);
                break;
            case '=':
                i = handleEquals(i);
                break;
            case '>':
                m_tokens.emplace_back(new Token{TokenType(T_OPERATOR), std::string(1, m_filedata[i]), i});
                break;
            case '?':
                m_tokens.emplace_back(new Token{TokenType(T_TERNARY), std::string(1, m_filedata[i]), i});
                break;
            case '@':
                break;
            case '[':
                m_tokens.emplace_back(new Token{TokenType(T_LBRACKET), std::string(1, m_filedata[i]), i});
                break;
            case '\\':
                break;
            case ']':
                m_tokens.emplace_back(new Token{TokenType(T_RBRACKET), std::string(1, m_filedata[i]), i});
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
    while(i < m_length && isdigit(m_filedata[i])){
        m_ctoken.emplace_back(m_filedata[i]);
        i++;
    }
    m_ctoken.emplace_back('\0');
    i--;
    m_tokens.emplace_back(new Token{TokenType(T_NUMBER), m_ctoken.data(), i});
    m_ctoken.clear();
    return i;
}

unsigned int Lexer::handleDoubleQuoteString(unsigned int i){
    i++;
    while(i < m_length && m_filedata[i] != '\"'){
        if(m_filedata[i] == '\\'){
            m_ctoken.emplace_back(m_filedata[i]);
            i++;
        }
        m_ctoken.emplace_back(m_filedata[i]);
        i++;
    }
    m_ctoken.emplace_back('\0');
    m_tokens.emplace_back(new Token{TokenType(T_STR), std::string(m_ctoken.data()), i});
    m_ctoken.clear();
    return i;
}

unsigned int Lexer::handleSingleQuoteString(unsigned int i){
    i++;
    while(i < m_length && m_filedata[i] != '\''){
        if(m_filedata[i] == '\\'){
            m_ctoken.emplace_back(m_filedata[i]);
            i++;
        }
        m_ctoken.emplace_back(m_filedata[i]);
        i++;
    }
    m_ctoken.emplace_back('\0');
    m_tokens.emplace_back(new Token{TokenType(T_STR), m_ctoken.data(), i});
    m_ctoken.clear();
    return i;
}

unsigned int Lexer::handleID(unsigned int i){
    while(i < m_length && (isalnum(m_filedata[i]) || m_filedata[i] == '_')){
        m_ctoken.emplace_back(m_filedata[i]);
        i++;
    }
    m_ctoken.emplace_back('\0');
    i--;
    std::string id = std::string(m_ctoken.data());
    if(isKeyword(id))
        m_tokens.emplace_back(new Token{TokenType(T_KEYWORD), std::to_string(getVal(id)), i});
    else
        m_tokens.emplace_back(new Token{TokenType(T_ID), id, i});
    m_ctoken.clear();
    return i;
}

unsigned int Lexer::handleEquals(unsigned int i){
    std::string val;
    Token token;
    if(i+1 < m_length && m_filedata[i+1] == '='){
        val = "==";
        m_tokens.emplace_back(new Token{TokenType(T_OPERATOR), val, i});
        i++;
    }else{
        val = "=";
        m_tokens.emplace_back(new Token{TokenType(T_ASSIGN), val, i});
    }
    return i;
}

unsigned int Lexer::handleLessThan(unsigned int i){
    std::string val;
    Token token;
    if(i+1 < m_length && m_filedata[i+1] == '='){
        val = "==";
        m_tokens.emplace_back(new Token{TokenType(T_OPERATOR), val, i});
        i++;
    }else{
        val = "<";
        m_tokens.emplace_back(new Token{TokenType(T_OPERATOR), std::string(1, m_filedata[i]), i});
    }
    return i;
}

unsigned int Lexer::handleComment(unsigned int i){
    if(i+1 < m_length && m_filedata[i+1] != '/'){
        m_tokens.emplace_back(new Token{TokenType(T_OPERATOR), std::string(1, m_filedata[i]), i});
    }
    else
        for(; i < m_length && m_filedata[i] != '\n'; i++){}
    return i;
}

void Lexer::printTokens(){
    std::cout << "\n";
    for (auto& t : m_tokens) {
        std:: cout << " {" << t->type << ", " << t->val << "} ";
    }
    std::cout << "\n";
}

void Lexer::initKeywords(){
    std::unordered_map<std::string, int> temp({
        {"$", K_AUTO}, {"char", K_CHAR}, {"uchar", K_UCHAR}, {"byte", K_BYTE}, {"ubyte", K_UBYTE}, {"int", K_INT}, 
        {"uint", K_UINT}, {"float", K_FLOAT}, {"double", K_DOUBLE}, {"long", K_LONG}, {"string", K_STRING}, 
        {"for", K_FOR}, {"while", K_WHILE}, {"return", K_RETURN}, {"if", K_IF}, {"elif", K_ELIF}, {"else", K_ELSE}, 
        {"switch", K_SWITCH}, {"case", K_CASE}, {"break", K_BREAK}

    });
    m_keywords = std::move(temp);
    DEBUG_PRINT_KEYWORDS(
        for(std::pair<std::string, int> element : m_keywords){
            std::cout << element.first << " :: " << element.second << "\n";
        }
    );
}

bool Lexer::isKeyword(std::string& key){
    return m_keywords.find(key) != m_keywords.end();
}

int Lexer::getVal(std::string& key){
    return m_keywords.find(key)->second;
}