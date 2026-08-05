#include <stdio.h>
#include "rebuild_abi.h"

extern "C" int* FABLE_FASTCALL Array_LinearSearchInt(
    int* begin, int* end, int* ignored, int* value);

int main()
{
    int values[9] = { 11, 22, 33, 44, 55, 66, 77, 88, 99 };
    int needle = 11;
    if (Array_LinearSearchInt(values, values, &needle, &needle) != values) return 1;
    for (int count = 1; count <= 9; ++count)
    {
        for (int index = 0; index < count; ++index)
        {
            needle = values[index];
            if (Array_LinearSearchInt(values, values + count, &needle, &needle) != values + index) return 2;
        }
        needle = 1234;
        if (Array_LinearSearchInt(values, values + count, &needle, &needle) != values + count) return 3;
    }
    printf("ARRAY_LINEAR_SEARCH_TEST PASS\n");
    return 0;
}
