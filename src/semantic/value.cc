#include "value.h"

NoneIdentify::NoneIdentify() {
  this->kind = NoneVIK;
}

VarIdentify::VarIdentify(ActualType *t) {
  this->kind = VarVIK;
  this->type = t;
}

FunctionIdentify::FunctionIdentify(ActualTypeList *f, ActualType *r) {
  this->kind = FunctionVIK;
  this->result = r;
  this->formals = f;
}