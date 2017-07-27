#ifndef TIGER_SYNTAX
#define TIGER_SYNTAX

#include "../utils/symbol.h"

namespace ast {
    typedef enum { Plus, Minus, Times, Divide, Eq, Neq, Lt, Le, Gt, Ge } Oper;

    class Lvalue {
    public:
//        virtual void semantic() = 0;
        virtual void print() = 0;
    };

    class Expr {
    public:
//        virtual void semantic() = 0;
        virtual void print() = 0;
    };

    class Declare {
    public:
//        virtual void semantic() = 0;
        virtual void print() = 0;
    };

    class Type {
    public:
//        virtual void semantic() = 0;
        virtual void print() = 0;
    };

    struct Location {
        int line;
        int offset;
    };

    class SimpleLvalue: public Lvalue {
        struct Location lo;
        Symbol simple;

    public:
        SimpleLvalue(Symbol sym);
        void print();
    };

    class FieldLvalue: public Lvalue {
        struct Location lo;
        struct Lvalue* lv;
        Symbol x;

    public:
        FieldLvalue(Lvalue* lv, Symbol sym);
        void print();
    };

    class SubscriptLvalue: public Lvalue {
        struct Location lo;
        struct Lvalue* lv;
        struct Expr* expr;

    public:
        SubscriptLvalue(Lvalue* lv, Expr* expr);
        void print();
    };

    class LvalueExpr: public Expr {
        struct Location lo;
        struct Lvalue* lv;

    public:
        LvalueExpr(Lvalue* lv);
        void print();
    };

    class IntExpr: public Expr {
        struct Location lo;
        int ival;

    public:
        IntExpr(int ival);
        void print();
    };

    class RealExpr: public Expr {
        struct Location lo;
        double dval;

    public:
        RealExpr(double dval);
        void print();
    };

    class StringExpr: public Expr {
        struct Location lo;
        char* sval;

    public:
        StringExpr(char* sval);
        void print();
    };

    class CallExpr: public Expr {
        struct Location lo;
        Symbol func;
        struct ExprList* args;

    public:
        CallExpr(Symbol func, struct ExprList* args);
        void print();
    };

    class OpExpr: public Expr {
        struct Location lo;
        Oper oper;
        Expr* left;
        Expr* right;

    public:
        OpExpr(Oper oper, Expr* left, Expr* right);
        void print();
    };

    class RecordExpr: public Expr {
        struct Location lo;
        Symbol type;
        struct ValfieldList* valfields;

    public:
        RecordExpr(Symbol type, struct ValfieldList* valfields);
        void print();
    };

    class ArrayExpr: public Expr {
        struct Location lo;
        Symbol type;
        Expr* size;

    public:
        ArrayExpr(Symbol t, Expr* s);
        void print();
    };

    class SeqExpr: public Expr {
        struct Location lo;
        struct ExprList* seq;

    public:
        SeqExpr(struct ExprList* seq);
        void print();
    };

    class AssignExpr: public Expr {
        struct Location lo;
        Lvalue *lv;
        Expr *expr;

    public:
        AssignExpr(Lvalue *lv, Expr *expr);
        void print();
    };

    class IfExpr: public Expr {
        struct Location lo;
        Expr *test;
        struct ExprList *then;
        struct ExprList *otherwise;

    public:
        IfExpr(Expr *test, struct ExprList *then, struct ExprList *otherwise);
        void print();
    };

    class WhileExpr: public Expr {
        struct Location lo;
        Expr* test;
        struct ExprList *body;

    public:
        WhileExpr(Expr *test, struct ExprList *body);
        void print();
    };

    class LetExpr: public Expr {
        struct Location lo;
        struct DeclareList* declares;
        struct ExprList* body;

    public:
        LetExpr(struct DeclareList* declares, struct ExprList* body);
        void print();
    };

    Expr* OrExpr(Expr* left, Expr* right);
    Expr* AndExpr(Expr* left, Expr* right);
    Expr* MinusExpr(Expr* expr);

    class NilExpr: public Expr {
        struct Location lo;
        Symbol sym;

    public:
        NilExpr();
        void print();
    };

    class BreakExpr: public Expr {
        struct Location lo;

    public:
        BreakExpr();
        void print();
    };

    class FunctionDeclare: public Declare {
        struct Location lo;
        Symbol name;
        struct TypefieldList* params;
        Symbol result;
        Expr* body;

    public:
        FunctionDeclare(Symbol name, struct TypefieldList* params, Symbol result, Expr* body);
        void print();
    };

    class VarDeclare: public Declare {
        struct Location lo;
        Symbol id;
        Symbol type;
        Expr *init;

    public:
        VarDeclare(Symbol id, Symbol type, Expr* init);
        void print();
    };

    class TypeDeclare: public Declare {
        struct Location lo;
        Symbol name;
        Type* def;

    public:
        TypeDeclare(Symbol name, struct Type* def);
        void print();
    };

    class NameType: public Type {
        struct Location lo;
        Symbol name;

    public:
        NameType(Symbol name);
        void print();
    };

    class RecordType: public Type {
        struct Location lo;
        struct TypefieldList *record;

    public:
        RecordType(struct TypefieldList *record);
        void print();
    };

    class ArrayType: public Type {
        struct Location lo;
        Symbol array;

    public:
        ArrayType(Symbol array);
        void print();
    };

    extern Expr* AST_ROOT;

    struct Typefield {
        Symbol name;
        Symbol type;
    };

    struct TypefieldList {
        struct Typefield* head;
        struct TypefieldList* tail;
    };

    struct ExprList {
        Expr* head;
        struct ExprList* tail;
    };

    struct DeclareList {
        Declare* head;
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

    struct ExprList* ExprList4(Expr* head, struct ExprList* tail);
    struct DeclareList* DeclareList4(Declare* head, struct DeclareList* tail);
    struct TypefieldList* TypefieldList4(Symbol name, Symbol type, struct TypefieldList* tail);
    struct ValfieldList* ValfieldList4(Symbol name, Expr* expr, struct ValfieldList* tail);
}

#endif