/**
 * provides a thin layer of abstraction
 * over the IRGen in ARCVM
 */

#ifndef ARCANE_IRGen_H
#define ARCANE_IRGen_H

#include "TypeInference.h"

struct ir_var {
    arcvm::IRValue value;
    arcvm::IRValue pointer;
};

class IRGen {
  public:
    IRGen();

    arcvm::Arcvm gen_project(Project*);
    void gen_file(File*, arcvm::Module*);
  private:
    std::vector<std::unordered_map<std::string, ir_var>> variable_table_;

    void gen_import(Import*);
    void gen_function(Function*, arcvm::Block*);
    void gen_function_args(std::vector<Arg>, arcvm::BasicBlock*);
    void gen_block(Block*, arcvm::Block*);
    void gen_statement(Statement*, arcvm::Block*);
    void gen_while(WhileStmnt*, arcvm::Block*);
    void gen_for(ForStmnt*, arcvm::Block*);
    void gen_if(IfStmnt*, arcvm::Block*);
    void gen_ret(RetStmnt*, arcvm::BasicBlock*);
    void gen_decl(Decl*, arcvm::BasicBlock*);
    arcvm::IRValue gen_expr(Expr*, arcvm::BasicBlock*);
    arcvm::IRValue gen_lrvalue_expr(Expr*, arcvm::BasicBlock*);
    arcvm::IRValue gen_rrvalue_expr(Expr*, arcvm::BasicBlock*);
    bool is_lrvalue_expr(TokenKind);

    arcvm::IRValue gen_immediate(u64, arcvm::BasicBlock*);
    arcvm::IRValue gen_immediate(f64, arcvm::BasicBlock*);
    arcvm::IRValue gen_immediate(std::string*, arcvm::BasicBlock*);

    arcvm::IRValue gen_var(std::string*, arcvm::BasicBlock*);
    arcvm::IRValue gen_var_load(std::string*, arcvm::Type, arcvm::BasicBlock*);
    arcvm::IRValue gen_fn_call(Expr*, arcvm::BasicBlock*);
    arcvm::IRValue gen_bin(Expr*, arcvm::BasicBlock*);
    arcvm::IRValue gen_unary(Expr*, arcvm::BasicBlock*);
};

#endif