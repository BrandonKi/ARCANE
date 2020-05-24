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
            if(checkFileType(argv[1], size)){
                lex(filedata, size);
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
    return memcmp("arcs", &data[length - 4], 4) == 0;
}


void lex(const char* buf, unsigned int length){
    for(unsigned int i = 0; i < length; i++){
        switch(buf[0]){
            case '!': 
                break;
            case '\"': 
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
                break;
            case ')':
                break;
            case '*':
                break;
            case '+':
                break;
            case ',':
                break;
            case '-':
                break;
            case '.':
                break;
            case '/':
                break;
            case '0':
                break;
            case '1':
                break;
            case '2':
                break;
            case '3':
                break;
            case '4':
                break;
            case '5':
                break;
            case '6':
                break;
            case '7':
                break;
            case '8':
                break;
            case '9':
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
            case 'A':
                break;
            case 'B':
                break;
            case 'C':
                break;
            case 'D':
                break;
            case 'E':
                break;
            case 'F':
                break;
            case 'G':
                break;
            case 'H':
                break;
            case 'I':
                break;
            case 'J':
                break;
            case 'K':
                break;
            case 'L':
                break;
            case 'M':
                break;
            case 'N':
                break;
            case 'O':
                break;
            case 'P':
                break;
            case 'Q':
                break;
            case 'R':
                break;
            case 'S':
                break;
            case 'T':
                break;
            case 'U':
                break;
            case 'V':
                break;
            case 'W':
                break;
            case 'X':
                break;
            case 'Y':
                break;
            case 'Z':
                break;
            case '[':
                break;
            case '\\':
                break;
            case ']':
                break;
            case '^':
                break;
            case '_':
                break;
            case '`':
                break;
            case 'a':
                break;
            case 'b':
                break;
            case 'c':
                break;
            case 'd':
                break;
            case 'e':
                break;
            case 'f':
                break;
            case 'g':
                break;
            case 'h':
                break;
            case 'i':
                break;
            case 'j':
                break;
            case 'k':
                break;
            case 'l':
                break;
            case 'm':
                break;
            case 'n':
                break;
            case 'o':
                break;
            case 'p':
                break;
            case 'q':
                break;
            case 'r':
                break;
            case 's':
                break;
            case 't':
                break;
            case 'u':
                break;
            case 'v':
                break;
            case 'w':
                break;
            case 'x':
                break;
            case 'y':
                break;
            case 'z':
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
}