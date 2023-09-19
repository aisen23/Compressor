#include "pch.h"

#include "Clock.h"

ai::Clock::TimePoint ai::Clock::Now() const {
    return std::chrono::steady_clock::now();
}

void ai::Clock::PrintDuration(const TimePoint& from) const {
    auto now = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration<float, std::milli>(now - from);
    float durationFloat = duration.count();

    std::cout << "Time elapsed: " << durationFloat << " ms\n";
}
