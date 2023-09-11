#include "pch.h"

#include "clock/Clock.h"
#include "DataUtils.h"

const size_t ARRAY_SIZE = 1000000;

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    auto srcArray = ai::data_utils::CreateAnArray(ARRAY_SIZE);

    // Just for benchmarking.
    ai::Clock clock;
    auto start = clock.Now();



    // Real test (and example) for compressing and decompressing.
    {   
        // Dumb operations.
        long sum = 0;
        for (long i = 0; i != 7000000; ++i) {
            sum += 1;
        }
    }



    clock.PrintDurationFrom(start);

    ai::data_utils::PrintArray("Source array", srcArray);
    //PrintArray("The decompressed array", decompArray);

    std::cin.get();
    return 0;
}
