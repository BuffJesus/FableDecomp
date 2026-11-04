// Fixture: CVsyncCallbackTimer::GetInfo (0x009F7050). The retail statics are
// defined here; the Interlocked pair is kernel32's real one, so the lock count
// must come back unchanged and only the latest block may be read.
#include <stdio.h>
#include <string.h>
#include "fable_display_viewport.h"

static int g_fail;
#define EXPECT(c) do { if (!(c)) { printf("FAIL %d: %s\n", __LINE__, #c); ++g_fail; } } while (0)

static volatile long* g_locked0;
static volatile long* g_locked1;
static volatile long* g_latest;

volatile CVsyncCallbackTimer::CGPUInfoBlock CVsyncCallbackTimer::GPUBlocks[2] = {
    { 10.5, 9.5, 100, 3 },
    { 20.25, 19.75, 200, 5 },
};
double CVsyncCallbackTimer::CPUTimeStamp = 1.25;
double CVsyncCallbackTimer::CPUPrevTimeStamp = 0.75;
long CVsyncCallbackTimer::CPUFrameCount = 42;
// Class-scope initializer: may take the private blocks' addresses.
volatile long CVsyncCallbackTimer::LatestBlockIndex =
    (g_locked0 = &GPUBlocks[0].Locked, g_locked1 = &GPUBlocks[1].Locked, g_latest = &LatestBlockIndex, 1);

int main()
{
    static unsigned char timer[64];
    memset(timer, 0xCD, sizeof(timer));
    CVsyncCallbackTimer* t = reinterpret_cast<CVsyncCallbackTimer*>(timer);
    CVsyncCallbackTimerInfo info = t->GetInfo();
    EXPECT(info.CPUTimeStamp == 1.25 && info.CPUPrevTimeStamp == 0.75 && info.CPUFrameCount == 42);
    EXPECT(info.GPUTimeStamp == 20.25 && info.GPUPrevTimeStamp == 19.75 && info.GPUFrameCount == 200);
    EXPECT(*g_locked1 == 5 && *g_locked0 == 3);
    *g_latest = 0;
    info = t->GetInfo();
    EXPECT(info.GPUTimeStamp == 10.5 && info.GPUPrevTimeStamp == 9.5 && info.GPUFrameCount == 100);
    EXPECT(info.CPUFrameCount == 42);
    EXPECT(*g_locked1 == 5 && *g_locked0 == 3);
    for (unsigned i = 0; i < sizeof(timer); ++i) if (timer[i] != 0xCD) { EXPECT(!"timer object written"); break; }
    if (g_fail) { printf("FAILED %d\n", g_fail); return 1; }
    printf("FABLETLC_VSYNC_TIMER_GET_INFO PASS\n");
    return 0;
}