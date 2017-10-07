#include "type.h"
#include "../utils/index.h"

char *ActualType::stringify() {
  return String("ActualType");
}

bool ActualType::equal(ActualType *t) {
  return t ? this->kind == t->kind : false;
}

ActualTypeList::ActualTypeList(ActualType *h, ActualTypeList *t) {
  this->head = h;
  this->tail = t;
}

ActualNone::ActualNone() {
  this->kind = NoneATK;
}

ActualNone::ActualNone(Symbol name) {
  this->kind = NoneATK;
  this->name = name;
}

ActualNil::ActualNil() {
  this->kind = NilATK;
}

ActualVoid::ActualVoid() {
  this->kind = VoidATK;
}

ActualInt::ActualInt() {
  this->kind = IntATK;
}

ActualReal::ActualReal() {
  this->kind = RealATK;
}

ActualString::ActualString() {
  this->kind = StringATK;
}

ActualArray::ActualArray(ActualType *t) {
  this->kind = ArrayATK;
  this->type = t;
}

bool ActualArray::equal(ActualType *t) {
  if (this->kind==t->kind) {
    auto t1 = this->type;
    auto t2 = ((ActualArray *) t)->type;
    return t1->equal(t2);
  } else {
    return false;
  }
}

FieldType::FieldType(Symbol n, ActualType *t) {
  this->name = n;
  this->type = t;
}

FieldTypeList::FieldTypeList(FieldType *v, FieldTypeList *n) {
  this->head = v;
  this->tail = n;
}

ActualRecord::ActualRecord(FieldTypeList *s) {
  this->kind = RecordATK;
  this->fields = s;
}

bool ActualRecord::equal(ActualType *t) {
  return t->kind == NilATK ? true : this==t;
}

bool ActualRecord::has(Symbol s) {
  auto fields = this->fields;

  while (fields) {
    auto v = fields->head;
    if (v->name==s) {
      return true;
    }
    fields = fields->tail;
  }

  return false;
}

ActualType *ActualRecord::getFieldType(Symbol s) {
  auto fields = this->fields;

  while (fields) {
    auto v = fields->head;
    if (v->name==s) {
      return v->type;
    }
    fields = fields->tail;
  }

  return nullptr;
}

void ActualRecord::update(Symbol s, ActualType *type) {
  auto fields = this->fields;

  while (fields) {
    auto v = fields->head;
    if (v->name==s) {
      v->type = type;
      break;
    }
    fields = fields->tail;
  }
}