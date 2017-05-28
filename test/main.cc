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
        cout << "\n---Running Tests\n" << endl;
        run();
        cout << "\n---" <<  tests.size() << " passing\n" << endl;

        return 0;
}
