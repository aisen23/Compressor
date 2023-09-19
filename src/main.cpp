#include "pch.h"

#include "clock/Clock.h"
#include "Compressor.h"
#include "Utils.h"

#include <stdlib.h>

/** All classes and functions optimized for
 * input data of this task (integers from 1 to 100)
 * */

int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // Init random source array.
    auto srcArray = ai::utils::CreateRandomArray();


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

    ai::utils::PrintArray("Source array", srcArray);

    ai::utils::PrintArray("Compressed array", compressed);

    ai::utils::PrintArray("Uncompressed array", restoredArray);

    std::cin.get();
    return 0;
}
