// get rid of all TYPE_UNKNOWN and check all operations are between compatible types
// every function with the "analyze" prefix will perform full semeantic analysis and type inference
// this means any of these functions will/can change the state of the tree
#include "AST.h"

class TypeInference {

public:
    TypeInference(Project*);

    void start();

private:
    Project* ast_;

    void analyze_project(Project*);
    void analyze_file(File*);
    void analyze_function(Function*);
    void analyze_decl(Decl*);
    type_handle analyze_expr(Expr*);
};