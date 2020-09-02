#include  "GenTAC.h"


void GenTAC::TAC_genLabel(std::string& label){
    table.push_back(Quad{TAC_LABEL, {std::string("")}, {std::string("")}, label});
}

void GenTAC::TAC_genStartFn(){
    table.push_back(Quad{TAC_FN_START, {std::string("")}, {std::string("")},  std::string("BeginFn")});
}

void GenTAC::TAC_genEndFn(){
    table.push_back(Quad{TAC_FN_END, {std::string("")}, {std::string("")}, std::string("EndFn")});
}

void GenTAC::TAC_genParam(std::string& var_name){
    table.push_back(Quad{TAC_PARAM, {std::string("")}, {std::string("")}, std::string("_") + var_name});
}

void GenTAC::TAC_genCallFn(std::string& label){
    
}

void GenTAC::TAC_genVarDecl(std::string& var_name, std::vector<Token*>* expr){
    TAC_genExpr(expr);
    table.push_back(Quad{TAC_EQUAL, table[table.size()-1].result, {std::string("")}, {var_name, getSymbolType(var_name)}});
}


void GenTAC::TAC_genExpr(std::vector<Token*>* expr_ptr){
    if(expr_ptr == nullptr)
        return;
    std::vector<Token*> expr = *expr_ptr;
    for(unsigned int i = 0; i < expr.size(); i++){
        if(expr[i]->type == T_NUMBER_LIT || expr[i]->type == T_FLOAT_LIT || expr[i]->type == T_ID){
            table.push_back(Quad{TAC_EQUAL, {expr[i]->val, T_TO_ST(expr[i]->type)}, {std::string(""), T_TO_ST(expr[i]->type)}, {std::string("_") + std::to_string(temp_num), T_TO_ST(expr[i]->type)}});
            temp_num++;
        }
        else if(expr[i]->type == T_OPERATOR){
            table.push_back(            //@ WARNING This will be a problem in the future. Operations between unlike types.
                Quad{T_TO_TAC(expr[i]->op_info), 
                {table[table.size()-2].result.data, table[table.size()-2].result.type}, 
                {table[table.size()-1].result.data, table[table.size()-1].result.type}, 
                {std::string("_") + std::to_string(temp_num), table[table.size()-1].result.type}});

            temp_num++;
        }
    }
}

void GenTAC::TAC_genRet(bool ret_void){
    if(ret_void)
        table.push_back(Quad{TAC_RET, {std::string("")}, {std::string("")}, std::string("ret")});
    else
        table.push_back(Quad{TAC_RET, {table[table.size()-1].result.data, table[table.size()-1].result.type}, {std::string("")}, std::string("ret")});
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
        logn(q.result.data << "  =  " << "  " << q.operand1.data << "  " << q.operand2.data << "  [" << opToString(q.op) << "]");
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

ST_TYPE GenTAC::T_TO_ST(T_Type type){
    switch(type){
        case T_CHAR:
            return ST_CHAR;
        case T_UCHAR:
            return ST_UCHAR;
        case T_NUMBER_LIT:
        case T_INT:
            return ST_INT;
        case T_UINT:
            return ST_UINT;
        case T_FLOAT_LIT:
        case T_FLOAT:
            return ST_FLOAT;
        case T_FN:
            return ST_FN;
    }
    return (ST_TYPE)0;
}

ST_TYPE GenTAC::getSymbolType(std::string& symbol){
    for(unsigned int i = symbol_table->size()-1; i >= 0; i--){
        return symbol_table->at(i).getSymbol(symbol)->type;
    }
}