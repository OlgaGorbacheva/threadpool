#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <functional>
#include <memory>
#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <boost/thread.hpp>

namespace my {
typedef std::function<void()> function;

class threadpool;

//template<class _FN, class... _ARGS>
//void add_to_pool(my::threadpool &pool, _FN _fn, _ARGS... _args);
}

class my::threadpool {
private:
    class work_thread; // собственно класс рабочего процесса
    typedef std::shared_ptr<work_thread> th_pointer;

    std::queue<my::function> fn_container; // контейнер с функциями
    std::vector<th_pointer> th_container; // контейнер с рабочими процессами
    unsigned int const th_count;   // кол-во рабочих процессов
    boost::mutex lock; //сколько нам нужно mutex-ов?
    boost::condition_variable cv;

public:
    friend class work_thread;

    class threadpool_exception;

    threadpool();
    threadpool(unsigned int const);
    ~threadpool();

    template<class _FN, class... _ARGS>
    void add(_FN fn, _ARGS... args);
};

class my::threadpool::work_thread {
private:
    bool on;
    my::threadpool *pool;
    boost::thread thread;

    void exec();

public:
    friend class threadpool;
    work_thread(my::threadpool *_pool);
    ~work_thread();
};

class my::threadpool::threadpool_exception: public std::exception {
private:
    char const *message;
public:
    threadpool_exception(char const *_message): message(_message){
        ;
    }
    const char* what() const noexcept {
        return message;
    }
};

#include "threadpool.hpp"

#endif // THREADPOOL_H
