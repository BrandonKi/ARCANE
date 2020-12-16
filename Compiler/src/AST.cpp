#include "AST.h"


AST::AST(){

}

AST::~AST(){

}

Project* newProjectNode(SourcePos pos, std::vector<File*>&){
    return nullptr;
}

File* newFileNode(SourcePos pos, std::vector<Import*>&, std::vector<Decl*>&, std::vector<Function*>&){
    return nullptr;
}

Import* newImportNode(SourcePos pos, std::string&, std::string&){
    return nullptr;
}

Function* newFunctionNode(SourcePos pos, std::vector<Type*>&, Block&){
    return nullptr;
}

Block* newBlockNode(SourcePos pos, std::vector<Statement*>&){
    return nullptr;
}

Statement* newStatementNode_decl(SourcePos pos, Decl*){
    return nullptr;
}

Statement* newStatementNode_expr(SourcePos pos, Expr*){
    return nullptr;
}

Expr* newExprNode_intLiteral(SourcePos pos, u64){
    return nullptr;
}

Expr* newExprNode_floatLiteral(SourcePos pos, f64){
    return nullptr;
}

Expr* newExprNode_stringLiteral(SourcePos pos, std::string&){
    return nullptr;
}

Expr* newExprNode_binExpr(SourcePos pos, Operator, Expr*, Expr*){
    return nullptr;
}

Expr* newExprNode_unaryExpr(SourcePos pos, Operator, Expr*){
    return nullptr;
}

Decl* newDeclNode(SourcePos pos, std::string&, Type, Expr*){
    return nullptr;
}