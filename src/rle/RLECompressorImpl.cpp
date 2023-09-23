#include "pch.h"

#include "RLECompressorImpl.h"

#include "Input.h"
#include "Utils.h"

namespace ai
{
    std::vector<uint8_t> RLECompressorImpl::Compress(const std::vector<unsigned>& arr) {
        std::vector<uint8_t> bytes;
        if (arr.size() == 0) {
            std::cerr << "Error: RLE::Compressor::Encode(): No input numbers" << std::endl;
            return {};
        }

        // RLE Range.
        size_t incCount = 0;
        size_t decCount = 0;
        uint8_t num = arr[0];
        uint8_t prev = num;
        uint8_t effCount = 2;
        bytes.push_back(num);
        for (size_t i = 1; i != arr.size(); ++i) {
            num = static_cast<uint8_t>(arr[i]);
            if (num == prev + 1) {
                ++incCount;
                // Put into bytes just if it gives us the space saving.
                if (incCount > effCount) {
                    if (incCount == effCount + 1) {
                        bytes.pop_back();
                        bytes.pop_back();
                        bytes.push_back(static_cast<uint8_t>(ai::MAX_ELEM + 1));
                    }
                    if (i != arr.size() - 1) {
                        prev = num;
                        continue;
                    }
                }
            }
            else {
                if (incCount > effCount) {
                    bytes.push_back(prev);
                }

                incCount = 0;
            }

            if (num == prev - 1) {
                ++decCount;
                if (decCount > effCount) {
                    if (decCount == effCount + 1) {
                        bytes.pop_back();
                        bytes.pop_back();
                        bytes.push_back(static_cast<uint8_t>(ai::MAX_ELEM + 2));
                    }
                    if (i != arr.size() - 1) {
                        prev = num;
                        continue;
                    }
                }
            }
            else {
                if (decCount > effCount) {
                    bytes.push_back(prev);
                }

                decCount = 0;
            }
            
            bytes.push_back(num);

            prev = static_cast<uint8_t>(arr[i]);
        }

        // RLE
        std::vector<uint8_t> data; 
        num = bytes[0];
        if (bytes.size() == 1) {
            num |= (1 << 7);
        }
        data.push_back(num);

        uint32_t rangeCount = 1;
        uint8_t rangeValue = bytes[0];
        effCount = 6;
        bool creatingRange = false;
        for (size_t i = 1; i != bytes.size(); ++i) {
            num = bytes[i];

            bool needAddRangeInfo = false;
            if (num == rangeValue) {
                ++rangeCount;
                if (!creatingRange && rangeCount > effCount) {
                    for (size_t i = 0; i != effCount; ++i) {
                        data.pop_back();
                    }

                    data.push_back(static_cast<uint8_t>(ai::MAX_ELEM + 3));
                    creatingRange = true;
                }
            }
            else {
                assert(i >= rangeCount);

                if (creatingRange) {
                    needAddRangeInfo = true;
                    creatingRange = false;
                }
                else {
                    rangeCount = 1;
                    rangeValue = num;
                }
            }

            // Last element will be with the highest bit 1.
            // Optimized to task input.
            bool isLast = (i == bytes.size() - 1);
            if (isLast && creatingRange) {
                needAddRangeInfo = true;
                creatingRange = false;
            }

            if (needAddRangeInfo) {
                std::vector<uint8_t> temp(sizeof(uint32_t));

                uint32_t networkRangeCount = ai::utils::htonl(rangeCount);
                std::memcpy(temp.data(), &networkRangeCount, sizeof(uint32_t));

                data.insert(data.end(), temp.begin(), temp.end());

                if (!isLast || num != rangeValue) {
                    data.push_back(rangeValue);
                }

                rangeCount = 1;
                rangeValue = num;
            }

            if (isLast) {
                num |= (1 << 7);
            }

            if (isLast || !creatingRange) {
                data.push_back(num);
            }

            prev = num;
        }

        return data;
    }

    std::vector<unsigned> RLECompressorImpl::Uncompress(const std::vector<uint8_t>& data, size_t offset) {
        if (data.size() == 0) {
            std::cerr << "Error: RLE::Compressor::Encode(): No input numbers" << std::endl;
            return {};
        }

        // RLE
        std::vector<unsigned> nums;
        uint8_t byte = ai::MAX_ELEM + 5;
        for (size_t i = offset; i != data.size();) {
            byte = data[i];
            // Restore last byte.
            if (i == data.size() - 1) {
                byte &= (~(1 << 7));
            }

            if (static_cast<unsigned>(byte) == ai::MAX_ELEM + 3) {
                uint32_t networkRangeValue;
                std::memcpy(&networkRangeValue, data.data() + i + 1, sizeof(uint32_t));
                uint32_t rangeValue = ai::utils::ntohl(networkRangeValue);
                
                size_t index = i + sizeof(uint32_t) + 1;
                uint8_t value = data[index];
                if (index == data.size() - 1) {
                    value &= (~(1 << 7));
                }
                unsigned num = static_cast<unsigned>(value);
                for (size_t i = 0; i < rangeValue; ++i) {
                    nums.push_back(num);
                }

                i += (sizeof(uint32_t) + 1);
            }
            else {
                nums.push_back(static_cast<unsigned>(byte));
            }

            ++i;
        }

        std::vector<unsigned> arr;
        unsigned prev = ai::MAX_ELEM + 5;
        unsigned next = ai::MAX_ELEM + 5;
        unsigned num = ai::MAX_ELEM + 5;
        for (size_t i = 0; i != nums.size();) {
            if (i < nums.size() - 1) {
                next = static_cast<unsigned>(nums[i + 1]);
            }

            num = nums[i];

            // RLE Range
            if (static_cast<unsigned>(num) == ai::MAX_ELEM + 1
                || static_cast<unsigned>(num == ai::MAX_ELEM + 2))
            {
                assert(i > 0 && i < nums.size() - 1);
                bool increasing = static_cast<unsigned>(num) == ai::MAX_ELEM + 1;
                for (uint8_t num = prev + (increasing ? 1 : -1);
                        num != next; 
                        increasing ? ++num : --num)
                {
                    arr.push_back(static_cast<unsigned>(num));
                }

                prev = num;
            }
            else {
                arr.push_back(static_cast<unsigned>(num));

                prev = num;
            }

            ++i;
        }

        return arr;
    }
} // ai
