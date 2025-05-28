#ifndef CHESS_THREADPOOL_H
#define CHESS_THREADPOOL_H
#include <iostream>
#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>

class ThreadPool {
public:
    ThreadPool(size_t num_threads);

    /**
     * Add new task to the thread pool.
     * @param task - task to add.
     */
    void enqueue(std::function<void()> task);

    ~ThreadPool();

    /**
     * Wait for all thread pool's tasks to finish.
     */
    void wait_for_all();

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queue_mutex;
    std::condition_variable condition;
    std::condition_variable idle_condition;
    std::atomic<bool> stop_flag;
    std::atomic<size_t> active_tasks;
};

#endif //CHESS_THREADPOOL_H
