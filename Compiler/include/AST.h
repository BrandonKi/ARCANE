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

enum Operator {
    OP_UNARY_SUB,
    OP_UNARY_NOT,
    OP_UNARY_PRE_INCREMENT,
    OP_UNARY_PRE_DECREMENT,
    OP_UNARY_POST_INCREMENT,
    OP_UNARY_POST_DECREMENT,

    OP_ADD,
    OP_SUB,
    OP_DIV,
    OP_MUL,
    OP_MOD,
    OP_BIN_OR,
    OP_BIN_AND,
    OP_LOGICAL_OR,
    OP_LOGICAL_AND,
    OP_LEFT_SHIFT,
    OP_RIGHT_SHIFT,
    OP_XOR,
    OP_LESSER,
    OP_GREATER,
    OP_NEGATE,
};

enum Type {
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
    std::vector<File*> files;
};

struct File : Node{
    std::vector<Import*> imports;
    std::vector<Decl*> decls;
    std::vector<Function*> functions;
    bool isMain;
};

struct Import : Node {
    std::string id;
    std::string filename;
    // std::vector<Decl*> decls;
    // std::vector<Function*> functions;
};

struct Function : Node {
    std::vector<Type> args;
    Block* body;
};

struct Block : Node {
    std::vector<Statement*> statements;
};

enum StatementType {
    EXPRESSION,
    DECLARATION
};

struct Statement : Node {
    StatementType type;
    union {
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
            u64 val;
        } intLiteral;
        struct {
            f64 val;
        } floatLiteral;
        struct {
            char* val;   //FIXME I don't like this too much
        } stringLiteral;
        struct {
            char* val;
        } id;
        struct {
            Operator op;
            Expr* left;
            Expr* right;
        } binaryExpr;
        struct {
            Operator op;
            Expr* expr;
        } unaryExpr;

    };
};

struct Decl : Node {
    char* id;
    Type type;
    Expr* val;
};

class AST {

    private:
        ArenaAlloc allocator;
    
    public:
        AST();
        ~AST();

        Project* newProjectNode(SourcePos, std::vector<File*>&);
        File* newFileNode(SourcePos, std::vector<Import*>&, std::vector<Decl*>&, std::vector<Function*>&, bool);
        Import* newImportNode(SourcePos, std::string&, std::string&);    // TODO add a way to keep track of imported symbols
        Function* newFunctionNode(SourcePos, std::vector<Type>&, Block*);
        Block* newBlockNode(SourcePos, std::vector<Statement*>&);
        Statement* newStatementNode_decl(SourcePos, Decl*);
        Statement* newStatementNode_expr(SourcePos, Expr*);
        Expr* newExprNode_intLiteral(SourcePos, u64);
        Expr* newExprNode_floatLiteral(SourcePos, f64);
        Expr* newExprNode_stringLiteral(SourcePos, std::string&);
        Expr* newExprNode_variable(SourcePos, std::string&);
        Expr* newExprNode_binExpr(SourcePos, Operator, Expr*, Expr*);
        Expr* newExprNode_unaryExpr(SourcePos, Operator, Expr*);
        Decl* newDeclNode(SourcePos, std::string&, Type, Expr*);


};
