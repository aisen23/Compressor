#pragma once

#include "Data.h"

#include <cstdint>

namespace ai
{
    class CompressorImpl
    {
    public:
        std::vector<uint8_t> Compress(const std::vector<int>& array) = 0;
        std::vector<int> UnCompress(uint8_t* buffer) = 0;
    };

    class Compressor
    {
    public:
        std::vector<uint8_t> Compress(const std::vector<int>& array);
        std::vector<int> Uncompress(const std::vector<uint8_t>& buffer);

    private:
        Compressor(const Compressor&) = delete;
        Compressor& operator=(const Compressor&) = delete;
    };
};
