#include <iostream>
#include <string>
#include <vector>

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
