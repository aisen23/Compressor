#include "pch.h"

#include "HuffmanCompressorImpl.h"

#include "HuffmanTree.h"
#include "Utils.h"

std::vector<uint8_t> ai::HuffmanCompressorImpl::Compress(const std::vector<unsigned>& arr) {
    std::vector<uint8_t> arr8(arr.size());
    for (size_t i = 0; i != arr.size(); ++i) {
        arr8[i] = static_cast<uint8_t>(arr[i]);
    }

    HuffmanTree tree;
    return tree.Encode(arr8);
}

std::vector<unsigned> ai::HuffmanCompressorImpl::Uncompress(const std::vector<uint8_t>& data, size_t offset) {
    if (data.size() < 2) {
        return {};
    }

    HuffmanTree tree;
    auto arr8 = tree.Decode(data, offset);

    std::vector<unsigned> arr(arr8.size());
    for (size_t i = 0; i != arr8.size(); ++i) {
        arr[i] = static_cast<unsigned>(arr8[i]);
    }

    return arr;
}

std::vector<uint8_t> ai::HuffmanCompressorImpl::CompressCombined(const std::vector<uint8_t>& bytes) {
    HuffmanTree tree;
    return tree.Encode(bytes);
}


