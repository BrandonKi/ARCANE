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
        _UBCONST_PUSH_,
        0xff,
        _NEW_ARR_,
        0x02,
        _DUP_,
        _UBCONST_PUSH_,
        0x08,
        _UBCONST_PUSH_,
        0x03,
        _UBA_STORE_,
        _REF_STORE_,
        0x00,
        _REF_LOAD_,
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


