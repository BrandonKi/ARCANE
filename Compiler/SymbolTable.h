#include "Lexer.h"


enum ST_TYPE{ST_CHAR, ST_UCHAR, ST_INT, ST_UINT, ST_LONG, ST_FLOAT, ST_DOUBLE, ST_STRING, ST_ARRAY, ST_FUNC, ST_INVALID};

struct ST_entry{
    std::string name;
    ST_TYPE type;
    unsigned int l_pos;
    unsigned int c_pos;
    bool declared;
    std::vector<Token*> pf_expr;
    std::vector<ST_entry*> subtable;       // @FIX this doesn't work.
};

class SymbolTable{

    private:
        std::unordered_map<std::string, ST_entry*> m_symbol_table;

    public:
        SymbolTable(){};
        void addSymbol(Token*, TokenType, std::vector<Token*>);
        void addSymbol(Token*, TokenType, std::vector<ST_entry*>);
        void addSymbol(Token*, TokenType);

        bool contains(std::string);

        void printSymbolTable();

    private:
        std::string token_vec_to_string(std::vector<Token*>);
        std::string ST_type_to_string(ST_TYPE);
        ST_TYPE T_type_to_ST_type(TokenType);

};