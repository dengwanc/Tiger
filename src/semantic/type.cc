#include "type.h"
#include "../utils/index.h"

ActualType::ActualType() : kind(_kind) {}

char *ActualType::stringify() {
  return String("ActualType");
}

bool ActualType::equal(ActualType *t) {
  return t ? this->kind==t->kind : false;
}

ActualTypeList::ActualTypeList(ActualType *h, ActualTypeList *t) {
  this->head = h;
  this->tail = t;
}

ActualNone::ActualNone() {
  this->_kind = NoneATK;
}

ActualNil::ActualNil() {
  this->_kind = NilATK;
}

ActualVoid::ActualVoid() {
  this->_kind = VoidATK;
}

ActualInt::ActualInt() {
  this->_kind = IntATK;
}

ActualReal::ActualReal() {
  this->_kind = RealATK;
}

ActualString::ActualString() {
  this->_kind = StringATK;
}

//ActualName::ActualName(Symbol n, ActualType *t): type(_type)
//{
//    this->_kind = NameATK;
//    this->symbol = n;
//    this->_type = t;
//}

//bool ActualName::equal(ActualType *t)
//{
//    if (this->kind == t->kind) {
//        auto t1 = this->type;
//        auto t2 = ((ActualName* )t)->type;
//        return t1->equal(t2);
//    } else {
//        return false;
//    }
//}

ActualArray::ActualArray(ActualType *t) : type(_type) {
  this->_kind = ArrayATK;
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

ActualRecord::ActualRecord(FieldTypeList *s) : fields(_fields) {
  this->_kind = RecordATK;
  this->_fields = s;
}

bool ActualRecord::equal(ActualType *t) {
  return this==t;
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

//bool ActualRecord::match(ast::RecordExpr *expr, ast::SemanticResult *&env, char *msg)
//{
//    auto typfields = this->_fields;
//
//    while(typfields) {
//        auto field = typfields->head->name;
//        if (expr->has(field)) {
//            auto type = expr->getFieldType(field, env);
//            auto deftype = typfields->head->type;
//            if (type && deftype && type->equal(deftype)) {
//                continue;
//            } else {
//                sprintf(msg, "field %s not matched %s", S_name(field), deftype->stringify());
//                break;
//            }
//        } else {
//            sprintf(msg, "record type has no field `%s`", S_name(field));
//            break;
//        }
//
//        typfields = typfields->tail;
//    }
//
//    return typfields == nullptr;
//}