#ifndef TIGER_SEMANTIC_ACTUAL_TYPE_H
#define TIGER_SEMANTIC_ACTUAL_TYPE_H

/**
 * This module for manage
 * expression's type
 * like
 * ```
 * type size_t = int
 * var a = 5
 * var b = "ssss"
 * var c: size_t
 * ```
 * type of a is ActualType
 * type of b is ActualString
 * type of c is int
 */

#include "../utils/symbol.h"

enum ActualTypeKind {
  RecordATK,
  NilATK,
  IntATK,
  StringATK,
  ArrayATK,
  VoidATK,
  RealATK,
  NoneATK
};

class ActualType {
public:
  ActualTypeKind kind;
  virtual bool equal(ActualType *t);
  virtual char *stringify();
};

class ActualTypeList {
public:
  ActualType *head;
  ActualTypeList *tail;
  ActualTypeList(ActualType *h, ActualTypeList *t);
};

/** means null pointer */
class ActualNil : public ActualType {
public:
  ActualNil();
};

/**
 * Just for occupy in semantic stage
 * NOT REAL TYPE !
 */
class ActualNone : public ActualType {
public:
  Symbol name;
  ActualNone();
  ActualNone(Symbol name);
};

class ActualVoid : public ActualType {
public:
  ActualVoid();
};

class ActualInt : public ActualType {
public:
  ActualInt();
};

class ActualString : public ActualType {
public:
  ActualString();
};

class ActualReal : public ActualType {
public:
  ActualReal();
};

class ActualArray : public ActualType {
public:
  ActualType *type;
  ActualArray(ActualType *t);
  bool equal(ActualType *t);
};

class FieldType {
public:
  Symbol name;
  ActualType *type;
  FieldType(Symbol n, ActualType *t);
};

class FieldTypeList {
public:
  FieldType *head;
  FieldTypeList *tail;
  FieldTypeList(FieldType *v, FieldTypeList *n);
};

class ActualRecord : public ActualType {
public:
  FieldTypeList *fields;
  ActualRecord(FieldTypeList *s);
  bool equal(ActualType *t);
  bool has(Symbol s);
  ActualType *getFieldType(Symbol s);
  void update(Symbol s, ActualType* type);
};

#endif


