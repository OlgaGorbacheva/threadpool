#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <memory>

using namespace std;

class TThreadFunc {
    size_t Id;
    static mutex Mutex;
public:
    TThreadFunc (size_t id) : Id(id) {}
    ~TThreadFunc() {}

    void operator()() const {
        lock_guard<mutex> guard(Mutex);
        cout << "Hello, World from [" << Id << "]" << endl;
    }
};

mutex TThreadFunc::Mutex;

int main() {
    vector<shared_ptr<thread>> threads;
    size_t thCount = 1000;
    size_t created = 0;
    try {
        for (created = 0; created < thCount; ++created) {
            threads.push_back(make_shared<thread>(TThreadFunc(created)));
        }
    } catch (const std::exception& e) {
        cerr << "exception handled after " << created << " threads." << endl << e.what() << endl;
    }
    for (size_t i = 0; i < created; ++i) {
        threads[i]->join();
    }
    return 0;
}
