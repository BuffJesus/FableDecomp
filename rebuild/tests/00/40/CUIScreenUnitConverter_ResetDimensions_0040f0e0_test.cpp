#include <stdio.h>
#include <string.h>

#include "fable_nui_manager.h"
#include "fable_gfuninitialise.h"
#include "fable_surface.h"

// Behaviour fixture for NUISystem::CUIScreenUnitConverter::ResetDimensions
// @ 0x0040F0E0: reads the display manager's render-target size through
// GFGetSystemManager()->GetDisplayManager() (+0x60) and caches it as floats.

float NUISystem::CUIScreenUnitConverter::BackBufferWidth = -1.0f;
float NUISystem::CUIScreenUnitConverter::BackBufferHeight = -1.0f;

// The cached dimensions are private statics; an explicit instantiation (which
// is exempt from access checks) hands the fixture their addresses.
template <typename Tag, float* Member>
struct FixtureRob
{
    friend float* FixturePeek(Tag) { return Member; }
};
struct WidthTag { friend float* FixturePeek(WidthTag); };
struct HeightTag { friend float* FixturePeek(HeightTag); };
template struct FixtureRob<WidthTag, &NUISystem::CUIScreenUnitConverter::BackBufferWidth>;
template struct FixtureRob<HeightTag, &NUISystem::CUIScreenUnitConverter::BackBufferHeight>;

static unsigned char g_system[0x200];
static unsigned char g_display[0x2C8];
static int g_seq, g_sysAt, g_dimsAt;
static const void* g_dimsThis;
static long g_w, g_h;

CSystemManager* FABLE_FASTCALL GFGetSystemManager()
{
    g_sysAt = ++g_seq;
    return reinterpret_cast<CSystemManager*>(g_system);
}

const C2DExtentsI CDisplayManager::GetRenderTargetDimensions()
{
    g_dimsAt = ++g_seq; g_dimsThis = this;
    return C2DExtentsI(g_w, g_h);
}

static bool RunCase(long w, long h)
{
    g_seq = g_sysAt = g_dimsAt = 0; g_dimsThis = 0;
    g_w = w; g_h = h;
    memset(g_system, 0, sizeof(g_system));
    memset(g_display, 0x62, sizeof(g_display));
    void* display = g_display;
    memcpy(g_system + 0x60, &display, 4);
    float* width = FixturePeek(WidthTag());
    float* height = FixturePeek(HeightTag());
    *width = -1.0f;
    *height = -1.0f;

    NUISystem::CUIScreenUnitConverter::ResetDimensions();

    bool ok = g_sysAt == 1 && g_dimsAt == 2 && g_dimsThis == g_display;
    ok = ok && *width == (float)w;
    ok = ok && *height == (float)h;
    for (int i = 0; i < 0x2C8; ++i)
        if (g_display[i] != 0x62) { ok = false; break; }
    if (!ok) printf("FAIL %ld x %ld\n", w, h);
    return ok;
}

int main()
{
    bool ok = RunCase(1024, 768);
    ok &= RunCase(1920, 1200);
    ok &= RunCase(640, 480);
    if (ok) printf("_global_0040f0e0_TEST PASS\n");
    return ok ? 0 : 1;
}
