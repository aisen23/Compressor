#include "pch.h"

#include "clock/Clock.h"
#include "Compressor.h"
#include "DataUtils.h"

const size_t ARRAY_SIZE = 1000000;

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Init random source array.
    auto srcArray = ai::data_utils::CreateAnArray(ARRAY_SIZE);


    // Real test (and example) for compressing and uncompressing.
    std::vector<uint8_t> compressed;
    std::vector<int> decompressedArray;

    {   
        ai::Clock clock;
        {
            auto startCompressTime = clock.Now();

            ai::Compressor compressor();
            compressed = compressor.Compress(srcArray);

            std::cout << "Compressing duration: ";
            clock.PrintDurationFrom(startCompressTime);
        }

        {
            auto startDecompressTime = clock.Now();

            ai::Uncompressor uncompressor();
            decompressedArray = uncompressor.Uncompress(compressed);

            std::cout << "Decompressing duration: ";
            clock.PrintDurationFrom(startDecompressTime);
            std::cout << "\n";
        }
    }



    // Benchmarking: 
    std::cout << "Source array size: " << sizeof(int) * srcArray.size() << std::endl;
    std::cout << "Compressed size: " << sizeof(uint8_t) * compressed.size() << "\n\n";

    ai::data_utils::PrintArray<int>("Source array", srcArray);

    ai::data_utils::PrintArray<uint8_t>("Compressed array", compressed);

    ai::data_utils::PrintArray<int>("Uncompressed array", decompressedArray);

    std::cin.get();
    return 0;
}
