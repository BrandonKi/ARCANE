#include "Lexer.h"


Lexer::Lexer(const char* filedata, unsigned int length):
    m_filedata(filedata), m_length(length), m_line(1), m_char(1), m_F_char(1)
{
    m_tokens.reserve(300);
    m_ctoken.reserve(30);
    initKeywords(); 
    start();
}

void Lexer::start(){
    unsigned int temp_int;
    for(unsigned int i = 0; i < m_length; i++){
        DEBUG_PRINT_STEP(printTokens(););
        Token* token;
        m_F_char++;
        m_char++;
        switch(m_filedata[i]){
            CASE_VALID_ID:
                temp_int = handleID(i) - i;
                i += temp_int;
                m_char += temp_int;
                m_F_char += temp_int;
                break;
            CASE_DIGITS:
                temp_int = handleDigit(i) - i;
                i += temp_int;
                m_char += temp_int;
                m_F_char += temp_int;
                break;
            case '!':
                m_tokens.emplace_back(new Token{T_Type(T_NOT), std::string(1, m_filedata[i]), m_line, m_char, m_F_char});
                break;
            case '\"':
                temp_int = handleDoubleQuoteString(i) - i;
                i += temp_int;
                m_char += temp_int;
                m_F_char += temp_int;
                break;
            case '#': 
                break;
            case '$': 
                break;
            case '%':
                temp_int = handleMod(i) - i;
                i += temp_int;
                m_char += temp_int;
                m_F_char += temp_int;
                break;
            case '&':
                temp_int = handleBinAnd(i) - i;
                i += temp_int;
                m_char += temp_int;
                m_F_char += temp_int;
                break;
            case '\'': 
                temp_int = handleSingleQuoteString(i) - i;
                i += temp_int;
                m_char += temp_int;
                m_F_char += temp_int;
                break;
            case '(':
                m_tokens.emplace_back(new Token{T_Type(T_LPAREN), std::string(1, m_filedata[i]), m_line, m_char, m_F_char});
                break;
            case ')':
                m_tokens.emplace_back(new Token{T_Type(T_RPAREN), std::string(1, m_filedata[i]), m_line, m_char, m_F_char});
                break;
            case '*':
                temp_int = handleMul(i) - i;
                i += temp_int;
                m_char += temp_int;
                m_F_char += temp_int;
                break;
            case '+':
                temp_int = handleAdd(i) - i;
                i += temp_int;
                m_char += temp_int;
                m_F_char += temp_int;
                break;
            case ',':
                m_tokens.emplace_back(new Token{T_Type(T_COMMA), std::string(1, m_filedata[i]), m_line, m_char, m_F_char});
                break;
            case '-':
                temp_int = handleSub(i) - i;
                i += temp_int;
                m_char += temp_int;
                m_F_char += temp_int;
                break;
            case '.':
                m_tokens.emplace_back(new Token{T_Type(T_DOT), std::string(1, m_filedata[i]), m_line, m_char, m_F_char});
                break;
            case '/':
                temp_int = handleComment(i) - i;
                i += temp_int;
                m_char += temp_int;
                m_F_char += temp_int;
                break;
            case ':':
                temp_int = handleColon(i) - i;
                i += temp_int;
                m_char += temp_int;
                m_F_char += temp_int;
                break;
            case ';':
                m_tokens.emplace_back(new Token{T_Type(T_SEMICOLON), std::string(1, m_filedata[i]), m_line, m_char, m_F_char});   
                break;
            case '<':
                temp_int = handleLessThan(i) - i;
                i += temp_int;
                m_char += temp_int;
                m_F_char += temp_int;
                break;
            case '=':
                temp_int = handleEquals(i) - i;
                i += temp_int;
                m_char += temp_int;
                m_F_char += temp_int;
                break;
            case '>':
                temp_int = handleGreaterThan(i) - i;
                i += temp_int;
                m_char += temp_int;
                m_F_char += temp_int;
                break;
            case '?':
                m_tokens.emplace_back(new Token{T_Type(T_TERNARY), std::string(1, m_filedata[i]), m_line, m_char, m_F_char});
                break;
            case '@':
                break;
            case '[':
                m_tokens.emplace_back(new Token{T_Type(T_LBRACKET), std::string(1, m_filedata[i]), m_line, m_char, m_F_char});
                break;
            case '\\':
                break;
            case ']':
                m_tokens.emplace_back(new Token{T_Type(T_RBRACKET), std::string(1, m_filedata[i]), m_line, m_char, m_F_char});
                break;
            case '^':
                break;
            case '`':
                break;
            case '{':
                m_tokens.emplace_back(new Token{T_Type(T_RBRACE), std::string(1, m_filedata[i]), m_line, m_char, m_F_char});
                break;
            case '|':
                temp_int = handleBinOr(i) - i;
                i += temp_int;
                m_char += temp_int;
                m_F_char += temp_int;
                break;
            case '}':
                m_tokens.emplace_back(new Token{T_Type(T_LBRACE), std::string(1, m_filedata[i]), m_line, m_char, m_F_char});
                break;
            case '~':
                break;
            case '\n':
                m_line++;
                m_char = 0;
                break;
            default:
                break;
            
        }
    }
}

unsigned int Lexer::handleDigit(unsigned int i){
    bool floating = false;
    while(i < m_length && isdigit(m_filedata[i])){
        m_ctoken.emplace_back(m_filedata[i]);
        i++;
        if(i < m_length && m_filedata[i] == '.'){
            floating = true;
            m_ctoken.emplace_back(m_filedata[i]);
            i++;
        }
    }
    m_ctoken.emplace_back('\0');
    i--;
    if(floating)
        m_tokens.emplace_back(new Token{T_Type(T_FLOAT_LIT), m_ctoken.data(), m_line, m_char, m_F_char});
    else if (std::stoll(m_ctoken.data()) > 0x7FFFFFFFFFFFFFFF){     // 64 bit int max
        m_tokens.emplace_back(new Token{T_Type(T_LONG_LIT), m_ctoken.data(), m_line, m_char, m_F_char});
    }
    else
        m_tokens.emplace_back(new Token{T_Type(T_NUMBER_LIT), m_ctoken.data(), m_line, m_char, m_F_char});
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
    if(m_ctoken.size() == 2)
        m_tokens.emplace_back(new Token{T_Type(T_CHAR_LIT), std::string(m_ctoken.data()), m_line, m_char, m_F_char});
    else
        m_tokens.emplace_back(new Token{T_Type(T_STR_LIT), std::string(m_ctoken.data()), m_line, m_char, m_F_char});
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
    if(m_ctoken.size() == 2)
        m_tokens.emplace_back(new Token{T_Type(T_CHAR_LIT), std::string(m_ctoken.data()), m_line, m_char, m_F_char});
    else
        m_tokens.emplace_back(new Token{T_Type(T_STR_LIT), std::string(m_ctoken.data()), m_line, m_char, m_F_char});
    m_ctoken.clear();
    return i;
}

unsigned int Lexer::handleID(unsigned int i){
    while(i < m_length && (isalnum(m_filedata[i]) || m_filedata[i] == '_')){
        m_ctoken.emplace_back(m_filedata[i]);
        i++;
    }
    i--;
    m_ctoken.emplace_back('\0');
    std::string id = std::string(m_ctoken.data());
    if(isKeyword(id))
        createKeywordToken(id);
    else
        m_tokens.emplace_back(new Token{T_Type(T_ID), id, m_line, m_char, m_F_char});
    m_ctoken.clear();
    return i;
}

unsigned int Lexer::handleColon(unsigned int i){
    if(i+1 < m_length && m_filedata[i+1] == '='){
        m_tokens.emplace_back(new Token{T_Type(T_INFER), std::string(":="), m_line, m_char, m_F_char});
        return ++i;
    }
    m_tokens.emplace_back(new Token{T_Type(T_COLON), std::string(1, m_filedata[i]), m_line, m_char, m_F_char});
    return i;
}

unsigned int Lexer::handleEquals(unsigned int i){
    std::string val;
    Token token;
    if(i+1 < m_length && m_filedata[i+1] == '='){
        val = "==";
        m_tokens.emplace_back(new Token{T_Type(T_OPERATOR), val, m_line, m_char, m_F_char});
        m_tokens.back()->op_info = OP_EQUAL;
        i++;
    }else{
        val = "=";
        m_tokens.emplace_back(new Token{T_Type(T_OPERATOR), val, m_line, m_char, m_F_char});
        m_tokens.back()->op_info = OP_ASSIGN;
    }
    return i;
}

unsigned int Lexer::handleSub(unsigned int i){
    std::string val;
    if(i+1 < m_length && m_filedata[i+1] == '='){
        val = "-=";
        m_tokens.emplace_back(new Token{T_Type(T_OPERATOR), val, m_line, m_char, m_F_char});
        m_tokens.back()->op_info = OP_SUB_EQUAL;
        i++;
    }else if(i+1 < m_length && m_filedata[i+1] == '-'){
        val = "--";
        Token* token =  new Token{T_Type(T_OPERATOR), val, m_line, m_char, m_F_char};
        if(m_tokens.back()->type = T_ID)
            token->op_info = OP_UNARY_POST_DEC;
        else
            token->op_info = OP_UNARY_PRE_DEC;
        m_tokens.emplace_back(token);
        i++;
    }
    else{                                                                      // unary sub needs some work
        Token* token = new Token{T_Type(T_OPERATOR), std::string(1, m_filedata[i]), m_line, m_char, m_F_char};
        if(m_tokens.back()->type != T_ID && m_tokens.back()->type != T_NUMBER_LIT && m_tokens.back()->type != T_FLOAT_LIT){
            token->op_info = OP_UNARY_SUB;
            token->val = "-u";
        }
        else
            token->op_info = OP_SUB;
        m_tokens.emplace_back(token);
    }
    return i;
}

unsigned int Lexer::handleAdd(unsigned int i){
    std::string val;
    if(i+1 < m_length && m_filedata[i+1] == '='){
        val = "+=";
        m_tokens.emplace_back(new Token{T_Type(T_OPERATOR), val, m_line, m_char, m_F_char});
        m_tokens.back()->op_info = OP_ADD_EQUAL;
        i++;
    }else if(i+1 < m_length && m_filedata[i+1] == '+'){
        val = "++";
        Token* token =  new Token{T_Type(T_OPERATOR), val, m_line, m_char, m_F_char};
        if(m_tokens.back()->type = T_ID)
            token->op_info = OP_UNARY_POST_INC;
        else
            token->op_info = OP_UNARY_PRE_INC;
        m_tokens.emplace_back(token);
        i++;
    }
    else{                                                                                   // unary plus needs some work
        Token* token = new Token{T_Type(T_OPERATOR), std::string(1, m_filedata[i]), m_line, m_char, m_F_char};
        if(m_tokens.back()->type != T_ID && m_tokens.back()->type != T_NUMBER_LIT && m_tokens.back()->type != T_FLOAT_LIT && m_tokens.back()->type != T_LONG_LIT){
            token->op_info = OP_UNARY_PLUS;
            token->val = "+u";
        }
        else
            token->op_info = OP_ADD;
        m_tokens.emplace_back(token);
    }
    return i;
}

unsigned int Lexer::handleDiv(unsigned int i){
    std::string val;
    if(i+1 < m_length && m_filedata[i+1] == '='){
        val = "/=";
        m_tokens.emplace_back(new Token{T_Type(T_OPERATOR), val, m_line, m_char, m_F_char});
        m_tokens.back()->op_info = OP_DIV_EQUAL;
        i++;
    }
    else{                                                                                
        m_tokens.emplace_back(new Token{T_Type(T_OPERATOR), std::string(1, m_filedata[i]), m_line, m_char, m_F_char});
        m_tokens.back()->op_info = OP_DIV;
    }
    return i;
}

unsigned int Lexer::handleMul(unsigned int i){
    std::string val;
    if(i+1 < m_length && m_filedata[i+1] == '='){
        val = "*=";
        m_tokens.emplace_back(new Token{T_Type(T_OPERATOR), val, m_line, m_char, m_F_char});
        m_tokens.back()->op_info = OP_MUL_EQUAL;
        i++;
    }
    else{                                                                                
        m_tokens.emplace_back(new Token{T_Type(T_OPERATOR), std::string(1, m_filedata[i]), m_line, m_char, m_F_char});
        m_tokens.back()->op_info = OP_MUL;
    }
    return i;
}

unsigned int Lexer::handleMod(unsigned int i){
    std::string val;
    if(i+1 < m_length && m_filedata[i+1] == '='){
        val = "%=";
        m_tokens.emplace_back(new Token{T_Type(T_OPERATOR), val, m_line, m_char, m_F_char});
        m_tokens.back()->op_info = OP_MOD_EQUAL;
        i++;
    }
    else{                                                                                
        m_tokens.emplace_back(new Token{T_Type(T_OPERATOR), std::string(1, m_filedata[i]), m_line, m_char, m_F_char});
        m_tokens.back()->op_info = OP_MOD;
    }
    return i;
}

unsigned int Lexer::handleBinAnd(unsigned int i){
    std::string val;
    if(i+1 < m_length && m_filedata[i+1] == '='){
        val = "&=";
        m_tokens.emplace_back(new Token{T_Type(T_OPERATOR), val, m_line, m_char, m_F_char});
        m_tokens.back()->op_info = OP_AND_EQUAL;
        i++;
    }
    else{                                                                                
        m_tokens.emplace_back(new Token{T_Type(T_OPERATOR), std::string(1, m_filedata[i]), m_line, m_char, m_F_char});
        m_tokens.back()->op_info = OP_BIN_AND;
    }
    return i;
}

unsigned int Lexer::handleBinOr(unsigned int i){
    std::string val;
    if(i+1 < m_length && m_filedata[i+1] == '='){
        val = "|=";
        m_tokens.emplace_back(new Token{T_Type(T_OPERATOR), val, m_line, m_char, m_F_char});
        m_tokens.back()->op_info = OP_OR_EQUAL;
        i++;
    }
    else{                                                                                
        m_tokens.emplace_back(new Token{T_Type(T_OPERATOR), std::string(1, m_filedata[i]), m_line, m_char, m_F_char});
        m_tokens.back()->op_info = OP_BIN_OR;
    }
    return i;
}

unsigned int Lexer::handleLessThan(unsigned int i){
    std::string val;
    if(i+1 < m_length && m_filedata[i+1] == '='){
        val = "<=";
        m_tokens.emplace_back(new Token{T_Type(T_OPERATOR), val, m_line, m_char, m_F_char});
        m_tokens.back()->op_info = OP_LESS_EQUAL;
        i++;
    }else if(i+1 < m_length && m_filedata[i+1] == '<'){
        val = "<<";
        m_tokens.emplace_back(new Token{T_Type(T_OPERATOR), val, m_line, m_char, m_F_char});
        m_tokens.back()->op_info = OP_LSHIFT;
        i++;
    }
    else{
        m_tokens.emplace_back(new Token{T_Type(T_OPERATOR), std::string(1, m_filedata[i]), m_line, m_char, m_F_char});
        m_tokens.back()->op_info = OP_LESS;
    }
    return i;
}

unsigned int Lexer::handleGreaterThan(unsigned int i){
    std::string val;
    if(i+1 < m_length && m_filedata[i+1] == '='){
        val = ">=";
        m_tokens.emplace_back(new Token{T_Type(T_OPERATOR), val, m_line, m_char, m_F_char});
        m_tokens.back()->op_info = OP_GREAT_EQUAL;
        i++;
    }else if(i+1 < m_length && m_filedata[i+1] == '>'){
        val = ">>";
        m_tokens.emplace_back(new Token{T_Type(T_OPERATOR), val, m_line, m_char, m_F_char});
        m_tokens.back()->op_info = OP_RSHIFT;
        i++;
    }
    else{
        m_tokens.emplace_back(new Token{T_Type(T_OPERATOR), std::string(1, m_filedata[i]), m_line, m_char, m_F_char});
        m_tokens.back()->op_info = OP_GREAT;
    }
    return i;
}

unsigned int Lexer::handleComment(unsigned int i){
    std::string val;
    if(i+1 < m_length && m_filedata[i+1] == '='){
        val = "/=";
        m_tokens.emplace_back(new Token{T_Type(T_OPERATOR), val, m_line, m_char, m_F_char});
        m_tokens.back()->op_info = OP_DIV_EQUAL;
        
    }else if(i+1 < m_length && m_filedata[i+1] != '/'){
        m_tokens.emplace_back(new Token{T_Type(T_OPERATOR), std::string(1, m_filedata[i]), m_line, m_char, m_F_char});
        m_tokens.back()->op_info = OP_DIV;
    }
    else if(i+1 < m_length && m_filedata[i+1] == '*'){
        for(; i < m_length && m_filedata[i] != '*' && m_filedata[i] != '/'; i++){}
    }
    else
        for(; i < m_length && m_filedata[i] != '\n'; i++){}             // eat the rest of the line
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
        {"$", T_AUTO}, {"char", T_CHAR}, {"uchar", T_UCHAR}, {"byte", T_BYTE}, {"ubyte", T_UBYTE}, {"int", T_INT}, 
        {"uint", T_UINT}, {"float", T_FLOAT}, {"double", T_DOUBLE}, {"long", T_LONG}, {"string", T_STRING}, 
        {"for", T_FOR}, {"while", T_WHILE}, {"ret", T_RET}, {"if", T_IF}, {"elif", T_ELIF}, {"else", T_ELSE}, 
        {"switch", T_SWITCH}, {"case", T_CASE}, {"break", T_BREAK}, {"fn", T_FN}, {"include", T_INCLUDE}, 
        {"import", T_IMPORT}

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

T_Type Lexer::getVal(std::string& key){
    return static_cast<T_Type>(m_keywords.find(key)->second);
}

void Lexer::createKeywordToken(std::string& id){
    m_tokens.emplace_back(new Token{getVal(id), id, m_line, m_char, m_F_char});
}