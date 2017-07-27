//#include <stdio.h>
//#include "index.h"
//#include <assert.h>
//#include "../utils/index.h"
//
//namespace syntax {
//    const char* OP_NAME_TABLE[] = {
//        "+",  "-", "*", "/", "=", "<>", "<", "<=", ">", ">="
//    };
//
//    static void printOp(struct Expr *expr)
//    {
//        assert(expr);
//
//        auto op = expr->u.op;
//        printExpr(op.left);
//        printf(" %s ", OP_NAME_TABLE[op.oper]);
//        printExpr(op.right);
//    }
//
//    static void printTypefield(struct Typefield* tfield)
//    {
//        assert(tfield);
//        printf("%s: ", S_name(tfield->name));
//        printf("%s", S_name((tfield->type)));
//    }
//
//    static void printTypefieldList(struct TypefieldList* tlist)
//    {
//        while (tlist) {
//            printTypefield(tlist->head);
//            tlist = tlist->tail;
//            if (tlist) printf(", ");
//        }
//    }
//
//    static void printType(struct Type* def)
//    {
//        assert(def);
//
//        switch (def->kind) {
//            case ArrayT:
//                printf("array of %s", S_name(def->u.array));
//                break;
//            case NameT:
//                printf("%s", S_name(def->u.name));
//                break;
//            case RecordT:
//                printf("{");
//                printTypefieldList(def->u.record);
//                printf("}");
//                break;
//        }
//    }
//
//    static void printFuncdec(struct Funcdec* func)
//    {
//        assert(func);
//
//        printf("function ");
//        printf("%s", S_name(func->name));
//        if (func->params) {
//            printf("(");
//            printTypefieldList(func->params);
//            printf(") ");
//        } else {
//            printf(" = ");
//        }
//
//        printExpr(func->body);
//    }
//
//    static void printDeclare(struct Declare* declare)
//    {
//        assert(declare);
//
//        switch (declare->kind) {
//            case Type: {
//                auto type = declare->u.type;
//                printf("type %s = ", S_name(type.name));
//                printType(type.def);
//                break;
//            }
//            case Var: {
//                auto var = declare->u.var;
//                printf("var %s", S_name(var.id));
//                if (var.type) {
//                    printf(": %s", S_name(var.type));
//                } else {
//                    printf(" = ");
//                    printExpr(var.init);
//                }
//                break;
//            }
//            case Function: {
//                printFuncdec(declare->u.function);
//                break;
//            }
//        }
//
//        printf("\n");
//    }
//
//    static void printExprList(struct ExprList* exprlist, char join)
//    {
//        while(exprlist) {
//            printExpr(exprlist->head);
//            exprlist = exprlist->tail;
//            if (exprlist) printf("%c ", join);
//        }
//    }
//
//    static void printExprList(struct ExprList* exprlist)
//    {
//        printExprList(exprlist, ';');
//    }
//
//    static void printLet(struct Expr* expr)
//    {
//        assert(expr);
//
//        auto decs = expr->u.let.decs;
//        printf("let\n");
//        while(decs) {
//            printDeclare(decs->head);
//            decs = decs->tail;
//        }
//
//        printf("in\n");
//        printExprList(expr->u.let.body);
//        printf("\nend");
//    }
//
//    static void printLvalue(struct Lvalue* lv)
//    {
//        assert(lv);
//
//        switch (lv->kind) {
//            case Simple:
//                printf("%s", S_name(lv->u.simple));
//                break;
//            case Field:
//                printLvalue(lv->u.field.lv);
//                printf(".%s", S_name(lv->u.field.x));
//                break;
//            case Subscript:
//                printLvalue(lv->u.subscript.lv);
//                printf("[");
//                printExpr(lv->u.subscript.expr);
//                printf("]");
//                break;
//        }
//    }
//
//    static void printArray(struct Expr* expr)
//    {
//        assert(expr);
//        auto arr = expr->u.array;
//        printf("[");
//        printExpr(arr.size);
//        printf("]");
//        printf("%s", S_name(arr.type));
//    }
//
//    static void printValfield(struct Valfield* vfield)
//    {
//        assert(vfield);
//        printf("%s: ", S_name(vfield->name));
//        printExpr(vfield->expr);
//    }
//
//    static void printValfieldList(struct ValfieldList* vlist)
//    {
//        while (vlist) {
//            printValfield(vlist->head);
//            vlist = vlist->tail;
//            if (vlist) printf(", ");
//        }
//    }
//
//    static void printRecord(struct Expr* expr)
//    {
//        assert(expr);
//        auto record = expr->u.record;
//        printf("{");
//        printValfieldList(record.valfields);
//        printf("}");
//        printf("%s", S_name(record.type));
//    }
//
//    static void printAssign(struct Expr* expr)
//    {
//        assert(expr);
//        auto assign = expr->u.assign;
//        printLvalue(assign.lv);
//        printf(" = ");
//        printExpr(assign.expr);
//    }
//
//    static void printIf(struct Expr* expr)
//    {
//        assert(expr);
//
//        auto ife = expr->u.ife;
//        printf("if ");
//        printExpr(ife.test);
//        printf(" then ");
//        printExprList(ife.then);
//        printf(" else ");
//        printExprList(ife.otherwise);
//        printf(" end");
//    }
//
//    static void printCall(struct Expr* expr)
//    {
//        assert(expr);
//
//        auto call = expr->u.call;
//        printf("%s", S_name(call.func));
//        printf("(");
//        printExprList(call.args, ',');
//        printf(")");
//
//    }
//
//    static void printWhile(struct Expr* expr)
//    {
//        assert(expr);
//
//        auto whilee = expr->u.whilee;
//        printf("while ");
//        printExpr(whilee.test);
//        printf(" { ");
//        printExprList(whilee.body);
//        printf(" }");
//    }
//
//    void printExpr(struct Expr *expr) {
//
//
//        switch (expr->kind) {
//            case OpE:
//                printOp(expr);
//                break;
//            case StringE:
//                printf("\"%s\"", expr->u.sval);
//                break;
//            case IntE:
//                printf("%d", expr->u.ival);
//                break;
//            case LetE:
//                printLet(expr);
//                break;
//            case LvalueE:
//                printLvalue(expr->u.lv);
//                break;
//            case ArrayE:
//                printArray(expr);
//                break;
//            case RecordE:
//                printRecord(expr);
//                break;
//            case SeqE:
//                printf("{ ");
//                printExprList(expr->u.seq);
//                printf(" }");
//                break;
//            case AssignE:
//                printAssign(expr);
//                break;
//            case IfE:
//                printIf(expr);
//                break;
//            case CallE:
//                printCall(expr);
//                break;
//            case NilE:
//                printf("nil");
//                break;
//            case WhileE:
//                printWhile(expr);
//                break;
//            default: {
//                char* alert = (char*)checkedMalloc(100);
//                sprintf(alert, "\nUNKNOWN EXPRESSION TYPE %d", expr->kind);
//                error(alert);
//            }
//        }
//    }
//
//}