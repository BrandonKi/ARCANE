#include "Parser.h"

class Compiler{

    public:
        Compiler();
        std::vector<u8, arena_allocator<u8>> compile();

    private:
        astring readFile(astring);

        std::vector<RawFile, arena_allocator<RawFile>> getProjectFiles();
        std::vector<astring, arena_allocator<astring>> parseProjectSpecFile(astring& file);

        void trim(astring&);

};