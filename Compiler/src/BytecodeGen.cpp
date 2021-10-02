#include "BytecodeGen.h"

arcvm::Arcvm BytecodeGen::gen_project(Project* project) {
    for(auto* file: project->files) {
        gen_file(file);
    }
    return arcvm::Arcvm{};
}

void BytecodeGen::gen_file(File* file) {
    for(auto* import: file->imports){
        // TODO
    }

    for(auto* decl: file->decls) {
        // TODO
    }

    for(auto* function: file->functions) {
        // gen_function();
    }

}

// TODO
void BytecodeGen::gen_import(Import* import) {
    static_cast<void>(import);
}

void BytecodeGen::gen_function(Function* function) {
    // TODO handle all the function arg/return type stuff
    // and generate with a function id
    for(auto* stmnt: function->body->statements) {
        gen_statement(stmnt);
    }
}

void BytecodeGen::gen_statement(Statement* statement) {
    switch(statement->type) {
        case WHILE:
            gen_while(statement->while_stmnt);
            break;
        case FOR:
            gen_for(statement->for_stmnt);
            break;
        case IF:
            gen_if(statement->if_stmnt);
            break;
        case RET:
            gen_ret(statement->ret_stmnt);
            break;
        case EXPRESSION:
            gen_expr(statement->expr);
            break;
        case DECLARATION:
            gen_decl(statement->decl);
            break;
    }
}

// TODO
void BytecodeGen::gen_while(WhileStmnt* while_stmnt) {
    static_cast<void>(while_stmnt);
}

// TODO
void BytecodeGen::gen_for(ForStmnt* for_stmnt) {
    static_cast<void>(for_stmnt);
}

// TODO
void BytecodeGen::gen_if(IfStmnt* if_stmnt) {
    static_cast<void>(if_stmnt);
}

// TODO
void BytecodeGen::gen_ret(RetStmnt* ret_stmnt) {
    static_cast<void>(ret_stmnt);
}

void BytecodeGen::gen_decl(Decl* decl) {

}

void BytecodeGen::gen_expr(Expr* expr) {

}

void BytecodeGen::gen_immediate(i64 immediate) {

}

void BytecodeGen::gen_immediate(f64 immediate) {

}

void BytecodeGen::gen_immediate(std::string immediate) {

}

void BytecodeGen::gen_id(std::string id) {

}

void BytecodeGen::gen_fn_call(Expr* expr) {

}

void BytecodeGen::gen_bin(Expr* expr) {

}

void BytecodeGen::gen_unary(Expr* expr) {

}