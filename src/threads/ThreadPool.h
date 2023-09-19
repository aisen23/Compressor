#pragma once

#include <vector>
#include <queue>
#include <thread>
#include <future>
#include <functional>
#include <mutex>
#include <condition_variable>

namespace ai
{
    class ThreadPool
    {
    public:
        static ThreadPool& Instance();
        ~ThreadPool();

        template <typename Function, typename... Args>
        auto Submit(Function&& f, Args&&... args) -> std::future<decltype(f(args...))> {
            using ReturnType = decltype(f(args...));
            std::future<ReturnType> result;
            {
                std::lock_guard lock(_tasksMutex);
                if (_stop) {
                    return result;
                }

                auto task = std::make_shared<std::packaged_task<ReturnType()> >(
                        std::bind(std::forward<Function>(f), std::forward<Args>(args)...)
                        );

                result = task->get_future();
                _tasks.emplace([task]() { (*task)(); });
            }

            _tasksCond.notify_one();
            return result;
        }

    private:
        explicit ThreadPool();

    private:
        std::vector<std::thread> _workers;
        std::queue<std::function<void()> > _tasks;

        std::mutex _tasksMutex;
        std::condition_variable _tasksCond;
        bool _stop = false;

    private:
        ThreadPool(const ThreadPool&) = delete;
        ThreadPool& operator=(const ThreadPool&) = delete;
    };
} // ai
