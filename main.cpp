#include "threadpool.h"
#include <mutex>
#include <iostream>

using namespace std;

void sum(int a, int b) {
    cout << a + b << endl;
}

void dif(int a, int b) {
    cout << a - b << endl;
}

void hello_world() {
    cout << "Hello, world!" << endl;
}

void sp_multy(int& a, int& b) {
    a = a * b;
}

int main() {
    int a = 16, b = 8; {
    my::threadpool pool;
    my::add_to_pool(pool, &sum, 5, 6);
    my::add_to_pool(pool, &dif, 4, 3);
    my::add_to_pool(pool, &hello_world);
    my::add_to_pool(pool, &sp_multy, a, b);
    }
    cout << a << endl;
    return 0;
}
