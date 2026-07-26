#include <cstddef>

class CGameScriptInterface
{
public:
    void __thiscall RemoveConversation(long conversationId, bool doCleanup) const;
};

namespace
{
struct CGameScriptInterface_RemoveConversation_WorldOverlay
{
    std::byte m_Pad00[0x7C];
    void* m_Field7C; // 0x7C
};

static_assert(
    offsetof(CGameScriptInterface_RemoveConversation_WorldOverlay, m_Field7C) == 0x7C);

struct CGameScriptInterface_RemoveConversation_SelfOverlay
{
    std::byte m_Pad00[0x04];
    CGameScriptInterface_RemoveConversation_WorldOverlay* m_pWorld; // 0x04
};

static_assert(
    offsetof(CGameScriptInterface_RemoveConversation_SelfOverlay, m_pWorld) == 0x04);

using CScriptConversation_RemoveAndCleanupFn =
    void (__thiscall*)(void* pThis, long conversationId, bool doCleanup);

inline constexpr auto CScriptConversation_RemoveAndCleanup =
    reinterpret_cast<CScriptConversation_RemoveAndCleanupFn>(0x006E598A);
}

void __thiscall CGameScriptInterface::RemoveConversation(
    long conversationId,
    bool doCleanup) const
{
    const auto* const self =
        reinterpret_cast<const CGameScriptInterface_RemoveConversation_SelfOverlay*>(this);
    CScriptConversation_RemoveAndCleanup(
        self->m_pWorld->m_Field7C,
        conversationId,
        doCleanup);
}