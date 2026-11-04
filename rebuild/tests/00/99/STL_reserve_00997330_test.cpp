// Behaviour fixture for _STL::vector<char>::reserve (0x00997330): grows to
// exactly the requested capacity, keeping the bytes; never shrinks.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fable_stlport_vector.h"

struct RawVector
{
    char* start;
    char* finish;
    char* endOfStorage;
};

int main()
{
    int fail = 0;
    _STL::vector<char> v;
    RawVector& raw = *reinterpret_cast<RawVector*>(&v);

    // Empty: allocates exactly n.
    v.reserve(8);
    if (!raw.start || raw.finish != raw.start || raw.endOfStorage != raw.start + 8) fail |= 1;

    // With contents: grows, keeps the bytes and the size.
    memcpy(raw.start, "abcde", 5);
    raw.finish = raw.start + 5;
    char* before = raw.start;
    v.reserve(32);
    if (raw.endOfStorage != raw.start + 32 || raw.finish != raw.start + 5 || memcmp(raw.start, "abcde", 5) != 0 ||
        raw.start == before)
        fail |= 2;

    // Smaller or equal: nothing changes.
    before = raw.start;
    v.reserve(16);
    v.reserve(32);
    if (raw.start != before || raw.endOfStorage != raw.start + 32) fail |= 4;

    free(raw.start);
    raw.start = raw.finish = raw.endOfStorage = 0;
    if (fail == 0)
        printf("FABLETLC_STLVECTOR_CHAR_RESERVE_00997330_PASS\n");
    else
        printf("FABLETLC_STLVECTOR_CHAR_RESERVE_00997330 FAIL %d\n", fail);
    return fail;
}