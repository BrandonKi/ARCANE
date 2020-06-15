#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <stack>

#define log(x) std::cout << x
#define logn(x) std::cout << x << "\n"
#define ERR(x) std::cout << x << "\n"; std::cin.get(); return -1

struct Token{
    std::string ID;
    union{
        struct{
            uint_fast8_t val;
        }character;
        struct{
            uint_fast64_t val;
        }integer;
        struct{
            std::string val; 
        }string;
        struct{
            std::string val;
        }name;
    };
};