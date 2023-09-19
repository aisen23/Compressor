#include "pch.h"

#include "HuffmanCompressorImpl.h"

#include "HuffmanTree.h"
#include "Utils.h"

ai::HuffmanCompressorImpl::HuffmanCompressorImpl()
    : CompressorImpl(eCompressorImplType::Huffman)
{}

std::vector<uint8_t> ai::HuffmanCompressorImpl::Compress(const std::vector<unsigned>& arr) {
    auto chars = utils::UintArrayToCharArray(arr);

    HuffmanTree tree;
    auto data = tree.Encode(chars);

    return data;
}

std::vector<unsigned> ai::HuffmanCompressorImpl::Uncompress(const std::vector<uint8_t>& data, size_t offset) {
    if (data.size() < 2) {
        return {};
    }

    HuffmanTree tree;
    auto chars = tree.Decode(data, offset);

    return utils::CharArrayToUintArray(chars);
}


