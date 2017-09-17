#ifndef TIGER_SEMANTIC_ENV_TABLE_H
#define TIGER_SEMANTIC_ENV_TABLE_H

/**
 * module for maintain value table.
 * for example
 * ===================================
 * a => {var, int}
 * f => {function, bool (int, string)}
 * ===================================
 * for a variable keep it's type
 * for a function keep it's function signature
 */

#include "type.h"

enum ValueIdentifyKind {
    VarVIK,
    FunctionVIK,
    NoneVIK,
};

class ValueIdentify {
public:
    ValueIdentifyKind kind;
//    virtual bool equal(ValueIdentify* v) = 0;
};

class NoneIdentify: public ValueIdentify {
public:
    NoneIdentify();
};

class VarIdentify: public ValueIdentify {
public:
    ActualType* type;
    VarIdentify(ActualType* t);
};

class FunctionIdentify: public ValueIdentify {
public:
    ActualType* result;
    ActualTypeList* formals;
    FunctionIdentify(ActualTypeList* f, ActualType* r);
};

#endif
