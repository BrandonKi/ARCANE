#include "AST.h"

AST::AST(){
    init();
    runTests();
}

void AST::init(){
    m_size = 5000;
    m_memory = malloc(m_size);
    m_memory_ptr = m_memory;
}

void AST::runTests(){
    print(newBinaryExpr(newIntegerExpr(892), newIntegerExpr(120), '/'));
    std::cout << "\n";
    print(newUnaryExpr(newIntegerExpr(120), '-'));
    std::cout << "\n";
    print(newBinaryExpr(newStringExpr("Brandon"), newStringExpr("Kirincich"), '+'));
    std::cout << "\n";
    print(newTernaryExpr(newBinaryExpr(newIntegerExpr(1), newIntegerExpr(5), '>'), newStringExpr("true"), newStringExpr("false")));
    std::cout << "\n";
}

Expr* AST::alloc_expr(){
    memset(m_memory_ptr, 0, sizeof(Expr));
    Expr* expr = (Expr*)m_memory_ptr;
    m_memory_ptr = (void*)((uintptr_t)m_memory_ptr + sizeof(Expr));
    return expr;
}

Expr* AST::newUnaryExpr(Expr* u_expr, char op){
    Expr* expr = alloc_expr();
    expr->type = E_UNARY;
    expr->e_unary.expr = u_expr;
    expr->e_unary.op = op;
    return expr;
}

Expr* AST::newBinaryExpr(Expr* left, Expr* right, char op){
    Expr* expr = alloc_expr();
    expr->type = E_BINARY;
    expr->e_binary.left = left;
    expr->e_binary.right = right;
    expr->e_binary.op = op;
    return expr;
}

Expr* AST::newTernaryExpr(Expr* eval, Expr* left, Expr* right){
    Expr* expr = alloc_expr();
    expr->type = E_TERNARY;
    expr->e_ternary.eval = eval;
    expr->e_ternary.left = left;
    expr->e_ternary.right = right;
    return expr;
}

Expr* AST::newIntegerExpr(uint val){
    Expr* expr = alloc_expr();
    expr->type = E_INTEGER;
    expr->e_integer.val = val;
    return expr;
}

Expr* AST::newStringExpr(std::string val){
    Expr* expr = alloc_expr();
    expr->type = E_STRING;
    expr->e_string.val = val;
    return expr;
}

void AST::print(Expr* expr){
    // Expr* expr = (Expr*)m_memory;
    // for(; expr < m_memory_ptr; expr += 1)
    switch(expr->type){
        case E_UNARY:
            std::cout << "{";
            print(expr->e_unary.expr);
            std::cout << ", ";
            std::cout << expr->e_unary.op;
            std::cout << "}";
            break;
        case E_BINARY:
            std::cout << "{";
            print(expr->e_binary.left);
            std::cout << ", ";
            print(expr->e_binary.right);
            std::cout << ", ";
            std::cout << expr->e_binary.op;
            std::cout << "}";
            break;
        case E_TERNARY:
            std::cout << "{";
            print(expr->e_ternary.eval);
            std::cout << " ? ";
            print(expr->e_ternary.left);
            std::cout << " : ";
            print(expr->e_ternary.right);
            std::cout << "}";
            break;
        case E_INTEGER:
            std::cout << "{" << expr->e_integer.val << "}";
            break;
        case E_FLOAT:
            std::cout << "{" << expr->e_float.val << "}";
        break;
        case E_STRING:
            std::cout << "{\"" << expr->e_string.val << "\"}";
            break;
        case E_INDEX:
        break;
        case E_FIELD:
        break;
    }
}
