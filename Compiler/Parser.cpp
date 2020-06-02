#include "Parser.h"

Parser::Parser(std::vector<Token>* token_param, std::unordered_map<std::string, Type> symboltable_param):
    tokens(*token_param), symbol_table(symboltable_param), currentTokenPointer(0)
{

}

Expr* Parser::parseExpr_10(){
    Expr* expr = parseExpr_9();
}

Expr* Parser::parseExpr_9(){
    Expr* expr = parseExpr_9();
}

Expr* Parser::parseExpr_8(){
    Expr* expr = parseExpr_9();
}

Expr* Parser::parseExpr_7(){
    Expr* expr = parseExpr_9();
}

Expr* Parser::parseExpr_6(){
    Expr* expr = parseExpr_9();
}

Expr* Parser::parseExpr_5(){
    Expr* expr = parseExpr_9();
}

Expr* Parser::parseExpr_4(){
    Expr* expr = parseExpr_9();
}

Expr* Parser::parseExpr_3(){
    Expr* expr = parseExpr_9();
}

Expr* Parser::parseExpr_2(){
    Expr* expr = parseExpr_9();
}

Expr* Parser::parseExpr_1(){
    Expr* expr = parseExpr_9();
}

Expr* Parser::parseExpr_0(){
    switch(currentToken()->type){
        case NUMBER:
            break;
        case FLOAT:
            break;
        case STRING:
            break;
        case NAME:
            break;
    }
}

void Parser::nextToken(){
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