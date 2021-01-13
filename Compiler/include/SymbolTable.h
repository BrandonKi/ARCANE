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
        void addSymbol(std::string&);
        bool isFunction(std::string&);
        bool isVariable(std::string&);

    private:
        std::unordered_map<std::string, Symbol> table;

};