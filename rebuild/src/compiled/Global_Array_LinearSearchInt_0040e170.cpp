#include "rebuild_abi.h"

int* FABLE_FASTCALL Array_LinearSearchInt(int* begin, int* end, int* value)
{
    int chunkCount = (reinterpret_cast<int>(end) - reinterpret_cast<int>(begin)) >> 4;
    if (0 < chunkCount)
    {
        const int needle = *value;
        do
        {
            if (begin[0] == needle) return begin;
            if (begin[1] == needle) return begin + 1;
            if (begin[2] == needle) return begin + 2;
            if (begin[3] == needle) return begin + 3;
            begin += 4;
            --chunkCount;
        } while (0 < chunkCount);
    }

    const int remainingCount =
        (reinterpret_cast<int>(end) - reinterpret_cast<int>(begin)) >> 2;
    if (remainingCount != 1)
    {
        if (remainingCount != 2)
        {
            if (remainingCount != 3) return end;
            if (*begin == *value) return begin;
            ++begin;
        }
        if (*begin == *value) return begin;
        ++begin;
    }
    if (*begin != *value) return end;
    return begin;
}
