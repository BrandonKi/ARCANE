#include "Parser.h"

class Compiler{

    public:
        Compiler();
        std::vector<u8> compile();

    private:
        astring readFile(astring);

        std::vector<RawFile> getProjectFiles();
        std::vector<astring> parseProjectSpecFile(astring& file);

        void trim(astring&);

};