#include "AST.h"

class Parser{

    public:
        Parser(std::vector<RawFile>&);
        Project* parse();

    private:
        std::vector<LexedFile> data;

        Project* parseProject();
        File* parseFile(std::vector<Token*>&);
};