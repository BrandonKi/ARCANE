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
    std::cout << "[" << err_token->l_pos << ", " << err_token->c_pos << "] " << F_RED(std::move(std::string("ERROR: ")));
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
        case ERR_INVALID_FN_DECL:
            std::cout << "Cannot declare function here";
            break;
    }
    /* Gets the first token in the current line */
    unsigned int current_l_pos = tokens[pos]->l_pos;
    while(tokens[pos]->l_pos == current_l_pos){
        pos--;
    }
    pos++;

    NEWLINE;
    log("    " << err_token->l_pos << " |    ");
    unsigned int file_pos = tokens[pos]->F_c_pos-2;
    /* Prints the whole line that contains the error */
    while(m_filedata[file_pos] != '\n' && m_filedata[file_pos] != '\r' && file_pos < strlen(m_filedata)){
        log(m_filedata[file_pos++]);
    }

    /* Outputs arrow pointing to error token */
    NEWLINE;
    unsigned int whitespace_offset = tokens[pos]->c_pos;
    log("    " << std::string(std::to_string(err_token->l_pos).length(), ' ') << " |    "); // that mess is to print the correct amount of spaces
    std::string indent(err_token->c_pos - whitespace_offset, ' ');
    std::cout << indent << F_DULL_CYAN(std::string("^")) << F_DULL_CYAN(std::string(err_token->val.length()-1, '~')) << '\n';
}

std::string ErrorHandler::F_BLACK(std::string str){
    return std::move("\033[1;30m" + str + "\033[0m");
}

std::string ErrorHandler::F_DULL_BLACK(std::string str){
    return std::move("\033[30m" + str + "\033[0m");
}

std::string ErrorHandler::F_RED(std::string str){
    return std::move("\033[1;31m" + str + "\033[0m");
}

std::string ErrorHandler::F_DULL_RED(std::string str){
    return std::move("\033[31m" + str + "\033[0m");
}

std::string ErrorHandler::F_GREEN(std::string str){
    return std::move("\033[1;32m" + str + "\033[0m");
}

std::string ErrorHandler::F_DULL_GREEN(std::string str){
    return std::move("\033[32m" + str + "\033[0m");
}

std::string ErrorHandler::F_YELLOW(std::string str){
    return std::move("\033[1;33m" + str + "\033[0m");
}

std::string ErrorHandler::F_DULL_YELLOW(std::string str){
    return std::move("\033[33m" + str + "\033[0m");
}

std::string ErrorHandler::F_BLUE(std::string str){
    return std::move("\033[1;34m" + str + "\033[0m");
}

std::string ErrorHandler::F_DULL_BLUE(std::string str){
    return std::move("\033[34m" + str + "\033[0m");
}

std::string ErrorHandler::F_PINK(std::string str){
    return std::move("\033[1;35m" + str + "\033[0m");
}

std::string ErrorHandler::F_DULL_PINK(std::string str){
    return std::move("\033[35m" + str + "\033[0m");
}

std::string ErrorHandler::F_CYAN(std::string str){
    return std::move("\033[1;36m" + str + "\033[0m");
}

std::string ErrorHandler::F_DULL_CYAN(std::string str){
    return std::move("\033[36m" + str + "\033[0m");
}

std::string ErrorHandler::F_WHITE(std::string str){
    return std::move("\033[1;37m" + str + "\033[0m");
}

std::string ErrorHandler::F_DULL_WHITE(std::string str){
    return std::move("\033[37m" + str + "\033[0m");
}