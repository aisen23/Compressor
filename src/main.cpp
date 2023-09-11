#include "pch.h"

#include "clock/Clock.h"
#include "DataUtils.h"

const size_t ARRAY_SIZE = 1000000;

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    auto srcArray = ai::data_utils::CreateAnArray(ARRAY_SIZE);

    // Just for benchmarking.


    char* compressed = nullptr;
    std::vector<int> decompressedArray;
    // Real test (and example) for compressing and decompressing.
    {   
        ai::Clock clock;
        auto startCompressTime = clock.Now();
        
        compressed = ai::data_utils::CompressArray(srcArray);

        std::cout << "Compressing duration: ";
        clock.PrintDurationFrom(startCompressTime);

        auto startDecompressTime = clock.Now();

        decompressedArray = ai::data_utils::DecompressToArray(compressed, ARRAY_SIZE);
        
        std::cout << "Decompressing duration: ";
        clock.PrintDurationFrom(startDecompressTime);
        std::cout << "\n";
    }



    std::cout << "Source array size: " << sizeof(int) * ARRAY_SIZE + sizeof(std::vector<int>) << std::endl;
    std::cout << "Compressed size: " << sizeof(char) * ARRAY_SIZE << "\n\n";

    ai::data_utils::PrintArray("Source array", srcArray);

    ai::data_utils::PrintCompressed(compressed, ARRAY_SIZE);

    ai::data_utils::PrintArray("Decompressed array", decompressedArray);

    if (compressed) {
        delete[] compressed;
    }

    std::cin.get();
    return 0;
}
