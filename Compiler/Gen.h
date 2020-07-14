#include "SymbolTable.h"

class Gen{

    private:

        std::string filepath;

    public:
        gen(std::string path): filepath(path){}

        void emitVarDecl();
        void emitExpr();

};