#include "pch.h"

#include "Utils.h"

#include "Input.h"

namespace ai::utils
{
    std::vector<unsigned> CreateRandomArray() {
        size_t size = ai::ARRAY_SIZE;
        std::vector<unsigned> arr(size);

        for (size_t i = 0; i != size; ++i) {
            arr[i] = rand() % (ai::MAX_ELEM - ai::MIN_ELEM + 1) + ai::MIN_ELEM;
        }

        return arr;
    }

    std::vector<unsigned> CreateConsecutiveArray() {
        size_t size = ai::ARRAY_SIZE;
        std::vector<unsigned> arr(size);

        int num = 1;
        for (size_t i = 0; i != size; ++i) {
            arr[i] = num;
            ++num;
            if (num > ai::MAX_ELEM) {
                num = ai::MIN_ELEM;
            }
        }

        return arr;
    }

    std::vector<char> UintArrayToCharArray(const std::vector<unsigned>& arr) {
        std::vector<char> chars;
        char divider = ' ';

        if (arr.size() > 0) {
            for (size_t i = 0; i != arr.size(); ++i) {
                unsigned num = arr[i];
                while (num != 0) {
                    unsigned digit = num % 10;
                    chars.push_back(UintToChar(digit));
                    num /= 10;
                }

                if (i != arr.size() - 1) {
                    chars.push_back(divider);
                }
            }
        }

        return chars;
    }
        
    std::vector<unsigned> CharArrayToUintArray(const std::vector<char>& chars) {
        std::vector<unsigned> arr;
        auto size = chars.size();
        char divider = ' ';

        if (size > 0) {
            unsigned num = 0;
            std::vector<unsigned> nums;
            for (size_t i = 0; i != size; ++i) {
                char ch = chars[i];
                if (std::isdigit(ch)) {
                    nums.push_back(CharToUint(ch));
                }
                
                bool isLast = i == size - 1;
                if (ch == divider || isLast) {
                    assert(ch == divider || isLast);
                    assert(!nums.empty());
                    int mult = 1;
                    for (auto n : nums) {
                        num += (n * mult);
                        mult *= 10;
                    }

                    arr.push_back(num);
                    num = 0;
                    nums.clear();
                }
            }
        }

        return arr;
    }

    char UintToChar(unsigned digit) {
        assert(digit < 10);
        if (digit > 9) {
            std::cerr << "Error: utils::IntToChar(" << digit << ")" << std::endl;
            return '$';
        }

        return '0' + static_cast<char>(digit);
    }

    unsigned CharToUint(char ch) {
        assert(std::isdigit(ch));

        if (!std::isdigit(ch)) {
            std::cerr << "Error: utils::CharToInt(" << ch << ")" << std::endl;
            return 9999999;
        }

        return 0 + static_cast<unsigned>(ch - '0');
    }
}
