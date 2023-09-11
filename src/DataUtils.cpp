#include "pch.h"

#include "DataUtils.h"

std::vector<int> ai::utils::CreateAnArray(size_t size) {
    std::vector<int> newArray;
    newArray.reserve(size);
    
    for (size_t i = 0; i < size; ++i) {
        newArray.push_back(rand() % 100 + 1);
    }

    return newArray;
}

template <typename T>
void ai::utils::PrintArray(const char* nameOfArray, const std::vector<T>& array) {
    std::cout << nameOfArray << ": ";

    size_t size = array.size();
    size_t count = 15;

    for (size_t i = 0; i != count; ++i) {
        assert(i < size);
        std::cout << array[i] << ", ";
    }

    std::cout << "....... ";

    for (size_t i = size / 2; i != size / 2 + count; ++i) {
        assert(i < size);
        std::cout << array[i] << ", ";
    }

    std::cout << "....... ";

    for (size_t i = size - count - 1; i != size; ++i) {
        assert(i < size);
        std::cout << array[i] << ", ";
    }

    std::cout << "\n";
}
