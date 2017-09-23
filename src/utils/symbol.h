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
Symbol Symbol4(const char *s);

/* Extract the underlying string from a symbol */
char *S_name(Symbol);
bool S_greaterthan(Symbol s1, Symbol s2);

#endif
