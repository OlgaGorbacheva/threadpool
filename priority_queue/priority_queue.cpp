#ifndef P_QUEUE_CPP
#define P_QUEUE_CPP

#include "priority_queue.h"

//////////////////////////////////node/////////////////////////////////////

template <class KeyT, class ValueT>
my::node<KeyT, ValueT>::node(KeyT k, ValueT v) {
    key = k;
    value = v;
    node_lock.unlock();
}

template <class KeyT, class ValueT>
my::node<KeyT, ValueT>::~node() {
    boost::lock_guard<boost::detail::spinlock> lock(node_lock);
}

template <class KeyT, class ValueT>
KeyT my::node<KeyT, ValueT>::get_key() {
    return key;
}

template <class KeyT, class ValueT>
ValueT my::node<KeyT, ValueT>::get_value() {
    return value;
}

template <class KeyT, class ValueT>
ValueT my::node<KeyT, ValueT>::set_value(ValueT _value) {
    boost::lock_guard<boost::detail::spinlock> lock(node_lock);
    std::swap(_value, value);
    return _value;
}

//////////////////////////////////queue//////////////////////////////////

//template <class KeyT, class ValueT>
//my::priority_queue<KeyT, ValueT>::priority_queue() {
//}

template <class KeyT, class ValueT>
size_t my::priority_queue<KeyT, ValueT>::parent(size_t i) {
    if (i > 0)
        return (i - 1) / 2;
    return 0;
}

template <class KeyT, class ValueT>
size_t my::priority_queue<KeyT, ValueT>::left(size_t i) {
    if ((i + 1) * 2 - 1 < heap.size())
        return (i + 1) * 2 - 1;
    return i;
}

template <class KeyT, class ValueT>
size_t my::priority_queue<KeyT, ValueT>::right(size_t i) {
    if ((i + 1) * 2 < heap.size())
        return (i + 1) * 2;
    return i;
}

template <class KeyT, class ValueT>
void my::priority_queue<KeyT, ValueT>::down_heapify(size_t i) {

}

//template <class KeyT, class ValueT>
//template <class KeyT, class ValueT>
//template <class KeyT, class ValueT>
//template <class KeyT, class ValueT>

#endif //P_QUEUE_CPP
