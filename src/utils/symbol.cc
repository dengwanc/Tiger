#include "index.h"
#include "symbol.h"
#include <assert.h>

struct S_symbol_ {
  char *name;
  Symbol next;
};

static Symbol mksymbol(char *name, Symbol next) {
  Symbol s = (Symbol) checkedMalloc(sizeof(*s));
  s->name = name;
  s->next = next;
  return s;
}

#define SIZE 109  /* should be prime */

static Symbol hashtable[SIZE];

static unsigned int hash(char *s0) {
  unsigned int h = 0;
  char *s;
  for (s = s0; *s; s++)
    h = h*65599 + *s;
  return h;
}

Symbol Symbol4(char *name) {
  int index = hash(name)%SIZE;
  Symbol syms = hashtable[index], sym;
  for (sym = syms; sym; sym = sym->next) {
    if (streq(sym->name, name))
      return sym;
  }
  sym = mksymbol(name, syms);
  hashtable[index] = sym;
  return sym;
}

Symbol Symbol4(const char *s) {
  return Symbol4(String(s));
}

char *S_name(Symbol sym) {
  /** Must Pass A Non-null pointer */
  if (!sym) {
    assert(0);
  }

  return sym->name;
}

bool S_greaterthan(Symbol s1, Symbol s2) {
  if (s1==s2)
    return false;

  char *n1 = s1->name;
  char *n2 = s2->name;
  for (; *n1 && *n2; n1++, n2++) {
    if (*n1 > *n2)
      return true;
    if (*n1 < *n2)
      return false;
  }

  return *n1 ? true : false;
}

