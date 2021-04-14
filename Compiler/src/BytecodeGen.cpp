#include "BytecodeGen.h"

using vm = Arcvm;

BytecodeGen::BytecodeGen(Project *ast):
    ast_(ast), code_()
{

}

BytecodeGen::~BytecodeGen() {

}

std::vector<u8, arena_allocator<u8>> BytecodeGen::gen_code() {
    gen_project(ast_);
    return code_;
}

void BytecodeGen::gen_project(const Project *project) {
    for(const auto* file : project->files) {
        gen_file(file);
    }
}

void BytecodeGen::gen_file(const File *file) {
    
    if(file->is_main) {
        generate_bootstrap();
    }

    for(const auto *import : file->imports) {
        gen_import(import);
    }
    
    for (const auto *decl : file->decls) {
        gen_decl(decl);
    }
    
    for (const auto *function : file->functions) {
        gen_function(function);
    }


}

void BytecodeGen::gen_import(const Import *import) {
    //TODO implement importing files/modules
}

void BytecodeGen::gen_function(const Function *function) {
    // do something with args and return type
    gen_block(function->body);
}

void BytecodeGen::gen_block(const Block *block) {
    for(const auto *statement : block->statements) {
        gen_statement(statement);
    }
}

void BytecodeGen::gen_statement(const Statement *statement) {
    switch(statement->type) {
        case WHILE:
            gen_while(statement->while_);
            break;
        case FOR:
            gen_for(statement->for_);
            break;
        case IF:
            gen_if(statement->if_);
            break;
        case RET:
            gen_ret(statement->ret);
            break;
        case EXPRESSION:
            gen_expr(statement->expr);
            break;
        case DECLARATION:
            gen_decl(statement->decl);
            break;
    }
}

void BytecodeGen::gen_while(const While_* w) {
    //TODO implement this
    gen_expr(w->expr);
    gen_block(w->block);
}

void BytecodeGen::gen_for(const For_* f) {
    //TODO implement this
    gen_decl(f->decl);
    gen_expr(f->expr1);
    gen_expr(f->expr2);
    gen_block(f->block);
}

void BytecodeGen::gen_if(const If_* i) {
    //TODO implement this
    gen_expr(i->expr);
    gen_block(i->block);
}

void BytecodeGen::gen_ret(const Ret* r) {
    //TODO implement this
    gen_expr(r->expr);
    push(vm::ret);
}

void BytecodeGen::gen_decl(const Decl *d) {
    //TODO implement this
    gen_expr(d->val);
}

void BytecodeGen::gen_expr(const Expr *e) {
    //TODO implement this
    switch(e->type) {
        case EXPR_INT_LIT:
            gen_int_lit(e->int_literal.val);
            break;
        case EXPR_FLOAT_LIT:
            gen_float_lit(e->float_literal.val);
            break;
        case EXPR_STRING_LIT:
            gen_string_lit(e->string_literal.val);
            break;
        case EXPR_ID:
            gen_id(e->string_literal.val);
            break;
        case EXPR_BIN:
            gen_bin(e);
            break;
        case EXPR_UNARY:
            gen_unary(e);
            break;
    }
}

void BytecodeGen::gen_int_lit(const u64 val) {
    push_64_bit_value(val);
}

void BytecodeGen::gen_float_lit(const f64 val) {
    
}

void BytecodeGen::gen_string_lit(const astring *val) {
    
}

void BytecodeGen::gen_id(const astring* id) {
    
}

void BytecodeGen::gen_bin(const Expr *expr) {
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
            break;
        case ARC_INFER:
            break;
        case ARC_ADD:
            gen_expr(expr->binary_expr.left);
            gen_expr(expr->binary_expr.right);
            push(vm::adds);
            break;
        case ARC_SUB:
            gen_expr(expr->binary_expr.left);
            gen_expr(expr->binary_expr.right);
            push(vm::subs);
            break;
        case ARC_DIV:
            gen_expr(expr->binary_expr.left);
            gen_expr(expr->binary_expr.right);
            push(vm::divs);
            break;
        case ARC_MUL:
            gen_expr(expr->binary_expr.left);
            gen_expr(expr->binary_expr.right);
            push(vm::muls);
            break;
        case ARC_MOD:
            gen_expr(expr->binary_expr.left);
            gen_expr(expr->binary_expr.right);
            push(vm::mods);
            break;
        case ARC_BIN_OR:
            gen_expr(expr->binary_expr.left);
            gen_expr(expr->binary_expr.right);
            push(vm::ors);
            break;
        case ARC_BIN_AND:
            gen_expr(expr->binary_expr.left);
            gen_expr(expr->binary_expr.right);
            push(vm::ands);
            break;
        case ARC_LEFT_SHIFT:
            break;
        case ARC_RIGHT_SHIFT:
            break;
        case ARC_XOR:
            break;
        case ARC_LESSER:
            gen_expr(expr->binary_expr.left);
            gen_expr(expr->binary_expr.right);
            push(vm::lts);
            break;
        case ARC_GREATER:
            gen_expr(expr->binary_expr.left);
            gen_expr(expr->binary_expr.right);
            push(vm::gts);
            break;
        case ARC_LOGICAL_OR:
            //FIXME for now bitwise or works fine because it
            // equivalent in most situations
            gen_expr(expr->binary_expr.left);
            gen_expr(expr->binary_expr.right);
            push(vm::ors);
            break;
        case ARC_LOGICAL_AND:
            //FIXME for now bitwise or works fine because it
            // equivalent in most situations
            gen_expr(expr->binary_expr.left);
            gen_expr(expr->binary_expr.right);
            push(vm::ands);
            break;
        default:
            // not binary op
            break;
        
    }
}

void BytecodeGen::gen_unary(const Expr* expr) {
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

void BytecodeGen::push(const u8 inst) {
    code_.push_back(inst);
}

void BytecodeGen::push(const std::vector<u8, arena_allocator<u8>>& vec) {
    code_.insert(code_.end(), vec.begin(), vec.end());
}

void BytecodeGen::push_64_bit_value(const u64 val) {
    push(vm::push_value_signed_64);
    code_.push_back(static_cast<u8>(val));
    code_.push_back(static_cast<u8>(val >> 8));
    code_.push_back(static_cast<u8>(val >> 16));
    code_.push_back(static_cast<u8>(val >> 24));
    code_.push_back(static_cast<u8>(val >> 32));
    code_.push_back(static_cast<u8>(val >> 40));
    code_.push_back(static_cast<u8>(val >> 48));
    code_.push_back(static_cast<u8>(val >> 56));
}

void BytecodeGen::generate_bootstrap() {

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
    
    push(vec);
}