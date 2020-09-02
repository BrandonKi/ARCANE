#include "Lexer.h"

struct ST_entry{
    std::string name;
    ST_TYPE type;
    unsigned int l_pos;
    unsigned int c_pos;
    bool declared;
    bool param;
    std::vector<Token*> data;
    
};


class SymbolTable{

    private:
        std::unordered_map<std::string, ST_entry*> m_symbol_table;

    public:
        SymbolTable(){};
        void addSymbol(Token*, T_Type, std::vector<Token*>);
        void addSymbol(Token*, T_Type, bool);

        bool contains(std::string&);
        ST_entry* getSymbol(std::string&);

        T_Type getReturnType(std::string);

        void clear(){m_symbol_table.clear();}

        void printSymbolTable();

    private:
        std::string token_vec_to_string(std::vector<Token*>);
        std::string ST_type_to_string(ST_TYPE);
        ST_TYPE T_type_to_ST_type(T_Type);

};