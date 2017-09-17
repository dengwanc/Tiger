#ifndef TIGER_SEMANTIC_ACTUAL_TYPE_H
#define TIGER_SEMANTIC_ACTUAL_TYPE_H

#include "../utils/symbol.h"

enum ActualTypeKind {
    RecordATK,
    NilATK,
    IntATK,
    StringATK,
    ArrayATK,
//    NameATK,
    VoidATK,
    RealATK,
    NoneATK
};

class ActualType {
protected:
    ActualTypeKind _kind;
public:
    ActualTypeKind const &kind;
    virtual bool equal(ActualType* t);
    virtual char* stringify() { return "ActualType"; };
    ActualType();
};

class ActualTypeList {
public:
    ActualType* head;
    ActualTypeList* tail;
    ActualTypeList(ActualType* h, ActualTypeList* t);
};

/** means null pointer */
class ActualNil: public ActualType {
public:
    ActualNil();
};

/**
 * Just for occupy in semantic stage
 * NOT REAL TYPE !
 */
class ActualNone: public ActualType {
public:
    ActualNone();
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

    ActualType* getFieldType(Symbol s);
};

#endif


