#include "ErrorHandler.h"

#define NEWLINE std::cout << '\n'

void ErrorHandler::init(const char* f_data){
    m_filedata = f_data;
}

void ErrorHandler::printError(ErrorType type, std::vector<Token*>& tokens, unsigned int pos){

    /* This is the problem token */
    Token* err_token = tokens[pos];

    NEWLINE;
    NEWLINE;
    std::cout << "[" << err_token->l_pos << ", " << err_token->c_pos << "] " << F_DULL_RED(std::string("ERROR: "));
    switch(type){
        case ERR_REDECL:
            std::cout << "Redeclaration of identifier '" << err_token->val << "'";
            break;
        case ERR_UNDEFINED_ID:
            std::cout << "Undefined identifier '" << err_token->val << "'";
            break;
        case ERR_CLOSE_EXTRA_PAREN:
            std::cout << "Extra closing parenthesis";
            break;
        case ERR_OPEN_EXTRA_PAREN:
            std::cout << "Expected closing parenthesis";
            break;
    }

    /* Gets the first token in the current line */
    while(tokens[pos]->c_pos > 1){
        pos--;
    }

    /* Prints the whole line that contains the error */
    NEWLINE;
    log("    " << err_token->l_pos << " |    ");
    // while(pos < tokens.size() && tokens[pos]->l_pos == err_token->l_pos){
    //     log(tokens[pos++]->val << " ");
    // }
    unsigned int file_pos = tokens[pos]->F_c_pos-2;
    while(m_filedata[file_pos] != '\n' && m_filedata[file_pos] != '\r' && file_pos < strlen(m_filedata)){
        log(m_filedata[file_pos++]);
    }

    /* Outputs arrow pointing to error token */
    NEWLINE;
    log("    " << std::string(std::to_string(err_token->l_pos).length(), ' ') << " |    "); // that mess in the middle is to print the correct amount of spaces
    std::string indent(err_token->c_pos-1, ' ');
    std::cout << indent << "^" << std::string(err_token->val.length()-1, '~') << '\n';
}

std::string ErrorHandler::F_RED(std::string str){
    return "\033[1;31m" + str + "\033[0m";
}

std::string ErrorHandler::F_DULL_RED(std::string str){
    return "\033[31m" + str + "\033[0m";
}

std::string ErrorHandler::F_YELLOW(std::string str){
    return "\033[1;33m" + str + "\033[0m";
}

std::string ErrorHandler::F_DULL_YELLOW(std::string str){
    return "\033[33m" + str + "\033[0m";
}

std::string ErrorHandler::F_BLUE(std::string str){
    return "\033[1;36m" + str + "\033[0m";
}

std::string ErrorHandler::F_DULL_BLUE(std::string str){
    return "\033[36m" + str + "\033[0m";
}