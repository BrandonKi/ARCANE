#include "BytecodeGen.h"

using vm = Arcvm;

BytecodeGen::BytecodeGen(Project *ast):
    ast_(ast), code_(), variable_table_(), local_variable_counter()
{

}

BytecodeGen::~BytecodeGen() {

}

code_block BytecodeGen::gen_code() {
    PROFILE();
    return gen_project(ast_);
}

code_block BytecodeGen::gen_project(Project *project) {
    PROFILE();
    std::vector<std::vector<linkable_function, arena_allocator<linkable_function>>> segmented_files;
    for(auto* file : project->files) {
        segmented_files.push_back(gen_file(file));
    }

    BytecodeLinker linker;
    linker.link_file(segmented_files.back());

    // right now only one file is supported or else everything breaks
    std::vector<code_block, arena_allocator<code_block>> generated_files;
    for(const auto& file : segmented_files) {
        generated_files.push_back(code_block{});
        for(const auto& function : file)
            push_block(generated_files.back(), function.code);
    }

    return generated_files[0];
}
// FIXME a good amount of data is being duplicated here
// the function_table_ and result of this function 
// both have their own copies of every function in every file
// kinda urgent that this is fixed
std::vector<linkable_function, arena_allocator<linkable_function>> BytecodeGen::gen_file(File *file) {
    PROFILE();
    std::vector<linkable_function, arena_allocator<linkable_function>> file_blocks;
    if(file->is_main) {
        function_table_.insert_or_assign("0__BOOTSTRAP__0", code_block{});
        bc_context bootstrap_ctx = bc_context{function_table_.at("0__BOOTSTRAP__0")};
        generate_bootstrap(bootstrap_ctx);
        file_blocks.push_back(linkable_function{new astring{"0__BOOTSTRAP__0"}, bootstrap_ctx.code});
   }

    for(const auto *import : file->imports) {
        // gen_import(ctx, import);
    }
    
    for (const auto *decl : file->decls) {
        // gen_decl(ctx, decl);
    }
    


    for (const auto *function : file->functions) {
        // because we use insert_or_assign ou can redefine functions
        // this should probably change in the future
        function_table_.insert_or_assign(function->id, code_block{});
        bc_context ctx = bc_context{function_table_.at(function->id)};
        gen_function(ctx, function);
        file_blocks.push_back(linkable_function{&function->id, ctx.code});
    }

    // move the main function after the bootstrap section so it gets called first
    auto& functions = file_blocks;

    auto it = std::find_if(functions.cbegin(), functions.cend(), [](const auto& fn){ return *fn.name == "main";});
    if(it == functions.end()) {
        errorLog.push({FATAL, nullptr, args.path, "could not find main function"});
        std::exit(-1);
    }
    auto main = *it;
    functions.erase(it);
    functions.insert(functions.begin()+1, main);

    // TODO LINK THE SHIT TOGETHER!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    return file_blocks;
}

void BytecodeGen::gen_import(bc_context& ctx, const Import *import) {
    PROFILE();
    static_cast<void>(import);
    //TODO implement importing files/modules
}

void BytecodeGen::gen_function(bc_context& ctx, const Function *function) {
    PROFILE();
    push(ctx.code, vm::allocate_locals);
    push(ctx.code, 0x00);   // placeholder for amount of local vars to allocate
    auto index = ctx.code.size() - 1;
    gen_block(ctx, function->body);
    push(ctx.code, vm::deallocate_locals);
    push(ctx.code, static_cast<u8>(local_variable_counter));
    // replace the placeholder value
    ctx.code[index] = static_cast<u8>(local_variable_counter);
    local_variable_counter = 0;
}

void BytecodeGen::gen_block(bc_context& ctx, const Block *block) {
    PROFILE();
    for(const auto *statement : block->statements) {
        gen_statement(ctx, statement);
    }
}

void BytecodeGen::gen_statement(bc_context& ctx, const Statement *statement) {
    PROFILE();
    switch(statement->type) {
        case WHILE:
            gen_while(ctx, statement->while_);
            break;
        case FOR:
            gen_for(ctx, statement->for_);
            break;
        case IF:
            gen_if(ctx, statement->if_);
            break;
        case RET:
            gen_ret(ctx, statement->ret);
            break;
        case EXPRESSION:
            gen_expr(ctx, statement->expr);
            break;
        case DECLARATION:
            gen_decl(ctx, statement->decl);
            break;
    }
}

void BytecodeGen::gen_while(bc_context& ctx, const While_* w) {
    PROFILE();
    //TODO implement this
    gen_expr(ctx, w->expr);
    gen_block(ctx, w->block);
}

void BytecodeGen::gen_for(bc_context& ctx, const For_* f) {
    PROFILE();
    //TODO implement this
    gen_decl(ctx, f->decl);
    gen_expr(ctx, f->expr1);
    gen_expr(ctx, f->expr2);
    gen_block(ctx, f->block);
}

void BytecodeGen::gen_if(bc_context& ctx, const If_* i) {
    PROFILE();
    //TODO implement this
    gen_expr(ctx, i->expr);
    gen_block(ctx, i->block);
}

void BytecodeGen::gen_ret(bc_context& ctx, const Ret* r) {
    PROFILE();
    //TODO implement this
    gen_expr(ctx, r->expr);
    push(ctx.code, vm::ret);
}

void BytecodeGen::gen_decl(bc_context& ctx, const Decl *d) {
    PROFILE();
    //TODO implement this
    ++local_variable_counter;
    variable_table_.insert_or_assign(*(d->id), local_variable_counter);
    gen_expr(ctx, d->val);
    push(ctx.code, vm::set_local);
    push(ctx.code, static_cast<u8>(local_variable_counter));    // local var counter
}

void BytecodeGen::gen_expr(bc_context& ctx, const Expr *e) {
    PROFILE();
    //TODO implement this
    switch(e->type) {
        case EXPR_INT_LIT:
            gen_int_lit(ctx, e->int_literal.val);
            break;
        case EXPR_FLOAT_LIT:
            gen_float_lit(ctx, e->float_literal.val);
            break;
        case EXPR_STRING_LIT:
            gen_string_lit(ctx, e->string_literal.val);
            break;
        case EXPR_ID:
            gen_id(ctx, e->id.val);
            break;
        case EXPR_BIN:
            gen_bin(ctx, e);
            break;
        case EXPR_UNARY:
            gen_unary(ctx, e);
            break;
    }
}

void BytecodeGen::gen_int_lit(bc_context& ctx, const u64 val) {
    PROFILE();
    push(ctx.code, vm::push_value_signed_64);
    push_value(ctx.code, val);
}

void BytecodeGen::gen_float_lit(bc_context& ctx, const f64 val) {
    PROFILE();
    push(ctx.code, vm::push_value_float_64);
    push_value(ctx.code, val);
}

void BytecodeGen::gen_string_lit(bc_context& code, const astring *val) {
    PROFILE();
    static_cast<void>(val);
}

void BytecodeGen::gen_id(bc_context& ctx, const astring* id) {
    PROFILE();
    // this is only for non assignable values
    // for ex. it would not be for "val = 1 + 1;"

    // if function
    // do something
    // otherwise do this
    if(variable_table_.contains(*id)) {
        const auto local_var_index = variable_table_.at(*id);
        push(ctx.code, vm::load_local);   // assume we are loading a local variable and not a function arg
        push(ctx.code, static_cast<u8>(local_var_index));
    }
    else if(function_table_.contains(*id)) {
        push(ctx.code, vm::call_short);
        push_string(ctx.code, *id);
        push(ctx.code, '\0');
    }
    else {
        errorLog.push({FATAL, nullptr, args.path, "how the fuck did you manage to get this error"});
        errorLog.flush();
        std::exit(-1);
    }
}

void BytecodeGen::gen_bin(bc_context& ctx, const Expr *expr) {
    PROFILE();
    switch(expr->binary_expr.op) {
        case ARC_ADD_EQUAL:
            break;
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
            break;
        case ARC_GREATER_EQUAL:
            break;
        case ARC_EQUAL:
            break;
        case ARC_ASSIGN:
        {
            if(expr->binary_expr.left->type != EXPR_ID)
                (void)1; // FIXME error here
            const auto index = variable_table_.at(*(expr->binary_expr.left->id.val));
            // TODO add error case if id is not found
            gen_expr(ctx, expr->binary_expr.right);
            push(ctx.code, vm::set_local);
            push(ctx.code, static_cast<u8>(index));
            break;
        }
        case ARC_INFER:
            break;
        case ARC_ADD:
            gen_expr(ctx, expr->binary_expr.left);
            gen_expr(ctx, expr->binary_expr.right);
            push(ctx.code, vm::adds);
            break;
        case ARC_SUB:
            gen_expr(ctx, expr->binary_expr.left);
            gen_expr(ctx, expr->binary_expr.right);
            push(ctx.code, vm::subs);
            break;
        case ARC_DIV:
            gen_expr(ctx, expr->binary_expr.left);
            gen_expr(ctx, expr->binary_expr.right);
            push(ctx.code, vm::divs);
            break;
        case ARC_MUL:
            gen_expr(ctx, expr->binary_expr.left);
            gen_expr(ctx, expr->binary_expr.right);
            push(ctx.code, vm::muls);
            break;
        case ARC_MOD:
            gen_expr(ctx, expr->binary_expr.left);
            gen_expr(ctx, expr->binary_expr.right);
            push(ctx.code, vm::mods);
            break;
        case ARC_BIN_OR:
            gen_expr(ctx, expr->binary_expr.left);
            gen_expr(ctx, expr->binary_expr.right);
            push(ctx.code, vm::ors);
            break;
        case ARC_BIN_AND:
            gen_expr(ctx, expr->binary_expr.left);
            gen_expr(ctx, expr->binary_expr.right);
            push(ctx.code, vm::ands);
            break;
        case ARC_LEFT_SHIFT:
            break;
        case ARC_RIGHT_SHIFT:
            break;
        case ARC_XOR:
            break;
        case ARC_LESSER:
            gen_expr(ctx, expr->binary_expr.left);
            gen_expr(ctx, expr->binary_expr.right);
            push(ctx.code, vm::lts);
            break;
        case ARC_GREATER:
            gen_expr(ctx, expr->binary_expr.left);
            gen_expr(ctx, expr->binary_expr.right);
            push(ctx.code, vm::gts);
            break;
        case ARC_LOGICAL_OR:
            //FIXME for now bitwise or works fine because it's
            // equivalent in most situations
            gen_expr(ctx, expr->binary_expr.left);
            gen_expr(ctx, expr->binary_expr.right);
            push(ctx.code, vm::ors);
            break;
        case ARC_LOGICAL_AND:
            //FIXME for now bitwise and works fine because it's
            // equivalent in most situations
            gen_expr(ctx, expr->binary_expr.left);
            gen_expr(ctx, expr->binary_expr.right);
            push(ctx.code, vm::ands);
            break;
        default:
            // not binary op
            break;
        
    }
}

void BytecodeGen::gen_unary(bc_context& ctx, const Expr* expr) {
    PROFILE();
    switch(expr->unary_expr.op) {
        case ARC_NOT:
            break;
        case ARC_NEGATE:
            break;
        case ARC_PRE_INCREMENT:
            break;
        case ARC_POST_INCREMENT:
            break;
        case ARC_PRE_DECREMENT:
            break;
        case ARC_POST_DECREMENT:
            break;
        default:
            // not unary op
            break;
    }
}

void BytecodeGen::push(code_block& code, const u8 inst) {
    PROFILE();
    code.push_back(inst);
}

void BytecodeGen::push_block(code_block& code, const code_block& vec) {
    PROFILE();
    code.insert(code.end(), vec.begin(), vec.end());
}

void BytecodeGen::push_string(code_block& code, const astring& str) {
    PROFILE();
    code.insert(code.end(), str.begin(), str.end());
}

void BytecodeGen::generate_bootstrap(bc_context& ctx) {
    PROFILE();

    const std::vector<u8, arena_allocator<u8>> vec = {
        0xfa,
        0xca,
        0xde,
        vm::push_value_unsigned_8,
        0x00,
        vm::call_short,
        0x00,
        0x0a,
        0x00,
        0x00,
        0x00,
        0x00,
        vm::exit,
    };
    
    push_block(ctx.code, vec);
}