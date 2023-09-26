#include "pch.h"

#include "RLECompressorImpl.h"

#include "Input.h"
#include "Utils.h"

namespace ai
{
    std::vector<uint8_t> RLECompressorImpl::Compress(const std::vector<unsigned>& arr) {
        if (arr.size() == 0) {
            std::cerr << "Error: RLECompressorImpl::Compress(): No input numbers" << std::endl;
            return {};
        }

        auto bytes = EncodeRLERange(arr);
        auto data = EncodeRLE(bytes);

        return data;
    }

    std::vector<unsigned> RLECompressorImpl::Uncompress(const std::vector<uint8_t>& data, size_t offset) {
        if (data.size() == 0) {
            std::cerr << "Error: RLECompressorImpl::Uncompress(): No data" << std::endl;
            return {};
        }

        auto nums = DecodeRLE(data, offset);
        auto arr = DecodeRLERange(nums);

        return arr;
    }

    std::vector<uint8_t> RLECompressorImpl::EncodeRLERange(const std::vector<unsigned>& arr) const {
        if (arr.size() == 0) {
            std::cerr << "Error: RLECompressorImpl::EncodeRLERange(): No input numbers" << std::endl;
            return {};
        }

        std::vector<uint8_t> data;
        // RLE Range.
        size_t incCount = 0;
        size_t decCount = 0;
        uint8_t num = arr[0];
        uint8_t prev = num;
        uint32_t effCount = 2;
        data.push_back(num);
        for (size_t i = 1; i != arr.size(); ++i) {
            num = static_cast<uint8_t>(arr[i]);
            if (num == prev + 1) {
                ++incCount;
                // Put into data just if it gives us the space saving.
                if (incCount > effCount) {
                    if (incCount == effCount + 1) {
                        data.pop_back();
                        data.pop_back();
                        data.push_back(static_cast<uint8_t>(ai::MAX_ELEM + 1));
                    }
                    if (i != arr.size() - 1) {
                        prev = num;
                        continue;
                    }
                }
            }
            else {
                if (incCount > effCount) {
                    data.push_back(prev);
                }

                incCount = 0;
            }

            if (num == prev - 1) {
                ++decCount;
                if (decCount > effCount) {
                    if (decCount == effCount + 1) {
                        data.pop_back();
                        data.pop_back();
                        data.push_back(static_cast<uint8_t>(ai::MAX_ELEM + 2));
                    }
                    if (i != arr.size() - 1) {
                        prev = num;
                        continue;
                    }
                }
            }
            else {
                if (decCount > effCount) {
                    data.push_back(prev);
                }

                decCount = 0;
            }
            
            data.push_back(num);

            prev = static_cast<uint8_t>(arr[i]);
        }

        return data;
    }

    std::vector<uint8_t> RLECompressorImpl::EncodeRLE(const std::vector<uint8_t>& bytes) const {
        if (bytes.size() == 0) {
            std::cerr << "Error: RLECompressorImpl::EncodeRLE(): No input numbers" << std::endl;
            return {};
        }

        std::vector<uint8_t> data; 
        uint8_t num = bytes[0];
        if (bytes.size() == 1 && !_combined) {
            num |= (1 << 7);
        }
        data.push_back(num);

        uint32_t rangeCount = 1;
        uint8_t rangeValue = bytes[0];
        uint32_t effCount = 6;
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

                uint32_t networkRangeCount = ai::utils::Htonl(rangeCount);
                std::memcpy(temp.data(), &networkRangeCount, sizeof(uint32_t));

                data.insert(data.end(), temp.begin(), temp.end());

                if (!isLast || num != rangeValue) {
                    data.push_back(rangeValue);
                }

                rangeCount = 1;
                rangeValue = num;
            }

            if (isLast && !_combined) {
                num |= (1 << 7);
            }

            if (isLast || !creatingRange) {
                data.push_back(num);
            }
        }

        return data;
    }

    std::vector<unsigned> RLECompressorImpl::DecodeRLE(const std::vector<uint8_t>& data, size_t offset) const {
        if (data.size() == 0) {
            std::cerr << "Error: RLECompressorImpl::DecodeRLE(): No data" << std::endl;
            return {};
        }

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
                uint32_t rangeValue = ai::utils::Ntohl(networkRangeValue);
                
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

        return nums;
    }

    std::vector<unsigned> RLECompressorImpl::DecodeRLERange(const std::vector<unsigned>& nums) const {
        if (nums.size() == 0) {
            std::cerr << "Error: RLECompressorImpl::DecodeRLE(): No numbers" << std::endl;
            return {};
        }

        std::vector<unsigned> arr;
        unsigned prev = ai::MAX_ELEM + 5;
        unsigned next = ai::MAX_ELEM + 5;
        unsigned num = ai::MAX_ELEM + 5;
        for (size_t i = 0; i != nums.size();) {
            if (i < nums.size() - 1) {
                next = nums[i + 1];
            }

            num = nums[i];

            // RLE Range
            if (static_cast<unsigned>(num) == ai::MAX_ELEM + 1
                || static_cast<unsigned>(num == ai::MAX_ELEM + 2))
            {
                assert(i > 0 && i < nums.size() - 1);
                bool increasing = (num == ai::MAX_ELEM + 1);
                for (unsigned num = prev + (increasing ? 1 : -1);
                        num != next; 
                        increasing ? ++num : --num)
                {
                    arr.push_back(num);
                }

                prev = num;
            }
            else {
                arr.push_back(num);

                prev = num;
            }

            ++i;
        }

        return arr;
    }

    std::vector<unsigned> RLECompressorImpl::UncompressCombined(const std::vector<unsigned>& nums) {
        if (nums.size() == 0) {
            std::cerr << "Error: RLECompressorImpl::UncompressCombined(): No data" << std::endl;
            return {};
        }

        auto nums2 = DecodeRLECombined(nums);
        auto arr = DecodeRLERange(nums2);

        return arr;
    }

    std::vector<unsigned> RLECompressorImpl::DecodeRLECombined(const std::vector<unsigned>& data) const {
        if (data.size() == 0) {
            std::cerr << "Error: RLECompressorImpl::DecodeRLECombined(): No data" << std::endl;
            return {};
        }

        std::vector<unsigned> nums;
        unsigned likeByte = ai::MAX_ELEM + 5;
        for (size_t i = 0; i != data.size();) {
            likeByte = data[i];

            if (likeByte == ai::MAX_ELEM + 3) {
                std::vector<uint8_t> temp(sizeof(uint32_t));
                for (int j = 0; j != sizeof(uint32_t); ++j) {
                    temp[j] = static_cast<uint8_t>(data[i + j + 1]);
                }

                uint32_t networkRangeValue;
                std::memcpy(&networkRangeValue, temp.data(), sizeof(uint32_t));
                uint32_t rangeValue = ai::utils::Ntohl(networkRangeValue);
                
                size_t index = i + sizeof(uint32_t) + 1;
                unsigned num = data[index];
                for (size_t i = 0; i < rangeValue; ++i) {
                    nums.push_back(num);
                }

                i += (sizeof(uint32_t) + 1);
            }
            else {
                nums.push_back(likeByte);
            }

            ++i;
        }

        return nums;
    }
} // ai
