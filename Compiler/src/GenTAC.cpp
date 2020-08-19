#include  "GenTAC.h"


void GenTAC::TAC_genLabel(std::string& label){
    table.push_back(Quad{TAC_LABEL, std::string(""), std::string(""), label});
}

void GenTAC::TAC_genStartFn(){
    table.push_back(Quad{TAC_FN_START, std::string(""), std::string(""),  std::string("BeginFn")});
}

void GenTAC::TAC_genEndFn(){
    table.push_back(Quad{TAC_FN_END, std::string(""), std::string(""), std::string("EndFn")});
}

void GenTAC::TAC_genParam(std::string& var_name){
    table.push_back(Quad{TAC_PARAM, std::string(""), std::string(""), std::string("_") + var_name});
}

void GenTAC::TAC_genCallFn(std::string& label){
    
}

void GenTAC::TAC_genVarDecl(std::string& var_name, std::vector<Token*>* expr){
    TAC_genExpr(expr);
    table.push_back(Quad{TAC_EQUAL, table[table.size()-1].result, std::string(""), var_name});
}


void GenTAC::TAC_genExpr(std::vector<Token*>* expr_ptr){
    if(expr_ptr == nullptr)
        return;
    std::vector<Token*> expr = *expr_ptr;
    for(unsigned int i = 0; i < expr.size(); i++){
        if(expr[i]->type == T_NUMBER_LIT || expr[i]->type == T_FLOAT_LIT || expr[i]->type == T_ID){
            table.push_back(Quad{TAC_EQUAL, expr[i]->val, std::string(""), std::string("_") + std::to_string(temp_num)});
            temp_num++;
        }
        if(expr[i]->type == T_OPERATOR){
            table.push_back(Quad{T_TO_TAC(expr[i]->op_info), table[table.size()-2].result, table[table.size()-1].result, std::string("_") + std::to_string(temp_num)});
            temp_num++;
        }
    }
}

void GenTAC::TAC_genRet(bool ret_void){
    if(ret_void)
        table.push_back(Quad{TAC_RET, std::string(""), std::string(""), std::string("ret")});
    else
        table.push_back(Quad{TAC_RET, table[table.size()-1].result, std::string(""), std::string("ret")});
}

OP GenTAC::T_TO_TAC(OperatorDescriptor t){
    switch(t){
        case OP_ADD:
            return TAC_ADD;
        case OP_SUB:
            return TAC_SUB;
        case OP_DIV:
            return TAC_DIV;
        case OP_MUL:
            return TAC_MUL;
        case OP_MOD:
            return TAC_MOD;
        default:
            return TAC_UNDEF;
    }
    return TAC_UNDEF;
}


void GenTAC::printTable(){
    log('\n');
    for(Quad q : table){
        logn("-------------------------");
        logn(q.result << "  =  " << "  " << q.operand1 << "  " << q.operand2 << "  [" << opToString(q.op) << "]");
    }
    logn("-------------------------");
    log('\n');
}

std::string GenTAC::opToString(OP op){
    switch(op){
        case TAC_ADD:
            return std::string("+");
        case TAC_SUB:
            return std::string("-");
        case TAC_DIV: 
            return std::string("/");
        case TAC_MUL:
            return std::string("*");
        case TAC_MOD:
            return std::string("%");
        default:
            return std::to_string((int)op);
    }
}