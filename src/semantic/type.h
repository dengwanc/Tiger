#ifndef TIGER_SEMANTIC_ACTUAL_TYPE_H
#define TIGER_SEMANTIC_ACTUAL_TYPE_H

#include "../utils/symbol.h"
#include "../ast/index.h"

enum ActualTypeKind {
    RecordATK,
    NilATK,
    IntATK,
    StringATK,
    ArrayATK,
//    NameATK,
    VoidATK,
    RealATK,
    SittingATK
};

class ActualType {
protected:
    ActualTypeKind _kind;
public:
    ActualTypeKind const &kind;
    virtual bool equal(ActualType* t);
    virtual char* stringify() = 0;
    ActualType();

static ActualType* pure();
};

class ActualTypeList {
public:
    ActualType* head;
    ActualTypeList* tail;
    ActualTypeList(ActualType* h, ActualTypeList* t);
};

class ActualNil: public ActualType {
public:
    ActualNil();
};

class ActualSitting: public ActualType {
public:
    ActualSitting();
};

class ActualVoid: public ActualType {
public:
    ActualVoid();
};

class ActualInt: public ActualType {
public:
    ActualInt();
};

class ActualString: public ActualType {
public:
    ActualString();
};

class ActualReal: public ActualType {
public:
    ActualReal();
};

// todo useless
class ActualName: public ActualType {
    Symbol symbol;
    ActualType* _type;
public:
    ActualType* const &type;
    ActualName(Symbol n, ActualType* t);
    bool equal(ActualType* t);
};

class ActualArray: public ActualType {
    ActualType* _type;
public:
    ActualType* const &type;
    ActualArray(ActualType* t);
    bool equal(ActualType* t);
};

class FieldType {
public:
    Symbol name;
    ActualType* type;
    FieldType(Symbol n, ActualType* t);
};

class FieldTypeList {
public:
    FieldType* head;
    FieldTypeList* tail;
    FieldTypeList(FieldType* v, FieldTypeList* n);
};

class ActualRecord: public ActualType {
    FieldTypeList* _fields;
public:
    FieldTypeList* const &fields;
    ActualRecord(FieldTypeList* s);
    bool equal(ActualType* t);
    bool has(Symbol s);

    // todo
    bool match(ast::RecordExpr *expr, ast::SemanticResult *&env, char *msg);
    ActualType* getFieldType(Symbol s);
};

#endif


