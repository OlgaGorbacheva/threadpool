#include "threadpool.h"
#include <mutex>
#include <iostream>

using namespace std;

void sum(int a, int b) {
    cout << a + b << endl;
}

class dif {
public:
    void operator() (int a, int b) {
        cout << a - b << endl;
    }
};

void hello_world() {
    cout << "Hello, world!" << endl;
}

void sp_multy(int &a, int &b) {
    a = a * b;
}

int main() {
    int a = 2, b = 3; {
        dif f;
    my::threadpool pool;
//    my::add_to_pool(pool, &sum, 5, 6);
//    my::add_to_pool(pool, &dif::operator (), 4, 3);
//    my::add_to_pool(pool, &hello_world);
//    my::add_to_pool(pool, &sp_multy, a, b);
    pool.add(std::bind(sum, 6, 5));
    pool.add(std::bind(f, 4, 3));
    pool.add(std::bind(hello_world));
    pool.add(std::bind(sp_multy, a, b));
    }
    cout << a << endl;
    return 0;
}
