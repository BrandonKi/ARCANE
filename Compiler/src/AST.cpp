#include "AST.h"

#define ARENA_ALLOCATOR__SIZE 5000

AST::AST():
    allocator() // arena allocator size is defined above
{
    PROFILE();
}

AST::~AST() {
    PROFILE();

}
//FIXME fix all of this absolute trash
// at the moment we are making copies of the vectors and moving some and none of it is good

Project* AST::newProjectNode(SourcePos pos, std::vector<File*>& files) {
    PROFILE();
    Project* ptr = reinterpret_cast<Project*>(allocator.allocate(sizeof(Project)));
    allocator.construct(ptr, Project{{pos}, files});
    return ptr;
}

File* AST::newFileNode(SourcePos pos, std::vector<Import*>& imports, std::vector<Decl*>& decls, std::vector<Function*>& functions, bool isMain) {
    PROFILE();
    File* ptr = reinterpret_cast<File*>(allocator.allocate(sizeof(File)));
    allocator.construct(ptr, File{{pos}, std::move(imports), std::move(decls), std::move(functions), isMain});
    return ptr;
}

Import* AST::newImportNode(SourcePos pos, astring& id, astring& filename) {
    PROFILE();
    Import* ptr = reinterpret_cast<Import*>(allocator.allocate(sizeof(Import)));
    allocator.construct(ptr, Import{{pos}, id, filename});
    return ptr;
}

Function* AST::newFunctionNode(SourcePos pos, std::vector<Type>& argTypes, Block* body) {
    PROFILE();
    Function* ptr = reinterpret_cast<Function*>(allocator.allocate(sizeof(Function)));
    allocator.construct(ptr, Function{{pos}, argTypes, body});
    return ptr;
} 

Block* AST::newBlockNode(SourcePos pos, std::vector<Statement*>& statements) {
    PROFILE();
    Block* ptr = reinterpret_cast<Block*>(allocator.allocate(sizeof(Block)));
    allocator.construct(ptr, Block{{pos}, statements});
    return ptr;
}

Statement* AST::newStatementNode_decl(SourcePos pos, Decl* decl) {
    PROFILE();
    Statement* ptr = reinterpret_cast<Statement*>(allocator.allocate(sizeof(Statement)));
    allocator.construct(ptr, Statement{ {pos}, DECLARATION});
    ptr->decl = decl;
    return ptr;
}

Statement* AST::newStatementNode_expr(SourcePos pos, Expr* expr) {
    PROFILE();
    Statement* ptr = reinterpret_cast<Statement*>(allocator.allocate(sizeof(Statement)));
    allocator.construct(ptr, Statement { {pos}, EXPRESSION});
    ptr->expr = expr;
    return ptr;
}

Expr* AST::newExprNode_intLiteral(SourcePos pos, u64 intLiteral) {
    PROFILE();
    Expr* ptr = reinterpret_cast<Expr*>(allocator.allocate(sizeof(Expr)));
    allocator.construct(ptr, Expr{{pos}, EXPR_INT_LIT});
    ptr->intLiteral.val = intLiteral;
    return ptr;
}

Expr* AST::newExprNode_floatLiteral(SourcePos pos, f64 floatLiteral) {
    PROFILE();
    Expr* ptr = reinterpret_cast<Expr*>(allocator.allocate(sizeof(Expr)));
    allocator.construct(ptr, Expr{{pos}, EXPR_FLOAT_LIT});
    ptr->floatLiteral.val = floatLiteral;
    return ptr;
}

Expr* AST::newExprNode_stringLiteral(SourcePos pos, astring& stringLiteral) {
    PROFILE();
    Expr* ptr = reinterpret_cast<Expr*>(allocator.allocate(sizeof(Expr)));
    allocator.construct(ptr, Expr{{pos}, EXPR_STRING_LIT});
    ptr->stringLiteral.val = &stringLiteral;
    return ptr;
}

Expr* AST::newExprNode_variable(SourcePos pos, astring* id) {
    PROFILE();
    Expr* ptr = reinterpret_cast<Expr*>(allocator.allocate(sizeof(Expr)));
    allocator.construct(ptr, Expr{{pos}, EXPR_ID});
    ptr->id.val = id;
    return ptr;
}

Expr* AST::newExprNode_binExpr(SourcePos pos, TokenKind op, Expr* left, Expr* right) {
    PROFILE();
    // Expr* ptr = allocator.alloc<Expr>();
    // *ptr = Expr{{pos}, EXPR_BIN};
    // Expr *ptr = new Expr{{pos}, EXPR_BIN};

    // Expr *ptr = reinterpret_cast<Expr*>(allocator.allocate(sizeof(Expr)));  //FIXME gcc is the only one that supports 
    // allocator.construct(ptr, SourcePos{pos}, EXPR_BIN);                     // contructing a struct that inherits from another struct in-place using contruct_at
                                                                               // so I'll wait till it's a more common feature
    Expr *ptr = new Expr{{pos}, EXPR_BIN};
    ptr->binaryExpr.op = op;
    ptr->binaryExpr.left = left;
    ptr->binaryExpr.right = right;
    return ptr;
}

Expr* AST::newExprNode_unaryExpr(SourcePos pos, TokenKind op, Expr* expr) {
    PROFILE();
    // Expr* ptr = allocator.alloc<Expr>();
    // *ptr = Expr{{pos}, EXPR_UNARY};
    // Expr *ptr = new Expr{{pos}, EXPR_UNARY};
    Expr *ptr = new Expr{{pos}, EXPR_UNARY};
    ptr->unaryExpr.op = op;
    ptr->unaryExpr.expr = expr;
    return ptr;
}

Decl* AST::newDeclNode(SourcePos pos, astring& id, Type type, Expr* val) {
    PROFILE(); 
    // Decl* ptr = allocator.alloc<Decl>();
    // size_t dataSize = id.size() + 1;
    // char* data = allocator.alloc<char>(dataSize);
    // memcpy_s(data, dataSize, id.c_str(), dataSize);
    // *ptr = Decl{{pos}, data, type, val};
    Decl *ptr = new Decl{{pos}, &id, type, val};
    return ptr;
}