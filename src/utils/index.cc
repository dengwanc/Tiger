#include <iostream>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "index.h"

void *checkedMalloc(size_t len)
{
	void *p = malloc(len);

	if (p == nullptr) throw OUT_OF_MEMORY;

	return p;
}

char* String(const char *s)
{
	if (s == nullptr) throw CONST_STRING;

	char* p = (char *)checkedMalloc(strlen(s) + 1);
	strcpy(p, s);
	return p;
}

bool streq(const char* s1, const char* s2)
{
	return strcmp(s1, s2) == 0;
}

void it(const char *s, std::function<void()> f)
{
	f();
	std::cout << "         ✅  " << s << std::endl;
}

int describe(const char *s, std::function<void()> f)
{
	std::cout << s << std::endl;
	f();
	return 1;
}

int crash(const char* s)
{
	std::cerr << s << std::endl;
	assert(false);
}

bool empty(const char* s) {
	return s[0] == '\0';
}
