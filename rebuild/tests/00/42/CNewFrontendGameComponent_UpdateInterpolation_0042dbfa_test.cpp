#include <stdio.h>
#include <string.h>

#include "fable_game.h"

// Behaviour fixture for CNewFrontendGameComponent::UpdateInterpolation @ 0x0042DBFA.
// Retail offsets: m_fLastUpdateTime +0xB8, m_LastInterp +0xCC (3 floats).
// info: Current +0x00, Last +0x0C, PausedCurrent +0x18, PausedLast +0x24.

static double g_time;
static int g_timeCalls;
double FABLE_FASTCALL GFGetTime()
{
    ++g_timeCalls;
    return g_time;
}

static unsigned char g_component[0x148];
static unsigned char g_info[0x70];

static bool RunCase(const char* name, double now, float last)
{
    g_timeCalls = 0;
    g_time = now;
    memset(g_component, 0x44, sizeof(g_component));
    memset(g_info, 0x55, sizeof(g_info));
    memcpy(g_component + 0xB8, &last, 4);
    float prev[3] = { 1.5f, -2.25f, 3.125f };
    memcpy(g_component + 0xCC, prev, 12);

    reinterpret_cast<CNewFrontendGameComponent*>(g_component)->UpdateInterpolation(
        *reinterpret_cast<CEngineInterpolationInfo*>(g_info));

    float expectZ = (float)((now - (double)last) * (double)14.999999046325684f);
    float cur[3] = { 0.0f, 0.0f, expectZ };
    bool ok = g_timeCalls == 1;
    ok = ok && memcmp(g_info + 0x00, cur, 12) == 0;
    ok = ok && memcmp(g_info + 0x0C, prev, 12) == 0;
    ok = ok && memcmp(g_info + 0x18, cur, 12) == 0;
    ok = ok && memcmp(g_info + 0x24, prev, 12) == 0;
    for (int i = 0x30; i < 0x70; ++i)
        if (g_info[i] != 0x55) { ok = false; break; }
    ok = ok && memcmp(g_component + 0xCC, cur, 12) == 0;
    for (int i = 0; i < 0x148; ++i)
    {
        if (i >= 0xB8 && i < 0xBC) continue;
        if (i >= 0xCC && i < 0xD8) continue;
        if (g_component[i] != 0x44) { ok = false; break; }
    }
    if (!ok)
    {
        float z; memcpy(&z, g_info + 8, 4);
        printf("FAIL %s z=%.9g expect=%.9g\n", name, z, expectZ);
    }
    return ok;
}

int main()
{
    bool ok = true;
    ok &= RunCase("half-frame", 10.0 + 1.0 / 30.0, 10.0f);
    ok &= RunCase("zero", 4.0, 4.0f);
    ok &= RunCase("two-frames", 7.25, 7.25f - 2.0f / 15.0f);
    if (ok) printf("OK_0x0042dbfa\n");
    return ok ? 0 : 1;
}
