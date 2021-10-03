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
    void gen_file(File*, arcvm::Module*);
  private:
    std::vector<std::unordered_map<std::string, arcvm::Value>> variable_table_;

    void gen_import(Import*);
    void gen_function(Function*, arcvm::Function*);
    void gen_statement(Statement*, arcvm::Function*);
    void gen_while(WhileStmnt*, arcvm::Function*);
    void gen_for(ForStmnt*, arcvm::Function*);
    void gen_if(IfStmnt*, arcvm::Function*);
    void gen_ret(RetStmnt*, arcvm::Function*);
    void gen_decl(Decl*, arcvm::Function*);
    arcvm::Value gen_expr(Expr*, arcvm::Function*);

    arcvm::Value gen_immediate(i64, arcvm::Function*);
    arcvm::Value gen_immediate(f64, arcvm::Function*);
    arcvm::Value gen_immediate(std::string*, arcvm::Function*);

    arcvm::Value gen_rvalue_var(std::string*, arcvm::Function*);
    arcvm::Value gen_fn_call(Expr*, arcvm::Function*);
    arcvm::Value gen_bin(Expr*, arcvm::Function*);
    arcvm::Value gen_unary(Expr*, arcvm::Function*);
};

#endif