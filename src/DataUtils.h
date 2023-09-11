#pragma once

#include <vector>

namespace ai
{
    namespace data_utils
    {
        // Generating source array with int from 1 to 100.
        std::vector<int> CreateAnArray(size_t size);

        // TODO: change void to some type.
        char* CompressArray(const std::vector<int>& srcArray);
        std::vector<int> DecompressToArray(char* dataToDecompress, size_t size);

        /** Test function:
         * printing first 15, mid 15 and last 15 elements of array
         * */
        void PrintArray(const char* nameOfArray, const std::vector<int>& arrayToPrint);
        void PrintCompressed(char* compressed, size_t size);
    } // data_utils
} // ai
