#ifndef TIGER_SYNTAX
#define TIGER_SYNTAX

#include "../utils/symbol.h"

void yyerror(const char *);

namespace syntax {
    extern struct Expr* ABSYN_ROOT;

    typedef enum { Plus, Minus, Times, Divide, Eq, Neq, Lt, Le, Gt, Ge } Oper;
    typedef enum { Simple, Field, Subscript } LvalueK;
    typedef enum {
        LvalueE, NilE, IntE, DoubleE, StringE,
        OpE, RecordE, seqE, AssignE, QuestionE, CallE,
        WhileE, BreakE, LetE, ArrayE, RealE
    } ExprK;
    typedef enum { Function, Var, Type } DeclareK;
    typedef enum { NameT, RecordT, ArrayT } TypeK;

    struct Location {
        int line;
        int offset;
    };

    struct Lvalue {
        LvalueK kind;

        struct Location lo;

        union {
            Symbol simple;

            struct {
                struct Lvalue* lv;
                Symbol x;
            } field;

            struct {
                struct Lvalue* lv;
                struct Expr* expr;
            } subscript;
        } u;
    };

    struct Expr {
        ExprK kind;

        struct Location lo;

        union {
            struct Lvalue* lv;

            int ival;

            double dval;

            char* sval;

            struct {
                Symbol func;
                struct ExprList* args;
            } call;

            struct {
                Oper oper;
                struct Expr* left;
                struct Expr* right;
            } op;

            struct {
                Symbol type;
                struct ValfieldList* valfields;
            } record;

            struct ExprList* seq;

            struct {
                struct Lvalue* lv;
                struct Expr* expr;
            } assign;

            struct {
                struct Expr* test;
                struct Expr* then;
                struct Expr* otherwise;
            } question;

            struct {
                struct Expr* test;
                struct Expr* body;
            } whilee;

            struct {
                struct DeclareList* decs;
                struct ExprList* body;
            } let;

            struct {
                Symbol type;
                struct Expr* size;
            } array;
        } u;
    };
    struct Declare {
        DeclareK kind;

        struct Location lo;

        union {
            struct Funcdec* function;

            struct {
                Symbol id;
                Symbol type;
                struct Expr* init;
            } var;

            struct {
                Symbol name;
                struct Type* def;
            } type;
        } u;
    };
    struct Type {
        TypeK kind;

        struct Location lo;

        union {
            Symbol name;
            struct TypefieldList* record;
            Symbol array;
        } u;
    };
    struct Typefield {
        Symbol name;
        Symbol type;
    };

    struct TypefieldList {
        struct Typefield* head;
        struct TypefieldList* tail;
    };

    struct ExprList {
        struct Expr* head;
        struct ExprList* tail;
    };

    struct Funcdec {
        struct Location lo;

        Symbol name;
        struct TypefieldList* params;
        Symbol result;
        struct Expr* body;
    };

    struct DeclareList {
        struct Declare* head;
        struct DeclareList* tail;
    };

    struct Valfield {
        Symbol name;
        struct Expr* expr;
    };
    struct ValfieldList {
        struct Valfield* head;
        struct ValfieldList* tail;
    };
    struct Lvalue *Lvalue4(Symbol sym);
    struct Lvalue *Lvalue4(struct Lvalue* lv, Symbol sym);
    struct Lvalue *Lvalue4(struct Lvalue* lv, struct Expr* expr);

    struct Expr* Expr4(struct Lvalue* lv);
    struct Expr* Expr4(int ival);
    struct Expr* Expr4(double dval);
    struct Expr* Expr4(char* s);
    struct Expr* Expr4(Symbol func, struct ExprList* args);
    struct Expr* Expr4(Oper oper, struct Expr* left, struct Expr* right);
    struct Expr* Expr4(Symbol type, struct ValfieldList* valfields);
    struct Expr* Expr4(struct ExprList* seq);
    struct Expr* Expr4(struct Lvalue* lv, struct Expr* expr);
    struct Expr* Expr4(struct Expr* test, struct Expr* then, struct Expr* otherwise);
    struct Expr* Expr4(struct Expr* test, struct Expr* body);
    struct Expr* Expr4(struct DeclareList* declares, struct ExprList* body);
    struct Expr* Expr4(Symbol type, struct Expr* size);
    struct Expr* OrExpr(struct Expr* left, struct Expr* right);
    struct Expr* AndExpr(struct Expr* left, struct Expr* right);
    struct Expr* MinusExpr(struct Expr* expr);
    struct Expr* NilExpr();
    struct Expr* BreakExpr();
    struct ExprList* ExprList4(struct Expr* head, struct ExprList* tail);

    struct Declare* Declare4(struct Funcdec* function);
    struct Declare* Declare4(Symbol id, Symbol type, struct Expr* init);
    struct Declare* Declare4(Symbol name, struct Type* type);
    struct DeclareList* DeclareList4(struct Declare* head, struct DeclareList* tail);

    struct Type* Type4(Symbol name);
    struct Type* Type4(struct TypefieldList* record);
    struct Type* ArrayType(Symbol array);

    struct Typefield* Typefield4(Symbol name, Symbol type);
    struct TypefieldList* TypefieldList4(Symbol name, Symbol type, struct TypefieldList* tail);

    struct Funcdec* Funcdec4(Symbol name, struct TypefieldList* params, Symbol result, struct Expr* body);

    struct Valfield* Valfield4(Symbol name, struct Expr* expr);
    struct ValfieldList* ValfieldList4(Symbol name, struct Expr* expr, struct ValfieldList* tail);
}

#endif