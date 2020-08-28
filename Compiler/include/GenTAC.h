#include "SymbolTable.h"

struct Quad{
    OP op;
    std::string operand1;
    std::string operand2;
    std::string result;
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