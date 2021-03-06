#include "AST.h"

extern ErrorHandler error_log;
extern TypeManager type_manager;

#define ARENA_ALLOCATOR__SIZE 5000

AST::AST():
    allocator_() // arena allocator size is defined above
{
    PROFILE();
}

AST::~AST() {
    PROFILE();

}
//FIXME fix all of this absolute trash
// at the moment we are making copies of the vectors and moving some and none of it is good

[[nodiscard]] Project* AST::new_project_node(const SourcePos pos, std::vector<File*, arena_allocator<File*>>& files) {
    PROFILE();
    auto *ptr = reinterpret_cast<Project*>(allocator_.allocate(sizeof(Project)));
    allocator_.construct(ptr, Project{{pos}, files});
    return ptr;
}

[[nodiscard]] File* AST::new_file_node(
        const SourcePos pos,
        const std::string name,
        std::vector<Import*, arena_allocator<Import*>>& imports,
        std::vector<Decl*, arena_allocator<Decl*>>& decls,
        std::vector<Function*, arena_allocator<Function*>>& functions,
        const bool is_main
    ) {
    PROFILE();
    auto *ptr = reinterpret_cast<File*>(allocator_.allocate(sizeof(File)));
    allocator_.construct(ptr, File{{pos}, name, std::move(imports), std::move(decls), std::move(functions), is_main});
    return ptr;
}

[[nodiscard]] Import* AST::new_import_node(const SourcePos pos, std::string& id, std::string& filename) {
    PROFILE();
    auto *ptr = reinterpret_cast<Import*>(allocator_.allocate(sizeof(Import)));
    allocator_.construct(ptr, Import{{pos}, id, filename});
    return ptr;
}

[[nodiscard]] Function* AST::new_function_node(const SourcePos pos, std::string& id, std::vector<Arg, arena_allocator<Arg>>& fn_args, type_handle type, Block* body, const bool is_main) {
    PROFILE();
    auto *ptr = reinterpret_cast<Function*>(allocator_.allocate(sizeof(Function)));
    allocator_.construct(ptr, Function{{pos}, id, fn_args, type, body, is_main });
    return ptr;
}

[[nodiscard]] Block* AST::new_block_node(const SourcePos pos, std::vector<Statement*, arena_allocator<Statement*>>& statements) {
    PROFILE();
    auto *ptr = reinterpret_cast<Block*>(allocator_.allocate(sizeof(Block)));
    allocator_.construct(ptr, Block{{pos}, statements});
    return ptr;
}

[[nodiscard]] While_* AST::new_while_node(const SourcePos pos, Expr* expr, Block* block) {
    PROFILE();
    auto *ptr = reinterpret_cast<While_*>(allocator_.allocate(sizeof(While_)));
    allocator_.construct(ptr, While_{{pos}, expr, block});
    return ptr;
}

[[nodiscard]] For_* AST::new_for_node(const SourcePos pos, Decl* decl, Expr* expr1, Expr* expr2, Block* block) {
    PROFILE();
    auto *ptr = reinterpret_cast<For_*>(allocator_.allocate(sizeof(For_)));
    allocator_.construct(ptr, For_{{pos}, decl, expr1, expr2, block});
    return ptr;
}

[[nodiscard]] If_* AST::new_if_node(const SourcePos pos, Expr* expr, Block* block) {
    PROFILE();
    auto *ptr = reinterpret_cast<If_*>(allocator_.allocate(sizeof(If_)));
    allocator_.construct(ptr, If_{{pos}, expr, block});
    return ptr;
}


[[nodiscard]] Ret* AST::new_ret_node(const SourcePos pos, Expr* expr) {
    PROFILE();
    auto *ptr = reinterpret_cast<Ret*>(allocator_.allocate(sizeof(Ret)));
    allocator_.construct(ptr, Ret{{pos}, expr});
    return ptr;
}

[[nodiscard]] Statement* AST::new_statement_node_while(const SourcePos pos, While_* while_) {
    PROFILE();
    auto *ptr = reinterpret_cast<Statement*>(allocator_.allocate(sizeof(Statement)));
    allocator_.construct(ptr, Statement{ {pos}, WHILE});
    ptr->while_ = while_;
    return ptr;
}

[[nodiscard]] Statement* AST::new_statement_node_for(const SourcePos pos, For_* for_) {
    PROFILE();
    auto *ptr = reinterpret_cast<Statement*>(allocator_.allocate(sizeof(Statement)));
    allocator_.construct(ptr, Statement{ {pos}, FOR});
    ptr->for_ = for_;
    return ptr;
}

[[nodiscard]] Statement* AST::new_statement_node_if(const SourcePos pos, If_* if_) {
    PROFILE();
    auto *ptr = reinterpret_cast<Statement*>(allocator_.allocate(sizeof(Statement)));
    allocator_.construct(ptr, Statement{ {pos}, IF});
    ptr->if_ = if_;
    return ptr;
}

[[nodiscard]] Statement* AST::new_statement_node_ret(const SourcePos pos, Ret* ret) {
    PROFILE();
    auto *ptr = reinterpret_cast<Statement*>(allocator_.allocate(sizeof(Statement)));
    allocator_.construct(ptr, Statement{ {pos}, RET});
    ptr->ret = ret;
    return ptr;
}

Statement* AST::new_statement_node_decl(const SourcePos pos, Decl* decl) {
    PROFILE();
    auto *ptr = reinterpret_cast<Statement*>(allocator_.allocate(sizeof(Statement)));
    allocator_.construct(ptr, Statement{ {pos}, DECLARATION});
    ptr->decl = decl;
    return ptr;
}

[[nodiscard]] Statement* AST::new_statement_node_expr(const SourcePos pos, Expr* expr) {
    PROFILE();
    auto *ptr = reinterpret_cast<Statement*>(allocator_.allocate(sizeof(Statement)));
    allocator_.construct(ptr, Statement { {pos}, EXPRESSION});
    ptr->expr = expr;
    return ptr;
}

[[nodiscard]] Expr* AST::new_expr_node_int_literal(const SourcePos pos, const i64 int_literal, type_handle type) {
    PROFILE();
    auto *ptr = reinterpret_cast<Expr*>(allocator_.allocate(sizeof(Expr)));
    allocator_.construct(ptr, Expr{{pos}, EXPR_INT_LIT, type});
    ptr->int_literal.val = int_literal;
    return ptr;
}

[[nodiscard]] Expr* AST::new_expr_node_float_literal(const SourcePos pos, const f64 float_literal, type_handle type) {
    PROFILE();
    auto *ptr = reinterpret_cast<Expr*>(allocator_.allocate(sizeof(Expr)));
    allocator_.construct(ptr, Expr{{pos}, EXPR_FLOAT_LIT, type});
    ptr->float_literal.val = float_literal;
    return ptr;
}

[[nodiscard]] Expr* AST::new_expr_node_string_literal(const SourcePos pos, std::string& string_literal, type_handle type) {
    PROFILE();
    auto *ptr = reinterpret_cast<Expr*>(allocator_.allocate(sizeof(Expr)));
    allocator_.construct(ptr, Expr{{pos}, EXPR_STRING_LIT, type});
    ptr->string_literal.val = new std::string(string_literal);
    return ptr;
}

[[nodiscard]] Expr* AST::new_expr_node_variable(const SourcePos pos, std::string& id, type_handle type) {
    PROFILE();
    auto *ptr = reinterpret_cast<Expr*>(allocator_.allocate(sizeof(Expr)));
    allocator_.construct(ptr, Expr{{pos}, EXPR_ID, type});
    // FIXME
    ptr->id.val = new std::string(id);
    return ptr;
}

[[nodiscard]] Expr* AST::new_expr_node_fn_call(const SourcePos pos, std::string& id, u32 argc, Expr** argv, type_handle type) {
    PROFILE();
    auto *ptr = reinterpret_cast<Expr*>(allocator_.allocate(sizeof(Expr)));
    allocator_.construct(ptr, Expr{{pos}, EXPR_FN_CALL, type});
    // FIXME
    ptr->fn_call.val = new std::string(id);
    ptr->fn_call.argc = argc;
    ptr->fn_call.args = argv;
    return ptr;
}


[[nodiscard]] Expr* AST::new_expr_node_bin_expr(const SourcePos pos, const TokenKind op, Expr* left, Expr* right, type_handle type) {
    PROFILE();
    auto *ptr = reinterpret_cast<Expr*>(allocator_.allocate(sizeof(Expr)));
    allocator_.construct(ptr, Expr{ {pos}, EXPR_BIN, type});
    ptr->binary_expr.op = op;
    ptr->binary_expr.left = left;
    ptr->binary_expr.right = right;
    return ptr;
}

[[nodiscard]] Expr* AST::new_expr_node_unary_expr(const SourcePos pos, const TokenKind op, Expr* expr, type_handle type) {
    PROFILE();
    auto* ptr = reinterpret_cast<Expr*>(allocator_.allocate(sizeof(Expr)));
    allocator_.construct(ptr, Expr{ {pos}, EXPR_UNARY, type});
    ptr->unary_expr.op = op;
    ptr->unary_expr.expr = expr;
    return ptr;
}

[[nodiscard]] Decl* AST::new_decl_node(const SourcePos pos, std::string& id, const type_handle type, Expr* val) {
    PROFILE(); 
    auto* ptr = reinterpret_cast<Decl*>(allocator_.allocate(sizeof(Decl)));
    // FIXME
    allocator_.construct(ptr, Decl{ {pos}, new std::string(id), type, val });
    return ptr;
}