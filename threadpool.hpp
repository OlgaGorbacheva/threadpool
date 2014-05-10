#ifndef THREADPOOL_HPP
#define THREADPOOL_HPP

#include "threadpool.h"

my::threadpool::threadpool(): fn_container(), th_container(), th_count(boost::thread::hardware_concurrency())  //как определить оптимальное число?
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
//    boost::unique_lock<boost::mutex> locker(lock);
    for (unsigned int i = 0; i < th_count; i++) {
        th_container[i]->on = false;
//        th_container[i]->thread.join();
    }
//    cv.notify_all();
}

template<class R, class FN, class... ARGS>
std::shared_ptr<my::Data<R>> my::threadpool::add(FN fn, ARGS... args) {
    if ()
    std::shared_ptr<Data<R>> ReturnData(new Data<R>());
    std::function<R()> rfn = std::bind(fn, args...);
    function pool_fn = [=]() {
        ReturnData->data = rfn();
        ReturnData->ready = true;
    };
    boost::unique_lock<boost::mutex> locker(lock);
    fn_container.push(pool_fn);
    cv.notify_all();
    return ReturnData;
}

/////////////////////////////////////////////
/////////////////////////////////////////////

my::threadpool::work_thread::work_thread(my::threadpool *_pool):
    on(true),
    pool(_pool),
    thread(&my::threadpool::work_thread::exec, this) {
    ;
}

my::threadpool::work_thread::~work_thread() {
    thread.join();
}

void my::threadpool::work_thread::exec() {
    while (on) {
        boost::unique_lock<boost::mutex> locker(pool->lock);
        pool->cv.wait(locker, [&](){return !pool->fn_container.empty() || !on;});
        if (!on)
            break;
        function current = pool->fn_container.front();
        pool->fn_container.pop();
        locker.unlock();
        current();
    }
}


#endif //THREADPOOL_HPP
