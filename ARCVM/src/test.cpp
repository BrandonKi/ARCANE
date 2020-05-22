#include <iostream>
#include <cmath>


int main(){
    signed char temp[] = {'B', 'R', 'A', 0x00};
    uint64_t temp1 = (uint64_t)temp;
    std::cout << std::hex << temp1 << std::dec << "\n";
    uint32_t temp2 = (temp1 & 0x00000000ffffffff);
    uint32_t temp3 = (temp1 & 0xffffffff00000000) >> 32;
    uint64_t result = ((uint64_t)temp3 << 32) | temp2;
    std::cout << std::hex << result << std::dec << "\n";
    std::cout << ((signed char*)(result)) << "\n";
    std::cin.get();
}