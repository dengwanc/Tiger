#include<string>
#include <cstdlib>
#include "../errormsg.h"

void *checked_malloc(int len)
{
	void *p = malloc(len);

	if (p == nullptr) throw OUT_OF_MEMORY;

	return p;
}

char* String(const char *s)
{
	if (s == nullptr) throw CONST_STRING;

	char* p = (char *)checked_malloc(strlen(s) + 1);
	strcpy(p, s);
	return p;
}
