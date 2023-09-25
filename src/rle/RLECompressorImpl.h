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
        std::vector<uint8_t> EncodeRLERange(const std::vector<unsigned>& arr) const;
        std::vector<uint8_t> EncodeRLE(const std::vector<uint8_t>& bytes) const;

        std::vector<unsigned> DecodeRLE(const std::vector<uint8_t>& data, size_t offset) const;
        std::vector<unsigned> DecodeRLERange(const std::vector<unsigned>& nums) const;

    private:
        RLECompressorImpl(const RLECompressorImpl&) = delete;
        RLECompressorImpl& operator=(const RLECompressorImpl&) = delete;
    };
} // ai
