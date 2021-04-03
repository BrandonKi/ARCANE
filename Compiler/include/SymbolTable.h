#include "ByteCodeGen.h"

enum SymbolType {
    VARIABLE,
    FUNCTION
};

struct Symbol {
    SymbolType type; 
    Type resultType;
    std::vector<Type, arena_allocator<Type>> args;
};



class SymbolTable {
    public:

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
        SymbolTable();
        void addSymbol(astring&, SymbolType, Type);
        void addFunction(astring&, std::vector<Type, arena_allocator<Type>>, SymbolType, Type); 
        bool has(astring&);
        bool scopeHas(astring&);
        SymbolType getKind(astring&);
        bool isFunction(astring&);
        bool isVariable(astring&);
        void pushScope();
        void popScope();

    private:

        hash_map global;
        std::vector<hash_map, arena_allocator<hash_map>> table;
                
        pair get(astring&);
        hash_map& currentScope();

};