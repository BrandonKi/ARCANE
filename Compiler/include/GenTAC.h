#include "SymbolTable.h"

struct value{
    std::string data;
    ST_TYPE type;
};

struct Quad{
    OP op;
    value operand1;
    value operand2;
    value result;
};

class GenTAC{

    private:
        std::vector<Quad> table;
        std::vector<SymbolTable>* symbol_table;
        unsigned int temp_num;

    public:
        GenTAC(std::vector<SymbolTable>* st): temp_num(0), symbol_table(st) {}
        std::vector<Quad> getTable(){ return table; }

        void TAC_genLabel(std::string&);
        void TAC_genStartFn();
        void TAC_genEndFn();
        void TAC_genRet(bool);
        void TAC_genParam(std::string&);
        void TAC_genCallFn(std::string&);
        void TAC_genVarDecl(std::string&, std::vector<Token*>*);
        void TAC_genExpr(std::vector<Token*>*);

        OP T_TO_TAC(OperatorDescriptor t);
        void printTable();
        std::string opToString(OP);
        ST_TYPE T_TO_ST(T_Type type);

        ST_TYPE getSymbolType(std::string&);

    private:


};