#include "IRGen.h"

#include <IRPrinter.h>

#include <Passes/CFResolutionPass.h>

extern ErrorHandler error_log;
extern TypeManager type_manager;

IRGen::IRGen(): variable_table_{} {}

arcvm::Arcvm IRGen::gen_project(Project* project) {
    PROFILE();
    arcvm::Arcvm vm;
    for(auto* file: project->files) {
        arcvm::IRGenerator ir_gen;
        auto* mod = ir_gen.create_module();

        gen_file(file, mod);

        vm.load_module(mod);

        if(!args.no_cleanup && !args.optimize)
            vm.run_pass_on_module<arcvm::CFResolutionPass>(mod);

        if(args.optimize)
            vm.optimize_module(mod);
        if(args.emit_ir)
             arcvm::IRPrinter::print(mod);
    }
    return vm;
}

void IRGen::gen_file(File* file, arcvm::Module* module) {
    PROFILE();
    for(auto* import: file->imports){
        // TODO
    }

    for(auto* decl: file->decls) {
        //gen_decl(decl, );
    }

    for(auto* function: file->functions) {
        auto name = function->id;
        auto args = std::vector<arcvm::Type>{};
        variable_table_.emplace_back();
        for(size_t i = 0; i < function->args.size(); ++i) {
            args.push_back(type_manager.to_ir_type(function->args[i].type));

        }
        auto ret_type = type_manager.to_ir_type(function->return_type);
        auto* ir_function = module->gen_function_def(function->id, args, ret_type);
        if(function->is_main)
            ir_function->add_attribute(arcvm::Attribute::entrypoint);

        gen_function(function, ir_function->block);
        variable_table_.pop_back();
    }
}

// TODO
void IRGen::gen_import(Import* import) {
    PROFILE();
    static_cast<void>(import);
}

void IRGen::gen_function(Function* function, arcvm::Block* ir_gen) {
    PROFILE();
    gen_function_args(function->args, ir_gen->get_bblock());
    gen_block(function->body, ir_gen);
}

void IRGen::gen_function_args(std::vector<Arg> args, arcvm::BasicBlock* ir_gen) {

    for(size_t i = 0; i < args.size(); ++i) {
        // TODO use correct type for now i32 is default
        auto type = type_manager.to_ir_type(args[i].type);
        auto var_ptr = ir_gen->gen_inst(arcvm::Instruction::alloc, {arcvm::IRValue{arcvm::IRValueType::type, type}});
        ir_gen->gen_inst(arcvm::Instruction::store, {var_ptr, arcvm::IRValue{arcvm::IRValueType::reference, i}});
        auto var_data = ir_var{arcvm::IRValue{arcvm::IRValueType::reference, i}, var_ptr};
        variable_table_.back()[args[i].id] = var_data;
    }
}

void IRGen::gen_block(Block* block, arcvm::Block* ir_gen) {
    PROFILE();
    for(auto* stmnt: block->statements) {
        gen_statement(stmnt, ir_gen);
    }
}

void IRGen::gen_statement(Statement* statement, arcvm::Block* ir_gen) {
    PROFILE();
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
void IRGen::gen_while(WhileStmnt* while_stmnt, arcvm::Block* ir_gen) {
    PROFILE();
    static_cast<void>(while_stmnt);
}

// TODO move this logic out of code gen
void IRGen::gen_for(ForStmnt* for_stmnt, arcvm::Block* ir_gen) {
    PROFILE();

    auto* loop_init = ir_gen->new_basic_block();
    auto* loop_cmp = ir_gen->new_basic_block();
    auto* loop_body = ir_gen->new_basic_block();
    auto* loop_inc = ir_gen->new_basic_block();
    auto* post_loop = ir_gen->new_basic_block();

    auto* loop_init_name = new std::string(loop_init->label.name);
    auto* loop_cmp_name = new std::string(loop_cmp->label.name);
    auto* loop_body_name = new std::string(loop_body->label.name);
    auto* loop_inc_name = new std::string(loop_inc->label.name);
    auto* post_loop_name = new std::string(post_loop->label.name);

    ir_gen->set_insertion_point(loop_init);    // init
    gen_decl(for_stmnt->decl, loop_init);
    auto* id = for_stmnt->decl->id;

    auto ind_var_type = type_manager.to_ir_type(for_stmnt->decl->type);
    ir_gen->set_insertion_point(loop_cmp);    // cmp
    auto op1 = gen_var_load(id, loop_cmp, ind_var_type);
    auto op2 = gen_expr(for_stmnt->expr, loop_cmp);
    auto expr_result = loop_cmp->gen_inst(arcvm::Instruction::lt, {op1, op2});
    loop_cmp->gen_inst(arcvm::Instruction::brnz, {expr_result,{arcvm::IRValue{arcvm::IRValueType::label, loop_body_name}},{arcvm::IRValue{arcvm::IRValueType::label, post_loop_name}}});

    ir_gen->set_insertion_point(loop_body);    // body
    gen_block(for_stmnt->block, ir_gen);

    ir_gen->set_insertion_point(loop_inc);    // inc
    // TODO could be any lvalue
    // also this is identical to ARC_ADD_EQUAL
    auto ind_ptr = variable_table_.back()[*id].pointer;
    auto ind_val = gen_var_load(id, loop_inc, ind_var_type);
    auto one_immediate = gen_immediate((u64)1, ir_gen->get_bblock());
    auto result = loop_inc->gen_inst(arcvm::Instruction::add, {ind_val, one_immediate});
    loop_inc->gen_inst(arcvm::Instruction::store, {ind_ptr, result});
    variable_table_.back()[*id].value = result;
    loop_inc->gen_inst(arcvm::Instruction::br, {arcvm::IRValue{arcvm::IRValueType::label, loop_cmp_name}});

    ir_gen->set_insertion_point(post_loop);    // post
}

void IRGen::gen_if(IfStmnt* if_stmnt, arcvm::Block* ir_gen) {
    PROFILE();
    auto bblock = ir_gen->get_bblock();
    auto expr_result = gen_expr(if_stmnt->expr, bblock);
    auto if_block = ir_gen->new_basic_block();
    gen_block(if_stmnt->block, ir_gen);
    auto else_block = ir_gen->new_basic_block();
    ir_gen->set_insertion_point(else_block);
    if(if_stmnt->else_stmnt) // if there is an else stmnt
        gen_block(if_stmnt->else_stmnt, ir_gen);
    auto then_block = ir_gen->new_basic_block();
    ir_gen->set_insertion_point(bblock);
    ir_gen->gen_if(expr_result, if_block, else_block, then_block);
    ir_gen->set_insertion_point(then_block);
}

void IRGen::gen_ret(RetStmnt* ret_stmnt, arcvm::BasicBlock* ir_gen) {
    PROFILE();
    auto expr_result = gen_expr(ret_stmnt->expr, ir_gen);
    ir_gen->gen_inst(arcvm::Instruction::ret, {expr_result});
}

// TODO allocate based on size of type
void IRGen::gen_decl(Decl* decl, arcvm::BasicBlock* ir_gen) {
    PROFILE();
    auto type = type_manager.to_ir_type(decl->type);
    auto val_ptr = ir_gen->gen_inst(arcvm::Instruction::alloc, {arcvm::IRValue{arcvm::IRValueType::type, type}});
    auto expr_result = gen_expr(decl->val, ir_gen);    // assumes types match
    ir_gen->gen_inst(arcvm::Instruction::store, {val_ptr, expr_result, type});
    //auto val = ir_gen->gen_inst(arcvm::Instruction::load, {val_ptr});
    //variable_table_.back()[*(decl->id)].value = val;
    variable_table_.back()[*(decl->id)].pointer = val_ptr;
}

arcvm::IRValue IRGen::gen_expr(Expr* expr, arcvm::BasicBlock* ir_gen) {
    PROFILE();
    auto type = type_manager.to_ir_type(expr->result_type);
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
            return gen_var_load(expr->id.val, ir_gen, type);
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

// TODO needs a special case for 64-bit immediates
// also needs to include type info with immediates in general
arcvm::IRValue IRGen::gen_immediate(u64 immediate, arcvm::BasicBlock* ir_gen) {
    PROFILE();
    return arcvm::IRValue{arcvm::IRValueType::immediate, immediate};
}

// TODO
arcvm::IRValue IRGen::gen_immediate(f64 immediate, arcvm::BasicBlock* ir_gen) {
    PROFILE();
    return {};
}

// TODO
arcvm::IRValue IRGen::gen_immediate(std::string* immediate, arcvm::BasicBlock* ir_gen) {
    PROFILE();
    return {};
}

arcvm::IRValue IRGen::gen_var(std::string* id, arcvm::BasicBlock* ir_gen) {
    PROFILE();
    auto var_ref = variable_table_.back()[*id].value;
    return var_ref;
}


arcvm::IRValue IRGen::gen_var_load(std::string* id, arcvm::BasicBlock* ir_gen, arcvm::Type type) {
    PROFILE();
    auto var_ptr = variable_table_.back()[*id].pointer;
    auto var = ir_gen->gen_inst(arcvm::Instruction::load, {var_ptr, type});
    return var;
}

arcvm::IRValue IRGen::gen_fn_call(Expr* expr, arcvm::BasicBlock* ir_gen) {
    PROFILE();
    auto val = expr->fn_call.val;
    auto argc = expr->fn_call.argc;
    auto argv = expr->fn_call.args;

    std::vector<arcvm::IRValue> ir_args{};
    ir_args.emplace_back(arcvm::IRValueType::fn_name, val);
    for(u32 i = 0; i < argc; ++i) {
        ir_args.push_back(gen_expr(argv[i], ir_gen));
    }
    ir_args.emplace_back(arcvm::Type::ir_i32);

    auto ret = ir_gen->gen_inst(arcvm::Instruction::call, ir_args);

    return ret;
}

arcvm::IRValue IRGen::gen_bin(Expr* expr, arcvm::BasicBlock* ir_gen) {
    PROFILE();
    if(is_lrvalue_expr(expr->binary_expr.op)) {
        return gen_lrvalue_expr(expr, ir_gen);
    }
    else { // if is_rrvalue_expr
        return gen_rrvalue_expr(expr, ir_gen);
    }
}

bool IRGen::is_lrvalue_expr(TokenKind bin_op) {
    PROFILE();
    switch(bin_op) {
        case ARC_ADD_EQUAL: // lhs has to be an lvalue
        case ARC_SUB_EQUAL:
        case ARC_DIV_EQUAL:
        case ARC_MUL_EQUAL:
        case ARC_MOD_EQUAL:
        case ARC_OR_EQUAL:
        case ARC_AND_EQUAL:
        case ARC_LEFT_SHIFT_EQUAL:
        case ARC_RIGHT_SHIFT_EQUAL:
        case ARC_XOR_EQUAL:
        case ARC_ASSIGN:
            return true;
        case ARC_INFER:    // TODO should this be a valid lrvalue_expr?
        case ARC_LESSER_EQUAL:
        case ARC_GREATER_EQUAL:
        case ARC_EQUAL:
        case ARC_NOT_EQUAL:
        case ARC_ADD:
        case ARC_SUB:
        case ARC_DIV:
        case ARC_MUL:
        case ARC_MOD:
        case ARC_BIN_OR:
        case ARC_BIN_AND:
        case ARC_LEFT_SHIFT:
        case ARC_RIGHT_SHIFT:
        case ARC_XOR:
        case ARC_LESSER:
        case ARC_GREATER:
        case ARC_LOGICAL_OR:
        case ARC_LOGICAL_AND:
        default:
            return false;
    }
}

arcvm::IRValue IRGen::gen_lrvalue_expr(Expr* expr, arcvm::BasicBlock* ir_gen) {
    PROFILE();
    // TODO assumes lhs is an id, but could be an array or member access
    // TODO gen lvalue_expression maybe????
    // auto lhs = gen_expr(expr->binary_expr.left, ir_gen);
    auto lhs_id_ptr = expr->binary_expr.left->id.val;
    auto lhs_id = *lhs_id_ptr;
    //auto lhs = gen_var_load(lhs_id_ptr, ir_gen);
    auto rhs = gen_expr(expr->binary_expr.right, ir_gen);

    // TODO pass type to arithmetic instructions also
    auto type = type_manager.to_ir_type(expr->result_type);

    auto lhs_ptr = variable_table_.back()[lhs_id].pointer;
    auto lhs_val = gen_var_load(lhs_id_ptr, ir_gen, type); // TODO make sure this is the right type to pass

    switch(expr->binary_expr.op) {
        case ARC_ADD_EQUAL: { // lhs has to be an lvalue
            auto result = ir_gen->gen_inst(arcvm::Instruction::add, {lhs_val, rhs});
            ir_gen->gen_inst(arcvm::Instruction::store, {lhs_ptr, result, type});
            variable_table_.back()[lhs_id].value = result;
            return result;
        }
        case ARC_SUB_EQUAL: {
            auto result = ir_gen->gen_inst(arcvm::Instruction::sub, {lhs_val, rhs});
            ir_gen->gen_inst(arcvm::Instruction::store, {lhs_ptr, result, type});
            variable_table_.back()[lhs_id].value = result;
            return result;
        }
        case ARC_DIV_EQUAL: {
            auto result = ir_gen->gen_inst(arcvm::Instruction::div, {lhs_val, rhs});
            ir_gen->gen_inst(arcvm::Instruction::store, {lhs_ptr, result, type});
            variable_table_.back()[lhs_id].value = result;
            return result;
        }
        case ARC_MUL_EQUAL: {
            auto result = ir_gen->gen_inst(arcvm::Instruction::mul, {lhs_val, rhs});
            ir_gen->gen_inst(arcvm::Instruction::store, {lhs_ptr, result, type});
            variable_table_.back()[lhs_id].value = result;
            return result;
        }
        case ARC_MOD_EQUAL: {
            auto result = ir_gen->gen_inst(arcvm::Instruction::mod, {lhs_val, rhs});
            ir_gen->gen_inst(arcvm::Instruction::store, {lhs_ptr, result, type});
            variable_table_.back()[lhs_id].value = result;
            return result;
        }
        case ARC_OR_EQUAL: {
            auto result = ir_gen->gen_inst(arcvm::Instruction::bin_or, {lhs_val, rhs});
            ir_gen->gen_inst(arcvm::Instruction::store, {lhs_ptr, result, type});
            variable_table_.back()[lhs_id].value = result;
            return result;
        }
        case ARC_AND_EQUAL: {
            auto result = ir_gen->gen_inst(arcvm::Instruction::bin_and, {lhs_val, rhs});
            ir_gen->gen_inst(arcvm::Instruction::store, {lhs_ptr, result, type});
            variable_table_.back()[lhs_id].value = result;
            return result;
        }
        case ARC_LEFT_SHIFT_EQUAL: {
            auto result = ir_gen->gen_inst(arcvm::Instruction::lshift, {lhs_val, rhs});
            ir_gen->gen_inst(arcvm::Instruction::store, {lhs_ptr, result, type});
            variable_table_.back()[lhs_id].value = result;
            return result;
        }
        case ARC_RIGHT_SHIFT_EQUAL: {
            auto result = ir_gen->gen_inst(arcvm::Instruction::rshift, {lhs_val, rhs});
            ir_gen->gen_inst(arcvm::Instruction::store, {lhs_ptr, result, type});
            variable_table_.back()[lhs_id].value = result;
            return result;
        }
        case ARC_XOR_EQUAL: {
            auto result = ir_gen->gen_inst(arcvm::Instruction::bin_xor, {lhs_val, rhs});
            ir_gen->gen_inst(arcvm::Instruction::store, {lhs_ptr, result, type});
            variable_table_.back()[lhs_id].value = result;
            return result;
        }
        case ARC_ASSIGN: {
            ir_gen->gen_inst(arcvm::Instruction::store, {lhs_ptr, rhs, type});
            variable_table_.back()[lhs_id].value = rhs;
            return rhs;
        }
        case ARC_INFER: { // TODO should this be a valid lrvalue expression?
            break;
        }
        default:
            assert(false); // not binary op
    }
    return {0xffffffff};
}

arcvm::IRValue IRGen::gen_rrvalue_expr(Expr* expr, arcvm::BasicBlock* ir_gen) {
    PROFILE();
    auto lhs = gen_expr(expr->binary_expr.left, ir_gen);
    auto rhs = gen_expr(expr->binary_expr.right, ir_gen);
    switch(expr->binary_expr.op) {
        case ARC_LESSER_EQUAL: {
            auto val = ir_gen->gen_inst(arcvm::Instruction::lte, {lhs, rhs});
            return val;
        }
        case ARC_GREATER_EQUAL: {
            auto val = ir_gen->gen_inst(arcvm::Instruction::gte, {lhs, rhs});
            return val;
        }
        case ARC_EQUAL: {
            auto val = ir_gen->gen_inst(arcvm::Instruction::eq, {lhs, rhs});
            return val;
        }
        case ARC_NOT_EQUAL: {
            auto val = ir_gen->gen_inst(arcvm::Instruction::neq, {lhs, rhs});
            return val;
        }
        case ARC_ADD: {
            auto val = ir_gen->gen_inst(arcvm::Instruction::add, {lhs, rhs});
            return val;
        }
        case ARC_SUB: {
            auto val = ir_gen->gen_inst(arcvm::Instruction::sub, {lhs, rhs});
            return val;
        }
        case ARC_DIV: {
            auto val = ir_gen->gen_inst(arcvm::Instruction::div, {lhs, rhs});
            return val;
        }
        case ARC_MUL: {
            auto val = ir_gen->gen_inst(arcvm::Instruction::mul, {lhs, rhs});
            return val;
        }
        case ARC_MOD: {
            auto val = ir_gen->gen_inst(arcvm::Instruction::mod, {lhs, rhs});
            return val;
        }
        case ARC_BIN_OR: {
            auto val = ir_gen->gen_inst(arcvm::Instruction::bin_or, {lhs, rhs});
            return val;
        }
        case ARC_BIN_AND: {
            auto val = ir_gen->gen_inst(arcvm::Instruction::bin_and, {lhs, rhs});
            return val;
        }
        case ARC_LEFT_SHIFT: {
            auto val = ir_gen->gen_inst(arcvm::Instruction::lshift, {lhs, rhs});
            return val;
        }
        case ARC_RIGHT_SHIFT: {
            auto val = ir_gen->gen_inst(arcvm::Instruction::rshift, {lhs, rhs});
            return val;
        }
        case ARC_XOR: {
            auto val = ir_gen->gen_inst(arcvm::Instruction::bin_xor, {lhs, rhs});
            return val;
        }
        case ARC_LESSER: {
            auto val = ir_gen->gen_inst(arcvm::Instruction::lt, {lhs, rhs});
            return val;
        }
        case ARC_GREATER: {
            auto val = ir_gen->gen_inst(arcvm::Instruction::gt, {lhs, rhs});
            return val;
        }
        case ARC_LOGICAL_OR: {
            assert(false);
        }
        case ARC_LOGICAL_AND: {
            assert(false);
        }
        default:
            assert(false); // not binary op
    }
    assert(false);
    return {0xffffffff};
}

// TODO
arcvm::IRValue IRGen::gen_unary(Expr* expr, arcvm::BasicBlock* ir_gen) {
    PROFILE();
    auto result = gen_expr(expr->binary_expr.left, ir_gen);
    switch(expr->binary_expr.op) {
        case ARC_NEGATE: {
            auto val = ir_gen->gen_inst(arcvm::Instruction::neg, {result});
            return val;
        }
        default:
            assert(false);
    }
    assert(false);
    return {};
}