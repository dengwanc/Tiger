#include <iostream>
#include <vector>

using namespace std;

vector<function<void()>> tests;

void run() {
        for (auto f: tests) {
                f();
        }
}

int main()
{
        cout << "---Running Tests" << endl;
        run();
        cout << "---" <<  tests.size() << " passing" << endl;

        return 0;
}
