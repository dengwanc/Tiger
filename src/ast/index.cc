#include "index.h"
#include "../utils/index.h"

/**
 * This module for
 * AST constructors
 */

namespace ast {
    Expr* AST_ROOT;

    static void setLocation(struct Location *lo) {
        lo->line = lexical::getLine();
        lo->offset = lexical::getOffset();
    }

    SimpleLvalue::SimpleLvalue(Symbol s): simple(s)
    {
        setLocation(&this->lo);
    }

    FieldLvalue::FieldLvalue(Lvalue *l, Symbol s): lv(l), x(s)
    {
        setLocation(&this->lo);
    }

    SubscriptLvalue::SubscriptLvalue(Lvalue *l, Expr *e): lv(l), expr(e)
    {
        setLocation(&this->lo);
    }

    LvalueExpr::LvalueExpr(Lvalue *l): lv(l)
    {
        setLocation(&this->lo);
    }

    IntExpr::IntExpr(int i): ival(i)
    {
        setLocation(&this->lo);
    }

    RealExpr::RealExpr(double d): dval(d)
    {
        setLocation(&this->lo);
    }

    StringExpr::StringExpr(char *s): sval(s)
    {
        setLocation(&this->lo);
    }

    CallExpr::CallExpr(Symbol f, struct ExprList* a): func(f), args(a)
    {
        setLocation(&this->lo);
    }

    OpExpr::OpExpr(Oper o, Expr *l, Expr *r): oper(o), left(l), right(r)
    {
        setLocation(&this->lo);
    }

    RecordExpr::RecordExpr(Symbol t, struct ValfieldList *v): type(t), valfields(v)
    {
        setLocation(&this->lo);
    }

    ArrayExpr::ArrayExpr(Symbol t, Expr *s): type(t), size(s)
    {
        setLocation(&this->lo);
    }

    SeqExpr::SeqExpr(struct ExprList *q): seq(q)
    {
        setLocation(&this->lo);
    }

    AssignExpr::AssignExpr(Lvalue *l, Expr *e): lv(l), expr(e)
    {
        setLocation(&this->lo);
    }

    IfExpr::IfExpr(Expr *t, struct ExprList *th, struct ExprList *o):
    test(t), then(th), otherwise(o)
    {
        setLocation(&this->lo);
    }

    WhileExpr::WhileExpr(Expr *t, struct ExprList *b): test(t), body(b)
    {
        setLocation(&this->lo);
    }

    LetExpr::LetExpr(struct DeclareList *d, struct ExprList *b): declares(d), body(b)
    {
        setLocation(&this->lo);
    }

    Expr* OrExpr(Expr* left, Expr* right)
    {
        auto then = ExprList4(new IntExpr(1), nullptr);
        auto otherwise = ExprList4(right, nullptr);
        return new IfExpr(left, then, otherwise);
    }

    Expr* AndExpr(Expr* left, Expr* right)
    {
        auto then = ExprList4(right, nullptr);
        auto otherwise = ExprList4(new IntExpr(0), nullptr);
        return new IfExpr(left, then, otherwise);
    }

    Expr* MinusExpr(Expr* expr)
    {
        auto left = new IntExpr(0);
        return new OpExpr(Minus, left, expr);
    }

    NilExpr::NilExpr()
    {
        setLocation(&this->lo);
    }

    BreakExpr::BreakExpr()
    {
        setLocation(&this->lo);
    }

    FunctionDeclare::FunctionDeclare(Symbol n, struct TypefieldList *p, Symbol r, Expr *b):
    name(n), params(p), result(r), body(b)
    {
        setLocation(&this->lo);
    }

    VarDeclare::VarDeclare(Symbol i, Symbol t, Expr *ini):
    id(i), type(t), init(ini)
    {
        setLocation(&this->lo);
    }

    TypeDeclare::TypeDeclare(Symbol n, struct Type *d):name(_name), def(_def)
    {
        this->_name = n;
        this->_def = d;
        setLocation(&this->lo);
    }

    NameType::NameType(Symbol n): name(n)
    {
        setLocation(&this->lo);
    }

    RecordType::RecordType(struct TypefieldList *r): record(r)
    {
        setLocation(&this->lo);
    }

    ArrayType::ArrayType(Symbol a): array(a)
    {
        setLocation(&this->lo);
    }

    struct ExprList *ExprList4(Expr *head, struct ExprList *tail) {
        struct ExprList *p = (struct ExprList*) checkedMalloc(sizeof(struct ExprList));
        p->head = head;
        p->tail = tail;
        return p;
    }

    struct DeclareList *DeclareList4(Declare *head, struct DeclareList *tail) {
        struct DeclareList *p = (struct DeclareList *) checkedMalloc(sizeof(struct DeclareList));
        p->head = head;
        p->tail = tail;
        return p;
    }

    struct Typefield* Typefield4(Symbol name, Symbol type) {
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

