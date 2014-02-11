#include "threadpool.h"

my::threadpool::threadpool(): fn_container(), th_container(), th_count(std::thread::hardware_concurrency())  //как определить оптимальное число?
{
    for (unsigned int i = 0; i < th_count; i++) {
        th_pointer worker(new work_thread());
        th_container.push_back(worker);
    }
}

my::threadpool::threadpool(const unsigned int th_num): fn_container(), th_container(), th_count(th_num)
{
    if (th_count == 0) {
        throw my::threadpool::threadpool_exception("too small number of threads");
    }
    for (unsigned int i = 0; i < th_count; i++) {
        th_pointer worker(new work_thread());
        th_container.push_back(worker);
    }
}

my::threadpool::~threadpool(){
    ;
}

void my::threadpool::add(function fn) {
    std::lock_guard<std::mutex> locker(lock);
    fn_container.push(fn);
}

template<class FN, class... ARGS>
void my::add_to_pool(threadpool &pool, FN fn, ARGS... args) {
    pool.add(std::bind(fn, args...));
}
