#include "pch.h"

#include "Compressor.h"
#include "huffman/HuffmanCompressorImpl.h"

ai::CompressorImpl::CompressorImpl(eCompressorImplType type)
    : _type(type)
{}

ai::Compressor::Compressor(ai::eCompressorType type)
    :_type(type)
{}

std::vector<uint8_t> ai::Compressor::Compress(const std::vector<unsigned>& arr) const {
    if (_type == eCompressorType::Uncompressor) {
        return {};
    }

    std::vector<std::vector<uint8_t>> datas;

    auto dataHuffman = WriteImplType(eCompressorImplType::Huffman);
    HuffmanCompressorImpl huffImpl;
    auto huffImplData = huffImpl.Compress(arr);
    dataHuffman.insert(dataHuffman.end(), huffImplData.begin(), huffImplData.end());
    datas.push_back(std::move(dataHuffman));



// -=-= -=-= -=- =- =- =- Calculate the effective compression =- = =- =-= -=- = -=- =- =

    size_t minIndex = 0;
    size_t minSize = std::numeric_limits<size_t>::max();
    for (size_t i = 0; i != datas.size(); ++i) {
        const auto& buf = datas[i];
        if (buf.size() < minSize) {
            minSize = buf.size();
            minIndex = i;
        }
    }

    // TODO: check address and std::move.
    return std::move(datas[minIndex]);
}

std::vector<unsigned> ai::Compressor::Uncompress(const std::vector<uint8_t>& data) const {
    if (_type == eCompressorType::Compressor) {
        return {};
    }

    size_t offset = 0;
    auto type = ReadImplType(data, offset);

    switch (type) {
        case eCompressorImplType::Huffman: {
            HuffmanCompressorImpl impl;
            return impl.Uncompress(data, offset);
        }
        case eCompressorImplType::Easy:
        case eCompressorImplType::None:
        default: {
            break;
        }
    }

    return {};
}

std::vector<uint8_t> ai::Compressor::WriteImplType(eCompressorImplType type) const {
    std::vector<uint8_t> data;
    data.push_back(static_cast<uint8_t>(type));

    return data;
}

ai::eCompressorImplType ai::Compressor::ReadImplType(const std::vector<uint8_t>& data, size_t& offset) const {
    auto type = eCompressorImplType::None;
    if (offset < data.size()) {
        type = static_cast<eCompressorImplType>(data[offset++]);
    }

    return type;
}
