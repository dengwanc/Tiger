#ifndef TIGER_UTILS
#define TIGER_UTILS

#include<stddef.h>
#include<iostream>

char* String(const char *s);
int crash(const char* s);
void* checkedMalloc(size_t len);
bool streq(const char* s1, const char* s2);
bool empty(const char* s);
void it(const char *s, std::function<void()> f);
int describe(const char *s, std::function<void()> f);

/**
 * crash information
 * usage::
 * ```
 * OUT_OF_MEMORY;
 * // or
 * throw OUT_OF_MEMORY;
 * ```
 */
#define OUT_OF_MEMORY crash("Out Of Memory")
#define NULLPTR crash("Catch Illegal Nullptr")
#define CONST_STRING crash("Const String Required")
#define CONST_ARRAY crash("Array Length Over the MAX")
#define UNKNOWN_TYPE crash("Unknown Type")
#define UNSUPPORT_TYPE crash("Unsupport Type")
#define MUST_FIND crash("Must find Value in Collected Got Nothing")
#define MUST_OPEN_FILE crash("Must Open File Got Error")
#define COMPILER_STRING_OVER crash("String Over The Max Size 4Kb")

#endif
