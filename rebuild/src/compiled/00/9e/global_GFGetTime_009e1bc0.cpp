#include "fable_stopwatch.h"
extern "C" __declspec(dllimport) unsigned long FABLE_STDCALL timeGetTime();
extern bool g_FableTimerInitialised_013CAA10;
extern bool g_FableHighResolutionTimer_013CAA00;
extern __int64 g_FableTimerFrequency_013CAA08;
extern __int64 g_FableTimerStartTick_013CB2F0;
double FABLE_FASTCALL GFGetTime() {
    __int64 ticks;
    if (!g_FableTimerInitialised_013CAA10) {
        g_FableHighResolutionTimer_013CAA00 = QueryPerformanceCounter(&ticks) == 1;
        QueryPerformanceFrequency(&g_FableTimerFrequency_013CAA08);
        g_FableTimerInitialised_013CAA10 = true;
    }
    if (g_FableHighResolutionTimer_013CAA00) {
        QueryPerformanceCounter(&ticks);
        return static_cast<double>(ticks - g_FableTimerStartTick_013CB2F0) / g_FableTimerFrequency_013CAA08;
    }
    return (static_cast<__int64>(timeGetTime()) - g_FableTimerStartTick_013CB2F0) * 0.001;
}