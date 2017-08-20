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

enum ValueIdentifyKind{
    VarVIK,
    FunctionVIK
};

class ValueIdentify {
protected:
    ValueIdentify _kind;
public:
    ValueIdentify const &kind;
    ValueIdentify();
};

class VarIdentify: public ValueIdentify {
    ActualType* type;
public:
    VarIdentify(ActualType* t);
};

class FunctionIdentify: public ValueIdentify {
    ActualType* _result;
    ActualTypeList* _formals;
public:
    ActualType* const &result;
    ActualTypeList* const &formals;
    FunctionIdentify(ActualTypeList* f, ActualType* r);
};



#endif
