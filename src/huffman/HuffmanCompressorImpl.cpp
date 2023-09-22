#include "pch.h"

#include "HuffmanCompressorImpl.h"

#include "HuffmanTree.h"
#include "Utils.h"

ai::HuffmanCompressorImpl::HuffmanCompressorImpl(bool isChar)
    : _isChar(isChar)
{
}

std::vector<uint8_t> ai::HuffmanCompressorImpl::Compress(const std::vector<unsigned>& arr) {
    std::vector<char> chars;
    if (_isChar) {
        // TODO: Check to std::move
        chars = utils::UintArrayToCharArray(arr);
    }
    else {
        chars.resize(arr.size());
        for (size_t i = 0; i != arr.size(); ++i) {
            chars[i] = static_cast<char>(arr[i]);
        }
    }

    HuffmanTree tree(_isChar);
    auto data = tree.Encode(chars);

    return data;
}

std::vector<unsigned> ai::HuffmanCompressorImpl::Uncompress(const std::vector<uint8_t>& data, size_t offset) {
    if (data.size() < 2) {
        return {};
    }

    HuffmanTree tree(_isChar);
    auto chars = tree.Decode(data, offset);

    if (_isChar) {
        return utils::CharArrayToUintArray(chars);
    }

    std::vector<unsigned> arr(chars.size());
    for (size_t i = 0; i != chars.size(); ++i) {
        arr[i] = static_cast<unsigned>(chars[i]);
    }

    return arr;
}


