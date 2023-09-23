#include "pch.h"

#include "Compressor.h"

#include "huffman/HuffmanCompressorImpl.h"
#include "easy/EasyCompressorImpl.h"
#include "rle/RLECompressorImpl.h"

ai::Compressor::Compressor(ai::eCompressorType type)
    :_type(type)
{}

std::vector<uint8_t> ai::Compressor::Compress(const std::vector<unsigned>& arr) const {
    if (_type == eCompressorType::Uncompressor) {
        return {};
    }

    std::vector<std::vector<uint8_t>> datas;

    datas.push_back(CompressWithEasy(arr));
    datas.push_back(CompressWithHuffman(arr));
    datas.push_back(CompressWithRLE(arr));


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
            return UncompressWithHuffman(data, offset);
        }
        case eCompressorImplType::Easy: {
            return UncompressWithEasy(data, offset);
        }
        case eCompressorImplType::RLE: {
            return UncompressWithRLE(data, offset);
        }
        case eCompressorImplType::None:
        default: {
            std::cerr << "Error: ai::Compressor::Uncompress(): incorrect data." << std::endl;
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

std::vector<uint8_t> ai::Compressor::CompressWithHuffman(const std::vector<unsigned>& arr) const {
    auto data = WriteImplType(eCompressorImplType::Huffman);
    HuffmanCompressorImpl impl;
    auto implData = impl.Compress(arr);
    data.insert(data.end(), implData.begin(), implData.end());
    return data;
}

std::vector<unsigned> ai::Compressor::UncompressWithHuffman(const std::vector<uint8_t>& data, size_t offset) const {
    HuffmanCompressorImpl impl;
    return impl.Uncompress(data, offset);
}

std::vector<uint8_t> ai::Compressor::CompressWithEasy(const std::vector<unsigned>& arr) const {
    auto data = WriteImplType(eCompressorImplType::Easy);
    EasyCompressorImpl impl;
    auto implData = impl.Compress(arr);
    data.insert(data.end(), implData.begin(), implData.end());
    return data;
}

std::vector<unsigned> ai::Compressor::UncompressWithEasy(const std::vector<uint8_t>& data, size_t offset) const {
    EasyCompressorImpl impl;
    return impl.Uncompress(data, offset);
}

std::vector<uint8_t> ai::Compressor::CompressWithRLE(const std::vector<unsigned>& arr) const {
    auto data = WriteImplType(eCompressorImplType::RLE);
    RLECompressorImpl impl;
    auto implData = impl.Compress(arr);
    data.insert(data.end(), implData.begin(), implData.end());
    return data;
}

std::vector<unsigned> ai::Compressor::UncompressWithRLE(const std::vector<uint8_t>& data, size_t offset) const {
    RLECompressorImpl impl;
    return impl.Uncompress(data, offset);
}
