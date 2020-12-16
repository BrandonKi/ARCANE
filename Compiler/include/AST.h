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



enum ExprType {
    EXPR_INT_LIT,
    EXPR_FLOAT_LIT,
    EXPR_STRING_LIT,

};

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
    std::vector<Decl*> globals;
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
            std::string* val;   //TODO I don't like this too much
        } stringLiteral;
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
    std::string id;
    Type type;
    Expr* val;
};

class AST {

    private:
    
    public:
        AST();
        ~AST();

        Project* newProjectNode(SourcePos pos, std::vector<File*>&);
        File* newFileNode(SourcePos pos, std::vector<Import*>&, std::vector<Decl*>&, std::vector<Function*>&);
        Import* newImportNode(SourcePos pos, std::string&, std::string&);    // TODO add a way to keep track of imported symbols
        Function* newFunctionNode(SourcePos pos, std::vector<Type*>&, Block&);
        Block* newBlockNode(SourcePos pos, std::vector<Statement*>&);
        Statement* newStatementNode_decl(SourcePos pos, Decl*);
        Statement* newStatementNode_expr(SourcePos pos, Expr*);
        Expr* newExprNode_intLiteral(SourcePos pos, u64);
        Expr* newExprNode_floatLiteral(SourcePos pos, f64);
        Expr* newExprNode_stringLiteral(SourcePos pos, std::string&);
        Expr* newExprNode_binExpr(SourcePos pos, Operator, Expr*, Expr*);
        Expr* newExprNode_unaryExpr(SourcePos pos, Operator, Expr*);
        Decl* newDeclNode(SourcePos pos, std::string&, Type, Expr*);


};
