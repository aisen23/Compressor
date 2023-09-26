#include "pch.h"

#include "Clock.h"

ai::Clock::TimePoint ai::Clock::Now() const {
#ifdef BENCHMARKING
    return std::chrono::steady_clock::now();
#endif
}

void ai::Clock::PrintDuration(const TimePoint& from, const char* name) const {
#ifdef BENCHMARKING
    auto now = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration<float, std::milli>(now - from);
    float durationFloat = duration.count();

    std::cout << name << " duration: " << durationFloat << " ms\n";
#endif
}
