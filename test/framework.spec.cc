#include <iostream>
#include <vector>
#include <cassert>

using namespace std;

extern vector<function<void()>> tests;

static int __ = [] {
	tests.push_back([] {
		assert(tests.size() == 1);
	});
	return 0;
}();


