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

enum Type {
    TYPE_INFER,
    TYPE_I8,
    TYPE_I16,
    TYPE_I32,
    TYPE_I64,
    TYPE_U8,
    TYPE_U16,
    TYPE_U32,
    TYPE_U64,
    TYPE_F32,
    TYPE_F64,
    TYPE_STRUCT,
    TYPE_STRING,
    TYPE_ARR,
};

struct Node {
    SourcePos pos;
};

struct Project : Node {
    std::vector<File*, arena_allocator<File*>> files;
};

struct File : Node{
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

struct Function : Node {
    //FIXME uuuhhh I don't have an ID here for some reason
    std::vector<Type, arena_allocator<Type>> args;
    Type return_type;
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
    EXPR_BIN,
    EXPR_UNARY
};

struct Expr : Node {
    ExprType type;
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
    Type type;
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

        Project* new_project_node(const SourcePos, std::vector<File*, arena_allocator<File*>>&);
        File* new_file_node(const SourcePos, std::vector<Import*, arena_allocator<Import*>>&, std::vector<Decl*, arena_allocator<Decl*>>&, std::vector<Function*, arena_allocator<Function*>>&, const bool);
        Import* new_import_node(const SourcePos, astring&, astring&);    // TODO add a way to keep track of imported symbols
        Function* new_function_node(const SourcePos, std::vector<Type, arena_allocator<Type>>&, Type, Block*, bool);
        Block* new_block_node(const SourcePos, std::vector<Statement*, arena_allocator<Statement*>>&);
        While_* new_while_node(const SourcePos, Expr*, Block*);
        For_* new_for_node(const SourcePos, Decl*, Expr*, Expr*, Block*);
        If_* new_if_node(const SourcePos, Expr*, Block*);
        Ret* new_ret_node(const SourcePos, Expr*);
        Statement* new_statement_node_while(const SourcePos, While_*);
        Statement* new_statement_node_for(const SourcePos, For_*);
        Statement* new_statement_node_if(const SourcePos, If_*);
        Statement* new_statement_node_ret(const SourcePos, Ret*);
        Statement* new_statement_node_decl(const SourcePos, Decl*);
        Statement* new_statement_node_expr(const SourcePos, Expr*);
        Expr* new_expr_node_int_literal(const SourcePos, const u64);
        Expr* new_expr_node_float_literal(const SourcePos, const f64);
        Expr* new_expr_node_string_literal(const SourcePos, astring&);
        Expr* new_expr_node_variable(const SourcePos, astring&);
        Expr* new_expr_node_bin_expr(const SourcePos, const TokenKind, Expr*, Expr*);
        Expr* new_expr_node_unary_expr(const SourcePos, const TokenKind, Expr*);
        Decl* new_decl_node(const SourcePos, astring&, Type, Expr*);


};

#endif