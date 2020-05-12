#include <fstream>
#include <iostream>
#include <cstring>
#include "MNEMONICS.h"

#define COUT

#define ERR(x) std::cout << x << "\n"; std::cin.get(); return -1
#define logn(x) std::cout << x << "\n"

#ifdef COUT
    #define OUTPUT(x) file << x; std::cout << x             //writes to file with console output
    #define NEWLINE file << "\n"; std::cout << "\n"
#else
    #define OUTPUT(x) file << x                             //writes to file without console output
    #define NEWLINE file << "\n"
#endif

inline bool checkFileType(char* data);                      //check if file is the correct format
inline void start(char* data, int size);                    //start the loop

int main(int argc, const char* argv[]){

    std::streampos size;
    std::ifstream file ("C:\\Users\\Kirin\\Desktop\\ARCANE\\ARCVM\\src\\example.arcb", std::ios::in|std::ios::binary|std::ios::ate);
    char* filedata;
    if (file.is_open()){
        size = file.tellg();
        filedata = (char*)malloc(size);
        file.seekg (0, std::ios::beg);
        file.read (filedata, size);
        file.close();
        if(checkFileType(filedata)){
            filedata = filedata + (16 * sizeof(char));
            start(filedata, (int)size);
            std::cin.get();
            return 0;
        }else{
            ERR("FILE_FORMAT_NOT_RECOGNIZED");
        }
    }else{
        ERR("UNABLE_TO_OPEN_FILE");
    }

}

inline bool checkFileType(char* data){
  return memcmp("BrandonKirincich", data, 16) == 0;
}

inline void start(char* data, int size){
    std::ofstream file("C:\\Users\\Kirin\\Desktop\\ARCANE\\ARCVM\\src\\example.arcb.READ", std::ios::out);
    unsigned char* UCdata = reinterpret_cast<unsigned char*>(data);
    if(file.is_open()){
        for(int i = 0; i < (int)size-16; i++){
            // logn("POINT " << i);
            switch(UCdata[i]){
                case _NOP_:
                {
                    OUTPUT("NOP");
                    break;
                }
                case _EXIT_:
                {
                    OUTPUT("EXIT\t\t\t");
                    OUTPUT((unsigned int)UCdata[i++]);
                    break;
                }
                case _NCONST_PUSH_:
                {
                    OUTPUT("NCONST_PUSH");
                    break;
                }
                case _SBCONST_PUSH_:
                {
                    OUTPUT("SBCONST_PUSH\t");
                    unsigned char temp = UCdata[++i];
                    OUTPUT((int)temp);
                    break;
                }
                case _UBCONST_PUSH_:
                {
                    OUTPUT("UBCONST_PUSH\t");
                    unsigned char temp = UCdata[++i];
                    OUTPUT((unsigned int)temp);
                    break;
                }
                case _SICONST_PUSH_:
                {
                    OUTPUT("SICONST_PUSH\t");
                    int temp = *reinterpret_cast<int*>(&UCdata[++i]);
                    i += 3;
                    OUTPUT((int)temp);
                    break;
                }
                case _UICONST_PUSH_:
                {
                    OUTPUT("UICONST_PUSH\t");
                    unsigned int temp = *reinterpret_cast<unsigned int*>(&UCdata[++i]);
                    i += 3;
                    OUTPUT((unsigned int)temp);
                    break;
                }
                case _FCONST_PUSH_:
                {
                    OUTPUT("FCONST_PUSH\t");
                    float temp = *reinterpret_cast<float*>(&UCdata[++i]);
                    i += 3;
                    OUTPUT((float)temp);
                    break;
                }
                case _DCONST_PUSH_:
                {
                    OUTPUT("DCONST_PUSH");
                    break;
                }
                case _LCONST_PUSH_:
                {
                    OUTPUT("LCONST_PUSH");
                    break;
                }
                case _SB_ADD_:
                {
                    OUTPUT("SB_ADD");
                    break;
                }
                case _UB_ADD_:
                {
                    OUTPUT("UB_ADD");
                    break;
                }
                case _SI_ADD_:
                {
                    OUTPUT("SI_ADD");
                    break;
                }
                case _UI_ADD_:
                {
                    OUTPUT("UI_ADD");
                    break;
                }
                case _F_ADD_:
                {
                    OUTPUT("F_ADD");
                    break;
                }
                case _D_ADD_:
                {
                    OUTPUT("D_ADD");
                    break;
                }
                case _L_ADD_:
                {
                    OUTPUT("L_ADD");
                    break;
                }
                case _SB_STORE_:
                {
                    OUTPUT("SB_STORE\t\t");
                    unsigned char temp = UCdata[++i];
                    OUTPUT((unsigned int)temp);
                    break;
                }
                case _UB_STORE_:
                {
                    OUTPUT("UB_STORE\t\t");
                    unsigned char temp = UCdata[++i];
                    OUTPUT((unsigned int)temp);
                    break;
                }
                case _SI_STORE_:
                {
                    OUTPUT("SI_STORE\t\t");
                    unsigned char temp = UCdata[++i];
                    OUTPUT((unsigned int)temp);
                    break;
                }
                case _UI_STORE_:
                {
                    OUTPUT("UI_STORE\t\t");
                    unsigned char temp = UCdata[++i];
                    OUTPUT((unsigned int)temp);
                    break;
                }
                case _F_STORE_:
                {
                    OUTPUT("F_STORE\t\t");
                    unsigned char temp = UCdata[++i];
                    OUTPUT((unsigned int)temp);
                    break;
                }
                case _D_STORE_:
                {
                    OUTPUT("D_STORE\t\t");
                    unsigned char temp = UCdata[++i];
                    OUTPUT((unsigned int)temp);
                    break;
                }
                case _L_STORE_:
                {
                    OUTPUT("L_STORE\t\t");
                    unsigned char temp = UCdata[++i];
                    OUTPUT((unsigned int)temp);
                    break;
                }
                case _SB_LOAD_:
                {
                    OUTPUT("SB_LOAD\t\t\t");
                    unsigned char temp = UCdata[++i];
                    OUTPUT((unsigned int)temp);
                    break;
                }
                case _UB_LOAD_:
                {
                    OUTPUT("UB_LOAD\t\t\t");
                    unsigned char temp = UCdata[++i];
                    OUTPUT((unsigned int)temp);
                    break;
                }
                case _SI_LOAD_:
                {
                    OUTPUT("SI_LOAD\t\t\t");
                    unsigned char temp = UCdata[++i];
                    OUTPUT((unsigned int)temp);
                    break;
                }
                case _UI_LOAD_:
                {
                    OUTPUT("UI_LOAD\t\t\t");
                    unsigned char temp = UCdata[++i];
                    OUTPUT((unsigned int)temp);
                    break;
                }
                case _F_LOAD_:
                {
                    OUTPUT("F_LOAD\t\t\t");
                    unsigned char temp = UCdata[++i];
                    OUTPUT((unsigned int)temp);
                    break;
                }
                case _D_LOAD_:
                {
                    OUTPUT("D_LOAD\t\t\t");
                    unsigned char temp = UCdata[++i];
                    OUTPUT((unsigned int)temp);
                    break;
                }
                case _L_LOAD_:
                {
                    OUTPUT("L_LOAD\t\t\t");
                    unsigned char temp = UCdata[++i];
                    OUTPUT((unsigned int)temp);
                    break;
                }
                case _GOTO_:
                {
                    OUTPUT("GOTO");
                    unsigned char temp = UCdata[++i];
                    OUTPUT((unsigned int)temp);
                    break;
                }
            }
            NEWLINE;
        }
    }
    file.close();
}