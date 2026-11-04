#include "fable_stopwatch.h"
#include <stdio.h>

extern bool g_FableHighResolutionTimer_013CAA00;
extern __int64 g_FableTimerFrequency_013CAA08;
extern bool g_FableTimerInitialised_013CAA10;
extern __int64 g_FableTimerStartTick_013CB2F0;

int main()
{
    if (g_FableHighResolutionTimer_013CAA00 || g_FableTimerFrequency_013CAA08 ||
        g_FableTimerInitialised_013CAA10 || g_FableTimerStartTick_013CB2F0) return 1;
    for (int i = 0; i < 16; ++i) {
        g_FableHighResolutionTimer_013CAA00 = (i & 1) != 0;
        g_FableTimerFrequency_013CAA08 = 1000000 + i;
        g_FableTimerInitialised_013CAA10 = (i & 2) != 0;
        g_FableTimerStartTick_013CB2F0 = (static_cast<__int64>(i) << 32) + i;
        if (g_FableHighResolutionTimer_013CAA00 != ((i & 1) != 0) ||
            g_FableTimerFrequency_013CAA08 != 1000000 + i ||
            g_FableTimerInitialised_013CAA10 != ((i & 2) != 0) ||
            g_FableTimerStartTick_013CB2F0 != (static_cast<__int64>(i) << 32) + i) return 2;
    }
    puts("GFGETTIME_DATA_PASS cases=16");
    return 0;
}
