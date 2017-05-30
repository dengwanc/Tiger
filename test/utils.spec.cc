#include <iostream>
#include <vector>
#include <cassert>
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
		auto tpl = "XML";
		auto s = String(tpl);
		assert(strcmp(s, tpl) == 0);
	});

    tests.push_back([] {
        try {
            while (1) {
                checked_malloc(1073741824);
            }
            assert(0);
        } catch(const char* msg) {
            assert(strcmp(msg, OUT_OF_MEMORY) == 0);
        }
    });

	return 0;
}();


