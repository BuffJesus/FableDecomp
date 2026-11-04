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
    FableTest::ImportRedirect clock("QueryPerformanceCounter", reinterpret_cast<void*>(&Clock));
    if (!clock.Installed()) return 20;

    for (int running = 0; running != 2; ++running) {
      for (int failure = 0; failure != 2; ++failure) {
        memset(&storage, 0xA5, sizeof(storage));
        CStopWatch* watch = reinterpret_cast<CStopWatch*>(storage.bytes);
        watch->m_fTimerPeriod = 0.25f;
        watch->m_nStartTick = 0xFFFFFFFEi64;
        watch->m_nPrevElapsedTicks = 0x100000001i64;
        watch->m_bIsRunning = running != 0;
        unsigned char expectedBytes[32]; memcpy(expectedBytes, storage.bytes, 32);
        CStopWatch* expected = reinterpret_cast<CStopWatch*>(expectedBytes);
        okClock = !failure; ticks = 0x100000002i64; calls = 0;
        expected->m_nStartTick = failure ? 0 : 0x100000002i64;
        expected->m_bIsRunning = true;
        watch->Start();
        if (calls != 1) return 1;
        if (memcmp(expectedBytes, storage.bytes, 32)) return 2;
      }
    }
    puts("FABLETLC_STOPWATCH_START PASS");
    return 0;
}