#ifndef TIGER_UTILS
#define TIGER_UTILS

#include<iostream>

/**
 * Common Support Lib
 */
char* String(const char *s);
void *checked_malloc(int len);

/**
 * Unit Test Support Lib
 */
void it(const char *s, std::function<void()> f);
int describe(const char *s, std::function<void()> f);

#endif