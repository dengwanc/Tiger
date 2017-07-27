#include <stdio.h>
#include "index.h"
#include <assert.h>

namespace ast {
    const char* OP_NAME_TABLE[] = {
            "+",  "-", "*", "/", "=", "<>", "<", "<=", ">", ">="
    };

    void OpExpr::print()
    {
        this->left->print();
        printf(" %s ", OP_NAME_TABLE[this->oper]);
        this->right->print();
    }

    static void printTypefield(struct Typefield* tfield)
    {
        assert(tfield);

        printf("%s: ", S_name(tfield->name));
        printf("%s", S_name((tfield->type)));
    }

    static void printTypefieldList(struct TypefieldList* tlist)
    {
        while (tlist) {
            printTypefield(tlist->head);
            tlist = tlist->tail;
            if (tlist) printf(", ");
        }
    }

    void ArrayType::print()
    {
        printf("array of %s", S_name(this->array));
    }

    void NameType::print() {
        printf("%s", S_name(this->name));
    }

    void RecordType::print() {
        printf("{");
        printTypefieldList(this->record);
        printf("}");
    }

    void FunctionDeclare::print() {
        printf("function ");
        printf("%s", S_name(this->name));
        if (this->params) {
            printf("(");
            printTypefieldList(this->params);
            printf(") ");
        } else {
            printf(" = ");
        }

        this->body->print();

        printf("\n");
    }

    void TypeDeclare::print()
    {
        printf("type %s = ", S_name(this->name));
        this->def->print();
        printf("\n");
    }

    void VarDeclare::print()
    {
        printf("var %s", S_name(this->id));
        if (this->type) {
            printf(": %s", S_name(this->type));
        } else {
            printf(" = ");
            this->init->print();
        }

        printf("\n");
    }

    static void printExprList(struct ExprList* exprlist, char join)
    {
        while(exprlist) {
            exprlist->head->print();
            exprlist = exprlist->tail;
            if (exprlist) printf("%c ", join);
        }
    }

    static void printExprList(struct ExprList* exprlist)
    {
        printExprList(exprlist, ';');
    }

    void LetExpr::print()
    {
        printf("let\n");

        auto decs = this->declares;
        while(decs) {
            decs->head->print();
            decs = decs->tail;
        }

        printf("in\n");
        printExprList(this->body);
        printf("\nend");
    }

    void SimpleLvalue::print()
    {
        printf("%s", S_name(this->simple));
    }

    void FieldLvalue::print()
    {
        this->lv->print();
        printf(".%s", S_name(this->x));
    }

    void SubscriptLvalue::print()
    {
        this->lv->print();
        printf("[");
        this->expr->print();
        printf("]");
    }

    void ArrayExpr::print()
    {
        printf("[");
        this->size->print();
        printf("]");
        printf("%s", S_name(this->type));
    }

    static void printValfield(struct Valfield* vfield)
    {
        assert(vfield);
        printf("%s: ", S_name(vfield->name));
        vfield->expr->print();
    }

    static void printValfieldList(struct ValfieldList* vlist)
    {
        while (vlist) {
            printValfield(vlist->head);
            vlist = vlist->tail;
            if (vlist) printf(", ");
        }
    }

    void RecordExpr::print()
    {
        printf("{");
        printValfieldList(this->valfields);
        printf("}");
        printf("%s", S_name(this->type));
    }

    void AssignExpr::print()
    {
        this->lv->print();
        printf(" = ");
        this->expr->print();
    }

    void IfExpr::print()
    {
        printf("if ");
        this->test->print();
        printf(" then ");
        printExprList(this->then);
        printf(" else ");
        printExprList(this->otherwise);
        printf(" end");
    }

    void CallExpr::print()
    {
        printf("%s", S_name(this->func));
        printf("(");
        printExprList(this->args, ',');
        printf(")");
    }

    void WhileExpr::print()
    {
        printf("while ");
        this->test->print();
        printf(" { ");
        printExprList(this->body);
        printf(" }");
    }

    void StringExpr::print()
    {
        printf("\"%s\"", this->sval);
    }

    void IntExpr::print()
    {
        printf("%d", this->ival);
    }

    void SeqExpr::print()
    {
        printf("{ ");
        printExprList(this->seq);
        printf(" }");
    }

    void NilExpr::print()
    {
        printf("nil");
    }

    void LvalueExpr::print()
    {
        this->lv->print();
    }

    void RealExpr::print()
    {
        printf("%f", this->dval);
    }

    void BreakExpr::print()
    {
        printf("break");
    }
}