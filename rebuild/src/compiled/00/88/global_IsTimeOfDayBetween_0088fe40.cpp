#pragma optimize("s",on)
namespace
{
struct CGameScriptInterface_IsTimeOfDayBetween_Overlay
{
    char m_Pad00[0x04];
    void* m_Unknown04; // 0x04
};

struct CGameScriptInterface_IsTimeOfDayBetween_ContextOverlay
{
    char m_Pad00[0x1C];
    void* m_Unknown1C; // 0x1C
};

extern bool __fastcall Game_IsTimeOfDayInRange(void* pTimeOfDayContext, long startTime, long endTime);
}

bool __fastcall CGameScriptInterface_IsTimeOfDayBetween(void* self, long startTime, long endTime)
{
    CGameScriptInterface_IsTimeOfDayBetween_Overlay* pInterface =
        reinterpret_cast<CGameScriptInterface_IsTimeOfDayBetween_Overlay*>(self);
    CGameScriptInterface_IsTimeOfDayBetween_ContextOverlay* pContext =
        reinterpret_cast<CGameScriptInterface_IsTimeOfDayBetween_ContextOverlay*>(pInterface->m_Unknown04);

    return Game_IsTimeOfDayInRange(pContext->m_Unknown1C, startTime, endTime);
}