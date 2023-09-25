#include "pch.h"

#include "CombinedCompressorImpl.h"

#include "huffman/HuffmanCompressorImpl.h"
#include "rle/RLECompressorImpl.h"

namespace ai
{
    std::vector<uint8_t> CombinedCompressorImpl::Compress(const std::vector<unsigned>& arr) {
        if (arr.empty() || _types.empty()) {
            std::cerr << "Error: CombinedCompressorImpl::Compress(): no input numbers or types to compress" << std::endl;
            return {};
        }

        std::vector<uint8_t> data;

        for (size_t i = 0; i != _types.size(); ++i) {
            auto type = _types[i];
            auto impl = CreateCompressorImpl(type);
            bool isFirst = (i == 0);

            auto temp = (isFirst ? impl->Compress(arr) : impl->CompressCombined(data));

            data.clear();
            data = std::move(temp);
        }

        // For sending this data to somewhere.
        if (_types.back() == eCompressorImplType::RLE) {
            data.back() |= (1 << 7);
        }

        return data;
    }

    std::vector<unsigned> CombinedCompressorImpl::Uncompress(const std::vector<uint8_t>& data, size_t offset) {
        if (data.size() <= offset || _types.empty()) {
            std::cerr << "Error: CombinedCompressorImpl::Uncompress(): no data" << std::endl;
            return {};
        }

        std::vector<unsigned> arr;

        for (size_t i = 0; i != _types.size(); ++i) {
            auto type = _types[i];
            auto impl = CreateCompressorImpl(type);
            bool isFirst = (i == 0);

            auto temp = (isFirst ? impl->Uncompress(data, offset) : impl->UncompressCombined(arr));

            arr.clear();
            arr = std::move(temp);
        }

        return arr;
    }
        
    std::shared_ptr<CompressorImpl> CombinedCompressorImpl::CreateCompressorImpl(eCompressorImplType type) const {
        std::shared_ptr<CompressorImpl> impl;
        switch (type) {
            case eCompressorImplType::RLE: {
                impl = std::make_shared<RLECompressorImpl>();
                break;
            }
            case eCompressorImplType::Huffman: {
                impl = std::make_shared<HuffmanCompressorImpl>();
                break;
            }
            default: {
                return nullptr;
            }
        }

        impl->_combined = true;
        return impl;
    }
}
