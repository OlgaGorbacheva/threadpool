#include "priority_queue.h"

int main() {
    my::node<int, int> n (4,4);
    std::cout << n.get_key() << std::endl;
    return 0;
}
