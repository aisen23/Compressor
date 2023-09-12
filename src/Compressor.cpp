#include "pch.h"

#include "Compressor.h"
#include "huffman/HuffmanCompressorImpl.h"

ai::CompressorImpl::CompressorImpl(Compressor* comp)
    : _comp(comp)
{}

ai::Compressor::Compressor(ai::eCompressorType type, size_t numThreads)
    :_type(type)
{
    _pool = new ThreadPool(numThreads);
}

ai::Compressor::~Compressor()
{
    if (_pool) {
        delete _pool;
    }
}

std::vector<uint8_t> ai::Compressor::Compress(const std::vector<int>& array) {
    if (_type == eCompressorType::Uncompressor) {
        return {};
    }

    HuffmanCompressorImpl impl(this);
    return impl.Compress(array);
}

std::vector<int> ai::Compressor::Uncompress(const std::vector<uint8_t>& buffer) {
    if (_type == eCompressorType::Compressor) {
        return {};
    }

    HuffmanCompressorImpl impl(this);
    return impl.Uncompress(buffer);
}
