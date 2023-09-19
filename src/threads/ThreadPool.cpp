#include "pch.h"

#include "ThreadPool.h"

ai::ThreadPool& ai::ThreadPool::Instance() {
    static ThreadPool instance;
    return instance;
}

ai::ThreadPool::ThreadPool()
{
    /*int numThreads = std::thread::hardware_concurrency() - 1;
    if (numThreads < 4) {
        numThreads = 8;
    }*/

    int numThreads = 1;

    for (size_t i = 0; i < numThreads; ++i) {
        _workers.emplace_back([this] {
            while (true) {
                std::function<void()> task;

                {
                    std::unique_lock lock(_tasksMutex);
                    _tasksCond.wait(lock, [this] { return _stop || !_tasks.empty(); });

                    if (_stop && _tasks.empty()) {
                        return;
                    }

                    task = std::move(_tasks.front());
                    _tasks.pop();
                }

                task();
            }
        });
    }
}

ai::ThreadPool::~ThreadPool()
{
    {
        std::lock_guard lock(_tasksMutex);
        _stop = true;
    }

    _tasksCond.notify_all();
    for (std::thread& worker : _workers) {
        worker.join();
    }
}
