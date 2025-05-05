#ifndef CHESS_PRIORITYQUEUE_H
#define CHESS_PRIORITYQUEUE_H
#include <iostream>
#include <list>
#include "EmptyQueueException.h"

/**
 * Comparator struct for priority queue sorting.
 * @tparam T - priority queue element type, must implement operator >
 */
template <class T>
struct MyComparator {
    T operator()(T a, T b) const {
        return a > b;
    }
};

/**
 * Priority queue class
 * @tparam T - elements type
 */
template <class T>
class PriorityQueue{
private:
    std::list<T> elements;
    MyComparator<T> comparator;
public:
    /**
     * Push element into the priority queue in O(n)
     * @param element(T) - element to push
     */
    void push(T element) {
        auto it = elements.begin();
        while (it != elements.end() && comparator(*it, element)) {
            it++;
        }
        elements.insert(it, element);
    }

    /**
     * Pull the top priority element from the queue in O(1)
     * @return T - top priority element
     */
    T pull() {
        if (elements.empty()) {
            throw EmptyQueueException("Queue is empty!");
        }

        T topElement = elements.front();
        elements.pop_front();
        return topElement;
    }
};

#endif //CHESS_PRIORITYQUEUE_H
