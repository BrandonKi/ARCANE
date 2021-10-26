#ifndef AST_H
#define AST_H

#include "Lexer.h"

struct Node;
struct Project;
struct File;
struct Import;
struct Function;
struct Block;
struct Statement;
struct Expr;
struct Decl;

struct Node {
    SourcePos pos;
};

struct Project : Node {
    std::vector<File*> files;
};

struct File : Node {
    std::string name;
    std::vector<Import*> imports;
    std::vector<Decl*> decls;
    std::vector<Function*> functions;
    bool is_main;
};

struct Import : Node {
    std::string id;
    std::string filename;
    // std::vector<Decl*> decls;
    // std::vector<Function*> functions;
};

struct Arg : Node {
    std::string id;
    type_handle type;
};

struct Function : Node {
    std::string id;
    std::vector<Arg> args;
    type_handle return_type;
    Block* body;
    bool is_main;
};

struct Block : Node {
    std::vector<Statement*> statements;
};

struct WhileStmnt : Node {
    Expr* expr;
    Block* block;
};

struct ForStmnt : Node {
    Decl* decl;
    Expr* expr1;
    Expr* expr2;
    Block* block;
};

struct IfStmnt : Node {
    Expr* expr;
    Block* block;

    // TODO add a vector of If_* for else if statements
    std::vector<IfStmnt*> elif_stmnts;

    Block* else_stmnt;
};

struct RetStmnt : Node {
    Expr* expr;
};

enum StatementType {
    WHILE,
    FOR,
    IF,
    RET,
    EXPRESSION,
    DECLARATION
};

struct Statement : Node {
    StatementType type;
    union {
        WhileStmnt* while_stmnt;
        ForStmnt* for_stmnt;
        IfStmnt* if_stmnt;
        RetStmnt* ret_stmnt;
        Expr* expr;
        Decl* decl;
    };
};

enum ExprType {
    EXPR_INT_LIT,
    EXPR_FLOAT_LIT,
    EXPR_STRING_LIT,
    EXPR_ID,
    EXPR_FN_CALL,
    EXPR_BIN,
    EXPR_UNARY
};

struct Expr : Node {
    ExprType type;
    type_handle result_type;
    union {
        struct {
            i64 val;
        } int_literal;
        struct {
            f64 val;
        } float_literal;
        struct {
            std::string* val;
        } string_literal;
        struct {
            std::string* val;
        } id;
        struct {
            std::string* val;
            u32 argc;
            Expr** args;
        } fn_call;
        struct {
            TokenKind op;   // op is expected to be a valid operator
            Expr* left;
            Expr* right;
        } binary_expr;
        struct {
            TokenKind op;   // op is expected to be a valid operator
            Expr* expr;
        } unary_expr;

    };
};

struct Decl : Node {
    std::string* id;
    type_handle type;
    Expr* val;
};

class AST {

    public:
        AST();
        ~AST();

        [[nodiscard]] Project* new_project_node(SourcePos, std::vector<File*>&);
        [[nodiscard]] File* new_file_node(SourcePos, std::string, std::vector<Import*>&, std::vector<Decl*>&, std::vector<Function*>&, const bool);
        [[nodiscard]] Import* new_import_node(SourcePos, std::string&, std::string&);    // TODO add a way to keep track of imported symbols
        [[nodiscard]] Function* new_function_node(SourcePos, std::string&, std::vector<Arg>&, type_handle, Block*, const bool);
        [[nodiscard]] Block* new_block_node(SourcePos, std::vector<Statement*>&);
        [[nodiscard]] WhileStmnt* new_while_node(SourcePos, Expr*, Block*);
        [[nodiscard]] ForStmnt* new_for_node(SourcePos, Decl*, Expr*, Expr*, Block*);
        [[nodiscard]] IfStmnt* new_if_node(SourcePos, Expr*, Block*, std::vector<IfStmnt*> = {}, Block* = nullptr);
        [[nodiscard]] RetStmnt* new_ret_node(SourcePos, Expr*);
        [[nodiscard]] Statement* new_statement_node_while(SourcePos, WhileStmnt*);
        [[nodiscard]] Statement* new_statement_node_for(SourcePos, ForStmnt*);
        [[nodiscard]] Statement* new_statement_node_if(SourcePos, IfStmnt*);
        [[nodiscard]] Statement* new_statement_node_ret(SourcePos, RetStmnt*);
        [[nodiscard]] Statement* new_statement_node_decl(SourcePos, Decl*);
        [[nodiscard]] Statement* new_statement_node_expr(SourcePos, Expr*);
        [[nodiscard]] Expr* new_expr_node_int_literal(SourcePos, i64, type_handle type = TYPE_UNKNOWN);
        [[nodiscard]] Expr* new_expr_node_float_literal(SourcePos, f64, type_handle type = TYPE_UNKNOWN);
        [[nodiscard]] Expr* new_expr_node_string_literal(SourcePos, std::string&,  type_handle type = TYPE_UNKNOWN);
        [[nodiscard]] Expr* new_expr_node_variable(SourcePos, std::string&, type_handle type = TYPE_UNKNOWN);
        [[nodiscard]] Expr* new_expr_node_fn_call(SourcePos, std::string&, u32, Expr**, type_handle type = TYPE_UNKNOWN);
        [[nodiscard]] Expr* new_expr_node_bin_expr(SourcePos, TokenKind, Expr*, Expr*, type_handle type = TYPE_UNKNOWN);
        [[nodiscard]] Expr* new_expr_node_unary_expr(SourcePos, TokenKind, Expr*, type_handle type = TYPE_UNKNOWN);
        [[nodiscard]] Decl* new_decl_node(SourcePos, std::string&, type_handle, Expr*);


};

#endif