#include "threadpool.h"

my::threadpool::threadpool(): fn_container(), th_container(), th_count(std::thread::hardware_concurrency())  //как определить оптимальное число?
{
    for (unsigned int i = 0; i < th_count; i++) {
        th_pointer worker(new work_thread(this));
        th_container.push_back(worker);
    }
}

my::threadpool::threadpool(const unsigned int th_num): fn_container(), th_container(), th_count(th_num)
{
    if (th_count == 0) {
        throw my::threadpool::threadpool_exception("too small number of threads");
    }
    for (unsigned int i = 0; i < th_count; i++) {
        th_pointer worker(new work_thread(this));
        th_container.push_back(worker);
    }
}

my::threadpool::~threadpool(){
    while (!fn_container.empty()) {
        ;
    }
    for (unsigned int i = 0; i < th_count; i++) {
        th_container[i]->on = false;
        cv.notify_one();
//        th_container[i]->thread.join();
    }
}

void my::threadpool::add(function fn) {
    std::unique_lock<std::mutex> locker(lock);
    fn_container.push(fn);
    cv.notify_all();
}

template<class FN, class... ARGS>
void my::add_to_pool(threadpool &pool, FN fn, ARGS... args) {
    pool.add(std::bind(fn, args...));
}
