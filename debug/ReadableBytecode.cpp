#include <fstream>
#include <iostream>
#include <cstring>
#include "../ARCVM/MNEMONICS.h"

// #define COUT
#define SILENT

#define ERR(x) std::cout << x << "\n"; std::cin.get(); return -1
#define logn(x) std::cout << x << "\n"

#ifdef COUT
    #define OUTPUT(x) file << x; std::cout << x             //writes to file with console output
    #define NEWLINE file << "\n"; std::cout << "\n"
#else
    #define OUTPUT(x) file << x                             //writes to file without console output
    #define NEWLINE file << "\n"
#endif

typedef uint64_t uint;
typedef int64_t sint;

inline bool checkFileType(char* data);                      //check if file is the correct format
inline void start(char* data, int size, std::string path);                    //start the loop

int main(int argc, const char* argv[]){
    // C:\Users\Kirin\Desktop\ARCANE\tests\example.arcb
    std::streampos size;
    std::ifstream file (argv[1], std::ios::in|std::ios::binary|std::ios::ate);
    char* filedata;
    if (file.is_open()){
        size = file.tellg();
        filedata = (char*)malloc(size);
        file.seekg (0, std::ios::beg);
        file.read (filedata, size);
        file.close();
        if(checkFileType(filedata)){
            filedata = filedata + (16 * sizeof(char));
            start(filedata, (int)size, argv[1]);
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

inline void start(char* data, int size, std::string path){
    path += ".READ";
    std::ofstream file(path, std::ios::out);
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
                    OUTPUT((unsigned int)UCdata[++i]);
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
                    sint temp = *reinterpret_cast<sint*>(&UCdata[++i]);
                    i += 7;
                    OUTPUT((sint)temp);
                    break;
                }
                case _UICONST_PUSH_:
                {
                    OUTPUT("UICONST_PUSH\t");
                    uint temp = *reinterpret_cast<uint*>(&UCdata[++i]);
                    i += 7;
                    OUTPUT((uint)temp);
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
                    OUTPUT("DCONST_PUSH\t");
                    double temp = *reinterpret_cast<double*>(&UCdata[++i]);
                    i += 7;
                    OUTPUT((double)temp);
                    break;
                }
                case _LCONST_PUSH_:
                {
                    OUTPUT("LCONST_PUSH");
                    break;
                }
                case _NEW_ARR_:
                {
                    OUTPUT("NEW_ARR\t\t\t");
                    switch(UCdata[++i]){
                        case _NULL_:
                            OUTPUT(_NULL_);
                            break;
                        case _SBYTE_:
                            OUTPUT(_SBYTE_);
                            break;
                        case _UBYTE_:                                                               
                            OUTPUT(_UBYTE_);
                            break;
                        case _SINT_:
                            OUTPUT(_SINT_);
                            break;
                        case _UINT_:
                            OUTPUT(_UINT_);
                            break;
                        case _FLOAT_:
                            OUTPUT(_FLOAT_);
                            break;
                        case _DOUBLE_:
                            OUTPUT(_DOUBLE_);
                            break;
                        case _LONG_:
                            OUTPUT(_LONG_);
                            break;
                        case _STRING_:
                            OUTPUT(_STRING_);
                            break;
                        case _REF_:
                            OUTPUT(_REF_);
                            break;
                        case _SPECIAL_:
                            OUTPUT(_SPECIAL_);
                            break;
                        default:
                            break;
                    }
                    break;
                }
                case _UBA_STORE_:
                {
                    OUTPUT("UBA_STORE");
                    break;
                }
                case _UBA_LOAD_:
                {
                    OUTPUT("UBA_LOAD");
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
                case _SB_SUB_:
                {
                    OUTPUT("SB_ADD");
                    break;
                }
                case _UB_SUB_:
                {
                    OUTPUT("UB_ADD");
                    break;
                }
                case _SI_SUB_:
                {
                    OUTPUT("SI_ADD");
                    break;
                }
                case _UI_SUB_:
                {
                    OUTPUT("UI_ADD");
                    break;
                }
                case _F_SUB_:
                {
                    OUTPUT("F_ADD");
                    break;
                }
                case _D_SUB_:
                {
                    OUTPUT("D_ADD");
                    break;
                }
                case _L_SUB_:
                {
                    OUTPUT("L_ADD");
                    break;
                }
                case _SB_MUL_:
                {
                    OUTPUT("SB_ADD");
                    break;
                }
                case _UB_MUL_:
                {
                    OUTPUT("UB_ADD");
                    break;
                }
                case _SI_MUL_:
                {
                    OUTPUT("SI_ADD");
                    break;
                }
                case _UI_MUL_:
                {
                    OUTPUT("UI_ADD");
                    break;
                }
                case _F_MUL_:
                {
                    OUTPUT("F_ADD");
                    break;
                }
                case _D_MUL_:
                {
                    OUTPUT("D_ADD");
                    break;
                }
                case _L_MUL_:
                {
                    OUTPUT("L_ADD");
                    break;
                }
                case _SB_DIV_:
                {
                    OUTPUT("SB_ADD");
                    break;
                }
                case _UB_DIV_:
                {
                    OUTPUT("UB_ADD");
                    break;
                }
                case _SI_DIV_:
                {
                    OUTPUT("SI_ADD");
                    break;
                }
                case _UI_DIV_:
                {
                    OUTPUT("UI_ADD");
                    break;
                }
                case _F_DIV_:
                {
                    OUTPUT("F_ADD");
                    break;
                }
                case _D_DIV_:
                {
                    OUTPUT("D_ADD");
                    break;
                }
                case _L_DIV_:
                {
                    OUTPUT("L_ADD");
                    break;
                }
                case _SB_REM_:
                {
                    OUTPUT("SB_ADD");
                    break;
                }
                case _UB_REM_:
                {
                    OUTPUT("UB_ADD");
                    break;
                }
                case _SI_REM_:
                {
                    OUTPUT("SI_ADD");
                    break;
                }
                case _UI_REM_:
                {
                    OUTPUT("UI_ADD");
                    break;
                }
                case _F_REM_:
                {
                    OUTPUT("F_ADD");
                    break;
                }
                case _D_REM_:
                {
                    OUTPUT("D_ADD");
                    break;
                }
                case _L_REM_:
                {
                    OUTPUT("L_ADD");
                    break;
                }
                case _SB_STORE_:
                {
                    OUTPUT("SB_STORE\t\t");
                    uint temp = UCdata[++i];
                    OUTPUT((uint)temp);
                    break;
                }
                case _UB_STORE_:
                {
                    OUTPUT("UB_STORE\t\t");
                    uint temp = UCdata[++i];
                    OUTPUT((uint)temp);
                    break;
                }
                case _SI_STORE_:
                {
                    OUTPUT("SI_STORE\t\t");
                    uint temp = UCdata[++i];
                    OUTPUT((uint)temp);
                    break;
                }
                case _UI_STORE_:
                {
                    OUTPUT("UI_STORE\t\t");
                    uint temp = UCdata[++i];
                    OUTPUT((uint)temp);
                    break;
                }
                case _F_STORE_:
                {
                    OUTPUT("F_STORE\t\t");
                    uint temp = UCdata[++i];
                    OUTPUT((uint)temp);
                    break;
                }
                case _D_STORE_:
                {
                    OUTPUT("D_STORE\t\t");
                    uint temp = UCdata[++i];
                    OUTPUT((uint)temp);
                    break;
                }
                case _L_STORE_:
                {
                    OUTPUT("L_STORE\t\t");
                    uint temp = UCdata[++i];
                    OUTPUT((uint)temp);
                    break;
                }
                case _REF_STORE_:
                {
                    OUTPUT("REF_STORE\t\t");
                    uint temp = UCdata[++i];
                    OUTPUT((uint)temp);
                    break;
                }
                case _SB_LOAD_:
                {
                    OUTPUT("SB_LOAD\t\t\t");
                    uint temp = UCdata[++i];
                    OUTPUT((uint)temp);
                    break;
                }
                case _UB_LOAD_:
                {
                    OUTPUT("UB_LOAD\t\t\t");
                    uint temp = UCdata[++i];
                    OUTPUT((uint)temp);
                    break;
                }
                case _SI_LOAD_:
                {
                    OUTPUT("SI_LOAD\t\t\t");
                    uint temp = UCdata[++i];
                    OUTPUT((uint)temp);
                    break;
                }
                case _UI_LOAD_:
                {
                    OUTPUT("UI_LOAD\t\t\t");
                    uint temp = UCdata[++i];
                    OUTPUT((uint)temp);
                    break;
                }
                case _F_LOAD_:
                {
                    OUTPUT("F_LOAD\t\t\t");
                    uint temp = UCdata[++i];
                    OUTPUT((uint)temp);
                    break;
                }
                case _D_LOAD_:
                {
                    OUTPUT("D_LOAD\t\t\t");
                    uint temp = UCdata[++i];
                    OUTPUT((uint)temp);
                    break;
                }
                case _L_LOAD_:
                {
                    OUTPUT("L_LOAD\t\t\t");
                    uint temp = UCdata[++i];
                    OUTPUT((uint)temp);
                    break;
                }
                case _REF_LOAD_:
                {
                    OUTPUT("REF_LOAD\t\t");
                    uint temp = UCdata[++i];
                    OUTPUT((uint)temp);
                    break;
                }
                case _POP_:
                {
                    OUTPUT("POP");
                    break;
                }
                case _POP2_:
                {
                    OUTPUT("POP2");
                    break;
                }
                case _DUP_:
                {
                    OUTPUT("DUP");
                    break;
                }
                case _DUP2_:
                {
                    OUTPUT("DUP2");
                    break;
                }
                case _DUP2X_:
                {
                    OUTPUT("DUP2X");
                    break;
                }
                case _SWAP_:
                {
                    OUTPUT("SWAP");
                    break;
                }
                case _JMP_:
                {
                    OUTPUT("JMP\t\t\t\t");
                    unsigned char temp = UCdata[++i];
                    OUTPUT((unsigned int)temp);
                    break;
                }
                case _JIFNE_:
                {
                    OUTPUT("JIFNE\t\t\t");
                    unsigned char temp = UCdata[++i];
                    OUTPUT((unsigned int)temp);
                    break;
                }
                case _JIFE_:
                {
                    OUTPUT("JIFE\t\t\t");
                    unsigned char temp = UCdata[++i];
                    OUTPUT((unsigned int)temp);
                    break;
                }
                case _JIFLS_:
                {
                    OUTPUT("JIFLS\t\t\t");
                    unsigned char temp = UCdata[++i];
                    OUTPUT((unsigned int)temp);
                    break;
                }
                case _JIFGT_:
                {
                    OUTPUT("JIFGT\t\t\t");
                    unsigned char temp = UCdata[++i];
                    OUTPUT((unsigned int)temp);
                    break;
                }
                case _JIFZ_:
                {
                    OUTPUT("JIFZ\t\t\t");
                    unsigned char temp = UCdata[++i];
                    OUTPUT((unsigned int)temp);
                    break;
                }
                case _JIFNZ_:
                {
                    OUTPUT("JIFNZ\t\t\t");
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