#pragma once

#include "Compressor.h"

namespace ai
{
    class EasyCompressorImpl : public CompressorImpl
    {
    public:
        EasyCompressorImpl();

        std::vector<uint8_t> Compress(const std::vector<unsigned>& arr) final override;
        std::vector<unsigned> Uncompress(const std::vector<uint8_t>& data, size_t offset) final override;

    private:
        EasyCompressorImpl(const EasyCompressorImpl&) = delete;
        EasyCompressorImpl& operator=(const EasyCompressorImpl&) = delete;
    };
} // ai
