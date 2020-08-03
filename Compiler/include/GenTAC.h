#include "SymbolTable.h"



enum OP { TAC_UNDEF, TAC_PARAM, TAC_CALL, TAC_LABEL, TAC_FN_START, TAC_FN_END, TAC_TAC_GOTO, TAC_RET, TAC_EQUAL, TAC_ADD, TAC_SUB, TAC_MUL, TAC_DIV, TAC_MOD};

struct Quad{
    OP op;
    std::string operand1;
    std::string operand2;
    std::string result;
};

class GenTAC{

    private:
        std::vector<Quad> table;
        std::vector<SymbolTable>* symbol_table_list;
        unsigned int temp_num;

    public:
        GenTAC(std::vector<SymbolTable> symbol_table_list): symbol_table_list(&symbol_table_list), temp_num(0) {}
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