#include <cstddef>

class CGameScriptInterface;

namespace
{
struct CGameScriptInterface_IsTimeOfDayBetween_Overlay
{
    std::byte m_Pad00[0x04];
    const void* m_Unknown04; // 0x04
};

static_assert(offsetof(CGameScriptInterface_IsTimeOfDayBetween_Overlay, m_Unknown04) == 0x04);

struct CGameScriptInterface_IsTimeOfDayBetween_ContextOverlay
{
    std::byte m_Pad00[0x1C];
    const void* m_Unknown1C; // 0x1C
};

static_assert(offsetof(CGameScriptInterface_IsTimeOfDayBetween_ContextOverlay, m_Unknown1C) == 0x1C);

extern bool __thiscall Game_IsTimeOfDayInRange(const void* pTimeOfDayContext, long startTime, long endTime);
}

bool __thiscall CGameScriptInterface::IsTimeOfDayBetween(long startTime, long endTime) const
{
    const auto* const pInterface =
        reinterpret_cast<const CGameScriptInterface_IsTimeOfDayBetween_Overlay*>(this);
    const auto* const pContext =
        reinterpret_cast<const CGameScriptInterface_IsTimeOfDayBetween_ContextOverlay*>(pInterface->m_Unknown04);

    return Game_IsTimeOfDayInRange(pContext->m_Unknown1C, startTime, endTime);
}