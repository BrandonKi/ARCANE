#include <fstream>
#include <iostream>

int main(){

    //  unsigned char data[] = 
    // {
    //     0xbb,
    //     0x09,
    //     0xbb,
    //     0x05,
    //     0xab,
    //     0x00
    // };
     unsigned char data[] = 
    {
        0x42, // start file signature
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
        0x68, // end file signature

        0x11,
        0x02,
        0x11,
        0x01,
        0xa1,

        0x01, // exit
        0x00  // with code 0
    };

    std::ofstream file("C:\\Users\\Kirin\\Desktop\\VM\\src\\example.dat", std::ios::out|std::ios::binary|std::ios::trunc);
    if(file.is_open()){
        file.write((char *)data, sizeof(data));  
    }
    file.close();
    // std::cin.get();
}


