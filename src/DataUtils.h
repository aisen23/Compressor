#pragma once

#include <vector>

namespace ai
{
    namespace utils
    {

        // Generating source array with int from 1 to 100.
        std::vector<int> CreateAnArray(size_t size);

        /** Test function:
         * printing first 15, mid 15 and last 15 elements of array
         * */
        template <typename T>
        void PrintArray(const char* nameOfArray, const std::vector<T>& array) {
            std::cout << nameOfArray << ": ";

            size_t size = array.size();
            if (size == 0) {
                return;
            }

            size_t count = 15;

            for (size_t i = 0; i != count; ++i) {
                assert(i < size);
                std::cout << array[i] << ", ";
            }

            std::cout << "....... ";

            for (size_t i = size / 2; i != size / 2 + count; ++i) {
                assert(i < size);
                std::cout << array[i] << ", ";
            }

            std::cout << "....... ";

            for (size_t i = size - count - 1; i != size; ++i) {
                assert(i < size);
                std::cout << array[i] << ", ";
            }

            std::cout << "\n";
        }

    } // utils
} // ai
