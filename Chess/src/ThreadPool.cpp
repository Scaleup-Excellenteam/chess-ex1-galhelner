#include "ThreadPool.h"

ThreadPool::ThreadPool(size_t num_threads) : stop_flag(false), active_tasks(0) {
    for (size_t i = 0; i < num_threads; ++i) {
        workers.emplace_back([this]() {
            while (true) {
                std::function<void()> task;

                {
                    std::unique_lock<std::mutex> lock(queue_mutex);
                    condition.wait(lock, [this]() {
                        return stop_flag || !tasks.empty();
                    });

                    if (stop_flag && tasks.empty()) return;

                    task = std::move(tasks.front());
                    tasks.pop();
                }

                // increment active tasks counter before the work
                ++active_tasks;
                task();
                // decrement active tasks counter after the work
                --active_tasks;

                // notify when a task finishes
                idle_condition.notify_one();
            }
        });
    }
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop_flag = true;
    }
    condition.notify_all();

    for (std::thread &worker : workers)
        worker.join();
}

void ThreadPool::enqueue(std::function<void()> task) {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        tasks.emplace(std::move(task));
    }
    condition.notify_one();
}

void ThreadPool::wait_for_all() {
    std::unique_lock<std::mutex> lock(queue_mutex);
    idle_condition.wait(lock, [this]() {
       return tasks.empty() && active_tasks == 0;
    });
}
