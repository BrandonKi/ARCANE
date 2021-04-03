#include "BytecodeGen.h"

using vm = Arcvm;

BytecodeGen::BytecodeGen(Project *ast):
	ast_(ast), code_()
{

}

BytecodeGen::~BytecodeGen() {

}

std::vector<u8, arena_allocator<u8>> BytecodeGen::genCode() {
	genProject(ast_);
	return code_;
}

void BytecodeGen::genProject(Project *project) {
	for(const auto* file : project->files) {
		genFile(file);
	}
}

void BytecodeGen::genFile(const File *file) {
	
	if(file->isMain) {
		generateBootstrap();
	}

	for(const auto *import : file->imports) {
		genImport(import);
	}
	
	for (const auto *decl : file->decls) {
		genDecl(decl);
	}
	
	for (const auto *function : file->functions) {
		genFunction(function);
	}


}

void BytecodeGen::genImport(const Import *import) {
	//TODO implement importing files/modules
}

void BytecodeGen::genFunction(const Function *function) {
	// do something with args and return type
	genBlock(function->body);
}

void BytecodeGen::genBlock(const Block *block) {
	for(const auto *statement : block->statements) {
		genStatement(statement);
	}
}

void BytecodeGen::genStatement(const Statement *statement) {
	switch(statement->type) {
		case WHILE:
			genWhile(statement->while_);
			break;
		case FOR:
			genFor(statement->for_);
			break;
		case IF:
			genIf(statement->if_);
			break;
		case RET:
			genRet(statement->ret);
			break;
		case EXPRESSION:
			genExpr(statement->expr);
			break;
		case DECLARATION:
			genDecl(statement->decl);
			break;
	}
}

void BytecodeGen::genWhile(const While_* w) {
	//TODO implement this
	genExpr(w->expr);
	genBlock(w->block);
}

void BytecodeGen::genFor(const For_* f) {
	//TODO implement this
	genDecl(f->decl);
	genExpr(f->expr1);
	genExpr(f->expr2);
	genBlock(f->block);
}

void BytecodeGen::genIf(const If_* i) {
	//TODO implement this
	genExpr(i->expr);
	genBlock(i->block);
}

void BytecodeGen::genRet(const Ret* r) {
	//TODO implement this
	genExpr(r->expr);
	push(vm::ret);
}

void BytecodeGen::genDecl(const Decl *d) {
	//TODO implement this
	genExpr(d->val);
}

void BytecodeGen::genExpr(const Expr *e) {
	//TODO implement this
	switch(e->type) {
		case EXPR_INT_LIT:
			genIntLit(e->intLiteral.val);
			break;
		case EXPR_FLOAT_LIT:
			genFloatLit(e->floatLiteral.val);
			break;
		case EXPR_STRING_LIT:
			genStringLit(e->stringLiteral.val);
			break;
		case EXPR_ID:
			genID(e->stringLiteral.val);
			break;
		case EXPR_BIN:
			genBin(e);
			break;
		case EXPR_UNARY:
			genUnary(e);
			break;
	}
}

void BytecodeGen::genIntLit(const u64 val) {
	push64BitValue(val);
}

void BytecodeGen::genFloatLit(const f64 val) {
	
}

void BytecodeGen::genStringLit(const astring *val) {
	
}

void BytecodeGen::genID(const astring* id) {
	
}

void BytecodeGen::genBin(const Expr *expr) {
	switch(expr->binaryExpr.op) {
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
			genExpr(expr->binaryExpr.left);
			genExpr(expr->binaryExpr.right);
			push(vm::addu);
			break;
		case ARC_SUB:
			break;
		case ARC_DIV:
			break;
		case ARC_MUL:
			break;
		case ARC_MOD:
			break;
		case ARC_BIN_OR:
			break;
		case ARC_BIN_AND:
			break;
		case ARC_NOT:
			break;
		case ARC_LEFT_SHIFT:
			break;
		case ARC_RIGHT_SHIFT:
			break;
		case ARC_XOR:
			break;
		case ARC_LESSER:
			break;
		case ARC_GREATER:
			break;
		case ARC_NEGATE:
			break;
		case ARC_LOGICAL_OR:
			break;
		case ARC_LOGICAL_AND:
			break;
		case ARC_PRE_INCREMENT:
			break;
		case ARC_POST_INCREMENT:
			break;
		case ARC_PRE_DECREMENT:
			break;
		case ARC_POST_DECREMENT:
			break;
		
	}
}

void BytecodeGen::genUnary(const Expr* expr) {
	
}

void BytecodeGen::push(const u8 inst) {
	code_.push_back(inst);
}

void BytecodeGen::push(const std::vector<u8, arena_allocator<u8>>& vec) {
	code_.insert(code_.end(), vec.begin(), vec.end());
}

void BytecodeGen::push64BitValue(const u64 val) {
	push(vm::push_value);
	code_.push_back(static_cast<u8>(val));
	code_.push_back(static_cast<u8>(val >> 8));
	code_.push_back(static_cast<u8>(val >> 16));
	code_.push_back(static_cast<u8>(val >> 24));
	code_.push_back(static_cast<u8>(val >> 32));
	code_.push_back(static_cast<u8>(val >> 40));
	code_.push_back(static_cast<u8>(val >> 48));
	code_.push_back(static_cast<u8>(val >> 56));
}

void BytecodeGen::generateBootstrap() {

	std::vector<u8, arena_allocator<u8>> vec = {
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