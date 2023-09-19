#pragma once

#include "Compressor.h"

namespace ai
{
    class HuffmanCompressorImpl : public CompressorImpl
    {
    public:
        explicit HuffmanCompressorImpl();

        std::vector<uint8_t> Compress(const std::vector<unsigned>& arr) final override;
        std::vector<unsigned> Uncompress(const std::vector<uint8_t>& data, size_t offset) final override;

    private:
        HuffmanCompressorImpl(const HuffmanCompressorImpl&) = delete;
        HuffmanCompressorImpl& operator=(const HuffmanCompressorImpl&) = delete;
    };
} // ai
