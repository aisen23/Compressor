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
char* ai::data_utils::CompressArray(const std::vector<int>& srcArray) {
    char* compressed = new char[srcArray.size()];

    for (size_t i = 0; i < srcArray.size(); ++i) {
        compressed[i] = static_cast<char>(srcArray[i]);
    }

    return compressed;
}

// TODO:
std::vector<int> ai::data_utils::DecompressToArray(char* compressed, size_t size) {
    std::vector<int> decompressed(size);

    for (size_t i = 0; i < size; ++i) {
        decompressed[i] = static_cast<int>(compressed[i]);
    }

    return decompressed;
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

void ai::data_utils::PrintCompressed(char* compressed, size_t size) {
    std::cout << "Compressed: ";

    size_t count = 15;

    for (size_t i = 0; i != count; ++i) {
        assert(i < size);
        std::cout << compressed[i] << ", ";
    }

    std::cout << "....... ";

    for (size_t i = size / 2; i != size / 2 + count; ++i) {
        assert(i < size);
        std::cout << compressed[i] << ", ";
    }

    std::cout << "....... ";

    for (size_t i = size - count - 1; i != size; ++i) {
        assert(i < size);
        std::cout << compressed[i] << ", ";
    }

    std::cout << "\n";
}
