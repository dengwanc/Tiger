#include "../utils/symbol.h"
#include "../utils/index.h"
#include "../lexical/index.h"
#include "index.h"

void yyerror(const char* msg)
{
    printf("@ line %d\n", lexical::getLine());
    error(msg);
}

namespace syntax {

    #define BASE(NAME) \
    static struct NAME* NAME##Base() \
    { \
        struct NAME* p = (struct NAME*)checkedMalloc(sizeof (struct NAME)); \
        setLocation(&p->lo); \
        return p; \
    }

    struct Expr *ABSYN_ROOT;

    static void setLocation(struct Location *lo) {
        lo->line = lexical::getLine();
        lo->offset = lexical::getOffset();
    }

    BASE(Lvalue);

    struct Lvalue *Lvalue4(Symbol sym) {
        struct Lvalue *p = LvalueBase();
        p->kind = Simple;
        p->u.simple = sym;
        return p;
    }

    struct Lvalue *Lvalue4(struct Lvalue *lv, Symbol sym) {
        struct Lvalue *p = LvalueBase();
        p->kind = Field;
        p->u.field.lv = lv;
        p->u.field.x = sym;
        return p;
    };

    struct Lvalue *Lvalue4(struct Lvalue *lv, struct Expr *expr) {
        struct Lvalue *p = LvalueBase();
        p->kind = Subscript;
        p->u.subscript.lv = lv;
        p->u.subscript.expr = expr;
        return p;
    }

    BASE(Expr);

    struct Expr *Expr4(struct Lvalue *lv) {
        struct Expr *p = ExprBase();
        p->kind = LvalueE;
        p->u.lv = lv;
        return p;
    }

    struct Expr *Expr4(int ival) {
        struct Expr *p = ExprBase();
        p->kind = IntE;
        p->u.ival = ival;
        return p;
    }

    struct Expr *Expr4(double dval) {
        struct Expr *p = ExprBase();
        p->kind = LvalueE;
        p->u.dval = dval;
        return p;
    }

    struct Expr *Expr4(char *s) {
        struct Expr *p = ExprBase();
        p->kind = StringE;
        p->u.sval = s;
        return p;
    }

    struct Expr *Expr4(Symbol func, struct ExprList *args) {
        struct Expr *p = ExprBase();
        p->kind = CallE;
        p->u.call.func = func;
        p->u.call.args = args;
        return p;
    }

    struct Expr *Expr4(Oper oper, struct Expr *left, struct Expr *right) {
        struct Expr *p = ExprBase();
        p->kind = OpE;
        p->u.op.oper = oper;
        p->u.op.left = left;
        p->u.op.right = right;
        return p;
    }

    struct Expr *Expr4(Symbol type, struct ValfieldList *valfields) {
        struct Expr *p = ExprBase();
        p->kind = RecordE;
        p->u.record.type = type;
        p->u.record.valfields = valfields;
        return p;
    }

    struct Expr *Expr4(struct ExprList *seq) {
        struct Expr *p = ExprBase();
        p->kind = seqE;
        p->u.seq = seq;
        return p;
    }

    struct Expr *Expr4(struct Lvalue *lv, struct Expr *expr) {
        struct Expr *p = ExprBase();
        p->kind = AssignE;
        p->u.assign.lv = lv;
        p->u.assign.expr = expr;
        return p;
    }

    struct Expr *Expr4(struct Expr *test, struct Expr *then, struct Expr *otherwise) {
        struct Expr *p = ExprBase();
        p->kind = QuestionE;
        p->u.question.test = test;
        p->u.question.then = then;
        p->u.question.otherwise = otherwise;
        return p;
    }

    struct Expr *Expr4(struct Expr *test, struct Expr *body) {
        struct Expr *p = ExprBase();
        p->kind = WhileE;
        p->u.whilee.test = test;
        p->u.whilee.body = body;
        return p;
    }

    struct Expr *Expr4(struct DeclareList *declares, struct ExprList *body) {
        struct Expr *p = ExprBase();
        p->kind = LetE;
        p->u.let.decs = declares;
        p->u.let.body = body;
        return p;
    }

    struct Expr *Expr4(Symbol type, struct Expr *size) {
        struct Expr *p = ExprBase();
        p->kind = ArrayE;
        p->u.array.type = type;
        p->u.array.size = size;
        return p;
    }

    struct Expr *OrExpr(struct Expr *left, struct Expr *right) {
        struct Expr *p = ExprBase();
        p->kind = QuestionE;
        p->u.question.test = left;
        p->u.question.then = Expr4(1);
        p->u.question.otherwise = right;
        return p;
    }

    struct Expr *AndExpr(struct Expr *left, struct Expr *right) {
        struct Expr *p = ExprBase();
        p->kind = QuestionE;
        p->u.question.test = left;
        p->u.question.then = right;
        p->u.question.otherwise = Expr4(0);
        return p;
    }

    struct Expr *MinusExpr(struct Expr *expr) {
        struct Expr *p = ExprBase();
        p->kind = OpE;
        p->u.op.oper = Minus;
        p->u.op.left = Expr4(0);
        p->u.op.right = expr;
        return p;
    }

    struct Expr *NilExpr() {
        struct Expr *p = ExprBase();
        p->kind = NilE;
        return p;
    }

    struct Expr *BreakExpr() {
        struct Expr *p = ExprBase();
        p->kind = BreakE;
        return p;
    }

    struct ExprList *ExprList4(struct Expr *head, struct ExprList *tail) {
        struct ExprList *p = (struct ExprList *) checkedMalloc(sizeof(struct ExprList));
        p->head = head;
        p->tail = tail;
        return p;
    }

    BASE(Declare);

    struct Declare *Declare4(struct Funcdec *function) {
        struct Declare *p = DeclareBase();
        p->kind = Function;
        p->u.function = function;
        return p;
    }

    struct Declare *Declare4(Symbol id, Symbol type, struct Expr *init) {
        struct Declare *p = DeclareBase();
        p->kind = Var;
        p->u.var.id = id;
        p->u.var.type = type;
        p->u.var.init = init;
        return p;
    }

    struct Declare *Declare4(Symbol name, struct Type *def) {
        struct Declare *p = DeclareBase();
        p->kind = Type;
        p->u.type.name = name;
        p->u.type.def = def;
        return p;
    }

    struct DeclareList *DeclareList4(struct Declare *head, struct DeclareList *tail) {
        struct DeclareList *p = (struct DeclareList *) checkedMalloc(sizeof(struct DeclareList));
        p->head = head;
        p->tail = tail;
        return p;
    }

    BASE(Type);

    struct Type *Type4(Symbol name) {
        struct Type *p = TypeBase();
        p->kind = NameT;
        p->u.name = name;
        return p;
    }

    struct Type *Type4(struct TypefieldList *record) {
        struct Type *p = TypeBase();
        p->kind = RecordT;
        p->u.record = record;
        return p;
    }

    struct Type *ArrayType(Symbol array) {
        struct Type *p = TypeBase();
        p->kind = ArrayT;
        p->u.array = array;
        return p;
    }

    struct Typefield *Typefield4(Symbol name, Symbol type) {
        struct Typefield *p = (struct Typefield *) checkedMalloc(sizeof(struct Typefield));
        p->name = name;
        p->type = type;
        return p;
    }

    struct TypefieldList *TypefieldList4(Symbol name, Symbol type, struct TypefieldList *tail) {
        struct Typefield *head = Typefield4(name, type);
        struct TypefieldList *p = (struct TypefieldList *) checkedMalloc(sizeof(struct TypefieldList));
        p->head = head;
        p->tail = tail;
        return p;
    }

    struct Funcdec *Funcdec4(Symbol name, struct TypefieldList *params, Symbol result, struct Expr *body) {
        struct Funcdec *p = (struct Funcdec *) checkedMalloc(sizeof(struct Funcdec));
        p->name = name;
        p->params = params;
        p->result = result;
        p->body = body;
        return p;
    }

    struct Valfield *Valfield4(Symbol name, struct Expr *expr) {
        struct Valfield *p = (struct Valfield *) checkedMalloc(sizeof(struct Valfield));
        p->name = name;
        p->expr = expr;
        return p;
    }

    struct ValfieldList *ValfieldList4(Symbol name, struct Expr *expr, struct ValfieldList *tail) {
        struct Valfield *head = Valfield4(name, expr);
        struct ValfieldList *p = (struct ValfieldList *) checkedMalloc(sizeof(struct ValfieldList));
        p->head = head;
        p->tail = tail;
        return p;
    }
}

