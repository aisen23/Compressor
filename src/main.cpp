#include "pch.h"

#include "clock/Clock.h"

int main() {
    std::cout << "Test1 is Started.\n";

    ai::Clock clock;
    auto start = clock.Now();

    int sum = 0;
    for (int i = 0; i != 1000000; ++i) {
        sum += 1;
    }

    clock.PrintDurationFrom(start);

    std::cin.get();
    return 0;
}
