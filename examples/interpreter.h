enum binop { plus, minus, times, div };
enum type {
	CompoundStm,
	AssignStm,
	PrintStm,
	IdExp,
	NumExp,
	OpExp,
	EseqExp,
	PairExpList,
	LastExpList
};

class Expression;
class ExprList;

class Statement {
	private:
	enum type kind;
	union U {
		struct {Statement* stm1; Statement* stm2;} compound;
		struct {char* id; Expression* expr;} assign;
		struct {ExprList* exprs;} print;
	} u;

	public:
	Statement();
	Statement(Statement* stm1, Statement* stm2);
	Statement(const char* id, Expression* expr);
	Statement(ExprList* exprs);
};

class ExprList {
	enum type kind;
	union {
		struct {Expression* head; ExprList* tail;} pair;
		Expression* last;
	} u;

	ExprList(Expression* head, ExprList tail);
	ExprList(Expression* last);
};

class Expression {
	enum type kind;
	union {
		char* id;
		int num;
		struct {Expression* left; binop oper; Expression* right;} op;
		struct {Statement* stm; Expression* expr;} eseq;
	} u;

	Expression(char* id);
	Expression(int num);
	Expression(Expression* left, binop oper, Expression* right);
	Expression(Statement* stm, Expression* expr);
};
