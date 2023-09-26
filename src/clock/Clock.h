#pragma once

#include <chrono>

namespace ai
{
    /** Class for measuring elapsed time
     * for benchmarking algorithm.
     * */
    class Clock
    {
    public:
        using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;

    public:
        Clock() = default;

        TimePoint Now() const;
        void PrintDuration(const TimePoint& from, const char* name) const;

    private:
        Clock(const Clock&) = delete;
        Clock& operator=(const Clock&) = delete;
    };
} // ai
