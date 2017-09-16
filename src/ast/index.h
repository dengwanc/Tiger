#ifndef TIGER_AST_H
#define TIGER_AST_H

#include "../utils/symbol.h"
#include "../utils/binary-tree.h"
#include "../lexical/index.h"
#include "../semantic/type.h"

namespace ast {
    typedef enum { Plus, Minus, Times, Divide, Eq, Neq, Lt, Le, Gt, Ge } Oper;
    typedef enum { FunctionDK, VarDK, TypeDK, NotDK } DeclareKind;
}

/** sementic check result */
namespace ast {
    class SemanticResult {
    public:
        BinaryTree* val_table;
        BinaryTree* typ_table;
        ActualType* type; // the type of expression
        SemanticResult(BinaryTree* v, BinaryTree* t, ActualType* a);
        SemanticResult* copy(ActualType* t);
        SemanticResult* copy();
    };

    SemanticResult* makeBaseEnvTable();
}

/** base define solve circular reference */
namespace ast {
    class Lvalue {
    public:
        virtual void print() = 0;
        // todo
        virtual char* stringify() { return nullptr; };
        virtual SemanticResult* semantic(SemanticResult* &env) = 0;
    };

    class Expr {
    public:
        virtual void print() = 0;
        // todo
        virtual char* stringify() { return nullptr; };
        virtual SemanticResult* semantic(SemanticResult* &env) = 0;
    };

    class Type {
    public:
        virtual void print() = 0;
        virtual ActualType* pure(SemanticResult*& env, struct DeclareList* decs) = 0;
    };

    class Declare {
    public:
        virtual void print() = 0;
        virtual DeclareKind getKind() = 0;
        virtual SemanticResult* preprocess(SemanticResult* &env) = 0;
        virtual SemanticResult* semantic(SemanticResult* &env, struct DeclareList* decs) = 0;
    };
}

/** left value */
namespace ast {
    class SimpleLvalue : public Lvalue {
        struct Location lo;
        Symbol simple;

    public:
        SimpleLvalue(Symbol sym);
        void print();
        SemanticResult *semantic(SemanticResult *&env);
    };

    class FieldLvalue : public Lvalue {
        struct Location lo;
        struct Lvalue *lv;
        Symbol x;

    public:
        FieldLvalue(Lvalue *lv, Symbol sym);
        void print();
        SemanticResult *semantic(SemanticResult *&env);
    };

    class SubscriptLvalue : public Lvalue {
        struct Location lo;
        struct Lvalue *lv;
        Expr *expr;

    public:
        SubscriptLvalue(Lvalue *lv, Expr *expr);
        void print();
        SemanticResult *semantic(SemanticResult *&env);
    };
}

/** expressions */
namespace ast {
    class LvalueExpr : public Expr {
        struct Location lo;
        struct Lvalue *lv;

    public:
        LvalueExpr(Lvalue *lv);
        void print();
        SemanticResult *semantic(SemanticResult *&env);
    };

    class IntExpr : public Expr {
        struct Location lo;
        int ival;

    public:
        IntExpr(int ival);
        void print();
        SemanticResult *semantic(SemanticResult *&env);
    };

    class RealExpr : public Expr {
        struct Location lo;
        double dval;

    public:
        RealExpr(double dval);
        void print();
        SemanticResult *semantic(SemanticResult *&env);
    };

    class StringExpr : public Expr {
        struct Location lo;
        char *sval;

    public:
        StringExpr(char *sval);
        void print();
        SemanticResult *semantic(SemanticResult *&env);
    };

    class CallExpr : public Expr {
        struct Location lo;
        Symbol func;
        struct ExprList *args;

    public:
        CallExpr(Symbol func, struct ExprList *args);
        void print();
        SemanticResult *semantic(SemanticResult *&env);
    };

    class OpExpr : public Expr {
        struct Location lo;
        Oper oper;
        Expr *left;
        Expr *right;
        ActualType *getOperatedType(ActualType *&type);

    public:
        OpExpr(Oper oper, Expr *left, Expr *right);
        void print();
        SemanticResult *semantic(SemanticResult *&env);
    };

    class RecordExpr : public Expr {
        struct Location lo;
        Symbol type;
        struct ValfieldList* valfields;
    public:
        // todo re-design
        RecordExpr(Symbol type, struct ValfieldList* valfields);
        void print();
        SemanticResult *semantic(SemanticResult *&env);
        bool has(Symbol s);
        ActualType *getFieldType(Symbol s, SemanticResult *&env);
    };

    class ArrayExpr : public Expr {
        struct Location lo;
        Symbol type;
        Expr *size;

    public:
        ArrayExpr(Symbol t, Expr *s);
        void print();
        SemanticResult *semantic(SemanticResult *&env);
    };

    class SeqExpr : public Expr {
        struct Location lo;
        struct ExprList *seq;

    public:
        SeqExpr(struct ExprList *seq);
        void print();
        SemanticResult *semantic(SemanticResult *&env);
    };

    class AssignExpr : public Expr {
        struct Location lo;
        Lvalue *lv;
        Expr *expr;

    public:
        AssignExpr(Lvalue *lv, Expr *expr);
        void print();
        SemanticResult *semantic(SemanticResult *&env);
    };

    class IfExpr : public Expr {
        struct Location lo;
        Expr *test;
        struct ExprList *then;
        struct ExprList *otherwise;

    public:
        IfExpr(Expr *test, struct ExprList *then, struct ExprList *otherwise);
        void print();
        SemanticResult *semantic(SemanticResult *&env);
    };

    class WhileExpr : public Expr {
        struct Location lo;
        Expr *test;
        struct ExprList *body;

    public:
        WhileExpr(Expr *test, struct ExprList *body);
        void print();
        SemanticResult *semantic(SemanticResult *&env);
    };

    class LetExpr : public Expr {
        struct Location lo;
        struct DeclareList *declares;
        struct ExprList *body;

    public:
        LetExpr(struct DeclareList *declares, struct ExprList *body);
        void print();
        SemanticResult *semantic(SemanticResult *&env);
    };

    class NilExpr : public Expr {
        struct Location lo;

    public:
        NilExpr();
        void print();
        SemanticResult *semantic(SemanticResult *&env);
    };

    class BreakExpr : public Expr {
        struct Location lo;

    public:
        BreakExpr();
        void print();
        SemanticResult *semantic(SemanticResult *&env);
    };

    Expr *OrExpr(Expr *left, Expr *right);

    Expr *AndExpr(Expr *left, Expr *right);

    Expr *MinusExpr(Expr *expr);

    struct ExprList {
        Expr* head;
        struct ExprList* tail;
    };
    struct ExprList* ExprList4(Expr* head, struct ExprList* tail);

    struct Valfield {
        Symbol name;
        struct Expr* expr;
    };
    struct ValfieldList {
        struct Valfield* head;
        struct ValfieldList* tail;
    };
    struct ValfieldList* ValfieldList4(Symbol name, Expr* expr, struct ValfieldList* tail);
}

/** declares */
namespace ast {
    class FunctionDeclare : public Declare {
        struct Location lo;
        Symbol name;
        struct TypefieldList *params;
        Symbol result;
        Expr *body;

    public:
        FunctionDeclare(Symbol name, struct TypefieldList *params, Symbol result, Expr *body);
        void print();
        DeclareKind getKind();
        SemanticResult *preprocess(SemanticResult *&env);
        SemanticResult *semantic(SemanticResult *&env, struct DeclareList *decs);
    };

    class VarDeclare : public Declare {
        struct Location lo;
        Symbol id;
        Symbol type;
        Expr *init;

    public:
        VarDeclare(Symbol id, Symbol type, Expr *init);
        void print();
        DeclareKind getKind();
        SemanticResult *preprocess(SemanticResult *&env);
        SemanticResult *semantic(SemanticResult *&env, struct DeclareList *decs);
    };

    class TypeDeclare : public Declare {
        struct Location lo;
        Symbol _name;
        Type *_def;

    public:
        Symbol const &name;
        Type *const &def;

        TypeDeclare(Symbol name, struct Type *def);
        void print();
        DeclareKind getKind();
        SemanticResult *semantic(SemanticResult *&env, struct DeclareList *decs);
        SemanticResult *preprocess(SemanticResult *&env);
    };

    struct DeclareList {
        Declare* head;
        struct DeclareList* tail;
    };
    struct DeclareList* DeclareList4(Declare* head, struct DeclareList* tail);
}

/** types */
namespace ast {
    class NameType : public Type {
        struct Location lo;
        Symbol name;
    public:
        NameType(Symbol name);
        void print();
        ActualType *pure(SemanticResult *&env, struct DeclareList *decs);
    };

    class RecordType : public Type {
        struct Location lo;
        struct TypefieldList *record;
    public:
        RecordType(struct TypefieldList *record);
        void print();
        ActualType *pure(SemanticResult *&env, struct DeclareList *decs);
    };

    class ArrayType : public Type {
        struct Location lo;
        Symbol array;
    public:
        ArrayType(Symbol array);
        void print();
        ActualType *pure(SemanticResult *&env, struct DeclareList *decs);
    };

    struct Typefield {
        Symbol name;
        Symbol type;
    };
    struct TypefieldList {
        struct Typefield* head;
        struct TypefieldList* tail;
    };
    struct TypefieldList* TypefieldList4(Symbol name, Symbol type, struct TypefieldList* tail);
}

namespace ast {
    extern Expr* AST_ROOT;
}

#endif