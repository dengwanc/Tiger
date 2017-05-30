#include <cstring>
#include <iostream>
#include <vector>
#include "interpreter.h"
#include "../src/utils/index.h"
#include "../src/errormsg.h"

Statement::Statement(Statement* stm1, Statement* stm2)
{
	if (stm1 == nullptr) throw NULLPTR;
	if (stm2 == nullptr) throw NULLPTR;

	kind = CompoundStm;
	u.compound.stm1 = stm1;
    u.compound.stm2 = stm2;
}

Statement::Statement(const char* id, Expression* expr)
{
	if (expr == nullptr) throw NULLPTR;

    kind = AssignStm;
	u.assign.id = String(id);
	u.assign.expr = expr;
}

Statement::Statement(ExprList* exprs)
{
	if (exprs == nullptr) throw NULLPTR;

    kind = PrintStm;
	u.print.exprs = exprs;
}


#define MAX_SIZE 500
#define ADD_Q(i, k, field, p) \
    queue[i].kind = (k); \
    queue[i].ptr.field = p; \
    (i)++

enum QKind {QExpr, QList, QStm, QNull} kind;
static struct Q {
    QKind kind;
    union {
        Expression* expr;
        ExprList* list;
        Statement* stm;
    } ptr;
} queue[MAX_SIZE];

int Statement::maxargs()
{
    auto max = 0;
    auto position = 0;
    std::vector<std::function<void(struct Q&)>> table;

    // map kind->function avoid many switch case
    // for distinguish different type Design Pattern
    table.push_back([&](struct Q& q) {
        auto e = q.ptr.expr;
        switch (e->kind) {
        case OpExp:
            ADD_Q(position, QExpr, expr, e->u.op.left);
            ADD_Q(position, QExpr, expr, e->u.op.right);
            break;
        case EseqExp:
            ADD_Q(position, QStm, stm, e->u.eseq.stm);
            ADD_Q(position, QExpr, expr, e->u.eseq.expr);
            break;
        case IdExp:
        case NumExp:
            break;
        default:
            throw UNSUPPORT_TYPE;
            break;
        }
    });
    table.push_back([&](struct Q& q) {
        auto l = q.ptr.list;
        switch (l->kind) {
        case PairExpList:
            ADD_Q(position, QExpr, expr, l->u.pair.head);
            ADD_Q(position, QList, list, l->u.pair.tail);
            break;
        case LastExpList:
            ADD_Q(position, QExpr, expr, l->u.last);
            break;
        default:
            throw UNKNOWN_TYPE;
        }
    });
    table.push_back([&](struct Q& q) {
        auto s = q.ptr.stm;
        switch  (s->kind) {
        case CompoundStm:
            ADD_Q(position, QStm, stm, s->u.compound.stm1);
            ADD_Q(position, QStm, stm, s->u.compound.stm2);
            break;
        case AssignStm:
            ADD_Q(position, QExpr, expr, s->u.assign.expr);
            break;
        case PrintStm: {
            auto exprs = s->u.print.exprs;
            auto length = exprs->length();
            ADD_Q(position, QList, list, exprs);

            if (length > max) {
                max = length;
            }

            break;
        }
        default:
            throw UNKNOWN_TYPE;
        }
    });

    // init kind=null for end Sign
    for (auto i = 0; i < MAX_SIZE; i++) {
        queue[i].kind = QNull;
    }

    // put Stm to Queue as first one
    ADD_Q(position, QStm, stm ,this);

    for (auto i = 0; queue[i].kind != QNull; i++) {
        auto q = queue[i];
        table[q.kind](q);
        if (i > MAX_SIZE) throw CONST_ARRAY;
    }

    return max;
}

ExprList::ExprList(Expression* head, ExprList* tail)
{
    if (head == nullptr) throw NULLPTR;
    if (tail == nullptr) throw NULLPTR;

    kind = PairExpList;
    u.pair.head = head;
    u.pair.tail = tail;
}

ExprList::ExprList(Expression* last)
{
    if (last == nullptr) throw NULLPTR;

    kind = LastExpList;
    u.last = last;
}

int ExprList::length()
{
    auto length = 1;
    auto k = kind;
    auto ptr = this;

    while(ptr->kind != LastExpList) {
        ptr = ptr->u.pair.tail;
        length++;
    }

    return length;
}

Expression::Expression(const char* id)
{
    if (id == nullptr) throw CONST_STRING;

    kind = IdExp;
    u.id = String(id);
}

Expression::Expression(int num) {
    kind = NumExp;
    u.num = num;
}

Expression::Expression(Expression* left, binop oper, Expression* right)
{
    if (left == nullptr) throw NULLPTR;
    if (right == nullptr) throw NULLPTR;

    kind = OpExp;
    u.op.left = left;
    u.op.right = right;
    u.op.oper = oper;
}

Expression::Expression(Statement* stm, Expression* expr)
{
    if (stm == nullptr) throw NULLPTR;
    if (expr == nullptr) throw NULLPTR;

    kind = EseqExp;
    u.eseq.stm = stm;
    u.eseq.expr = expr;
}

ListTable::ListTable()
{
    id = String("__$@default???__");
    value = 0x1e6f86;
    tail = nullptr;
}

ListTable::ListTable(char* id, int value, ListTable* tail):id(id), value(value), tail(tail)
{
    if (id == nullptr) throw NULLPTR;
    if (tail == nullptr) throw NULLPTR;
}

int ListTable::lookup(char *key)
{
    auto list = this;

    while(list) {
        if (strcmp(key, list->id) == 0) {
            return list->value;
        }
        list = list->tail;
    }

    throw MUST_FIND;
}


Table* ListTable::update(char *id, int value, Table* base)
{
    return new ListTable(id, value, (ListTable* )base);
}

#define __TABLE__ ListTable

void Statement::interpret()
{
    eval(new __TABLE__);
}

struct Result Expression::eval(Table *table)
{
    struct Result r1, r2;

    switch (kind) {
    case NumExp:
        r1.caculated = u.num;
        r1.table = table;
        return r1;
    case IdExp:
        r1.caculated = table->lookup(u.id);
        r1.table = table;
        return r1;
    case OpExp:
        r1 = u.op.left->eval(table);
        r2 = u.op.right->eval(table);
        switch (u.op.oper) {
        case plus: return (r2.caculated += r1.caculated, r2);
        case minus:return (r2.caculated -= r1.caculated, r2);
        case times:return (r2.caculated *= r1.caculated, r2);
        case divi: return (r2.caculated /= r1.caculated, r2);
        }
    case EseqExp: {
        auto t = u.eseq.stm->eval(table);
        return u.eseq.expr->eval(t);
    }
    default: throw UNKNOWN_TYPE;
    }
}

Table* Statement::eval(Table* table)
{
    switch (kind) {
    case CompoundStm: {
        auto t1 = u.compound.stm1->eval(table);
        auto t2 = u.compound.stm2->eval(t1);
        return t2;
    }
    case AssignStm: {
        auto r = u.assign.expr->eval(table);
        auto t = r.table->update(u.assign.id, r.caculated, r.table);
        return t;
    }
    case PrintStm: {
        auto exprs = u.print.exprs;
        auto updatedTable = table;
        auto handle = [&updatedTable](Expression* e) {
            auto r = e->eval(updatedTable);

            // implement print semantic
            // immutable print
            std::cout << r.caculated << std::endl;

            // update table
            updatedTable = r.table;
        };

        while(exprs->kind == PairExpList) {
            handle(exprs->u.pair.head);
            exprs = exprs->u.pair.tail;
        }

        if (exprs->kind == LastExpList) {
            handle(exprs->u.last);
        } else {
            throw UNKNOWN_TYPE;
        }

        return updatedTable;
    }
    default: throw UNKNOWN_TYPE;
    }
}