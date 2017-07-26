#ifndef TIGER_UTILS_SYMBOL
#define TIGER_UTILS_SYMBOL

/*
 * symbol.h - Symbols and symbol-tables
 *
 */
typedef struct S_symbol_ *Symbol;

/* Make a unique symbol from a given string.
 *  Different calls to S_Symbol("foo") will yield the same Symbol
 *  value, even if the "foo" strings are at different locations. */
Symbol Symbol4(char *s);
Symbol Symbol4(const char* s);

/* Extract the underlying string from a symbol */
char *S_name(Symbol);

/* S_table is a mapping from Symbol->any, where "any" is represented
 *     here by void*  */
// typedef struct TAB_table_ *S_table;

/* Make a new table */
// S_table S_empty(void);

/* Enter a binding "sym->value" into "t", shadowing but not deleting
 *    any previous binding of "sym". */
// void S_enter(S_table t, Symbol sym, void *value);

/* Look up the most recent binding of "sym" in "t", or return NULL
 *    if sym is unbound. */
// void* S_look(S_table t, Symbol sym);

/* Start a new "scope" in "t".  Scopes are nested. */
// void S_beginScope(S_table t);

/* Remove any bindings entered since the current scope began,
   and end the current scope. */
// void S_endScope(S_table t);

bool S_lessthan(Symbol s1, Symbol s2);
bool S_greaterthan(Symbol s1, Symbol s2);

#endif
