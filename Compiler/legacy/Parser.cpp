#include "Parser.h"

Parser::Parser(std::vector<Token> token_param):
    tokens(token_param), currentTokenPointer(0)
{
    ast.print(parseExpr_10());
    std::cout << "\n";
    nextToken();
    ast.print(parseExpr_10());
    std::cout << "\n";
    nextToken();
    ast.print(parseExpr_10());
    std::cout << "\n";
    nextToken();
    ast.print(parseExpr_10());
    std::cout << "\n";
    nextToken();
}

Expr* Parser::parseExpr_10(){
    Expr* expr = parseExpr_9();
    return expr;
}

Expr* Parser::parseExpr_9(){
    Expr* expr = parseExpr_8();
    return expr;
}

Expr* Parser::parseExpr_8(){
    Expr* expr = parseExpr_7();
    return expr;
}

Expr* Parser::parseExpr_7(){
    Expr* expr = parseExpr_6();
    return expr;
}

Expr* Parser::parseExpr_6(){
    Expr* expr = parseExpr_5();
    return expr;
}

Expr* Parser::parseExpr_5(){
    Expr* expr = parseExpr_4();
    return expr;
}

Expr* Parser::parseExpr_4(){
    Expr* expr = parseExpr_3();
    return expr;
}

Expr* Parser::parseExpr_3(){
    Expr* expr = parseExpr_2();
    return expr;
}

Expr* Parser::parseExpr_2(){
    Expr* expr = parseExpr_1();
    return expr;
}

Expr* Parser::parseExpr_1(){
    Expr* expr = parseExpr_0();
    return expr;
}

Expr* Parser::parseExpr_0(){
    switch(currentToken()->type){
        case NUMBER:
            return ast.newIntegerExpr(std::stoi(currentToken()->data));
            break;
        case FLOAT:
            return ast.newFloatExpr(std::stod(currentToken()->data));
            break;
        case STRING:
            return ast.newStringExpr(currentToken()->data);
            break;
        case NAME:
            return ast.newNameExpr(currentToken()->data);
            break;
        case LPAREN:
            Expr *expr = parseExpr_10();
            nextToken();
            expectToken(RPAREN);                       // need better errors later
            return expr;
    }
    std::cout << "Unexpected token";
    exit(1);
}

void Parser::parseDecl(){
    if(currentToken()->data == "int"){
        
    }
}
// Expr *expr = parse_expr();
// expect_token(TOKEN_RPAREN);
// return new_expr_paren(pos, expr);

inline Token* Parser::nextToken(){
    currentTokenPointer++;
    return currentToken();
}

void Parser::expectToken(TokenType t){
    if(currentToken()->type != t){
        std::cout << "Unexpected token";
        exit(1);
    }
    currentTokenPointer++;
}

inline Token* Parser::currentToken(){
    return &tokens[currentTokenPointer];
}



// Expr *parse_expr_ternary(void) {
//     Expr *expr = parse_expr_or();
//     if (match_token(TOKEN_QUESTION)) {
//         Expr *then_expr = parse_expr_ternary();
//         expect_token(TOKEN_COLON);
//         Expr *else_expr = parse_expr_ternary();
//         expr = new_expr_ternary(pos, expr, then_expr, else_expr);
//     }
//     return expr;
// }