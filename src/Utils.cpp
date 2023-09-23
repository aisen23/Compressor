#include "pch.h"

#include "Utils.h"

#include "Input.h"

#ifdef __linux__
#include <arpa/inet.h>
#elif _WIN32
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#endif

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

    std::vector<unsigned> CreateRandomArrayWithConsecutiveOrRangeElements() {
        size_t size = ai::ARRAY_SIZE;
        std::vector<unsigned> arr(size);

        std::vector<int> types = { 0, 0, 0 };
        unsigned prev = 0;
        for (size_t i = 0; i != size; ++i) {
            if ((types[0]--) > 0) {
                arr[i] = prev + 1;
                if (arr[i] >= ai::MAX_ELEM) {
                    arr[i] = ai::MAX_ELEM;
                    types[0] = 0;
                }
            }
            else if ((types[1]--) > 0) {
                arr[i] = prev - 1;
                if (arr[i] <= ai::MIN_ELEM) {
                    arr[i] = ai::MIN_ELEM;
                    types[1] = 0;
                }
            }
            else if (prev > 0 && (types[2]--) > 0) {
                arr[i] = prev;
            }
            else {
                arr[i] = rand() % (ai::MAX_ELEM - ai::MIN_ELEM + 1) + ai::MIN_ELEM;

                int type = rand() % 20;
                if (type < 3) {
                    int rNum = rand() % (ai::MAX_ELEM - ai::MIN_ELEM + 1) + ai::MIN_ELEM; 
                    types[type] = rNum;
                }
            }
            prev = arr[i];
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

    bool CompareArrays(const std::vector<unsigned>& arr1, const std::vector<unsigned>& arr2) {
        if (arr1.size() != arr2.size()) {
            return false;
        }

        for (size_t i = 0; i != arr1.size(); ++i) {
            if (arr1[i] != arr2[i]) {
                return false;
            }
        }

        return true;
    }

    uint32_t htonl(uint32_t hostValue) {
        return ::htonl(hostValue);
    }

    uint32_t ntohl(uint32_t networkValue) {
        return ::ntohl(networkValue);
    }
}
