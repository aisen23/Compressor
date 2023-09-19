#include "pch.h"

#include "HuffmanTree.h"

#ifdef __linux__
#include <arpa/inet.h>
#elif _WIN32
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#endif

ai::HuffmanTree::HuffmanTree(HuffmanTree&& src) noexcept
{
    _root = src._root;
    _freqTable = std::move(src._freqTable);

    src._root = nullptr;
}

ai::HuffmanTree::~HuffmanTree()
{
    if (_root) {
        Free(_root);
    }
}

void ai::HuffmanTree::Free(Node* node) {
    if (!node) {
        return;
    }

    Free(node->left);
    Free(node->right);

    delete node;
    node = nullptr;
}

std::vector<uint8_t> ai::HuffmanTree::Encode(const std::vector<char>& chars) {
    InitFreqTable(chars);
    assert(!_freqTable.empty());    
    if (_freqTable.empty()) {
        std::cerr << "Error: ai::HuffmanTree::Encode(): char frequency table is empty" << std::endl;
        return {};
    }

    InitTree();
    assert(_root != nullptr);    
    if (_root == nullptr) {
        std::cerr << "Error: ai::HuffmanTree::Encode(): root is NULL" << std::endl;
        return {};
    }

    InitCodesTable();
    assert(!_codesTable.empty());    
    if (_codesTable.empty()) {
        std::cerr << "Error: ai::HuffmanTree::Encode(): char codes table is empty" << std::endl;
        return {};
    }


    std::vector<uint8_t> arr;
    size_t offset = 0;
    WriteTable(arr, offset);

    uint32_t bitsSize = 0;
    std::vector<uint8_t> codes;
    for (auto ch : chars) {
        const auto& code = _codesTable[ch];
        assert(!code.empty());
        if (code.empty()) {
            std::cerr << "Error: read encoded data" << std::endl;
            return {};
        }

        auto node = _root;
        for (auto c : code) {
            size_t index = bitsSize / 8;
            if (index == codes.size()) {
                codes.push_back(0);
            }

            auto& byte = codes[index];
            size_t bitPos = bitsSize % 8;

            if (0 == c) {
                node = node->left;
            }
            else {
                node = node->right;
                byte |= (1 << bitPos);
            }

            ++bitsSize;
        }
    }

    uint32_t networkBitsSize = htonl(bitsSize);
    arr.resize(offset + 4);
    std::memcpy(arr.data() + offset, &networkBitsSize, sizeof(uint32_t));

    arr.insert(arr.end(), codes.begin(), codes.end());

    return arr;
}

std::vector<char> ai::HuffmanTree::Decode(const std::vector<uint8_t>& data, size_t offset) {
    HuffmanTree tree;
    
    ReadFreqTable(data, offset);
    assert(!_freqTable.empty());    
    if (_freqTable.empty()) {
        std::cerr << "Error: ai::HuffmanTree::Decode(): char frequency table is empty" << std::endl;
        return {};
    }

    InitTree();
    assert(_root != nullptr);    
    if (_root == nullptr) {
        std::cerr << "Error: ai::HuffmanTree::Decode(): root is NULL" << std::endl;
        return {};
    }
#ifdef DEBUG_BUILD
    InitCodesTable();
#endif

    uint32_t networkBitsSize;
    std::memcpy(&networkBitsSize, data.data() + offset, sizeof(uint32_t));
    uint32_t bitsSize = ntohl(networkBitsSize);

    offset += 4;

    if (bitsSize == 0) {
        return {};
    }

    std::vector<char> chars;
    auto node = _root;
    for (size_t i = 0; i != bitsSize; ++i) {
        size_t id = offset + i / 8;
        unsigned bitPos = i % 8;

        bool isBitSet = (data[id] & (1 << bitPos)) != 0;
        node = (isBitSet ? node->right : node->left);

        if (node->value != 0) {
            chars.push_back(node->value);
            node = _root;
        }
    }

    return chars;
}

void ai::HuffmanTree::InitCodesTable() {
    if (!_root) {
        return;
    }
    assert(!_freqTable.empty());

    std::queue<std::pair<ai::HuffmanTree::Node*, std::vector<uint8_t>>> queue;
    queue.push(std::make_pair(_root, std::vector<uint8_t>()));
    while (!queue.empty()) {
        auto& front = queue.front();

        auto node = front.first;
        auto code = std::move(front.second);

        queue.pop();

        if (node->value != 0) {
#ifdef DEBUG_BUILD
            std::cout << "Char: " << node->value << ", freq: " << node->freq << ", ";
            for (auto c : code) {
                std::cout << static_cast<int>(c);
            }
            std::cout << "\n";
#endif
            _codesTable[node->value] = std::move(code);
            code.clear();

            assert(node->freq == _freqTable[node->value]);
        }

        if (node->left) {
            code.push_back(0);
            queue.push(std::make_pair(node->left, code));
            code.pop_back();
        }

        if (node->right) {
            code.push_back(1);
            queue.push(std::make_pair(node->right, std::move(code)));
        }
    }
}

void ai::HuffmanTree::InitTree() {

    if (_freqTable.empty()) {
        _root = nullptr;
        return;
    }

    std::priority_queue<HuffmanTree::Node*, std::vector<HuffmanTree::Node*>, HuffmanTree::NodeCompare> queue;
    for (auto& [ch, freq] : _freqTable) {
        auto node = new HuffmanTree::Node;
        node->freq = freq;
        node->value = ch;
        queue.push(node);
    }

    // TODO: Check order.
    while (queue.size() > 1) {
        auto left = queue.top();
        queue.pop();
        auto right = queue.top();
        queue.pop();

        auto newNode = new HuffmanTree::Node;
        newNode->left = left;
        newNode->right = right;
        newNode->freq = left->freq + right->freq;

        queue.push(newNode);
    }

    _root = queue.top();
}

void ai::HuffmanTree::ReadFreqTable(const std::vector<uint8_t>& data, size_t& offset) {
    assert(_freqTable.empty());
    assert(offset < data.size());
    if (offset >= data.size()) {
        _freqTable.clear();
        return;
    }

    // Optimized for this task input.
    size_t size = static_cast<size_t>(data[offset++]);

    // 5 bytes. First byte - char, others - uint32_t frequency of this char.
    
    for (size_t i = 0; i <= 5 * size - 5; i += 5) {
        size_t index = offset + i;
        char ch = static_cast<char>(data[index]);

        uint32_t networkFreq;
        std::memcpy(&networkFreq, data.data() + index + 1, sizeof(uint32_t));

        _freqTable[ch] = ntohl(networkFreq);
    }

    offset += 5 * size;
}


bool ai::HuffmanTree::NodeCompare::operator()(const ai::HuffmanTree::Node* l, const ai::HuffmanTree::Node* r) const {
    assert(l && r);
    if (!l || !r) {
        return false;
    }
    return l->freq > r->freq;
}

void ai::HuffmanTree::InitFreqTable(const std::vector<char>& chars) {
    assert(_freqTable.empty());
    if (!_freqTable.empty()) {
        _freqTable.clear();
    }

    for (char c : chars) {
        ++_freqTable[c];
    }
}

void ai::HuffmanTree::WriteTable(std::vector<uint8_t>& data, size_t& offset) const {
    assert(!_freqTable.empty());

    // Optimized to this task input.
    data.push_back(static_cast<uint8_t>(_freqTable.size()));
    ++offset;

    data.resize(offset + 5 * _freqTable.size());
    for (auto& [ch, value] : _freqTable) {
        data[offset++] = static_cast<uint8_t>(ch);

        // For compatibility with other platforms...
        uint32_t networkValue = htonl(value);
        std::memcpy(data.data() + offset, &networkValue, sizeof(uint32_t));
        offset += 4;
    }
}
