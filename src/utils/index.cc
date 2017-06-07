#include <iostream>
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

void it(const char *s, std::function<void()> f)
{
	f();
	std::cout << "        " << s << " ✅" << std::endl;
}

int describe(const char *s, std::function<void()> f)
{
	std::cout << s << std::endl;
	f();
	return 1;
}
