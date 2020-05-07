#include <fstream>
#include <iostream>
#include <cstring>

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
    std::ifstream file ("C:\\Users\\Kirin\\Desktop\\VM\\src\\example.dat", std::ios::in|std::ios::binary|std::ios::ate);
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
    std::ofstream file("C:\\Users\\Kirin\\Desktop\\VM\\src\\example.READ", std::ios::out);
    unsigned char* UCdata = reinterpret_cast<unsigned char*>(data);
    if(file.is_open()){
        for(int i = 0; i < (int)size-16; i++){
            logn("POINT " << i);
            switch(UCdata[i]){
                case 0x00:
                {
                    OUTPUT("NOP");
                    break;
                }
                case 0x01:
                {
                    OUTPUT("EXIT\t");
                    OUTPUT((unsigned int)UCdata[i++]);
                    break;
                }
                case 0x02:
                {
                    OUTPUT("NCONST_PUSH");
                    break;
                }
                case 0x10:
                {
                    OUTPUT("SBCONST_PUSH\t");
                    OUTPUT((int)UCdata[++i]);
                    break;
                }
                case 0x11:
                {
                    OUTPUT("UBCONST_PUSH\t");
                    OUTPUT((unsigned int)UCdata[++i]);
                    break;
                }
                case 0x12:
                {
                    OUTPUT("SICONST_PUSH\t");
                    int temp = UCdata[++i];
                    i += 2;
                    OUTPUT((int)temp);
                    break;
                }
                case 0x13:
                {
                    OUTPUT("UICONST_PUSH\t");
                    unsigned int temp = *reinterpret_cast<unsigned int*>(&UCdata[++i]);
                    i += 3;
                    OUTPUT((unsigned int)temp);
                    break;
                }
                case 0x14:
                {
                    OUTPUT("FCONST_PUSH\t");
                    float temp = *reinterpret_cast<float*>(&UCdata[++i]);
                    i += 3;
                    OUTPUT((float)temp);
                    break;
                }
                case 0x15:
                {
                    OUTPUT("DCONST_PUSH");
                    break;
                }
                case 0xa0:
                {
                    OUTPUT("SB_ADD");
                    break;
                }
                case 0xa1:
                {
                    OUTPUT("UB_ADD");
                    break;
                }
                case 0xa2:
                {
                    OUTPUT("SI_ADD");
                    break;
                }
                case 0xa3:
                {
                    OUTPUT("UI_ADD");
                    break;
                }
                case 0xa4:
                {
                    OUTPUT("F_ADD");
                    break;
                }
                case 0xa5:
                {
                    OUTPUT("D_ADD");
                    break;
                }
                case 0xc0:
                {
                    OUTPUT("SB_STORE\t");
                    i++;
                    OUTPUT((int)UCdata[i]);
                    break;
                }
                case 0xc1:
                {
                    OUTPUT("UB_STORE\t");
                    i++;
                    OUTPUT((unsigned int)UCdata[i]);
                    break;
                }
                case 0xc2:
                {
                    OUTPUT("SI_STORE\t");
                    OUTPUT(*reinterpret_cast<int*>(&UCdata[++i]));
                    i += 3;
                    break;
                }
                case 0xc3:
                {
                    OUTPUT("UI_STORE\t");
                    OUTPUT(*reinterpret_cast<unsigned int*>(&UCdata[++i]));
                    i += 3;
                    break;
                }
                
                case 0xee:
                {
                    OUTPUT("GOTO");
                    break;
                }
            }
            NEWLINE;
        }
    }
    file.close();
}