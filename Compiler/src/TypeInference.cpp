#include "TypeInference.h"

extern ErrorHandler error_log;
extern TypeManager type_manager;

TypeInference::TypeInference(Project* ast):
    ast_(ast)
{
    PROFILE();
}

void TypeInference::start() {
    PROFILE();
    analyze_project(ast_);
}

void TypeInference::analyze_project(Project* project) {
    PROFILE();
    for (auto* file : ast_->files) {
        analyze_file(file);
    }
}

void TypeInference::analyze_file(File* file) {
    PROFILE();
    current_filename_ = file->name;
    for (auto* decl : file->decls) {
        analyze_decl(decl);
    }

    for (auto* function : file->functions) {
        analyze_function(function);
    }
}

void TypeInference::update_symbol_table(std::vector<Arg, arena_allocator<Arg>>& args) {
    for(const auto& arg : args)
        s_table_.add_symbol(arg.id, VARIABLE, arg.type);
}

void TypeInference::analyze_function(Function* function) {
    PROFILE();
    update_symbol_table(function->args);
    for (auto* stmnt : function->body->statements) {
        switch(stmnt->type) {
            case WHILE:
                break;
            case FOR:
                break;
            case IF:
                break;
            case RET:
                analyze_ret(stmnt->ret, function->return_type);
                break;
            case EXPRESSION:
                analyze_expr(stmnt->expr);
                break;
            case DECLARATION:
                analyze_decl(stmnt->decl);
                break;
            default:
                break;
        }
    }
}

void TypeInference::analyze_ret(Ret* ret, type_handle ret_type) {
    PROFILE();
    analyze_expr(ret->expr);

    if(ret->expr->result_type != ret_type) {
        auto err = strtoastr(
            "type of expression does not match function return type\n"
            "expected:    " + fmt(astrtostr(type_manager.get_type(ret_type).name), BRIGHT_BLUE, UNDERLINE) + "\n"
            "instead got: " + fmt(astrtostr(type_manager.get_type(ret->expr->result_type).name), BRIGHT_BLUE, UNDERLINE)
            );
        error_log.exit(ErrorMessage{FATAL, ret->expr->pos, current_filename_, err});
    }
}

void TypeInference::analyze_decl(Decl* decl) {
    PROFILE();
    analyze_expr(decl->val);

    // if we have to infer the type
    if(decl->type == TYPE_UNKNOWN) {
        if(decl->val->result_type == TYPE_UNKNOWN) {
            error_log.exit(ErrorMessage{FATAL, decl->val->pos, current_filename_, "cannot infer type of expression"});
        }
        decl->type = decl->val->result_type;
    }
    // types don't match
    if (decl->type != decl->val->result_type) {
        error_log.exit(ErrorMessage{FATAL, decl->val->pos, current_filename_, "type of expression does not match type of declaration"});
    }

    s_table_.add_symbol(*(decl->id), VARIABLE, decl->type);

}

type_handle TypeInference::analyze_expr(Expr* expr) {
    PROFILE();
    switch(expr->type) {
        case EXPR_INT_LIT:
            expr->result_type = TYPE_I64;
            break;
        case EXPR_FLOAT_LIT:
            expr->result_type = TYPE_F64;
            break;
        case EXPR_STRING_LIT:
            break;
        case EXPR_ID:
        {
            auto type = s_table_.get_type(*(expr->id.val));

            if(type == TYPE_UNKNOWN) 
                error_log.exit(ErrorMessage{FATAL, expr->pos, current_filename_, "cannot infer type of expression"});
            
            expr->result_type = type;
            break;
        }
        case EXPR_BIN:
        {
            const auto lhs_type = analyze_expr(expr->binary_expr.left);
            const auto rhs_type = analyze_expr(expr->binary_expr.right);
            if(!type_manager.operator_exists(expr->binary_expr.op, lhs_type, rhs_type)) {
                // TODO error
                // operator between two types does not exist
            }
            expr->result_type = type_manager.get_operator_result_type(expr->binary_expr.op, lhs_type, rhs_type);
            break;
        }
        case EXPR_UNARY:
            break;
        default:
            break;
    }
    return expr->result_type;
}