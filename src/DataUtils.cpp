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
