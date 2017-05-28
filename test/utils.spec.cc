#include <iostream>
#include <vector>
#include <cassert>
#include <cstring>
#include "../src/utils/index.h"
#include "../src/errormsg.h"

using namespace std;

extern vector<function<void()>> tests;

static int __ = [] {
	tests.push_back([] {
		try {
			String(nullptr);
		} catch(const char* msg) {
			assert(strcmp(msg, CONST_STRING) == 0);
		}
	});

	tests.push_back([] {
		const char* tpl = "XML";
		char* s = String(tpl);
		assert(strcmp(s, tpl) == 0);
	});

	return 0;
}();


