#pragma once

#include <cstdint>
#include <vector>

namespace ai
{
    enum class eDataType : uint8_t {
        None,
        HuffmanTree,
        Body
    };

    struct DataHeader
    {
        eDataType type = eDataType::None;
        uint8_t size = 0;
    };

    struct Data
    {
        DataHeader header{};
        std::vector<uint8_t> body;

        size_t Size() const { return sizeof(header) + body.size(); }
    };
} // ai
