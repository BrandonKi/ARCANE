#include "AST.h"


AST::AST(){

}

AST::~AST(){

}

Decl AST::newDeclNode(){
    return Decl{};
}

Expr AST::newExprNode(){
    return Expr{};
}