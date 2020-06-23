#include "Lexer.h"


enum SymbolTableEntryType{ST_INT, ST_UINT, ST_CHAR, ST_UCHAR, ST_FLOAT, ST_DOUBLE, ST_LONG, ST_STRING, ST_ARRAY, ST_FUNC};

struct SymbolTableEntry{
    unsigned int c_pos;
    unsigned int l_pos;
    SymbolTableEntryType type;
};


class Parser{
    private:
        unsigned int pos_ptr;
        std::vector<Token*> m_tokens;
        std::stack<Token*> m_stack;
        std::unordered_map<std::string, SymbolTableEntry> m_symbol_table;

    public:
        Parser(std::vector<Token*> tokens, std::unordered_map<std::string, int> keywords): m_tokens(tokens), pos_ptr(0){start();}

    private:
        void start();

        void parseKeyword();
        void parseExpr();

        unsigned int precedence(OperatorDescriptor);

        Token* nextToken();
        Token* peekNextToken();
        Token* currentToken();

        void printSymbolTable();
        std::string ST_type_to_string(SymbolTableEntryType);
};