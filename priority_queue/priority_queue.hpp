#ifndef P_QUEUE_CPP
#define P_QUEUE_CPP

#include "priority_queue.h"

//////////////////////////////////node/////////////////////////////////////

template <class KeyT, class ValueT>
my::node<KeyT, ValueT>::node(KeyT k, ValueT v) {
    key = k;
    value = v;
    n_mutex.unlock();
}

template <class KeyT, class ValueT>
my::node<KeyT, ValueT>::~node() {
    boost::lock_guard<boost::detail::spinlock> lock(n_mutex);
}

//template <class KeyT, class ValueT>
//KeyT my::node<KeyT, ValueT>::get_key() {
//    return key;
//}

//template <class KeyT, class ValueT>
//ValueT my::node<KeyT, ValueT>::get_value() {
//    return value;
//}

//template <class KeyT, class ValueT>
//ValueT my::node<KeyT, ValueT>::set_value(ValueT _value) {
//    boost::lock_guard<boost::detail::spinlock> lock(node_lock);
//    std::swap(_value, value);
//    return _value;
//}

//////////////////////////////////queue//////////////////////////////////

template <class KeyT, class ValueT>
my::priority_queue<KeyT, ValueT>::priority_queue(): on(true) {
    ;
}

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
    boost::unique_lock<boost::detail::spinlock> lock(heap[i].n_mutex);
    boost::unique_lock<boost::detail::spinlock> left_lock(heap[left(i)].n_mutex);
    if (i != left(i) && heap[i].key < heap[left(i)].key) {
        std::swap(heap[i], heap[left(i)]);
        lock.unlock();
        left_lock.unlock();
        down_heapify(left(i));
        return;
    }
    left_lock.unlock();
    boost::unique_lock<boost::detail::spinlock> right_lock(heap[right(i)].n_mutex);
    if (i != right(i) && heap[i].key < heap[right(i)].key) {
        std::swap(heap[i], heap[right(i)]);
        lock.unlock();
        right_lock.unlock();
        down_heapify(right(i));
        return;
    }
    lock.unlock();
    right_lock.unlock();
}

template <class KeyT, class ValueT>
void my::priority_queue<KeyT, ValueT>::up_heapify(size_t i) {
    if (i != parent(i) && heap[i].key > heap[parent(i)].key){
        std::swap(heap[i], heap[right(i)]);
        up_heapify(parent(i));
    }
}

template <class KeyT, class ValueT>
void my::priority_queue<KeyT, ValueT>::put(const ValueT &value, const KeyT &key) {
    boost::upgrade_lock<boost::shared_mutex> lock(sh_m);
    boost::upgrade_to_unique_lock<boost::shared_mutex> u_lock(lock);
    node<KeyT, ValueT> new_n(key, value);
    heap.push_back(new_n);
    up_heapify(heap.size() - 1);
    cv.notify_one();
}

template <class KeyT, class ValueT>
bool my::priority_queue<KeyT, ValueT>::get(ValueT& result) {
    boost::shared_lock<boost::shared_mutex> sh_lock(sh_m);
    boost::unique_lock<boost::mutex> f_lock(first);
    cv.wait(sh_lock, [this]()->bool {
        return !heap.empty() || !on;
    });
    if (!heap.empty()) {
        if ((heap.size() > 1))
            std::swap(heap[0], heap[heap.size() - 1]);
        result = heap[heap.size() - 1].value;
        heap.pop_back();
        int i = 0;
        boost::unique_lock<boost::detail::spinlock> lock(heap[i].n_mutex);
        boost::unique_lock<boost::detail::spinlock> left_lock(heap[left(i)].n_mutex);
        if (i != left(i) && heap[i].key < heap[left(i)].key) {
            std::swap(heap[i], heap[left(i)]);
            lock.unlock();
            left_lock.unlock();
            f_lock.unlock();
            down_heapify(left(i));
        } else {
            left_lock.unlock();
            boost::unique_lock<boost::detail::spinlock> right_lock(heap[right(i)].n_mutex);
            if (i != right(i) && heap[i].key < heap[right(i)].key) {
                std::swap(heap[i], heap[right(i)]);
                lock.unlock();
                right_lock.unlock();
                f_lock.unlock();
                down_heapify(right(i));
            } else {
                lock.unlock();
                right_lock.unlock();
            }
        }
        return true;
    }
    return false;
}

template <class KeyT, class ValueT>
void my::priority_queue<KeyT, ValueT>::finish() {
    on = false;
}

template <class KeyT, class ValueT>
bool my::priority_queue<KeyT, ValueT>::is_finished() {
    return !on && heap.empty();
}

#endif //P_QUEUE_CPP
