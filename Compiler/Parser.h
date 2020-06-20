#include "Lexer.h"


enum SymbolTableEntryType{ST_INT, ST_UINT, ST_CHAR, ST_UCHAR, ST_FLOAT, ST_DOUBLE, ST_STRING, ST_ARRAY};

struct SymbolTableEntry{
    std::string ID;
    unsigned int pos;
    SymbolTableEntryType type;
};


class Parser{
    private:
        unsigned int pos_ptr;
        std::vector<Token*> m_tokens;
        std::stack<Token*> m_stack;

    public:
        Parser(std::vector<Token*> tokens, std::unordered_map<std::string, int> keywords): m_tokens(tokens), pos_ptr(0){start();}

    private:
        void start();

        void parseKeyword();
        void parseExpr();

        unsigned int precedence(std::string&);

        Token* nextToken();
        Token* currentToken();

};