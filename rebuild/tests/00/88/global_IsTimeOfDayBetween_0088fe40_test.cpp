#include <cstdio>

namespace
{
struct CGameScriptInterface_IsTimeOfDayBetween_Overlay
{
    char m_Pad00[0x04];
    void* m_Unknown04;
};

struct CGameScriptInterface_IsTimeOfDayBetween_ContextOverlay
{
    char m_Pad00[0x1C];
    void* m_Unknown1C;
};
}

static long g_lastStart = 0;
static long g_lastEnd = 0;
static void* g_lastCtx = 0;
static bool g_stub_result = false;

extern "C" bool __fastcall Game_IsTimeOfDayInRange(void* pTimeOfDayContext, long startTime, long endTime)
{
    g_lastCtx = pTimeOfDayContext;
    g_lastStart = startTime;
    g_lastEnd = endTime;
    return g_stub_result;
}

static bool __fastcall CGameScriptInterface_IsTimeOfDayBetween(void* self, long startTime, long endTime)
{
    CGameScriptInterface_IsTimeOfDayBetween_Overlay* pInterface =
        reinterpret_cast<CGameScriptInterface_IsTimeOfDayBetween_Overlay*>(self);
    CGameScriptInterface_IsTimeOfDayBetween_ContextOverlay* pContext =
        reinterpret_cast<CGameScriptInterface_IsTimeOfDayBetween_ContextOverlay*>(pInterface->m_Unknown04);

    return Game_IsTimeOfDayInRange(pContext->m_Unknown1C, startTime, endTime);
}

int main()
{
    CGameScriptInterface_IsTimeOfDayBetween_ContextOverlay ctx;
    ctx.m_Unknown1C = (void*)0x12345678;

    CGameScriptInterface_IsTimeOfDayBetween_Overlay obj;
    obj.m_Unknown04 = &ctx;

    g_stub_result = true;
    bool r = CGameScriptInterface_IsTimeOfDayBetween(&obj, 100, 200);

    if (r == true && g_lastCtx == ctx.m_Unknown1C && g_lastStart == 100 && g_lastEnd == 200)
    {
        printf("MARKER_OK_IsTimeOfDayBetween\n");
        return 0;
    }

    printf("MARKER_FAIL\n");
    return 1;
}