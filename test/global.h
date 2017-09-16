#ifndef TIGER_TEST_GLOBAL
#define TIGER_TEST_GLOBAL

#include <iostream>
#include <string>
#include <vector>

/**
 * This module for common
 * data & function
 * shared in /test dir.
 * Just fot Convenient.
 */

using namespace std;

static const char* TIGER_DEFAULT_INPUT = "../mocks/editable.tig";
static vector<string> TIGS;

static int __init__ = [] {
    for (auto i = 1; i <= 49; i++) {
        TIGS.push_back(string("test") + to_string(i) + string(".tig"));
    }

    TIGS.push_back(string("merge.tig"));
    TIGS.push_back(string("queens.tig"));

    for (auto i = 0; i < TIGS.size(); i++) {
        TIGS[i] = string("../mocks/") + TIGS[i];
    }

    return 0;
}();

#endif
