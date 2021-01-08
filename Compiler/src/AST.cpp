#include "AST.h"

AST::AST():
    allocator(5000)
{

}

AST::~AST(){

}

Project* AST::newProjectNode(SourcePos pos, std::vector<File*>& files){
    // Project* ptr = allocator.alloc<Project>();
    // *ptr = Project{pos, files};
    Project* ptr = new Project{{pos}, files};  // FIXME temporary solution
    return ptr;
}

File* AST::newFileNode(SourcePos pos, std::vector<Import*>& imports, std::vector<Decl*>& decls, std::vector<Function*>& functions, bool isMain){
    // File* ptr = allocator.alloc<File>();
    // *ptr = File{pos, imports, decls, functions, isMain};
    File* ptr = new File{{pos}, std::move(imports), std::move(decls), std::move(functions), isMain};  // FIXME temporary solution
    return ptr;
}

Import* AST::newImportNode(SourcePos pos, std::string& id, std::string& filename){
    // Import* ptr = allocator.alloc<Import>();
    // *ptr = Import{pos, id, filename};
    Import* ptr = new Import{{pos}, id, filename};  // FIXME temporary solution
    return ptr;
}

Function* AST::newFunctionNode(SourcePos pos, std::vector<Type>& argTypes, Block* body){
    // Function* ptr = allocator.alloc<Function>();
    // *ptr = Function{pos, argTypes, body};
    Function* ptr = new Function{{pos}, argTypes, body};  // FIXME temporary solution
    return ptr;
} 

Block* AST::newBlockNode(SourcePos pos, std::vector<Statement*>& statements){
    // Block* ptr = allocator.alloc<Block>();
    // *ptr = Block{pos, statements};
    Block* ptr = ptr = new Block{{pos}, statements};  // FIXME temporary solution
    return ptr;
}

Statement* AST::newStatementNode_decl(SourcePos pos, Decl* decl){
    Statement* ptr = allocator.alloc<Statement>();
    *ptr = Statement{ {pos}, DECLARATION};
    ptr->decl = decl;
    return ptr;
}

Statement* AST::newStatementNode_expr(SourcePos pos, Expr* expr){
    Statement* ptr = allocator.alloc<Statement>();
    *ptr = Statement{ {pos}, EXPRESSION};
    ptr->expr = expr;
    return ptr;
}

Expr* AST::newExprNode_intLiteral(SourcePos pos, u64 intLiteral){
    Expr* ptr = allocator.alloc<Expr>();
    *ptr = Expr{{pos}, EXPR_INT_LIT};
    ptr->intLiteral.val = intLiteral;
    return ptr;
}

Expr* AST::newExprNode_floatLiteral(SourcePos pos, f64 floatLiteral){
    Expr* ptr = allocator.alloc<Expr>();
    *ptr = Expr{{pos}, EXPR_FLOAT_LIT};
    ptr->floatLiteral.val = floatLiteral;
    return ptr;
}

Expr* AST::newExprNode_stringLiteral(SourcePos pos, const char* stringLiteral){
    Expr* ptr = allocator.alloc<Expr>();
    *ptr = Expr{{pos}, EXPR_STRING_LIT};
    size_t dataSize = strlen(stringLiteral) + 1;
    char* data = allocator.alloc<char>(dataSize);
    memcpy_s(data, dataSize, stringLiteral, dataSize);
    ptr->stringLiteral.val = data;
    return ptr;
}

Expr* AST::newExprNode_variable(SourcePos pos, const char* id){
    Expr* ptr = allocator.alloc<Expr>();
    *ptr = Expr{{pos}, EXPR_ID};
    size_t dataSize = strlen(id) + 1;
    char* data = allocator.alloc<char>(dataSize);
    memcpy_s(data, dataSize, id, dataSize);
    ptr->id.val = data;
    return ptr;
}

Expr* AST::newExprNode_binExpr(SourcePos pos, TokenKind op, Expr* left, Expr* right){
    Expr* ptr = allocator.alloc<Expr>();
    *ptr = Expr{{pos}, EXPR_BIN};
    ptr->binaryExpr.op = op;
    ptr->binaryExpr.left = left;
    ptr->binaryExpr.right = right;
    return ptr;
}

Expr* AST::newExprNode_unaryExpr(SourcePos pos, TokenKind op, Expr* expr){
    Expr* ptr = allocator.alloc<Expr>();
    *ptr = Expr{{pos}, EXPR_UNARY};
    ptr->unaryExpr.op = op;
    ptr->unaryExpr.expr = expr;
    return ptr;
}

Decl* AST::newDeclNode(SourcePos pos, std::string& id, Type type, Expr* val){   
    Decl* ptr = allocator.alloc<Decl>();
    size_t dataSize = id.size() + 1;
    char* data = allocator.alloc<char>(dataSize);
    memcpy_s(data, dataSize, id.c_str(), dataSize);
    *ptr = Decl{{pos}, data, type, val};
    return ptr;
}