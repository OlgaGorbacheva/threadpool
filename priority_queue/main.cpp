#include "priority_queue.h"

using namespace std;

int main() {
    my::priority_queue<int, int> h;
//    std::vector<my::node<int, int>> v;
//    my::node<int, int> nn = {0, 0};
//    v.push_back(nn);
//    nn = {1, 1};
//    v.push_back(nn);
    h.put(0, 1);
    h.put(1, 2);
    h.put(2, 4);
    h.put(3, 2);
    h.put(4, 0);
    h.put(5);
    h.put(6, 3);
    int a[7];
    h.finish();
    for (int i = 0; i < 8; i++) {
        h.get(a[i]);
        cout << a[i] << endl;
    }
    return 0;
}
