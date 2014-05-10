#include "threadpool.h"
#include <mutex>
#include <iostream>

using namespace std;

int sum(int a, int b) {
//    cout << a + b << endl;
    return a + b;
}

class d {
public:
    void operator() (int a, int b) {
        cout << a - b << endl;
    }
};

void hello_world() {
    cout << "Hello, world!" << endl;
}

int main() {
//    int a = 2, b = 3;
//    d f;
    my::threadpool pool;
//    my::add_to_pool(pool, &sum, 5, 6);
//    my::add_to_pool(pool, &dif::operator (), 4, 3);
//    my::add_to_pool(pool, &hello_world);
//    my::add_to_pool(pool, &sp_multy, a, b);
    std::shared_ptr<my::Data<int>> val = pool.add<int>(sum, 6, 5);
    while (!val->ready);
    cout << val->data<< endl;
//    pool.add(std::bind(f, 4, 3));
//    pool.add<void, void()>(hello_world);
//    runAsync(&hello_world);
//    pool.add(std::bind(sp_multy, a, b));
    return 0;
}
