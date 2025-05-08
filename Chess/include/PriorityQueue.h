#ifndef CHESS_PRIORITYQUEUE_H
#define CHESS_PRIORITYQUEUE_H
#include <iostream>
#include <list>
#include "EmptyQueueException.h"
#include "Move.h"
using namespace std;

// Forward declaration
template<typename T>
class PriorityQueue;
template<typename T>
std::ostream& operator<<(std::ostream& os, PriorityQueue<T> pq);

/**
 * Comparator struct for priority queue sorting.
 * @tparam T - priority queue element type, must implement operator >
 */
template <class T>
struct MyComparator {
    bool operator()(T a, T b) const {
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

    friend ostream& operator<< <>(std::ostream& os, PriorityQueue<T> pq);
};

template<typename T>
std::ostream& operator<<(std::ostream& os, PriorityQueue<T> pq) {
    os << "Recommended moves:\n";
    for (int i = 0; i < 3; i++) {
        try {
            T move = pq.pull();
            char srcRow = 'a' + move.source.first;
            int srcCol = move.source.second + 1;
            char destRow = 'a' + move.destination.first;
            int destCol = move.destination.second + 1;

            os << i + 1 << ". " << srcRow << srcCol << destRow << destCol << "\n";
        } catch (EmptyQueueException& e) {
            cerr << e.what() << endl;
        }
    }
    return os;
}

#endif //CHESS_PRIORITYQUEUE_H
