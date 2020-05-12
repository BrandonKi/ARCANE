#include <fstream>

#include "MNEMONICS.h"

int main(){

     unsigned char data[] = 
    {
        0x42,           // start file signature
        0x72,
        0x61,
        0x6e,
        0x64,
        0x6f,
        0x6e,
        0x4b,
        0x69,
        0x72,
        0x69,
        0x6e,
        0x63,
        0x69,
        0x63,
        0x68,           // end file signature

                        // start program
        _FCONST_PUSH_,
        0x00,
        0x80,
        0x16,
        0x43,
        _FCONST_PUSH_,
        0x00,
        0x80,
        0x16,
        0x43,
        _F_ADD_,
        _F_STORE_,
        0x00,
                        // end program

        _EXIT_,         // exit
        0x00 
    };

    std::ofstream file("C:\\Users\\Kirin\\Desktop\\ARCANE\\ARCVM\\src\\example.arcb", std::ios::out|std::ios::binary|std::ios::trunc);
    if(file.is_open())
        file.write((char *)data, sizeof(data));  
    file.close();
}


