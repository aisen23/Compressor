#pragma once

#include <chrono>

namespace ai
{
    class Clock
    {
    public:
        using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;

    public:
        Clock() = default;

        TimePoint Now() const;
        void PrintDurationFrom(const TimePoint& from) const;

    private:
        Clock(const Clock&) = delete;
        Clock& operator=(const Clock&) = delete;
    };
} // ai
