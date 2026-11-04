#include "fable_game.h"
#include "fable_stopwatch.h"
#include "test_support/import_redirect.h"
#include <stdio.h>
#pragma comment(linker, "/alternatename:__imp__timeGetTime@0=_g_TestTimeGetTime")
bool g_FableTimerInitialised_013CAA10;
bool g_FableHighResolutionTimer_013CAA00;
__int64 g_FableTimerFrequency_013CAA08;
__int64 g_FableTimerStartTick_013CB2F0;
static __int64 ticks;
static int queryResult;
static int counterCalls, frequencyCalls, millisecondCalls;
static unsigned long milliseconds;
static int FABLE_STDCALL Counter(__int64* value) { ++counterCalls; *value=ticks; return queryResult; }
static int FABLE_STDCALL Frequency(__int64* value) { ++frequencyCalls; *value=4; return 1; }
static unsigned long FABLE_STDCALL Milliseconds() { ++millisecondCalls; return milliseconds; }
extern "C" unsigned long (FABLE_STDCALL *g_TestTimeGetTime)() = &Milliseconds;
int main() {
    FableTest::ImportRedirect counter("QueryPerformanceCounter", reinterpret_cast<void*>(&Counter));
    FableTest::ImportRedirect frequency("QueryPerformanceFrequency", reinterpret_cast<void*>(&Frequency));
    if (!counter.Installed() || !frequency.Installed()) return 20;
    for (int result=0; result<3; ++result) {
        g_FableTimerInitialised_013CAA10=false;
        g_FableHighResolutionTimer_013CAA00=false;
        g_FableTimerFrequency_013CAA08=-1;
        g_FableTimerStartTick_013CB2F0=80;
        counterCalls=frequencyCalls=millisecondCalls=0;
        ticks=100; milliseconds=580; queryResult=result;
        volatile double actual=GFGetTime();
        if (actual!=(result==1 ? 5.0 : 0.5)) return 1;
        if (!g_FableTimerInitialised_013CAA10 || g_FableHighResolutionTimer_013CAA00!=(result==1) ||
            g_FableTimerFrequency_013CAA08!=4 || g_FableTimerStartTick_013CB2F0!=80) return 2;
        if (counterCalls!=(result==1 ? 2 : 1) || frequencyCalls!=1 || millisecondCalls!=(result==1 ? 0 : 1)) return 3;
        ticks=108; milliseconds=1080;
        actual=GFGetTime();
        if (actual!=(result==1 ? 7.0 : 1.0) || frequencyCalls!=1 || counterCalls!=(result==1 ? 3 : 1)) return 4;
    }
    g_FableHighResolutionTimer_013CAA00=true;
    g_FableTimerFrequency_013CAA08=8;
    g_FableTimerStartTick_013CB2F0=0xFFFFFFF0i64;
    ticks=0x100000010i64; queryResult=1; counterCalls=frequencyCalls=millisecondCalls=0;
    volatile double actual=GFGetTime();
    if (actual!=4.0 || counterCalls!=1 || frequencyCalls || millisecondCalls) return 5;
    puts("FABLETLC_GF_GET_TIME PASS"); return 0;
}