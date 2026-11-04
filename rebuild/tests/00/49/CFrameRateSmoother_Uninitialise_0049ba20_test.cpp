#include <stdio.h>
#include <string.h>
#include "fable_game.h"

static bool g_valid;
static int g_order;
static int g_isValidAt;
static int g_uninitialiseAt;
static const void* g_isValidReceiver;
static const void* g_uninitialiseReceiver;

bool CInitBaseClass::IsValid() const
{
    g_isValidAt = ++g_order;
    g_isValidReceiver = this;
    return g_valid;
}

void CInitBaseClass::Uninitialise()
{
    g_uninitialiseAt = ++g_order;
    g_uninitialiseReceiver = this;
}

struct FrameRateSmootherBytes
{
    unsigned char base[8];
    float* start;
    float* finish;
    float* endOfStorage;
    unsigned char rest[0x1C];
};

int main()
{
    FABLE_STATIC_ASSERT(sizeof(FrameRateSmootherBytes) == sizeof(CFrameRateSmoother));
    float samples[4] = { 1.0f, 2.0f, 3.0f, 4.0f };
    const int filled[] = { 3, 0, 4 };
    for (int valid = 0; valid < 2; ++valid)
    {
        for (int i = 0; i < 3; ++i)
        {
            union
            {
                double align;
                FrameRateSmootherBytes raw;
            } storage;
            memset(&storage, 0x77, sizeof(storage));
            storage.raw.start = samples;
            storage.raw.finish = samples + filled[i];
            storage.raw.endOfStorage = samples + 4;
            g_valid = valid != 0;
            g_order = 0;
            reinterpret_cast<CFrameRateSmoother*>(&storage.raw)->Uninitialise();
            if (g_isValidAt != 1 || g_uninitialiseAt != 2) return 1;
            if (g_isValidReceiver != &storage.raw || g_uninitialiseReceiver != &storage.raw)
                return 2;
            float* expectedFinish = valid ? samples : samples + filled[i];
            if (storage.raw.finish != expectedFinish) return 3;
            // clear() keeps the allocation and the samples themselves.
            if (storage.raw.start != samples || storage.raw.endOfStorage != samples + 4) return 4;
            if (samples[0] != 1.0f || samples[3] != 4.0f) return 5;
            if (storage.raw.rest[0] != 0x77 || storage.raw.base[0] != 0x77) return 6;
        }
    }
    puts("FABLETLC_FRAME_SMOOTHER_UNINITIALISE PASS");
    return 0;
}