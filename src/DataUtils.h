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
        void PrintArray(const char* nameOfArray, const std::vector<T>& array);

    } // utils
} // ai
