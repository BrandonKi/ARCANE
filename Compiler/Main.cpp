#include <iostream>
#include <fstream>
#include <cstring>

#define ERR(x) std::cout << x << "\n"; return -1

inline bool checkFileType(const char* temp);

int main(int argc, const char* argv[]){
    for(int i = 0; i < argc; i++)
        std::cout << argv[i] << "\n";
    std::cout << sizeof(argv[1]) << "\n";
    std::cout << strlen(argv[1]) << "\n";
    if(argc > 1){
        std::streampos size;
        std::ifstream file (argv[1], std::ios::in|std::ios::ate);
        char* filedata;
        if (file.is_open()){
            size = file.tellg();
            filedata = new char [size];
            file.seekg (0, std::ios::beg);
            file.read (filedata, size);
            file.close();
            if(checkFileType(argv[1])){
                std::cin.get();
                filedata = filedata + (16 * sizeof(char));
                return 0;
            }else
                ERR("FILE_FORMAT_NOT_RECOGNIZED");
        }else
            ERR("UNABLE_TO_OPEN_FILE");
    }else
        ERR("NO_FILE_PROVIDED");
}

inline bool checkFileType(const char* data){   // Makes sure the extension is correct. There is no file signature because it is stored as plain text.
    return memcmp("arcs", &data[strlen(data) - 4], 4) == 0;
}