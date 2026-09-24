#include "fable_ui_swapping.h"
#include <stdio.h>

int main(int argc, char** argv)
{
    if (argc != 2) return 2;
    FILE* input = fopen(argv[1], "r");
    if (!input) return 3;
    unsigned count, changed, random;
    float time;
    FableUiSwappingState state;
    while (fscanf(input, "%u %lu %lu %lu %f %f %u %u", &count,
        &state.CurrentState, &state.TargetState, &state.Seed,
        &state.LastSwapTime, &time, &changed, &random) == 8)
    {
        if (count > 8) return 4;
        FableUiSwapEntry entries[8];
        for (unsigned i = 0; i != count; ++i)
            if (fscanf(input, "%lu %f", &entries[i].State, &entries[i].Dwell) != 2)
                return 5;
        fable_u32 requested = 0xDEADBEEF;
        bool selected = state.SelectAfterUpdate(time, changed != 0, random != 0,
            entries, count, requested);
        printf("%u %lu %lu %.9g\n", selected ? 1 : 0, requested, state.Seed, state.LastSwapTime);
    }
    fclose(input);
    return 0;
}
