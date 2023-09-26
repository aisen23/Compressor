#include "pch.h"

#include "HuffmanTree.h"

#include "Utils.h"

ai::HuffmanTree::~HuffmanTree()
{
    Free(_root);
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

std::vector<uint8_t> ai::HuffmanTree::Encode(const std::vector<uint8_t>& arr) {
    InitFreqTable(arr);
    assert(!_freqTable.empty());    
    if (_freqTable.empty()) {
        std::cerr << "Error: ai::HuffmanTree::Encode(): int8_t frequency table is empty" << std::endl;
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
        std::cerr << "Error: ai::HuffmanTree::Encode(): int8_t codes table is empty" << std::endl;
        return {};
    }

    std::vector<uint8_t> data;

    size_t offset = 0;
    WriteFreqTable(data, offset);

    uint32_t bitsSize = 0;
    std::vector<uint8_t> codes;
    for (auto ch : arr) {
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

    uint32_t networkBitsSize = utils::Htonl(bitsSize);
    data.resize(offset + sizeof(uint32_t));
    std::memcpy(data.data() + offset, &networkBitsSize, sizeof(uint32_t));

    data.insert(data.end(), codes.begin(), codes.end());

    return data;
}

std::vector<uint8_t> ai::HuffmanTree::Decode(const std::vector<uint8_t>& data, size_t offset) {
    ReadFreqTable(data, offset);
    assert(!_freqTable.empty());    
    if (_freqTable.empty()) {
        std::cerr << "Error: ai::HuffmanTree::Decode(): int8_t frequency table is empty" << std::endl;
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
    uint32_t bitsSize = utils::Ntohl(networkBitsSize);

    offset += sizeof(uint32_t);

    if (bitsSize == 0) {
        return {};
    }

    std::vector<uint8_t> arr;
    auto node = _root;
    for (size_t i = 0; i != bitsSize; ++i) {
        size_t id = offset + i / 8;
        unsigned bitPos = i % 8;

        bool isBitSet = (data[id] & (1 << bitPos)) != 0;
        node = (isBitSet ? node->right : node->left);

        if (static_cast<int8_t>(node->value) != -1) {
            arr.push_back(node->value);
            node = _root;
        }
    }

    return arr;
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

        if (static_cast<int8_t>(node->value) != -1) {
#ifdef DEBUG_BUILD
            std::cout << "Char: " << static_cast<int>(node->value) << ", freq: " << node->freq << ", ";
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
    assert(_root == nullptr && !_freqTable.empty());
    Free(_root);

    if (_freqTable.empty()) {
        std::cerr << "Error: ai::HuffmanTree::InitTree(): no frequency table" << std::endl;
        return;
    }

    std::priority_queue<HuffmanTree::Node*, std::vector<HuffmanTree::Node*>, HuffmanTree::NodeCompare> queue;
    for (auto& [ch, freq] : _freqTable) {
        auto node = new HuffmanTree::Node;
        node->freq = freq;
        node->value = ch;
        queue.push(node);
    }

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

    unsigned step = 1 + sizeof(uint32_t);
    for (size_t i = 0; i <= step * size - step; i += step) {
        size_t index = offset + i;
        int8_t ch = static_cast<uint8_t>(data[index]);

        uint32_t networkFreq;
        std::memcpy(&networkFreq, data.data() + index + 1, sizeof(uint32_t));

        _freqTable[ch] = utils::Ntohl(networkFreq);
    }

    offset += step * size;
}


bool ai::HuffmanTree::NodeCompare::operator()(const ai::HuffmanTree::Node* l, const ai::HuffmanTree::Node* r) const {
    assert(l && r);
    if (!l || !r) {
        return false;
    }
    return l->freq > r->freq;
}

void ai::HuffmanTree::InitFreqTable(const std::vector<uint8_t>& arr) {
    assert(_freqTable.empty());
    if (!_freqTable.empty()) {
        _freqTable.clear();
    }

    for (int8_t c : arr) {
        ++_freqTable[c];
    }
}

void ai::HuffmanTree::WriteFreqTable(std::vector<uint8_t>& data, size_t& offset) const {
    assert(!_freqTable.empty());

    // Optimized to this task input.
    data.push_back(static_cast<uint8_t>(_freqTable.size()));
    ++offset;

    data.resize(offset + (1 + sizeof(uint32_t)) * _freqTable.size());
    for (auto& [ch, value] : _freqTable) {
        data[offset++] = static_cast<uint8_t>(ch);

        // For compatibility with other platforms...
        uint32_t networkValue = utils::Htonl(value);
        std::memcpy(data.data() + offset, &networkValue, sizeof(uint32_t));
        offset += sizeof(uint32_t);
    }
}
