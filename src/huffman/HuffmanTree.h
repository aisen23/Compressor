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
                char value = 0;
                uint32_t freq = 0;

                Node* left = nullptr;
                Node* right = nullptr;
            };

            struct NodeCompare {
                bool operator()(const ai::HuffmanTree::Node* l, const ai::HuffmanTree::Node* r) const;
            };

        public:
            explicit HuffmanTree(bool isChar) : _isChar(isChar) {};
            ~HuffmanTree();

        private:
            HuffmanTree() = default;

            void InitFreqTable(const std::vector<char>& chars);
            void InitTree();

            void Free(Node* node);

            std::vector<uint8_t> Encode(const std::vector<char>& chars);
            std::vector<char> Decode(const std::vector<uint8_t>& data, size_t offset);

            void InitCodesTable();

            void WriteFreqTable(std::vector<uint8_t>& data, size_t& offset) const;
            void ReadFreqTable(const std::vector<uint8_t>& data, size_t& offset);

        private:
            friend class HuffmanCompressorImpl;
            Node* _root = nullptr;
            std::unordered_map<char, std::vector<uint8_t>> _codesTable;
            std::map<char, uint32_t> _freqTable;
            bool _isChar = false;

        private:
            HuffmanTree(const HuffmanTree&) = delete;
            HuffmanTree& operator=(const HuffmanTree&) = delete;
    };
} // ai
