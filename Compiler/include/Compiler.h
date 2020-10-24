#include "Parser.h"

class Compiler{

    public:
        Compiler();
        std::vector<u8> compile(const std::string&);
        std::string filedata();

    private:
        std::string data;

        std::string readFile(const std::string&);


};