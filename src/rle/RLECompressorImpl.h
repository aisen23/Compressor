#pragma once

#include "Compressor.h"

namespace ai
{
    class RLECompressorImpl : public CompressorImpl
    {
    public:
        RLECompressorImpl() = default;

        std::vector<uint8_t> Compress(const std::vector<unsigned>& arr) final override;
        std::vector<unsigned> Uncompress(const std::vector<uint8_t>& data, size_t offset) final override;

    private:
        RLECompressorImpl(const RLECompressorImpl&) = delete;
        RLECompressorImpl& operator=(const RLECompressorImpl&) = delete;
    };
} // ai
