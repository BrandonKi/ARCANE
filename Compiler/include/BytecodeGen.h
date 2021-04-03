#include "AST.h"

// include this to get the instructions
#include "Arcvm.h" 

class BytecodeGen {
    public:
        BytecodeGen(Project*);
        ~BytecodeGen();

        std::vector<u8, arena_allocator<u8>> genCode();
    
    private:
        Project* ast_;
        std::vector<u8, arena_allocator<u8>> code_;
            
        void genProject(Project *project);
        void genFile(const File*);
        void genImport(const Import*);
        void genFunction(const Function*);
        void genBlock(const Block*);
        void genStatement(const Statement*);
        void genWhile(const While_*);
        void genFor(const For_*);
        void genIf(const If_*);
        void genRet(const Ret*);
        void genDecl(const Decl*);
        void genExpr(const Expr*);

        void genIntLit(const u64 val);
        void genFloatLit(const f64 val);
        void genStringLit(const astring* val);
        void genID(const astring* id);
        void genBin(const Expr *expr);
        void genUnary(const Expr* expr);

        void push(const u8);
        void push(const std::vector<u8, arena_allocator<u8>>&);

        void push64BitValue(const u64);

    
        void generateBootstrap();


};