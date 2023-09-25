#pragma once

#include <cstdint>
#include <vector>

namespace ai
{
    enum class eCompressorImplType : int {
        None,
        Easy,
        Huffman,
        RLE, // Run-Length Encoding with Range Encoding.
        RLE_Huffman
    };

    class Compressor;
    class CompressorImpl
    {
    public:
        virtual ~CompressorImpl() = default;

        virtual std::vector<uint8_t> Compress(const std::vector<unsigned>& arr) = 0;
        virtual std::vector<unsigned> Uncompress(const std::vector<uint8_t>& data, size_t offset) = 0;

    protected:
        friend class CombinedCompressorImpl;
        virtual std::vector<uint8_t> CompressCombined(const std::vector<uint8_t>& bytes) { return {}; };
        virtual std::vector<unsigned> UncompressCombined(const std::vector<unsigned>& nums) { return {}; };

    protected:
        bool _combined = false;
    };


    enum class eCompressorType : int {
        Compressor,
        Uncompressor
    };


    class Compressor
    {
    public:
        explicit Compressor(eCompressorType type);
        ~Compressor() = default;

        std::vector<uint8_t> Compress(const std::vector<unsigned>& arr) const;
        std::vector<unsigned> Uncompress(const std::vector<uint8_t>& data) const;

    private:
        std::vector<uint8_t> WriteImplType(eCompressorImplType type) const;
        eCompressorImplType ReadImplType(const std::vector<uint8_t>& data, size_t& offset) const;

        std::vector<uint8_t> CompressWithHuffman(const std::vector<unsigned>& arr) const;
        std::vector<unsigned> UncompressWithHuffman(const std::vector<uint8_t>& data, size_t offset) const;

        std::vector<uint8_t> CompressWithEasy(const std::vector<unsigned>& arr) const;
        std::vector<unsigned> UncompressWithEasy(const std::vector<uint8_t>& data, size_t offset) const;

        std::vector<uint8_t> CompressWithRLE(const std::vector<unsigned>& arr) const;
        std::vector<unsigned> UncompressWithRLE(const std::vector<uint8_t>& data, size_t offset) const;

        std::vector<uint8_t> CompressWithCombined(const std::vector<unsigned>& arr, eCompressorImplType type) const;
        std::vector<unsigned> UncompressWithCombined(const std::vector<uint8_t>& data, size_t offset, eCompressorImplType type) const;

        std::vector<eCompressorImplType> GetCombinedCompressorTypes(eCompressorImplType type) const;
        std::vector<eCompressorImplType> GetCombinedUncompressorTypes(eCompressorImplType type) const;

    private:
        eCompressorType _type;

    private:
        Compressor(const Compressor&) = delete;
        Compressor& operator=(const Compressor&) = delete;
    };
};
