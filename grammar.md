precedence:
    expr_eight: [] () a()
    expr_seven: ! ++ --
    expr_six: << >> & |
    expr_five: * / %
    expr_four: + -
    expr_three: == != <= >= > < >
    expr_two: && ||
    expr_one: = += -= *= /= >>= <<= &= |= ^=
    expr: ?:



var = {(KEYWORD NAME '=' expr) ('\n' | ';')}

expr_five = {(expr_six) | (expr_six BIN_MUL_SUB expr_six)}
expr_four = {(expr_five) | (expr_five BIN_ADD_SUB expr_five)}
expr_three = {(expr_four) | (expr_four ASSIGNMENT_OP expr_four)}
expr_two = {(expr_three) | (expr_three LOGICAL_AND_OR expr_three)}
expr_one = {(expr_two) | (NAME ASSIGN_OP expr_two)}
expr = {(expr_one) | (expr_one '?' expr_one ':' expr_one)}