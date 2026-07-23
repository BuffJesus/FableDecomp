#include <cstdint>

int* __fastcall Array_LinearSearchInt(int* begin, int* end, int* value)
{
    int chunkCount =
        (static_cast<int>(reinterpret_cast<std::intptr_t>(end)) -
         static_cast<int>(reinterpret_cast<std::intptr_t>(begin))) >>
        4;

    if (0 < chunkCount) {
        const int needle = *value;

        do {
            if (begin[0] == needle) {
                return begin;
            }
            if (begin[1] == needle) {
                return begin + 1;
            }
            if (begin[2] == needle) {
                return begin + 2;
            }
            if (begin[3] == needle) {
                return begin + 3;
            }

            begin += 4;
            chunkCount = chunkCount + -1;
        } while (0 < chunkCount);
    }

    const int remainingCount =
        (static_cast<int>(reinterpret_cast<std::intptr_t>(end)) -
         static_cast<int>(reinterpret_cast<std::intptr_t>(begin))) >>
        2;

    if (remainingCount != 1) {
        if (remainingCount != 2) {
            if (remainingCount != 3) {
                return end;
            }

            if (*begin == *value) {
                return begin;
            }
            begin = begin + 1;
        }

        if (*begin == *value) {
            return begin;
        }
        begin = begin + 1;
    }

    if (*begin != *value) {
        return end;
    }

    return begin;
}