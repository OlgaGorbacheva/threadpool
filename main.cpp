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
    int a = 2, b = 3;
    d f;
    my::threadpool pool;
    std::shared_ptr<my::Data<int>> val(new my::Data<int>());
    pool.add<int>(0, val, sum, 6, 5);
    pool.add<void>(0, hello_world);
    int c = val->get();
    cout << c << endl;
    pool.add<void>(0, f, a, b);
    return 0;
}
