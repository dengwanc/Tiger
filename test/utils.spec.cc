#include <iostream>
#include <cassert>
#include "../src/utils/index.h"
#include "../src/errormsg.h"

using namespace std;


static int __TIGER_UNIT_TEST = describe("utils", [] {
	it("String cannot constructor nullptr", [] {
		try {
			String(nullptr);
		} catch(const char* msg) {
			assert(strcmp(msg, CONST_STRING) == 0);
		}
	});

	it("String constructor should be correct", [] {
		auto tpl = "XML";
		auto s = String(tpl);
		assert(strcmp(s, tpl) == 0);
	});

	it("if malloc cannot alloc throw error", [] {
		try {
			while (1) {
				checked_malloc(1073741824);
			}
			assert(0);
		} catch(const char* msg) {
			assert(strcmp(msg, OUT_OF_MEMORY) == 0);
		}
	});
});
