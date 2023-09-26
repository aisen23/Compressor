#pragma once

#include <vector>

namespace ai
{
    namespace utils
    {
        std::vector<unsigned> CreateRandomArray();

        std::vector<unsigned> CreateRandomArrayWithConsecutiveOrRangeElements();

        std::vector<unsigned> CreateConsecutiveArray();

        char UintToChar(unsigned digit);
        unsigned CharToUint(char ch);

        bool CompareArrays(const std::vector<unsigned>& arr1,
                            const std::vector<unsigned>& arr2);

        uint32_t Htonl(uint32_t hostValue);
        uint32_t Ntohl(uint32_t networkValue);

        /** Test function:
         * printing first 10, mid 10 and last 10 elements of array
         * */
        template <typename T>
        void PrintArray(const char* nameOfArray, const std::vector<T>& arr) {
            std::cout << nameOfArray << ": ";

            size_t size = arr.size();
            if (size == 0) {
                return;
            }

            size_t count = 10;

            for (size_t i = 0; i != count; ++i) {
                assert(i < size);
                std::cout << arr[i] << ", ";
            }

            std::cout << "....... ";

            for (size_t i = size / 2; i != size / 2 + count; ++i) {
                assert(i < size);
                std::cout << arr[i] << ", ";
            }

            std::cout << "....... ";

            for (size_t i = size - count - 1; i != size; ++i) {
                assert(i < size);
                std::cout << arr[i] << ", ";
            }

            std::cout << "\nSize: " << arr.size() << "\n";
            std::cout << "Bytes: " << arr.size() * sizeof(T) << "\n\n";
        }

        template <typename T, typename Cast>
        void PrintArray(const char* nameOfArray, const std::vector<T>& arr) {
            std::cout << nameOfArray << ": ";

            size_t size = arr.size();
            if (size == 0) {
                return;
            }

            for (size_t i = 0; i != size; ++i) {
                std::cout << static_cast<Cast>(arr[i]) << ", ";
            }

            std::cout << "\nSize: " << arr.size() << "\n";
            std::cout << "Bytes: " << arr.size() * sizeof(T) << "\n\n";
        }

    } // utils
} // ai
