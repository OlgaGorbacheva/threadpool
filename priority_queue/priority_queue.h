#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>

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
    std::vector<std::pair<KeyT, ValueT>> heap;
    boost::condition_variable cv;
    boost::shared_mutex sh_m;
    bool end;

    size_t parent(size_t i);
    size_t left(size_t i);
    size_t right(size_t i);

    void up_heapify(); // однопоточный
    void down_heapify(); // многопоточный
public:
    my::priority_queue();
    ~priority_queue();

    void put(ValueT value, KeyT key = 0); // многопоточный // тип?
    bool get(std::shared_ptr<std::pair<KeyT, ValueT>> result);

    void finish();
    bool is_finished();
};

template <class KeyT, class ValueT>
class my::node {
private:
    boost::mutex NodeLock; // заменить на что-то более приемлемое
    KeyT key;
    ValueT value;
public:
    my::node(KeyT k, ValueT v);
    ~node();

    bool lock (); // возвращает true, если lock прошел успешно
    bool unlock (); // возвращает true, если lock прошел успешно
    KeyT get_key (); // тип?
    ValueT get_value (); // тип?
    ValueT set_value (ValueT new_value);
};
