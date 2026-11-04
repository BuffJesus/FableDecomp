// lib_global_tools.cpp, retail 0x009E1CB0 (Ego_r ?GFSetSystemStartTime@@YIXXZ):
// the tick GFGetTime (0x009E1BC0) measures from. That is the performance
// counter when it works, else timeGetTime(). Its neighbours GFGetFastSubFrameTimer
// 0x009E1C70 and GFRecalibrateFastSubFrameTimer 0x009E24B0 read rdtsc, which
// VC7.1 can only express as inline asm, so they are not reconstructed.
#include "fable_stopwatch.h"

extern "C" __declspec(dllimport) unsigned long FABLE_STDCALL timeGetTime();
extern __int64 g_FableTimerStartTick_013CB2F0;

void FABLE_FASTCALL GFSetSystemStartTime()
{
    __int64 ticks;
    if (QueryPerformanceCounter(&ticks) == 1)
    {
        QueryPerformanceCounter(&ticks);
        g_FableTimerStartTick_013CB2F0 = ticks;
    }
    else
        g_FableTimerStartTick_013CB2F0 = timeGetTime();
}
