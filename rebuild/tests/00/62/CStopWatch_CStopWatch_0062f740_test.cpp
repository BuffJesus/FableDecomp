#include "fable_stopwatch.h"
#include "test_support/import_redirect.h"
#include <new>
#include <stdio.h>
static __int64 ticks;
static bool okClock;
static int calls;
static int FABLE_STDCALL Clock(__int64* output) {
    ++calls;
    if (!okClock) return 0;
    *output = ticks;
    return 1;
}
int main() {
    union { double alignment; unsigned char bytes[32]; } storage;
    FableTest::ImportRedirect clock("QueryPerformanceFrequency", reinterpret_cast<void*>(&Clock));
    if (!clock.Installed()) return 20;

    for (int failure = 0; failure != 2; ++failure) {
        memset(&storage, 0xA5, sizeof(storage));
        okClock = !failure; ticks = 4; calls = 0;
        CStopWatch* watch = new (storage.bytes) CStopWatch;
        if (calls != 1 || watch->m_fTimerPeriod != (failure ? 1.0f : 0.25f) ||
            watch->m_nStartTick || watch->m_nPrevElapsedTicks || watch->m_bIsRunning) return 1;
        for (int i = 4; i < 8; ++i) if (storage.bytes[i] != 0xA5) return 2;
        for (int i = 25; i < 32; ++i) if (storage.bytes[i] != 0xA5) return 3;
    }
    puts("FABLETLC_STOPWATCH_CSTOPWATCH PASS");
    return 0;
}