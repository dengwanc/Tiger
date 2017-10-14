#include <stdio.h>
#include <assert.h>
#include "../ast/index.h"
#include "../utils/index.h"
#include "errcode.h"

/**
 * Semantic Error Message (Abbreviation `sem`)
 * Bill Gates said
 * 64K IS ENOUGH FOR ANYONE
 */
static char sem[1024*64];
static int sec = 0; // semantic error code

/** inner functions */
namespace ast {
static void handleError(struct Location lo) {
  if (empty(sem))
    return;

  reportSemanticError(sem, lo);
  sem[0] = '\0';
}

static SemanticResult *semanticExprList(struct ExprList *l, SemanticResult *env) {
  SemanticResult *end = nullptr;

  while (l) {
    end = l->head->semantic(env);
    l = l->tail;
  }

  return end;
}

struct GroupedDeclare {
  struct DeclareList *value;
  struct DeclareList *type;
};

static struct GroupedDeclare &groupDeclares(struct DeclareList *declares) {
  DeclareList *value = nullptr;
  DeclareList *type = nullptr;
  struct GroupedDeclare group;

  while (declares) {
    auto dec = declares->head;

    switch (dec->getKind()) {
    case FunctionDK:
    case VarDK:value = DeclareList4(declares->head, value);
      break;
    case TypeDK:type = DeclareList4(declares->head, type);
      break;
    default:crash("Declare object must has a certain type");
    }

    declares = declares->tail;
  }

  return group = {reverse(value), reverse(type)};
}

static Declare *getDeclareByName(struct DeclareList *decs, Symbol name) {
  while (decs) {
    auto dec = (TypeDeclare *) decs->head;
    if (dec->name==name) {
      return dec;
    }
    decs = decs->tail;
  }

  return nullptr;
}

static ActualType *getActualType(Symbol s, SemanticResult *env, struct DeclareList *decs, bool recursion) {
  auto looked_type = (ActualType *) env->typ_table->lookup(s);

  if (looked_type) {
    if (looked_type->kind==NoneATK) {
      auto dec = (TypeDeclare *) getDeclareByName(decs, s);
      assert(dec); // must get dec or sth wrong
      return (ActualType*)(recursion
        ? dec->def->transform(env, decs)
        : new ActualNone(s));
    } else {
      return looked_type;
    }
  } else {
    return nullptr;
  }
}
}

/** SemanticResult  */
namespace ast {
SemanticResult::SemanticResult(BinaryTree *v, BinaryTree *t, ActualType *a) {
  this->val_table = v;
  this->typ_table = t;
  this->type = a;
  this->errcode = 0;
}

SemanticResult::SemanticResult(BinaryTree *v, BinaryTree *t, ActualType *a, int errcode) {
  this->val_table = v;
  this->typ_table = t;
  this->type = a;
  this->errcode = errcode;
}

SemanticResult *SemanticResult::copy(ActualType *t) {
  return new SemanticResult(this->val_table, this->typ_table, t, this->errcode);
}

SemanticResult* SemanticResult::copy(ActualType *t, int errcode) {
  return new SemanticResult(this->val_table, this->typ_table, t, errcode);
}

SemanticResult *SemanticResult::copy() {
  return new SemanticResult(this->val_table, this->typ_table, this->type, this->errcode);
}

/**
 * Simple Export Single int value
 * better design is:
 * `[error_obj_1, error_obj_2, error_obj3, ...]`
 * here just for Finish Semantic Module :)
 */
int getErrorCode() {
  return sec;
}

void clearErrorCode() {
  sec = 0;
}
}

/** semantic implements */
namespace ast {
SemanticResult *IntExpr::semantic(SemanticResult *env) {
  return env->copy(new ActualInt());
}

SemanticResult *StringExpr::semantic(SemanticResult *env) {
  return env->copy(new ActualString());
}

SemanticResult *RealExpr::semantic(SemanticResult *env) {
  return env->copy(new ActualReal());
}

SemanticResult *SimpleLvalue::semantic(SemanticResult *env) {
  auto looked_type = (VarIdentify*)env->val_table->lookup(this->simple);

  if (looked_type) {
    return env->copy(looked_type->type);
  } else {
    sprintf(sem, "%d# var %s is not defined", sec = VAR_NOT_DEFINED, S_name(this->simple));
  }

  handleError(this->lo);

  return env->copy(nullptr);
}

SemanticResult *FieldLvalue::semantic(SemanticResult *env) {
  auto tmp = this->lv->semantic(env);
  auto lvalue_type = tmp->type;

  if (lvalue_type->kind==RecordATK) {
    auto has_field = ((ActualRecord *) lvalue_type)->has(this->x);

    if (has_field) {
      auto field_type = ((ActualRecord *) lvalue_type)->getFieldType(this->x);

      if (field_type) {
        env->type = field_type;
        return env->copy();
      } else {
        sprintf(sem, "unknown field type %s", S_name(this->x));
      }

    } else {
      sprintf(sem, "%s has no field %s", this->lv->stringify(), S_name(this->x));
    }

  } else {
    sprintf(sem, "%s is not record type", this->lv->stringify());
  }

  handleError(this->lo);

  return env->copy(nullptr);
}

SemanticResult *SubscriptLvalue::semantic(SemanticResult *env) {
  auto tmp = this->lv->semantic(env);
  auto lvalue_type = tmp->type;

  if (lvalue_type->kind==ArrayATK) {
    auto subscript_type = this->expr->semantic(env);
    auto int_type = new ActualInt();
    if (subscript_type->type->equal(int_type)) {
      env->type = int_type;
      return env->copy();
    } else {
      sprintf(sem, "subscript %s is not int type", this->expr->stringify());
    }
  } else {
    sprintf(sem, "%s is not array type", this->lv->stringify());
  }

  handleError(this->lo);

  return env->copy(nullptr);
}

SemanticResult *LvalueExpr::semantic(SemanticResult *env) {
  return env->copy(this->lv->semantic(env)->type);
}

SemanticResult *CallExpr::semantic(SemanticResult *env) {
  auto looked_value = (FunctionIdentify *) env->val_table->lookup(this->func);
  auto func_name = S_name(this->func);

  if (looked_value) {
    auto args = this->args;
    auto formals = looked_value->formals;
    while (args && formals) {
      auto tmp = args->head->semantic(env);
      if (tmp->type==nullptr) {
        return env->copy(nullptr);
      }

      if (!tmp->type->equal(formals->head)) {
        sprintf(sem, "parameters %s is not %s type", args->head->stringify(), formals->head->stringify());
        break;
      }
      args = args->tail;
      formals = formals->tail;
    }

    if (formals) {
      sprintf(sem, "short parameters in %s call", func_name);
    }

    if (args) {
      sprintf(sem, "overflow parameters in %s call", func_name);
    }

    if (formals==nullptr && args==nullptr) {
      return env->copy(looked_value->result);
    }

  } else {
    sprintf(sem, "%s is not defined", func_name);
  }

  handleError(this->lo);

  return env->copy(nullptr);
}

SemanticResult *OpExpr::semantic(SemanticResult *env) {
  auto left = this->left->semantic(env);
  auto right = this->right->semantic(env);

  if (left->type && right->type && left->type->equal(right->type)) {
    auto type = left->type;
    switch (type->kind) {
    case NilATK:sprintf(sem, "nil cannot operated");
      break;
    case VoidATK:sprintf(sem, "void cannot operated");
      break;
    case RecordATK:sprintf(sem, "record cannot operated");
    case ArrayATK:sprintf(sem, "array cannot operated");
      break;
    case StringATK:sprintf(sem, "string cannot operated");
      break;
    default:return env->copy(this->getOperatedType(type));
    }
  } else {
    sprintf(sem, "%d# different type cannot operated", sec = OPERATOR_DIFF_TYPE);
  }

  handleError(this->lo);

  return env->copy(nullptr);
}

SemanticResult *RecordExpr::semantic(SemanticResult *env) {
  auto looked_type = (ActualType *) env->typ_table->lookup(this->type);
  auto record_name = S_name(this->type);

  if (looked_type) {
    if (looked_type->kind==RecordATK) {
      auto matched = this->match((ActualRecord*)looked_type, env);

      if (matched) {
        return env->copy(looked_type);
      } else {
        sprintf(sem, "record expr not matched record type");
      }

    } else {
      sprintf(sem, "type %s is not record type", record_name);
    }
  } else {
    sprintf(sem, "type %s is not defined", record_name);
  }

  handleError(this->lo);

  return env->copy(nullptr);
}

SemanticResult *ArrayExpr::semantic(SemanticResult *env) {
  auto looked_type = (ActualType *) env->typ_table->lookup(this->type);
  auto array_name = S_name(this->type);

  if (looked_type) {
    auto tmp = this->size->semantic(env);

    if (tmp) {
      if (tmp->type->kind==IntATK) {
        return env->copy(new ActualArray(looked_type));
      } else {
        sprintf(sem, "type %s is not integer for array type", this->size->stringify());
      }
    }

  } else {
    sprintf(sem, "type %s is not defined", array_name);
  }

  env->type = nullptr;
  handleError(this->lo);

  return env->copy();
}

SemanticResult *SeqExpr::semantic(SemanticResult *env) {
  auto tmp = semanticExprList(this->seq, env);
  return env->copy(tmp->type);
}

SemanticResult *AssignExpr::semantic(SemanticResult *env) {
  auto tmp1 = this->lv->semantic(env);
  auto tmp2 = this->expr->semantic(env);
  auto type1 = tmp1->type;
  auto type2 = tmp2->type;

  if (type1 && type2) {
    if (type1->equal(type2)) {
      return env->copy(type1);
    } else {
      sprintf(sem, "type %s is not assignable to type %s", type1->stringify(), type2->stringify());
    }
  }

  handleError(this->lo);

  return env->copy(nullptr);
}

SemanticResult *IfExpr::semantic(SemanticResult *env) {
  auto test = this->test->semantic(env);

  if (test->type) {
    if (test->type->equal(new ActualInt())) {
      if (!this->otherwise) goto end; // no else part

      auto tmp1 = semanticExprList(this->then, env);
      auto tmp2 = semanticExprList(this->otherwise, env);
      auto type1 = tmp1->type;
      auto type2 = tmp2->type;

      if (type1 && type2) {
        if (type1->equal(type2)) {
          return env->copy(type1);
        } else {
          sec = IF_EXPR_ERROR1;
          sprintf(sem, "%d# type %s must same as type %s", sec, type1->stringify(), type2->stringify());
        }
      }

    } else {
      sprintf(sem, "int expression expected");
    }
  }

end:
  handleError(this->lo);

  return env->copy(nullptr);
}

SemanticResult *WhileExpr::semantic(SemanticResult *env) {
  auto test = this->test->semantic(env);

  if (test->type) {
    if (test->type->equal(new ActualInt())) {
      semanticExprList(this->body, env);
      return env->copy(new ActualVoid());
    } else {
      sprintf(sem, "int expression expected");
    }
  }

  handleError(this->lo);

  return env->copy(nullptr);
}

SemanticResult *NilExpr::semantic(SemanticResult *env) {
  return env->copy(new ActualNil());
}

SemanticResult *BreakExpr::semantic(SemanticResult *env) {
  // TODO figure out a way slove:: the
  // TODO break must in while body rule
  return env->copy(new ActualVoid());
}

SemanticResult *LetExpr::semantic(SemanticResult *env) {
  auto decs = this->declares;
  auto scope = env;
  auto group = groupDeclares(decs);

  for (auto types = group.type; types; types = types->tail) {
    scope = types->head->preprocess(scope);
  }

  for (auto types = group.type; types; types = types->tail) {
    scope = types->head->semantic(scope, types);
  }

  for (auto values = group.value; values; values = values->tail) {
    scope = values->head->semantic(scope, values);
  }

  auto tmp = semanticExprList(this->body, scope);

  return env->copy(tmp->type);
}

SemanticResult *TypeDeclare::semantic(SemanticResult *env, struct DeclareList *decs) {
  auto looked_type = (ActualType *) env->typ_table->lookup(this->name);

  if (looked_type) {
    if (looked_type->kind==NoneATK) {
      return new SemanticResult(
          env->val_table,
          env->typ_table->updateImmutable(this->name, this->def->transform(env, decs)),
          new ActualVoid()
      );
    } else { // already had transform type
      return env->copy(new ActualVoid());
    }
  } else {
    sprintf(sem, "type %s is not defined", S_name(this->name));
  }

  handleError(this->lo);

  return env->copy(nullptr);
}

SemanticResult *FunctionDeclare::semantic(SemanticResult *env, struct DeclareList *decs) {
  sem[0] = '\0'; // mark no error

  auto params = this->params;
  auto list = (ActualTypeList *) nullptr;
  auto val_table = env->val_table;
  auto new_var = (VarIdentify *) nullptr;
  for (; params; params = params->tail) { // walk arguments
    auto arg = params->head->type;
    auto arg_name = params->head->name;
    auto arg_type = (ActualType *) env->typ_table->lookup(arg);

    if (!arg_type) {
      sprintf(sem, "type %s is not defined", S_name(arg));
    }

    new_var = new VarIdentify(arg_type);
    list = new ActualTypeList(arg_type, list);
    val_table = val_table->updateImmutable(arg_name, new_var);
  };

  auto result_type = (ActualType*)(this->result
    ? env->typ_table->lookup(this->result)
    : new ActualNone()); // not declare return type

  if (!result_type) {
    sprintf(sem, "type %s is not defined", S_name(this->result));
  }

  if (sem[0]=='\0') {
    auto func = new FunctionIdentify(list, result_type);
    auto body_val_table = val_table->updateImmutable(this->name, func);
    auto func_scope = new SemanticResult(body_val_table, env->typ_table, nullptr);

    this->body->semantic(func_scope);

    return new SemanticResult(
        env->val_table->updateImmutable(this->name, func),
        env->typ_table,
        new ActualVoid()
    );
  }

  handleError(this->lo);

  return env->copy(nullptr);
}

SemanticResult *VarDeclare::semantic(SemanticResult *env, struct DeclareList *decs) {
  auto var_type = this->type
    ? (ActualType *)env->typ_table->lookup(this->type)
    : (this->init->semantic(env))->type;

  if (var_type) {
    auto new_var = new VarIdentify(var_type);
    auto ret = new SemanticResult(
        env->val_table->updateImmutable(this->id, new_var),
        env->typ_table,
        new ActualVoid()
    );

    if (this->init) {
      auto tmp = this->init->semantic(env);
      if (var_type->equal(tmp->type)) {
        return ret;
      } else {
        sprintf(sem, "var declare use difference type init");
      }
    } else { // ok with just define var's type
      return ret;
    }
  } else {
    if (this->type) sprintf(sem, "type %s is not defined", S_name(this->type));
  }

  handleError(this->lo);

  return env->copy(nullptr);
}

}

/** helpers */
namespace ast {
/**
* =====================================
* int a
* int b
* a + b // getOperatedType is ActualInt
* =====================================
* ! danger callee
* ! make sure left type totally equal right type
* ! and type must can be operated
* @param type left or right ActualType
* @return calculated ActualType
* @api private
*/
ActualType *OpExpr::getOperatedType(ActualType *type) {
  switch (this->oper) {
  case Plus:
  case Minus:
  case Times:
  case Divide:return type;
  case Eq:
  case Neq:
  case Lt:
  case Le:
  case Gt:
  case Ge:return new ActualInt();
  }
}

bool RecordExpr::has(Symbol s) {
  auto valfields = this->valfields;

  while (valfields) {
    if (valfields->head->name==s) {
      return true;
    }
    valfields = valfields->tail;
  }

  return false;
}

/**
 * record type is matched record expr
 * with Side Effect!
 * may update `ActualRecord *record_type`
 * runtime based update design
 */
bool RecordExpr::match(ActualRecord *record_type, SemanticResult *env) {
  auto valfields = this->valfields;

  while (valfields) {
    auto name = valfields->head->name;
    auto type_val = this->getFieldType(name, env);
    auto type_def = record_type->getFieldType(name);
    if (!type_def) return false;
    if (!type_val) return false;
    if (type_def->kind == NoneATK) {
      auto def = (ActualNone*) type_def;
      auto actual = (ActualType*)env->typ_table->lookup(def->name);
      assert(actual);
      type_def = actual;
      record_type->update(name, actual);
    }
    if (!type_def->equal(type_val)) return false;

    valfields = valfields->tail;
  }

  return true;
}

ActualType *RecordExpr::getFieldType(Symbol s, SemanticResult *env) {
  auto valfields = this->valfields;

  while (valfields) {
    if (valfields->head->name==s) {
      return valfields->head->expr->semantic(env)->type;
    }
    valfields = valfields->tail;
  }

  return nullptr;
}

ActualType *NameType::transform(SemanticResult *env, struct DeclareList *decs) {
  auto type = getActualType(this->name, env, decs, true);

  if (type) {
    return type;
  } else {
    sprintf(sem, "type %s is not defined", S_name(this->name));
  }

  handleError(this->lo);

  return nullptr;
}

ActualType *RecordType::transform(SemanticResult *env, struct DeclareList *decs) {
  auto tmp = this->record;
  auto field_list = (FieldTypeList *) nullptr;

  while (tmp) {
    auto type = getActualType(tmp->head->type, env, decs, false);
    if (type) {
      auto field = new FieldType(tmp->head->name, type);
      field_list = new FieldTypeList(field, field_list);
    } else {
      sprintf(sem, "type %s is not defined", S_name(tmp->head->type));
      break;
    }
    tmp = tmp->tail;
  }

  if (tmp) { // error happened
    handleError(this->lo);
    return nullptr;
  } else {
    return new ActualRecord(field_list);
  }
}

ActualType *ArrayType::transform(SemanticResult *env, struct DeclareList *decs) {
  auto type = getActualType(this->array, env, decs, true);

  if (type) {
    return new ActualArray(type);
  } else {
    sprintf(sem, "type %s is not defined", S_name(this->array));
  }

  handleError(this->lo);

  return nullptr;
}

DeclareKind FunctionDeclare::getKind() {
  return FunctionDK;
}

DeclareKind VarDeclare::getKind() {
  return VarDK;
}

DeclareKind TypeDeclare::getKind() {
  return TypeDK;
}

SemanticResult *TypeDeclare::preprocess(SemanticResult *env) {
  auto looked_type = env->typ_table->lookup(this->name);

  if (looked_type) {
    sprintf(sem, "duplicate identifier `%s`", S_name(this->name));
  } else {
    return new SemanticResult(
        env->val_table,
        env->typ_table->updateImmutable(this->name, new ActualNone()),
        new ActualVoid()
    );
  }

  handleError(this->lo);

  return env->copy(nullptr);
}
}

/** env init bases */
namespace ast {
SemanticResult *makeBaseEnvTable() {
  auto value_table = new BinaryTree();
  auto type_table = new BinaryTree();

  type_table->update(Symbol4("int"), new ActualInt());
  type_table->update(Symbol4("string"), new ActualString());
  type_table->update(Symbol4("real"), new ActualReal());

  return new ast::SemanticResult(value_table, type_table, nullptr);
}
}