#include "fable_frontend_random.h"
#include <stdio.h>

int main()
{
    const fable_u32 counts[] = {0, 1, 2, 3, 4, 7, 0xFFFFFFFFu};
    const fable_u32 seeds[] = {13, 0, 1, 0xFFFFFFFFu, 0x80000000u};
    for (unsigned c = 0; c != 7; ++c)
        for (unsigned s = 0; s != 5; ++s)
        {
            fable_u32 seed = seeds[s], previous = 0;
            for (unsigned step = 0; step != 128; ++step)
            {
                const fable_u32 before = seed;
                const fable_u32 selected = FableFrontendRandomFrame(counts[c], previous, seed);
                printf("%lu %lu %lu %lu %lu\n", counts[c], previous, before, selected, seed);
                previous = selected;
            }
        }
    return 0;
}
