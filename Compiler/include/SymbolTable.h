#include "AST.h"

enum SymbolType {
    FUNCTION,
    VARIABLE
};

struct Symbol {
    
    SymbolType type; 
    Type resultType;
};

class SymbolTable {
    public:
        void addSymbol(astring&);
        bool isFunction(astring&);
        bool isVariable(astring&);

    private:
        std::unordered_map<astring, Symbol> table;

};