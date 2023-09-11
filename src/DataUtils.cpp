#include "pch.h"

#include "DataUtils.h"

std::vector<int> ai::data_utils::CreateAnArray(size_t size) {
    std::vector<int> newArray;
    newArray.reserve(size);
    
    for (size_t i = 0; i < size; ++i) {
        newArray.push_back(rand() % 100 + 1);
    }

    return newArray;
}
// TODO: change void to some type.
void ai::data_utils::CompressArray(const std::vector<int>& srcArray) {
}

// TODO:
std::vector<int> ai::data_utils::DecompressArray(int* dataToDecompress) {
    return std::vector<int>();
}

void ai::data_utils::PrintArray(const char* nameOfArray, const std::vector<int>& arrayToPrint) {
    std::cout << nameOfArray << ": ";

    size_t size = arrayToPrint.size();
    size_t count = 15;

    for (size_t i = 0; i != count; ++i) {
        assert(i < size);
        std::cout << arrayToPrint[i] << ", ";
    }

    std::cout << "....... ";

    for (size_t i = size / 2; i != size / 2 + count; ++i) {
        assert(i < size);
        std::cout << arrayToPrint[i] << ", ";
    }

    std::cout << "....... ";

    for (size_t i = size - count - 1; i != size; ++i) {
        assert(i < size);
        std::cout << arrayToPrint[i] << ", ";
    }

    std::cout << "\n";
}
