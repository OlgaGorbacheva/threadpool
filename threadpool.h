#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <functional>
#include <memory>
#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace my {
typedef std::function<void()> function;

////припилить контейнер шаблоном
class threadpool;

template<class _FN, class... _ARGS>
void add_to_pool(my::threadpool &pool, _FN _fn, _ARGS... _args);
}

class my::threadpool {
private:
    class work_thread; // собственно класс рабочего процесса
    typedef std::shared_ptr<work_thread> th_pointer;

    std::queue<my::function> fn_container; // контейнер с функциями
    std::vector<th_pointer> th_container; // контейнер с рабочими процессами
    unsigned int const th_count;   // кол-во рабочих процессов
    std::mutex lock; //сколько нам нужно mutex-ов?
    std::condition_variable cv;

public:
    friend class work_thread;

    class threadpool_exception;

    threadpool();
    threadpool(unsigned int const);
    ~threadpool();

    void add(my::function fn);
};

class my::threadpool::work_thread {
private:
    bool on;
    my::threadpool *pool;
    std::thread thread;

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

#endif // THREADPOOL_H
