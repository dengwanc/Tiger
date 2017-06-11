#include <iostream>
#include <cassert>
#include <cstring>
#include <cstdlib>
#include "../src/utils/index.h"

using namespace std;


static int __TIGER_UNIT_TEST = describe("utils", [] {

	it("String constructor should be correct", [] {
		auto tpl = "XML";
		auto s = String(tpl);
		assert(strcmp(s, tpl) == 0);
	});

	it("alloc memory should be correct", [] {
		free(checked_malloc(1024));
	});

	it("streq shoud compare two strings correct", [] {
		assert(streq("1234", "1234"));
		assert(streq("1234", "12344") == false);
	});
});
