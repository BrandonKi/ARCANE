#include "Lexer.h"


int main(int argc, const char* argv[]){
    for(int i = 0; i < argc; i++)
        std::cout << argv[i] << "\n";
    unsigned int length = sizeof(argv[1]);
    if(argc > 1){
        std::ifstream file(argv[1]);
        if (file.is_open()){
            std::string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            file.close();
            const char* filedata = contents.c_str();
            Lexer lexer(filedata, (unsigned int)contents.length());
            std::vector<Token*> tokens = lexer.getTokens();
            std::cin.get();
            return 0;
        }else
            ERR("UNABLE_TO_OPEN_FILE");
    }else
        ERR("NO_FILE_PROVIDED");
}