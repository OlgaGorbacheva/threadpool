#include "threadpool.h"

#include <iostream>

my::threadpool::work_thread::work_thread(my::threadpool *_pool):  on(true), pool(_pool), thread(&my::threadpool::work_thread::exec, this) {
    ;
}

my::threadpool::work_thread::~work_thread() {
    thread.join();
}

void my::threadpool::work_thread::exec() {
    while (on) {
        std::unique_lock<std::mutex> locker(pool->lock);
        pool->cv.wait(locker, [&](){return !pool->fn_container.empty() || !on;});
        if (!on)
            break;
        function current = pool->fn_container.front();
        pool->fn_container.pop();
        locker.unlock();
        current();
    }
}
