#pragma once

#include <cstdint>
#include <map>
#include <vector>
#include <unordered_map>

namespace ai
{
    class HuffmanTree
    {
        private:
            struct Node
            {
                int8_t value = static_cast<uint8_t>(-1);
                uint32_t freq = 0;

                Node* left = nullptr;
                Node* right = nullptr;
            };

            struct NodeCompare {
                bool operator()(const ai::HuffmanTree::Node* l, const ai::HuffmanTree::Node* r) const;
            };

        public:
            ~HuffmanTree();

        private:
            HuffmanTree() = default;

            void InitFreqTable(const std::vector<uint8_t>& arr);
            void InitTree();

            void Free(Node* node);

            std::vector<uint8_t> Encode(const std::vector<uint8_t>& arr);
            std::vector<uint8_t> Decode(const std::vector<uint8_t>& data, size_t offset);

            void InitCodesTable();

            void WriteFreqTable(std::vector<uint8_t>& data, size_t& offset) const;
            void ReadFreqTable(const std::vector<uint8_t>& data, size_t& offset);

        private:
            friend class HuffmanCompressorImpl;
            Node* _root = nullptr;
            std::unordered_map<uint8_t, std::vector<uint8_t>> _codesTable;
            std::map<uint8_t, uint32_t> _freqTable;

        private:
            HuffmanTree(const HuffmanTree&) = delete;
            HuffmanTree& operator=(const HuffmanTree&) = delete;
    };
} // ai
