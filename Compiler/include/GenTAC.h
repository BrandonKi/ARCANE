#include "SymbolTable.h"

struct value{
    std::string data;
    T_Type type;
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
        unsigned int temp_num;

    public:
        GenTAC(): temp_num(0) {}
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

    private:


};