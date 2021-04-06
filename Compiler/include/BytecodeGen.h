#include "AST.h"

// include this to get the instructions
#include "Arcvm.h" 

class BytecodeGen {
    public:
        BytecodeGen(Project*);
        ~BytecodeGen();

        std::vector<u8, arena_allocator<u8>> gen_code();
    
    private:
        Project* ast_;
        std::vector<u8, arena_allocator<u8>> code_;
            
        void gen_project(Project *project);
        void gen_file(const File*);
        void gen_import(const Import*);
        void gen_function(const Function*);
        void gen_block(const Block*);
        void gen_statement(const Statement*);
        void gen_while(const While_*);
        void gen_for(const For_*);
        void gen_if(const If_*);
        void gen_ret(const Ret*);
        void gen_decl(const Decl*);
        void gen_expr(const Expr*);

        void gen_int_lit(const u64 val);
        void gen_float_lit(const f64 val);
        void gen_string_lit(const astring* val);
        void gen_id(const astring* id);
        void gen_bin(const Expr *expr);
        void gen_unary(const Expr* expr);

        void push(const u8);
        void push(const std::vector<u8, arena_allocator<u8>>&);

        void push_64bit_value(const u64);

    
        void generate_bootstrap();


};