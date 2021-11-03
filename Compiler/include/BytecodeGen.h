/**
 * provides a thin layer of abstraction
 * over the IRGenerator in ARCVM
 */

#ifndef ARCANE_BYTECODEGEN_H
#define ARCANE_BYTECODEGEN_H

#include "TypeInference.h"

class BytecodeGen {
  public:
    BytecodeGen();

    arcvm::Arcvm gen_project(Project*);
    void gen_file(File*, arcvm::Module*);
  private:
    std::vector<std::unordered_map<std::string, arcvm::Value>> variable_table_;

    void gen_import(Import*);
    void gen_function(Function*, arcvm::Block*);
    void gen_block(Block*, arcvm::Block*);
    void gen_statement(Statement*, arcvm::Block*);
    void gen_while(WhileStmnt*, arcvm::Block*);
    void gen_for(ForStmnt*, arcvm::Block*);
    void gen_if(IfStmnt*, arcvm::Block*);
    void gen_ret(RetStmnt*, arcvm::BasicBlock*);
    void gen_decl(Decl*, arcvm::BasicBlock*);
    arcvm::Value gen_expr(Expr*, arcvm::BasicBlock*);
    arcvm::Value gen_lrvalue_expr(Expr*, arcvm::BasicBlock*);
    arcvm::Value gen_rrvalue_expr(Expr*, arcvm::BasicBlock*);
    bool is_lrvalue_expr(TokenKind);

    arcvm::Value gen_immediate(i64, arcvm::BasicBlock*);
    arcvm::Value gen_immediate(f64, arcvm::BasicBlock*);
    arcvm::Value gen_immediate(std::string*, arcvm::BasicBlock*);

    arcvm::Value gen_var(std::string*, arcvm::BasicBlock*);
    arcvm::Value gen_fn_call(Expr*, arcvm::BasicBlock*);
    arcvm::Value gen_bin(Expr*, arcvm::BasicBlock*);
    arcvm::Value gen_unary(Expr*, arcvm::BasicBlock*);
};

#endif