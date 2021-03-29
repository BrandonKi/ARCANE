#include "AST.h"

enum SymbolType {
    VARIABLE,
    FUNCTION
};

struct Symbol {
    SymbolType type; 
    Type resultType;
};



class SymbolTable {
    using pair = std::pair<astring, Symbol>;
    using hash_map =
        std::unordered_map<
            astring,
            Symbol,
            std::hash<astring>,
            std::equal_to<astring>,
            arena_allocator<
                std::pair<
                    const astring,
                    Symbol
                >
            >
        >;

    public:
        void addSymbol(astring&, SymbolType, Type);
        bool has(astring&);
        bool isFunction(astring&);
        bool isVariable(astring&);

    private:

        hash_map table;



};