#include <fstream>
#include <iostream>

int main(){

     unsigned char data[] = 
    {
        0xbb,
        0x09,
        0xbb,
        0x05,
        0xaa
    };
    std::ofstream file("C:\\Users\\Kirin\\Desktop\\VM\\example.dat", std::ios::out|std::ios::binary|std::ios::trunc);
    if(file.is_open()){
        file.write((char *)&data, sizeof(data));  
    }
    file.close();
    std::cin.get();
}


