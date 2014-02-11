#include "threadpool.h"

my::threadpool::work_thread::work_thread(my::threadpool *_pool): thread(&my::threadpool::work_thread::exec, this), on(true), pool(_pool) {
    ;
}

my::threadpool::work_thread::~work_thread() {
    on = false;
    pool->cv.notify_one();
    thread.join();
}

void my::threadpool::work_thread::exec() {
    while (on) {
        std::unique_lock<std::mutex> locker(pool->lock);
        pool->cv.wait(locker, [&](){return !pool->fn_container.empty();});
        function current = pool->fn_container.front();
        pool->fn_container.pop();
        locker.unlock();
        current();
    }
}
