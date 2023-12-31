#include "pch.h"

#include "Compressor.h"

#include "clock/Clock.h"
#include "combined/CombinedCompressorImpl.h"
#include "Input.h"
#include "huffman/HuffmanCompressorImpl.h"
#include "easy/EasyCompressorImpl.h"
#include "rle/RLECompressorImpl.h"
#include "threads/ThreadPool.h"

ai::Compressor::Compressor(ai::eCompressorType type)
    :_type(type)
{}

std::vector<uint8_t> ai::Compressor::Compress(const std::vector<unsigned>& arr) const {
    if (_type == eCompressorType::Uncompressor) {
        return {};
    }

    std::vector<std::vector<uint8_t>> datas;

    bool multithreading = ai::THREADS_NUM > 0;

    if (multithreading) {
        std::vector<std::future<std::vector<uint8_t>>> futures;
        auto& pool = ThreadPool::Instance();

        futures.push_back(pool.Submit([this, &arr]() {
            return this->CompressWithEasy(arr);
        }));

        futures.push_back(pool.Submit([this, &arr]() {
            return this->CompressWithHuffman(arr);
        }));

        futures.push_back(pool.Submit([this, &arr]() {
            return this->CompressWithRLE(arr);
        }));

        futures.push_back(pool.Submit([this, &arr]() {
            return this->CompressWithCombined(arr, eCompressorImplType::RLE_Huffman);
        }));

        for (auto& f : futures) {
            datas.push_back(f.get());
        }
    }
    else {
        datas.push_back(CompressWithEasy(arr));
        datas.push_back(CompressWithHuffman(arr));
        datas.push_back(CompressWithRLE(arr));
        datas.push_back(CompressWithCombined(arr, eCompressorImplType::RLE_Huffman));
    }


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
        case eCompressorImplType::RLE_Huffman: {
            return UncompressWithCombined(data, offset, type);
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
    Clock clock;
    auto start = clock.Now();

    auto data = WriteImplType(eCompressorImplType::Huffman);
    HuffmanCompressorImpl impl;
    auto implData = impl.Compress(arr);
    data.insert(data.end(), implData.begin(), implData.end());

    clock.PrintDuration(start, "HuffmanCompressor");

    return data;
}

std::vector<unsigned> ai::Compressor::UncompressWithHuffman(const std::vector<uint8_t>& data, size_t offset) const {
    Clock clock;
    auto start = clock.Now();
    
    HuffmanCompressorImpl impl;
    auto arr = impl.Uncompress(data, offset);

    clock.PrintDuration(start, "HuffmanUncompressor");

    return arr;
}

std::vector<uint8_t> ai::Compressor::CompressWithEasy(const std::vector<unsigned>& arr) const {
    Clock clock;
    auto start = clock.Now();

    auto data = WriteImplType(eCompressorImplType::Easy);
    EasyCompressorImpl impl;
    auto implData = impl.Compress(arr);
    data.insert(data.end(), implData.begin(), implData.end());

    clock.PrintDuration(start, "EasyCompressor");

    return data;
}

std::vector<unsigned> ai::Compressor::UncompressWithEasy(const std::vector<uint8_t>& data, size_t offset) const {
    Clock clock;
    auto start = clock.Now();

    EasyCompressorImpl impl;
    auto arr = impl.Uncompress(data, offset);

    clock.PrintDuration(start, "EasyCompressor");

    return arr;
}

std::vector<uint8_t> ai::Compressor::CompressWithRLE(const std::vector<unsigned>& arr) const {
    Clock clock;
    auto start = clock.Now();

    auto data = WriteImplType(eCompressorImplType::RLE);
    RLECompressorImpl impl;
    auto implData = impl.Compress(arr);
    data.insert(data.end(), implData.begin(), implData.end());

    clock.PrintDuration(start, "RLECompressor");

    return data;
}

std::vector<unsigned> ai::Compressor::UncompressWithRLE(const std::vector<uint8_t>& data, size_t offset) const {
    Clock clock;
    auto start = clock.Now();

    RLECompressorImpl impl;
    auto arr = impl.Uncompress(data, offset);

    clock.PrintDuration(start, "RLEUncompressor");

    return arr;
}

std::vector<uint8_t> ai::Compressor::CompressWithCombined(const std::vector<unsigned>& arr, eCompressorImplType type) const {
    Clock clock;
    auto start = clock.Now();

    auto data = WriteImplType(type);
    CombinedCompressorImpl impl(GetCombinedCompressorTypes(type));
    auto implData = impl.Compress(arr);
    data.insert(data.end(), implData.begin(), implData.end());

    clock.PrintDuration(start, "CombinedCompressor");

    return data;
}

std::vector<unsigned> ai::Compressor::UncompressWithCombined(const std::vector<uint8_t>& data, size_t offset, eCompressorImplType type) const {
    Clock clock;
    auto start = clock.Now();

    CombinedCompressorImpl impl(GetCombinedUncompressorTypes(type));
    auto arr = impl.Uncompress(data, offset);

    clock.PrintDuration(start, "CombinedUncompressor");

    return arr;
}

std::vector<ai::eCompressorImplType> ai::Compressor::GetCombinedCompressorTypes(eCompressorImplType type) const {
    switch (type) {
        case eCompressorImplType::RLE_Huffman: {
            return { eCompressorImplType::RLE, eCompressorImplType::Huffman };
        }
        default: {
            return {};
        }
    }
}

std::vector<ai::eCompressorImplType> ai::Compressor::GetCombinedUncompressorTypes(eCompressorImplType type) const {
    switch (type) {
        case eCompressorImplType::RLE_Huffman: {
            return { eCompressorImplType::Huffman, eCompressorImplType::RLE };
        }
        default: {
            return {};
        }
    }
}
