#include "pch.h"

#include "Compressor.h"
#include "huffman/HuffmanCompressorImpl.h"

std::vector<uint8_t> ai::Compressor::Compress(const std::vector<int>& array) const {
    HuffmanCompressorImpl impl;
    return impl.Compress(array);
}

std::vector<int> ai::Compressor::Uncompress(const std::vector<uint8_t>& buffer) const {
    HuffmanCompressorImpl impl;
    return impl.Uncompress(buffer);
}
