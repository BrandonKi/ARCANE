#include "BytecodeGen.h"

extern ErrorHandler error_log;
extern TypeManager type_manager;

using vm = Arcvm;

BytecodeGen::BytecodeGen(Project *ast):
    ast_(ast), code_(), variable_table_(), function_table_(), function_args_(), local_variable_counter()
{
    PROFILE();
}

BytecodeGen::~BytecodeGen() {
    PROFILE();
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
        code_block bootstrap_code = function_table_.at("0__BOOTSTRAP__0");
        generate_bootstrap(bootstrap_code);
        file_blocks.push_back(linkable_function{new astring{"0__BOOTSTRAP__0"}, bootstrap_code});
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
        code_block code = function_table_.at(function->id);
        gen_function(code, function);
        file_blocks.push_back(linkable_function{&function->id, code});
    }

    // move the main function after the bootstrap section so it gets called first
    auto& functions = file_blocks;

    auto it = std::find_if(functions.cbegin(), functions.cend(), [](const auto& fn){ return *fn.name == "main";});
    if(it == functions.end()) {
        error_log.exit({FATAL, INVALID_SRC_POS, args.path, "could not find main function"});
    }
    auto main = *it;
    functions.erase(it);
    functions.insert(functions.begin()+1, main);

    return file_blocks;
}

void BytecodeGen::gen_import(code_block& code, const Import *import) {
    PROFILE();
    static_cast<void>(import);
    //TODO implement importing files/modules
}

void BytecodeGen::gen_function(code_block& code, const Function *function) {
    PROFILE();
    local_variable_counter = 0;
    // FIXME unnecessary copy
    function_args_ = function->args;
    push(code, vm::allocate_locals);
    push(code, 0x00);   // placeholder for amount of local vars to allocate
    auto index = code.size() - 1;
    gen_block(code, function->body);
    deallocate_local_vars(code, index);
}

void BytecodeGen::gen_block(code_block& code, const Block *block) {
    PROFILE();
    for(const auto *statement : block->statements) {
        gen_statement(code, statement);
    }
}

void BytecodeGen::gen_statement(code_block& code, const Statement *statement) {
    PROFILE();
    switch(statement->type) {
        case WHILE:
            gen_while(code, statement->while_);
            break;
        case FOR:
            gen_for(code, statement->for_);
            break;
        case IF:
            gen_if(code, statement->if_);
            break;
        case RET:
            gen_ret(code, statement->ret);
            break;
        case EXPRESSION:
            gen_expr(code, statement->expr);
            break;
        case DECLARATION:
            gen_decl(code, statement->decl);
            break;
    }
}

void BytecodeGen::gen_while(code_block& code, const While_* w) {
    PROFILE();
    //TODO implement this
    gen_expr(code, w->expr);
    gen_block(code, w->block);
}

void BytecodeGen::gen_for(code_block& code, const For_* f) {
    PROFILE();
    //TODO implement this
    gen_decl(code, f->decl);
    gen_expr(code, f->expr1);
    gen_expr(code, f->expr2);
    gen_block(code, f->block);
}

void BytecodeGen::gen_if(code_block& code, const If_* i) {
    PROFILE();
    //TODO implement this
    gen_expr(code, i->expr);
    gen_block(code, i->block);
}

void BytecodeGen::gen_ret(code_block& code, const Ret* r) {
    PROFILE();
    //TODO implement this
    gen_expr(code, r->expr);
    push(code, vm::ret);
}

void BytecodeGen::gen_decl(code_block& code, const Decl *d) {
    PROFILE();
    //TODO implement this
    ++local_variable_counter;
    variable_table_.insert_or_assign(*(d->id), local_variable_counter);
    gen_expr(code, d->val);
    push(code, vm::set_local);
    push(code, static_cast<u8>(local_variable_counter));    // local var counter
}

void BytecodeGen::gen_expr(code_block& code, const Expr *e) {
    PROFILE();
    //TODO implement this
    switch(e->type) {
        case EXPR_INT_LIT:
            gen_int_lit(code, e->int_literal.val);
            break;
        case EXPR_FLOAT_LIT:
            gen_float_lit(code, e->float_literal.val);
            break;
        case EXPR_STRING_LIT:
            gen_string_lit(code, e->string_literal.val);
            break;
        case EXPR_ID:
            gen_id(code, e->id.val);
            break;
        case EXPR_FN_CALL:
            gen_fn_call(code, e);
            break;
        case EXPR_BIN:
            gen_bin(code, e);
            break;
        case EXPR_UNARY:
            gen_unary(code, e);
            break;
    }
}

void BytecodeGen::gen_int_lit(code_block& code, const u64 val) {
    PROFILE();
    push(code, vm::push_value_signed_64);
    push_value(code, val);
}

void BytecodeGen::gen_float_lit(code_block& code, const f64 val) {
    PROFILE();
    push(code, vm::push_value_float_64);
    push_value(code, val);
}

void BytecodeGen::gen_string_lit(code_block& code, const astring *val) {
    PROFILE();
    static_cast<void>(val);
}

void BytecodeGen::gen_id(code_block& code, const astring* id) {
    PROFILE();
    // this is only for non assignable values
    // for ex. it would not be for "val = 1 + 1;"

    if(is_variable(*id)) {
        const auto local_var_index = variable_table_.at(*id);
        push(code, vm::load_local);   // assume we are loading a local variable and not a function arg
        push(code, static_cast<u8>(local_var_index));
    }
    else if(is_function_arg(*id)) {
        // TODO implement this plz
        auto index = get_function_arg_index(*id);
        push(code, vm::load_arg);
        push(code, static_cast<u8>(index));
    }
    else {
        error_log.exit({FATAL, INVALID_SRC_POS, args.path, "how did you manage to get this error?"});
    }
}

void BytecodeGen::gen_fn_call(code_block& code, const Expr *expr) {
    PROFILE();
    auto val = expr->fn_call.val;
    auto argc = expr->fn_call.argc;
    auto args = expr->fn_call.args;

    for(u32 i = 0; i < argc; ++i) {
        gen_expr(code, args[i]);
    }

    if(function_table_.contains(*val)) {
        push(code, vm::call_short);
        push_string(code, *val);
        push(code, '\0');
    }
    else {
        // TODO error here I guess
        // "can't find function"
    }

}


void BytecodeGen::gen_bin(code_block& code, const Expr *expr) {
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
                (void)1; // FIXME error here, also this shouldn't even be an issue at this stage
            const auto index = variable_table_.at(*(expr->binary_expr.left->id.val));
            // TODO add error case if id is not found
            gen_expr(code, expr->binary_expr.right);
            push(code, vm::set_local);
            push(code, static_cast<u8>(index));
            break;
        }
        case ARC_INFER:
            break;
        case ARC_ADD:   // TODO fix all the types plz
            switch(expr->result_type) {
                case TYPE_F64:
                    gen_expr(code, expr->binary_expr.left);
                    gen_expr(code, expr->binary_expr.right);
                    push(code, vm::addf);
                    break;
                case TYPE_I8:
                case TYPE_I16:
                case TYPE_I32:
                case TYPE_I64:
                    gen_expr(code, expr->binary_expr.left);
                    gen_expr(code, expr->binary_expr.right);
                    push(code, vm::adds);
                    break;
                case TYPE_U8:
                case TYPE_U16:
                case TYPE_U32:
                case TYPE_U64:
                    gen_expr(code, expr->binary_expr.left);
                    gen_expr(code, expr->binary_expr.right);
                    push(code, vm::addu);
                    break;
            }
            break;
        case ARC_SUB:   // TODO all these types are just plain incorrect :(
            if(expr->result_type == TYPE_F64) { 
                gen_expr(code, expr->binary_expr.left);
                gen_expr(code, expr->binary_expr.right);
                push(code, vm::subf);
            }
            else if(expr->result_type == TYPE_I64) {
                gen_expr(code, expr->binary_expr.left);
                gen_expr(code, expr->binary_expr.right);
                push(code, vm::subs);
            }
            break;
        case ARC_DIV:
            if(expr->result_type == TYPE_F64) { 
                gen_expr(code, expr->binary_expr.left);
                gen_expr(code, expr->binary_expr.right);
                push(code, vm::divf);
            }
            else if(expr->result_type == TYPE_I64) {
                gen_expr(code, expr->binary_expr.left);
                gen_expr(code, expr->binary_expr.right);
                push(code, vm::divs);
            }
            break;
        case ARC_MUL:
            if(expr->result_type == TYPE_F64) { 
                gen_expr(code, expr->binary_expr.left);
                gen_expr(code, expr->binary_expr.right);
                push(code, vm::mulf);
            }
            else if(expr->result_type == TYPE_I64) {
                gen_expr(code, expr->binary_expr.left);
                gen_expr(code, expr->binary_expr.right);
                push(code, vm::muls);
            }
            break;
        case ARC_MOD:
            if(expr->result_type == TYPE_F64) { 
                // no mod for floats :(
            }
            else if(expr->result_type == TYPE_I64) {
                gen_expr(code, expr->binary_expr.left);
                gen_expr(code, expr->binary_expr.right);
                push(code, vm::mods);
            }
            break;
        case ARC_BIN_OR:
            if(expr->result_type == TYPE_F64) { 
                gen_expr(code, expr->binary_expr.left);
                gen_expr(code, expr->binary_expr.right);
                push(code, vm::oru);    // FIXME I feel like this will break
            }
            else if(expr->result_type == TYPE_I64) {
                gen_expr(code, expr->binary_expr.left);
                gen_expr(code, expr->binary_expr.right);
                push(code, vm::ors);
            }
            break;
        case ARC_BIN_AND:
            if(expr->result_type == TYPE_F64) { 
                gen_expr(code, expr->binary_expr.left);
                gen_expr(code, expr->binary_expr.right);
                push(code, vm::andu);    // FIXME I feel like this will break
            }
            else if(expr->result_type == TYPE_I64) {
                gen_expr(code, expr->binary_expr.left);
                gen_expr(code, expr->binary_expr.right);
                push(code, vm::ands);
            }
            break;
        case ARC_LEFT_SHIFT:
            break;
        case ARC_RIGHT_SHIFT:
            break;
        case ARC_XOR:
            break;
        case ARC_LESSER:
            gen_expr(code, expr->binary_expr.left);
            gen_expr(code, expr->binary_expr.right);
            push(code, vm::lts);
            break;
        case ARC_GREATER:
            gen_expr(code, expr->binary_expr.left);
            gen_expr(code, expr->binary_expr.right);
            push(code, vm::gts);
            break;
        case ARC_LOGICAL_OR:
            //FIXME for now bitwise or works fine because it's
            // equivalent in most situations
            gen_expr(code, expr->binary_expr.left);
            gen_expr(code, expr->binary_expr.right);
            push(code, vm::ors);
            break;
        case ARC_LOGICAL_AND:
            //FIXME for now bitwise and works fine because it's
            // equivalent in most situations
            gen_expr(code, expr->binary_expr.left);
            gen_expr(code, expr->binary_expr.right);
            push(code, vm::ands);
            break;
        default:
            // not binary op
            break;
        
    }
}

void BytecodeGen::gen_unary(code_block& code, const Expr* expr) {
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

void BytecodeGen::generate_bootstrap(code_block& code) {
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
    
    push_block(code, vec);
}

bool BytecodeGen::is_variable(const astring& id) {
    return variable_table_.contains(id);
}

bool BytecodeGen::is_function(const astring& id) {
    return function_table_.contains(id);
}

bool BytecodeGen::is_function_arg(const astring& id) {
    auto result = std::find_if(function_args_.cbegin(), function_args_.cend(), [&](auto& arg){
        return arg.id == id;
    });
    return result != function_args_.end();
}

i64 BytecodeGen::get_function_arg_index(const astring& id) {
    return std::distance(function_args_.cbegin(), std::find_if(function_args_.cbegin(), function_args_.cend(), [&](auto& arg){
        return arg.id == id;
    }));
}

void BytecodeGen::deallocate_local_vars(code_block& code, u64 index) {
    push(code, vm::deallocate_locals);
    push(code, static_cast<u8>(local_variable_counter));
    // replace the placeholder value
    code[index] = static_cast<u8>(local_variable_counter);
}