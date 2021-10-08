#include "BytecodeGen.h"

BytecodeGen::BytecodeGen(): variable_table_{} {}

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
        auto* ir_function = module->gen_function_def(function->id, args, ret_type);
        if(function->is_main)
            ir_function->add_attribute(arcvm::Attribute::entrypoint);
        // push and pop scope before and after function gen
        variable_table_.emplace_back();
        // FIXME temporary fix. update the backend instead
        gen_function(function, &(ir_function->block));
        variable_table_.pop_back();
    }
}

// TODO
void BytecodeGen::gen_import(Import* import) {
    static_cast<void>(import);
}

void BytecodeGen::gen_function(Function* function, arcvm::Block* ir_gen) {
    for(auto* stmnt: function->body->statements) {
        gen_statement(stmnt, ir_gen);
    }
}

void BytecodeGen::gen_statement(Statement* statement, arcvm::Block* ir_gen) {
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
        case RET: {
            auto bblock = ir_gen->get_bblock();
            gen_ret(statement->ret_stmnt, bblock);
            break;
        }
        case EXPRESSION: {
            auto bblock = ir_gen->get_bblock();
            gen_expr(statement->expr, bblock);
            break;
        }
        case DECLARATION: {
            auto bblock = ir_gen->get_bblock();
            gen_decl(statement->decl, bblock);
            break;
        }
    }
}

// TODO
void BytecodeGen::gen_while(WhileStmnt* while_stmnt, arcvm::Block* ir_gen) {
    static_cast<void>(while_stmnt);
}

// TODO
void BytecodeGen::gen_for(ForStmnt* for_stmnt, arcvm::Block* ir_gen) {
    static_cast<void>(for_stmnt);
}

// TODO
void BytecodeGen::gen_if(IfStmnt* if_stmnt, arcvm::Block* ir_gen) {
    static_cast<void>(if_stmnt);
}

// TODO
void BytecodeGen::gen_ret(RetStmnt* ret_stmnt, arcvm::BasicBlock* ir_gen) {
    auto expr_result = gen_expr(ret_stmnt->expr, ir_gen);
    ir_gen->gen_inst(arcvm::Instruction::ret, {expr_result});
}

// TODO allocate based on size of type
// TODO add reference to decl in symbol table for later use
void BytecodeGen::gen_decl(Decl* decl, arcvm::BasicBlock* ir_gen) {
    auto val_ptr = ir_gen->gen_inst(arcvm::Instruction::alloc, {arcvm::Value{arcvm::ValueType::type, arcvm::Type::ir_i32}});
    auto expr_result = gen_expr(decl->val, ir_gen);
    ir_gen->gen_inst(arcvm::Instruction::store, {val_ptr, expr_result});
    auto val = ir_gen->gen_inst(arcvm::Instruction::load, {val_ptr});
    variable_table_.back()[*(decl->id)] = val;
}

arcvm::Value BytecodeGen::gen_expr(Expr* expr, arcvm::BasicBlock* ir_gen) {
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
            return gen_rvalue_var(expr->id.val, ir_gen);
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

arcvm::Value BytecodeGen::gen_immediate(i64 immediate, arcvm::BasicBlock* ir_gen) {
    auto val_ptr = ir_gen->gen_inst(arcvm::Instruction::alloc, {arcvm::Value{arcvm::ValueType::type, arcvm::Type::ir_i32}});
    ir_gen->gen_inst(arcvm::Instruction::store, {val_ptr, arcvm::Value{arcvm::ValueType::immediate, immediate}});
    auto val = ir_gen->gen_inst(arcvm::Instruction::load, {val_ptr});
    return val;
}
// TODO
arcvm::Value BytecodeGen::gen_immediate(f64 immediate, arcvm::BasicBlock* ir_gen) {
    return {};
}

// TODO
arcvm::Value BytecodeGen::gen_immediate(std::string* immediate, arcvm::BasicBlock* ir_gen) {
    return {};
}

// TODO need to load from symbol table
arcvm::Value BytecodeGen::gen_rvalue_var(std::string* id, arcvm::BasicBlock* ir_gen) {
    auto var_ref = variable_table_.back().at(*id);
    return var_ref;
}

// TODO
arcvm::Value BytecodeGen::gen_fn_call(Expr* expr, arcvm::BasicBlock* ir_gen) {
    return {};
}

// TODO
arcvm::Value BytecodeGen::gen_bin(Expr* expr, arcvm::BasicBlock* ir_gen) {
    PROFILE();
    auto lhs = gen_expr(expr->binary_expr.left, ir_gen);
    auto rhs = gen_expr(expr->binary_expr.right, ir_gen);
    switch(expr->binary_expr.op) {
        case ARC_ADD_EQUAL: // TODO lhs has to be an lvalue
            break;          // for ARC_XXX_EQUAL stuff
        case ARC_SUB_EQUAL:
            break;
        case ARC_DIV_EQUAL:
            break;
        case ARC_MUL_EQUAL:
            break;
        case ARC_MOD_EQUAL:
            break;
        case ARC_OR_EQUAL:
            break;
        case ARC_AND_EQUAL:
            break;
        case ARC_NOT_EQUAL:
            break;
        case ARC_LEFT_SHIFT_EQUAL:
            break;
        case ARC_RIGHT_SHIFT_EQUAL:
            break;
        case ARC_XOR_EQUAL:
            break;
        case ARC_LESSER_EQUAL:
        {
            auto val = ir_gen->gen_inst(arcvm::Instruction::lte, {lhs, rhs});
            return val;
        }
        case ARC_GREATER_EQUAL:
        {
            auto val = ir_gen->gen_inst(arcvm::Instruction::gte, {lhs, rhs});
            return val;
        }
        case ARC_EQUAL:
            break;
        case ARC_ASSIGN:
            break;
        case ARC_INFER: // TODO should this be a valid expr???
            break;
        case ARC_ADD:
        {
            auto val = ir_gen->gen_inst(arcvm::Instruction::add, {lhs, rhs});
            return val;
        }
        case ARC_SUB:
        {
            auto val = ir_gen->gen_inst(arcvm::Instruction::sub, {lhs, rhs});
            return val;
        }
        case ARC_DIV:
        {
            auto val = ir_gen->gen_inst(arcvm::Instruction::div, {lhs, rhs});
            return val;
        }
        case ARC_MUL:
        {
            auto val = ir_gen->gen_inst(arcvm::Instruction::mul, {lhs, rhs});
            return val;
        }
        case ARC_MOD:
        {
            auto val = ir_gen->gen_inst(arcvm::Instruction::mod, {lhs, rhs});
            return val;
        }
        case ARC_BIN_OR:
        {
            auto val = ir_gen->gen_inst(arcvm::Instruction::bin_or, {lhs, rhs});
            return val;
        }
        case ARC_BIN_AND:
        {
            auto val = ir_gen->gen_inst(arcvm::Instruction::bin_and, {lhs, rhs});
            return val;
        }
        case ARC_LEFT_SHIFT:
        {
            auto val = ir_gen->gen_inst(arcvm::Instruction::lshift, {lhs, rhs});
            return val;
        }
        case ARC_RIGHT_SHIFT:
        {
            auto val = ir_gen->gen_inst(arcvm::Instruction::rshift, {lhs, rhs});
            return val;
        }
        case ARC_XOR:
        {
            auto val = ir_gen->gen_inst(arcvm::Instruction::bin_xor, {lhs, rhs});
            return val;
        }
        case ARC_LESSER:
        {
            auto val = ir_gen->gen_inst(arcvm::Instruction::lt, {lhs, rhs});
            return val;
        }
        case ARC_GREATER:
        {
            auto val = ir_gen->gen_inst(arcvm::Instruction::gt, {lhs, rhs});
            return val;
        }
        case ARC_LOGICAL_OR:
        {
            auto val = ir_gen->gen_inst(arcvm::Instruction::log_or, {lhs, rhs});
            return val;
        }
        case ARC_LOGICAL_AND:
        {
            auto val = ir_gen->gen_inst(arcvm::Instruction::log_and, {lhs, rhs});
            return val;
        }
        default:
            // not binary op
            break;
    }
    return {0xffffffff};
}

// TODO
arcvm::Value BytecodeGen::gen_unary(Expr* expr, arcvm::BasicBlock* ir_gen) {
    return {};
}