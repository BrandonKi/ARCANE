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
    std::vector<File*, arena_allocator<File*>> files;
};

struct File : Node {
    astring name;
    std::vector<Import*, arena_allocator<Import*>> imports;
    std::vector<Decl*, arena_allocator<Decl*>> decls;
    std::vector<Function*, arena_allocator<Function*>> functions;
    bool is_main;
};

struct Import : Node {
    astring id;
    astring filename;
    // std::vector<Decl*> decls;
    // std::vector<Function*> functions;
};

struct Arg : Node {
    astring id;
    type_handle type;
};

struct Function : Node {
    astring id;
    std::vector<Arg, arena_allocator<Arg>> args;
    type_handle return_type;
    Block* body;
    bool is_main;
};

struct Block : Node {
    std::vector<Statement*, arena_allocator<Statement*>> statements;
};

struct While_ : Node {
    Expr* expr;
    Block* block;
};

struct For_ : Node {
    Decl* decl;
    Expr* expr1;
    Expr* expr2;
    Block* block;
};

struct If_ : Node {
    Expr* expr;
    Block* block;
    // TODO add a vector of If_* for else if statements
};

struct Ret : Node {
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
        While_* while_;
        For_* for_;
        If_* if_;
        Ret* ret;
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
            astring* val;
        } string_literal;
        struct {
            astring* val;
        } id;
        struct {
            astring* val;
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
    astring* id;
    type_handle type;
    Expr* val;
};

class AST {

    private:
        //TODO this would be a good use case for a polymorphic allocator
        // everything that needs to be allocated is of different types
        // for now we emulate a polymorphic allocator by manually allocating bytes for each type
        arena_allocator<u8> allocator_;

    
    public:
        AST();
        ~AST();

        [[nodiscard]] Project* new_project_node(SourcePos, std::vector<File*, arena_allocator<File*>>&);
        [[nodiscard]] File* new_file_node(SourcePos, astring, std::vector<Import*, arena_allocator<Import*>>&, std::vector<Decl*, arena_allocator<Decl*>>&, std::vector<Function*, arena_allocator<Function*>>&, const bool);
        [[nodiscard]] Import* new_import_node(SourcePos, astring&, astring&);    // TODO add a way to keep track of imported symbols
        [[nodiscard]] Function* new_function_node(SourcePos, astring&, std::vector<Arg, arena_allocator<Arg>>&, type_handle, Block*, const bool);
        [[nodiscard]] Block* new_block_node(SourcePos, std::vector<Statement*, arena_allocator<Statement*>>&);
        [[nodiscard]] While_* new_while_node(SourcePos, Expr*, Block*);
        [[nodiscard]] For_* new_for_node(SourcePos, Decl*, Expr*, Expr*, Block*);
        [[nodiscard]] If_* new_if_node(SourcePos, Expr*, Block*);
        [[nodiscard]] Ret* new_ret_node(SourcePos, Expr*);
        [[nodiscard]] Statement* new_statement_node_while(SourcePos, While_*);
        [[nodiscard]] Statement* new_statement_node_for(SourcePos, For_*);
        [[nodiscard]] Statement* new_statement_node_if(SourcePos, If_*);
        [[nodiscard]] Statement* new_statement_node_ret(SourcePos, Ret*);
        [[nodiscard]] Statement* new_statement_node_decl(SourcePos, Decl*);
        [[nodiscard]] Statement* new_statement_node_expr(SourcePos, Expr*);
        [[nodiscard]] Expr* new_expr_node_int_literal(SourcePos, i64, type_handle type = TYPE_UNKNOWN);
        [[nodiscard]] Expr* new_expr_node_float_literal(SourcePos, f64, type_handle type = TYPE_UNKNOWN);
        [[nodiscard]] Expr* new_expr_node_string_literal(SourcePos, astring&,  type_handle type = TYPE_UNKNOWN);
        [[nodiscard]] Expr* new_expr_node_variable(SourcePos, astring&, type_handle type = TYPE_UNKNOWN);
        [[nodiscard]] Expr* new_expr_node_bin_expr(SourcePos, TokenKind, Expr*, Expr*, type_handle type = TYPE_UNKNOWN);
        [[nodiscard]] Expr* new_expr_node_unary_expr(SourcePos, TokenKind, Expr*, type_handle type = TYPE_UNKNOWN);
        [[nodiscard]] Decl* new_decl_node(SourcePos, astring&, type_handle, Expr*);


};

#endif