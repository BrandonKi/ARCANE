#include "BytecodeGen.h"

BytecodeGen::BytecodeGen() {}

arcvm::Arcvm BytecodeGen::gen_project(Project* project) {
    arcvm::Arcvm vm;
    for(auto* file: project->files) {
        arcvm::IRGenerator ir_gen;
        auto* mod = ir_gen.create_module();
        gen_file(file, mod);
        vm.load_module(mod);
    }
    return vm;
}

void BytecodeGen::gen_file(File* file, arcvm::Module* module) {
    for(auto* import: file->imports){
        // TODO
    }

    for(auto* decl: file->decls) {
        // TODO
    }

    for(auto* function: file->functions) {
        auto name = function->id;
        // TODO convert arg types to std::vector<arcvm::Type>
        auto args = std::vector<arcvm::Type>{};
        // TODO convert type_handle to arcvm::Type
        // auto ret_type = function->return_type;
        auto ret_type = arcvm::Type::ir_i32;
        auto ir_function = module->gen_function_def(function->id, args, ret_type);
        if(function->is_main)
            ir_function->add_attribute(arcvm::Attribute::entrypoint);
        gen_function(function, ir_function);
    }

}

// TODO
void BytecodeGen::gen_import(Import* import) {
    static_cast<void>(import);
}

void BytecodeGen::gen_function(Function* function, arcvm::Function* ir_gen) {
    for(auto* stmnt: function->body->statements) {
        gen_statement(stmnt, ir_gen);
    }
}

void BytecodeGen::gen_statement(Statement* statement, arcvm::Function* ir_gen) {
    switch(statement->type) {
        case WHILE:
            gen_while(statement->while_stmnt, ir_gen);
            break;
        case FOR:
            gen_for(statement->for_stmnt, ir_gen);
            break;
        case IF:
            gen_if(statement->if_stmnt, ir_gen);
            break;
        case RET:
            gen_ret(statement->ret_stmnt, ir_gen);
            break;
        case EXPRESSION:
            gen_expr(statement->expr, ir_gen);
            break;
        case DECLARATION:
            gen_decl(statement->decl, ir_gen);
            break;
    }
}

// TODO
void BytecodeGen::gen_while(WhileStmnt* while_stmnt, arcvm::Function* ir_gen) {
    static_cast<void>(while_stmnt);
}

// TODO
void BytecodeGen::gen_for(ForStmnt* for_stmnt, arcvm::Function* ir_gen) {
    static_cast<void>(for_stmnt);
}

// TODO
void BytecodeGen::gen_if(IfStmnt* if_stmnt, arcvm::Function* ir_gen) {
    static_cast<void>(if_stmnt);
}

// TODO
void BytecodeGen::gen_ret(RetStmnt* ret_stmnt, arcvm::Function* ir_gen) {
    auto expr_result = gen_expr(ret_stmnt->expr, ir_gen);
    ir_gen->gen_inst(arcvm::Instruction::ret, {expr_result});
}

void BytecodeGen::gen_decl(Decl* decl, arcvm::Function* ir_gen) {

}

arcvm::Value BytecodeGen::gen_expr(Expr* expr, arcvm::Function* ir_gen) {
    switch(expr->type) {
        case EXPR_INT_LIT:
            return gen_immediate(expr->int_literal.val, ir_gen);
            break;
        case EXPR_FLOAT_LIT:
            return gen_immediate(expr->float_literal.val, ir_gen);
            break;
        case EXPR_STRING_LIT:
            return gen_immediate(expr->string_literal.val, ir_gen);
            break;
        case EXPR_ID:
            return gen_id(expr->id.val, ir_gen);
            break;
        case EXPR_FN_CALL:
            return gen_fn_call(expr, ir_gen);
            break;
        case EXPR_BIN:
            return gen_bin(expr, ir_gen);
            break;
        case EXPR_UNARY:
            return gen_unary(expr, ir_gen);
            break;
        default:
            std::cout << "expr gen error\n";
            return {};
    }
}

arcvm::Value BytecodeGen::gen_immediate(i64 immediate, arcvm::Function* ir_gen) {
    auto val_ptr = ir_gen->gen_inst(arcvm::Instruction::alloc, {arcvm::Value{arcvm::ValueType::type, arcvm::Type::ir_i32}});
    ir_gen->gen_inst(arcvm::Instruction::store, {val_ptr, arcvm::Value{arcvm::ValueType::immediate, immediate}});
    auto val = ir_gen->gen_inst(arcvm::Instruction::load, {val_ptr});
    return val;
}
// TODO
arcvm::Value BytecodeGen::gen_immediate(f64 immediate, arcvm::Function* ir_gen) {
    return {};
}

// TODO
arcvm::Value BytecodeGen::gen_immediate(std::string* immediate, arcvm::Function* ir_gen) {
    return {};
}

// TODO
arcvm::Value BytecodeGen::gen_id(std::string* id, arcvm::Function* ir_gen) {
    return {};
}

// TODO
arcvm::Value BytecodeGen::gen_fn_call(Expr* expr, arcvm::Function* ir_gen) {
    return {};
}

// TODO
arcvm::Value BytecodeGen::gen_bin(Expr* expr, arcvm::Function* ir_gen) {
    return {};
}

// TODO
arcvm::Value BytecodeGen::gen_unary(Expr* expr, arcvm::Function* ir_gen) {
    return {};
}