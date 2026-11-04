#include "fable_stopwatch.h"
#include "test_support/import_redirect.h"
#include <stdio.h>
typedef __int64 (CStopWatch::*StopWatchTicksMethod)() const;
extern const StopWatchTicksMethod g_FableStopWatchGetTicks_0062F790;
static bool succeed;
static int calls;
static int FABLE_STDCALL Counter(__int64* value) {
    ++calls;
    if (!succeed) return 0;
    *value = 0x123456789ABCDEi64;
    return 1;
}
int main() {
    FableTest::ImportRedirect counter("QueryPerformanceCounter", reinterpret_cast<void*>(&Counter));
    if (!counter.Installed()) return 20;
    unsigned long storage[8] = {0};
    const CStopWatch* watch = reinterpret_cast<const CStopWatch*>(storage);
    succeed = true;
    if ((watch->*g_FableStopWatchGetTicks_0062F790)() != 0x123456789ABCDEi64) return 1;
    succeed = false;
    if ((watch->*g_FableStopWatchGetTicks_0062F790)() != 0 || calls != 2) return 2;
    puts("STOPWATCH_GET_TICKS PASS");
    return 0;
}
