#pragma once

#include <vector>

namespace ai
{
    namespace utils
    {

        // Generating source array with int from 1 to 100.
        std::vector<unsigned> CreateRandomArray();

        std::vector<unsigned> CreateConsecutiveArray();

        std::vector<char> UintArrayToCharArray(const std::vector<unsigned>& arr);
        std::vector<unsigned> CharArrayToUintArray(const std::vector<char>& chars);
        char UintToChar(unsigned digit);
        unsigned CharToUint(char ch);

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

    } // utils
} // ai
