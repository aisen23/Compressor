#include "pch.h"

#include "clock/Clock.h"
#include "Compressor.h"
#include "Utils.h"

#include <stdlib.h>

/** All classes and functions optimized for
 * input data of this task (integers from 1 to 100)
 * */

void TestCases();

int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    bool test = true;
    if (test) {
        TestCases();
    }
    else {
        auto srcArray = ai::utils::CreateRandomArrayWithConsecutiveOrRangeElements();

        // Real test (and example) for compressing and uncompressing.
        std::vector<uint8_t> compressed;
        std::vector<unsigned> restoredArray;

        {
            ai::Clock clock;
            {
                auto startCompressTime = clock.Now();

                ai::Compressor compressor(ai::eCompressorType::Compressor);
                compressed = compressor.Compress(srcArray);

                std::cout << "Compressing duration: ";
                clock.PrintDuration(startCompressTime);
            }

            {
                auto startDecompressTime = clock.Now();

                ai::Compressor uncompressor(ai::eCompressorType::Uncompressor);
                restoredArray = uncompressor.Uncompress(compressed);

                std::cout << "Decompressing duration: ";
                clock.PrintDuration(startDecompressTime);
                std::cout << "\n";
            }
        }

        // Benchmark results: 
        std::cout << "Source array size: " << sizeof(unsigned) * srcArray.size() << std::endl;
        std::cout << "Compressed size: " << sizeof(uint8_t) * compressed.size() << "\n\n";

        ai::utils::PrintArray("Source array", srcArray);

        ai::utils::PrintArray("Compressed array", compressed);

        ai::utils::PrintArray("Restored array", restoredArray);

        bool equal = ai::utils::CompareArrays(srcArray, restoredArray);
        assert(equal);
        if (equal) {
            std::cout << "Arrays are equal" << std::endl;
        }
    }

    std::cin.get();
    return 0;
}

void TestCases() {
    std::vector<std::vector<unsigned>> tests = { 
        { 10,9,8,7,6,5,6,7,8,9,10 },
        { 10,9,8,7,6,5,1,2,1,2,1 },
        { 1,2,1,2,1,2,3,4,5 },
        { 1,2,1,1,1,1,1,2,1 },
        { 1,2,1,1,1,1,1,1,1,1,1,1,1,1,2,1 },
        { 6,5,4,3,2,1,1,1,1,1,1,1,1,1,1,1,1,2,3,4,5,6 },
        { 1,2,3,4,5,6,6,6,6,6,6,6,6,6,6,5,4,3,2,1 },
        { 6,6,6,6,6,6,6,6,6,6,1,8,3,3,15 },
        { 6,6,6,6,6,6,6,6,6,6,1,2,3,4,5,6,7,8,8,8,8,8,8,8,8,8,8,8 },
        { 1,8,3,3,15,6,6,6,6,6,6,6,6,6,6 },
        { 1,8,3,3,9,10,11,12,13,14,15,6,6,6,6,6,6,6 },
        { 1,8,3,3,15,6,6,6,6,6,6 },
        { 1,8,3,6,7,6,6,6,6,6,6,6,7 },
        { 1,8,3,6,7,6,6,6,6,6,6,7 }
    };

    for (const auto& srcArray : tests) {
        // Real test (and example) for compressing and uncompressing.
        std::vector<uint8_t> compressed;
        std::vector<unsigned> restoredArray;

        {
            ai::Clock clock;
            {
                auto startCompressTime = clock.Now();

                ai::Compressor compressor(ai::eCompressorType::Compressor);
                compressed = compressor.Compress(srcArray);

                std::cout << "Compressing duration: ";
                clock.PrintDuration(startCompressTime);
            }

            {
                auto startDecompressTime = clock.Now();

                ai::Compressor uncompressor(ai::eCompressorType::Uncompressor);
                restoredArray = uncompressor.Uncompress(compressed);

                std::cout << "Decompressing duration: ";
                clock.PrintDuration(startDecompressTime);
                std::cout << "\n";
            }
        }



        // Benchmarking: 
        std::cout << "Source array size: " << sizeof(unsigned) * srcArray.size() << std::endl;
        std::cout << "Compressed size: " << sizeof(uint8_t) * compressed.size() << "\n\n";

        ai::utils::PrintArray<unsigned, unsigned>("Source array", srcArray);

        ai::utils::PrintArray<uint8_t, unsigned>("Compressed", compressed);

        ai::utils::PrintArray<unsigned, unsigned>("Restored array", restoredArray);

        bool equal = ai::utils::CompareArrays(srcArray, restoredArray);
        assert(equal);
        if (equal) {
            std::cout << "Arrays are equal\n\n\n\n";
        }
    }
}
