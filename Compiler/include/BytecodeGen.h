/**
 * provides a thin layer of abstraction
 * over the IRGenerator in ARCVM
 */

#ifndef ARCANE_BYTECODEGEN_H
#define ARCANE_BYTECODEGEN_H

#include "TypeInference.h"

#include <Arcvm.h>

class BytecodeGen {
  public:
    BytecodeGen();

    arcvm::Arcvm gen_project(Project*);
    void gen_file(File*);
  private:
    void gen_import(Import*);
    void gen_function(Function*);
    void gen_statement(Statement*);
    void gen_while(WhileStmnt*);
    void gen_for(ForStmnt*);
    void gen_if(IfStmnt*);
    void gen_ret(RetStmnt*);
    void gen_decl(Decl*);
    void gen_expr(Expr*);

    void gen_immediate(i64);
    void gen_immediate(f64);
    void gen_immediate(std::string);

    void gen_id(std::string);
    void gen_fn_call(Expr*);
    void gen_bin(Expr*);
    void gen_unary(Expr*);
};

#endif