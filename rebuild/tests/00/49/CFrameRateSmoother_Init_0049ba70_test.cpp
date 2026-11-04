#include <stdio.h>
#include <string.h>
#include "fable_game.h"

static bool g_initialiseResult;
static int g_initialiseCalls;
static const void* g_initialiseReceiver;
static int g_resizeCalls;
static const void* g_resizeReceiver;
static size_t g_resizeCount;
static float g_resizeValue;

bool CInitBaseClass::Initialise()
{
    ++g_initialiseCalls;
    g_initialiseReceiver = this;
    return g_initialiseResult;
}

template <>
void _STL::vector<float, _STL::allocator<float> >::resize(size_t n, const float& value)
{
    ++g_resizeCalls;
    g_resizeReceiver = this;
    g_resizeCount = n;
    g_resizeValue = value;
}

struct FrameRateSmootherBytes
{
    unsigned char base[8];
    unsigned char times[12];
    long MaxNoTimes;
    long NoTimes;
    long NoFramesToChangeFrameRateOver;
    long FirstTime;
    long LastTime;
    double SmoothedTime;
};

int main()
{
    FABLE_STATIC_ASSERT(sizeof(FrameRateSmootherBytes) == sizeof(CFrameRateSmoother));
    union
    {
        double align;
        FrameRateSmootherBytes raw;
    } storage;
    CFrameRateSmoother* smoother = reinterpret_cast<CFrameRateSmoother*>(&storage.raw);

    // Base refuses: nothing is written and resize is not called.
    memset(&storage, 0xEE, sizeof(storage));
    g_initialiseResult = false;
    if (smoother->Init(10, 4)) return 1;
    if (g_initialiseCalls != 1 || g_initialiseReceiver != &storage.raw) return 2;
    if (g_resizeCalls != 0 || storage.raw.MaxNoTimes != (long)0xEEEEEEEE) return 3;

    const long capacity[] = { 10, 1, 0 };
    const long frames[] = { 4, -2, 7 };
    for (int i = 0; i < 3; ++i)
    {
        memset(&storage, 0xEE, sizeof(storage));
        g_initialiseResult = true;
        g_resizeValue = -1.0f;
        if (!smoother->Init(capacity[i], frames[i])) return 10 + i;
        if (g_resizeCalls != i + 1) return 20 + i;
        if (g_resizeReceiver != storage.raw.times) return 30 + i;
        if (g_resizeCount != (size_t)capacity[i] || g_resizeValue != 0.0f) return 40 + i;
        if (storage.raw.MaxNoTimes != capacity[i]) return 50 + i;
        if (storage.raw.NoFramesToChangeFrameRateOver != frames[i]) return 60 + i;
        if (storage.raw.NoTimes != 0 || storage.raw.FirstTime != 0 || storage.raw.LastTime != 0)
            return 70 + i;
        // Retail widens the float literal 0.1f, not the double 0.1.
        if (storage.raw.SmoothedTime != (double)0.1f || storage.raw.SmoothedTime == 0.1)
            return 80 + i;
        if (storage.raw.base[0] != 0xEE || storage.raw.times[0] != 0xEE) return 90 + i;
    }
    puts("FABLETLC_FRAME_SMOOTHER_INIT PASS");
    return 0;
}