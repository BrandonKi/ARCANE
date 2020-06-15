#include "Parser.h"

#define ERR(x) std::cout << x << "\n"; std::cin.get(); return -1

inline bool checkFileType(const char* data, unsigned int length);

int main(int argc, const char* argv[]){
    for(int i = 0; i < argc; i++)
        std::cout << argv[i] << "\n";
    unsigned int length = sizeof(argv[1]);
    if(argc > 1){
        std::ifstream file(argv[1]);
        char* filedata;
        if (file.is_open()){
            std::string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            unsigned int size = contents.length();
            filedata = strdup(contents.c_str());
            file.close();
            if(checkFileType(argv[1], length)){
                Lexer lexer(filedata, size);
                std::vector<Token> tokens = lexer.getTokens();
                for (auto& t : tokens) 
                    std:: cout << " {" << t.type << ", " << t.data << "} ";
                std::cout << '\n';
                Parser parser(tokens);
                std::cout << "DONE";
                std::cin.get();
                return 0;
            }else
                ERR("FILE_FORMAT_NOT_RECOGNIZED");
        }else
            ERR("UNABLE_TO_OPEN_FILE");
    }else
        ERR("NO_FILE_PROVIDED");
}

inline bool checkFileType(const char* data, unsigned int length){   // Makes sure the extension is correct. There is no file signature because it is stored as plain text.
    return memcmp("arcs", &data[length], 4) == 0;
}