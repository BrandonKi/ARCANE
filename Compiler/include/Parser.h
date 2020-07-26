#include "SymbolTable.h"

class Parser{
    private:
        bool inner_scope;
        unsigned int pos_ptr;
        std::vector<Token*> m_tokens;
        std::stack<Token*> m_stack;
        SymbolTable symbol_table;
        SymbolTable sub_table;
 
    public:
        Parser(std::vector<Token*> tokens, std::unordered_map<std::string, int> keywords): m_tokens(tokens), pos_ptr(0), inner_scope(false){start();}

    private:
        void start();

        void parseStatementBlock();
        void parseStatement();

        void parseInferDecl();
        void parseExplicitDecl();

        void parseFnDecl();
        void parseFnBody();

        void parseIfStatement();
        void parseForStatement();
        void parseWhileStatement();

        std::vector<Token*> parseExpr(T_Type);

        unsigned int precedence(OperatorDescriptor);

        Token* nextToken();
        Token* peekNextToken();
        Token* peekTwoTokens();
        Token* currentToken();
};