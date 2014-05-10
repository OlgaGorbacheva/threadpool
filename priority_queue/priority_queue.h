#ifndef P_QUEUE_H
#define P_QUEUE_H

#include <boost/thread.hpp>
#include <boost/smart_ptr/detail/spinlock.hpp>
#include <vector>
#include <memory>

namespace my {

template <class KeyT, class ValueT>
class priority_queue;

template <class KeyT, class ValueT>
class node;

}

template <class KeyT, class ValueT>
class my::priority_queue {
private:
    std::vector<my::node<KeyT, ValueT>> heap;
    boost::condition_variable cv;
    boost::shared_mutex sh_m;
    bool end;

    size_t parent(size_t i);
    size_t left(size_t i);
    size_t right(size_t i);

    void up_heapify(size_t i); // однопоточный
    void down_heapify(size_t i); // многопоточный
public:
    priority_queue();
    ~priority_queue();

    void put(ValueT value, KeyT key = 0); // многопоточный // тип?
    bool get(std::shared_ptr<std::pair<KeyT, ValueT>> result); // однопоточный

    void finish();
    bool is_finished();
};

template <class KeyT, class ValueT>
class my::node {
private:
    KeyT key;
    ValueT value;
public:
    node(KeyT k, ValueT v);
    ~node();

    KeyT get_key ();
    ValueT get_value ();
    ValueT set_value (ValueT new_value);

    boost::detail::spinlock node_lock;
};


#include "priority_queue.cpp"

#endif //P_QUEUE_H
