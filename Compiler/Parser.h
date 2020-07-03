#include "SymbolTable.h"

class Parser{
    private:
        unsigned int pos_ptr;
        std::vector<Token*> m_tokens;
        std::stack<Token*> m_stack;
        SymbolTable symbol_table;

    public:
        Parser(std::vector<Token*> tokens, std::unordered_map<std::string, int> keywords): m_tokens(tokens), pos_ptr(0){start();}

    private:
        void start();

        void parseStatement();

        void parseInferDecl();
        void parseExplicitDecl();

        void parseIfStatement();
        void parseForStatement();
        void parseWhileStatement();

        std::vector<Token*> parseExpr(TokenType);

        unsigned int precedence(OperatorDescriptor);

        Token* nextToken();
        Token* peekNextToken();
        Token* peekTwoTokens();
        Token* currentToken();

        void printError(ErrorType);
};