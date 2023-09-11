#pragma once

#include <vector>

namespace ai
{
    namespace data_utils
    {
        // Generating source array with int from 1 to 100.
        std::vector<int> CreateAnArray(size_t size);

        // TODO: change void to some type.
        void CompressArray(const std::vector<int>& srcArray);
        std::vector<int> DecompressArray(int* dataToDecompress);

        /** Test function:
         * printing first 15, mid 15 and last 15 elements of array
         * */
        void PrintArray(const char* nameOfArray, const std::vector<int>& arrayToPrint);
    } // data_utils
} // ai
