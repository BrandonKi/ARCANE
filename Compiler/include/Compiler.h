#include "Parser.h"

class Compiler{

    public:
        Compiler();
        std::vector<u8> compile();

    private:
        std::string readFile(const std::string&);

        std::vector<RawFile> getProjectFiles();
        std::vector<std::string> parseProjectSpecFile(std::string& file);

        void trim(std::string&);

};