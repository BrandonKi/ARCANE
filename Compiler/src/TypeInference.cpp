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
    for (auto* file : project->files) {
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

void TypeInference::update_symbol_table(std::vector<Arg>& fn_args) {
    for(const auto& arg : fn_args)
        s_table_.add_symbol(arg.id, VARIABLE, arg.type);
}

void TypeInference::analyze_function(Function* function) {
    PROFILE();
    update_symbol_table(function->args);
    analyze_block(function->body, function->return_type);
}

void TypeInference::analyze_block(Block* block, type_handle ret_type) {
    for (auto* stmnt : block->statements) {
        switch(stmnt->type) {
            case WHILE:
                break;
            case FOR:
                break;
            case IF:
                analyze_if(stmnt->if_stmnt, ret_type);
                break;
            case RET:
                analyze_ret(stmnt->ret_stmnt, ret_type);
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

void TypeInference::analyze_if(IfStmnt* if_stmnt, type_handle ret_type) {
    PROFILE();
    analyze_expr(if_stmnt->expr);
    analyze_block(if_stmnt->block, ret_type);
    if(if_stmnt->else_stmnt)
        analyze_block(if_stmnt->else_stmnt, ret_type);
}

void TypeInference::analyze_ret(RetStmnt* ret_stmnt, type_handle ret_type) {
    PROFILE();
    analyze_expr(ret_stmnt->expr);

    if(ret_stmnt->expr->result_type != ret_type && !type_manager.conversion_exists(ret_stmnt->expr->result_type, ret_type)) {
        auto err =
            "type of expression does not match function return type and could not find a valid conversion\n"
            "expected:    " + fmt(type_manager.get_type(ret_type).name, BRIGHT_BLUE, UNDERLINE) + "\n"
            "instead got: " + fmt(type_manager.get_type(ret_stmnt->expr->result_type).name, BRIGHT_BLUE, UNDERLINE);
        error_log.exit(ErrorMessage{FATAL, ret_stmnt->expr->pos, current_filename_, err});
    }
    else if(args.warning.implicitConversion &&
            ret_stmnt->expr->result_type != ret_type &&
            type_manager.conversion_exists(ret_stmnt->expr->result_type, ret_type)) {
        auto err =
            "implicit conversion from " + fmt(type_manager.get_type(ret_stmnt->expr->result_type).name, BRIGHT_BLUE, UNDERLINE) +
            " to " +
            fmt(type_manager.get_type(ret_type).name, BRIGHT_BLUE, UNDERLINE);

        error_log.push(ErrorMessage{WARN, ret_stmnt->expr->pos, current_filename_, err});
    }
}

void TypeInference::analyze_decl(Decl* decl) {
    PROFILE();
    analyze_expr(decl->val);

    // if we have to infer the type
    if(decl->type == TYPE_unknown) {
        if(decl->val->result_type == TYPE_unknown) {
            error_log.exit(ErrorMessage{FATAL, decl->val->pos, current_filename_, "cannot infer type of expression"});
        }
        decl->type = decl->val->result_type;
    }
    // types don't match
    if (decl->type != decl->val->result_type && !type_manager.conversion_exists(decl->type, decl->val->result_type)) {
        error_log.exit(ErrorMessage{FATAL, decl->val->pos, current_filename_, "type of expression does not match type of declaration"});
    }

    s_table_.add_symbol(*(decl->id), VARIABLE, decl->type);
}

type_handle TypeInference::analyze_expr(Expr* expr) {
    PROFILE();
    switch(expr->type) {
        case EXPR_INT_LIT:
            expr->result_type = calc_literal_size(expr->int_literal.val);
            //expr->result_type = TYPE_I64;    // TODO calculate min size of literal
            break;
        case EXPR_FLOAT_LIT:
            expr->result_type = TYPE_f64;
            break;
        case EXPR_STRING_LIT:
            break;
        case EXPR_ID:
        {
            auto type = s_table_.get_type(*(expr->id.val));

            if(type == TYPE_unknown)
                error_log.exit(ErrorMessage{FATAL, expr->pos, current_filename_, "cannot infer type of expression"});

            expr->result_type = type;
            break;
        }
        case EXPR_BIN:
        {
            const auto lhs_type = analyze_expr(expr->binary_expr.left);
            const auto rhs_type = analyze_expr(expr->binary_expr.right);

            if(!type_manager.operator_exists(expr->binary_expr.op, lhs_type, rhs_type)) {
                // construct error message
                auto err =
                    "operator does not exist between these types (" +
                    fmt(type_manager.get_type(lhs_type).name, BRIGHT_BLUE, UNDERLINE) + " " +
                    get_string(expr->binary_expr.op) + " " +
                    fmt(type_manager.get_type(rhs_type).name, BRIGHT_BLUE, UNDERLINE) + ")";
                auto left_expr = expr->binary_expr.left;
                auto right_expr = expr->binary_expr.right;

                auto pos = SourcePos{left_expr->pos.src_line, left_expr->pos.src_char, left_expr->pos.start_pos, right_expr->pos.end_pos};
                error_log.exit(ErrorMessage{FATAL, pos, current_filename_, err});
            }
            expr->result_type = type_manager.get_operator_result_type(expr->binary_expr.op, lhs_type, rhs_type);
            break;
        }
        case EXPR_UNARY:
        {
            const auto expr_type = analyze_expr(expr->unary_expr.expr);
            //TODO error message
            expr->result_type = expr_type;    // TODO get the correct result type see above ^^
            break;
        }
        default:
            break;
    }
    return expr->result_type;
}

// TODO fix issues with compatibility between unsigned and signed types
// specifically, the top bit is not taken into acount at the moment
type_handle TypeInference::calc_literal_size(u64 literal) {
    auto needed_space = sizeof(u64) * 8 - std::countl_zero(literal);
    if(needed_space <= 7) // disregard top bit for now
        return TYPE_i8;
    else if(needed_space <= 15)
        return TYPE_i16;
    else if(needed_space <= 31)
        return TYPE_i32;
    else if(needed_space <= 63)
        return TYPE_i64;
    else
        assert(false);    // literals that require unsigned are not implemented yet
}