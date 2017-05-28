#include<string>
#include "interpreter.h"
#include "../src/utils/index.h"
#include "../src/errormsg.h"

Statement::Statement(Statement* stm1, Statement* stm2)
{
	if (stm1 == nullptr) throw NULLPTR;
	if (stm2 == nullptr) throw NULLPTR;

	kind = CompoundStm;
	stm1 = stm1;
  	stm2 = stm2;
}

Statement::Statement(const char* id, Expression* expr)
{
	if (expr == nullptr) throw NULLPTR;

	u.assign.id = String(id);
	u.assign.expr = expr;
}

Statement::Statement(ExprList* exprs) {
	if (exprs == nullptr) throw NULLPTR;

	u.print.exprs = exprs;
}
