#include "Parser.h"


int main(int argc, const char* argv[]){
    OS::init();
    for(int i = 0; i < argc; i++)
        std::cout << argv[i] << "\n";
    // unsigned int length = sizeof(argv[1]);
    if(argc > 1){
        std::ifstream file(argv[1]);
        if (file.is_open()){
            std::string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            file.close();
            const char* filedata = contents.c_str();
            ErrorHandler::init(filedata);
            Lexer lexer(filedata, (unsigned int)contents.length());
            std::vector<Token*> tokens = lexer.getTokens();
            DEBUG_PRINT_TOKENS(
                for (Token* t : tokens) 
                    std:: cout << " {" << t->type << ", " << t->val << "} ";
                std::cout << '\n';
            );
            Parser parser(tokens, lexer.getKeywords());
            std::cin.get();
            return 0;
        }else
            REPORT_ERROR("UNABLE_TO_OPEN_FILE");
    }else
        REPORT_ERROR("NO_FILE_PROVIDED");
}