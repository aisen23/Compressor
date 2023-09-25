#pragma once

#include "Compressor.h"

#include <memory>

namespace ai
{
    class CombinedCompressorImpl : public CompressorImpl
    {
    public:
        CombinedCompressorImpl(std::vector<eCompressorImplType>&& types)
            : _types(std::move(types)) {}

        std::vector<uint8_t> Compress(const std::vector<unsigned>& arr) final override;
        std::vector<unsigned> Uncompress(const std::vector<uint8_t>& data, size_t offset) final override;

    private:
        std::shared_ptr<CompressorImpl> CreateCompressorImpl(eCompressorImplType type) const;

    private:
        std::vector<eCompressorImplType> _types;

    private:
        CombinedCompressorImpl(const CombinedCompressorImpl&) = delete;
        CombinedCompressorImpl& operator=(const CombinedCompressorImpl&) = delete;
    };
} // ai
