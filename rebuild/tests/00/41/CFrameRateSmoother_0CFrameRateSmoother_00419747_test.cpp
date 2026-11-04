#include <new>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_game.h"

static int g_baseConstructs;
static int g_baseDestroys;
static const void* g_baseReceiver;

CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CInitBaseClass::CInitBaseClass()
{
    ++g_baseConstructs;
    g_baseReceiver = this;
}
CInitBaseClass::~CInitBaseClass()
{
    ++g_baseDestroys;
    g_baseReceiver = this;
}
void CInitBaseClass::Validate() const {}

struct FrameRateSmootherBytes
{
    void* vtable;
    unsigned char base[4];
    float* start;
    float* finish;
    float* endOfStorage;
    unsigned char rest[0x1C];
};

int main()
{
    FABLE_STATIC_ASSERT(sizeof(FrameRateSmootherBytes) == sizeof(CFrameRateSmoother));
    union
    {
        double align;
        unsigned char bytes[sizeof(CFrameRateSmoother)];
    } storage;
    memset(storage.bytes, 0xCC, sizeof(storage.bytes));
    CFrameRateSmoother* smoother = new (storage.bytes) CFrameRateSmoother;
    FrameRateSmootherBytes* raw = reinterpret_cast<FrameRateSmootherBytes*>(storage.bytes);
    if (g_baseConstructs != 1 || g_baseReceiver != storage.bytes) return 1;
    if (raw->vtable == 0 || raw->vtable == reinterpret_cast<void*>(0xCCCCCCCC)) return 2;
    if (raw->start != 0 || raw->finish != 0 || raw->endOfStorage != 0) return 3;
    // Counters and SmoothedTime wait for Init.
    for (unsigned int i = 0; i < sizeof(raw->rest); ++i)
        if (raw->rest[i] != 0xCC) return 4;
    if (raw->base[0] != 0xCC) return 5;  // Valid is the stubbed base's business

    // The implicit destructor frees the sample storage, then runs the base.
    raw->start = static_cast<float*>(malloc(4 * sizeof(float)));
    raw->finish = raw->start + 2;
    raw->endOfStorage = raw->start + 4;
    smoother->~CFrameRateSmoother();
    if (g_baseDestroys != 1 || g_baseReceiver != storage.bytes) return 6;

    // Empty storage skips the free and still reaches the base.
    new (storage.bytes) CFrameRateSmoother;
    smoother->~CFrameRateSmoother();
    if (g_baseConstructs != 2 || g_baseDestroys != 2) return 7;

    // Virtual delete reaches the scalar deleting destructor in vtable slot 0.
    CFrameRateSmoother* heap = new CFrameRateSmoother;
    delete heap;
    if (g_baseConstructs != 3 || g_baseDestroys != 3) return 8;
    puts("FABLETLC_FRAME_SMOOTHER_CTOR PASS");
    return 0;
}