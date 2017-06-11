#ifndef TIGER_UTILS
#define TIGER_UTILS

#include<iostream>

/**
 * Common Support Lib
 */
char* String(const char *s);
void *checked_malloc(int len);
bool streq(const char* s1, const char* s2);
const char* error(const char* s);

/**
 * Unit Test Support Lib
 */
void it(const char *s, std::function<void()> f);
int describe(const char *s, std::function<void()> f);

/**
 * this part is coding error message
 * for dev debugging
 */
#define OUT_OF_MEMORY error("Out Of Memory")
#define NULLPTR error("Catch Illegal Nullptr")
#define CONST_STRING error("Const String Required")
#define CONST_ARRAY error("Array Length Over the MAX")
#define UNKNOWN_TYPE error("Unknown Type")
#define UNSUPPORT_TYPE error("Unsupport Type")
#define MUST_FIND error("Must find Value in Collected Got Nothing")
#define MUST_OPEN_FILE error("Must Open File Got Error")

/**
 * this part is
 * compiler parsing tig file
 * user error message
 */
#define COMPILER_STRING_OVER error("String Over The Max Size 4Kb")

#endif