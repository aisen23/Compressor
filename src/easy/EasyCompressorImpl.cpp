#include "pch.h"

#include "EasyCompressorImpl.h"

std::vector<uint8_t> ai::EasyCompressorImpl::Compress(const std::vector<unsigned>& arr) {
    if (arr.empty()) {
        std::cerr << "Error: ai::EasyCompressorImpl::Compress(): array is empty()" << std::endl;
        return {};
    }

    std::vector<uint8_t> data(arr.size());

    // Because elements of arr (ai::MIN_ELEM and ai::MAX_ELEM) fit int uint8_t.
    for (size_t i = 0; i != arr.size(); ++i) {
        data[i] = static_cast<uint8_t>(arr[i]);
        bool isLast = i == arr.size() - 1;
        if (isLast && !_combined) {
            // Last element with the highest bit. It can be useful when we send this info to other machine.
            data[i] |= (1 << 7);
        }
    }
    
    return data;
}

std::vector<unsigned> ai::EasyCompressorImpl::Uncompress(const std::vector<uint8_t>& data, size_t offset) {
    if (data.size() <= offset) {
        std::cerr << "Error: ai::EasyCompressorImpl::Uncompress(): no data" << std::endl;
        return {};
    }

    std::vector<unsigned> arr(data.size() - offset);
    size_t i = offset;
    bool isLast = false;
    while (!isLast) {
        isLast = (data[i] & (1 << 7)) != 0;
        unsigned num = static_cast<unsigned>(data[i]);
        if (isLast) {
            num &= ~(1 << 7);
        }
        arr[i - offset] = num;
        ++i;
    }

    return arr;
}
