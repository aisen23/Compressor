#include "pch.h"

#include "ThreadPool.h"

ai::ThreadPool::ThreadPool(size_t numThreads)
{
    for (size_t i = 0; i < numThreads; ++i) {
        _workers.emplace_back([this] {
            while (true) {
                std::packaged_task<void()> task;

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
        std::guard_lock lock(_tasksMutex);
        _stop = true;
    }

    _tasksCond.notify_all();
    for (std::thread& worker : _workers) {
        _worker.join();
    }
}
