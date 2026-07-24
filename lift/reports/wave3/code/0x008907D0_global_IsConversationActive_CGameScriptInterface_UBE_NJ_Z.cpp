#include <cstddef>
#include <cstdint>

class CDisplayManager;
class CGameScriptInterface;

class CDisplayManager
{
public:
    void __thiscall CopyBackBufferToTexture();
};

class CGameScriptInterface
{
public:
    void __thiscall IsConversationActive(std::int32_t) const;
};

namespace
{
struct CGameScriptInterface_IsConversationActive_SelfOverlay
{
    void* m_Vftable; // 0x00
    void* m_Field04; // 0x04
};
static_assert(
    offsetof(CGameScriptInterface_IsConversationActive_SelfOverlay, m_Field04) == 0x04);

struct CGameScriptInterface_IsConversationActive_Field04Overlay
{
    std::byte m_Pad00[0x7C];
    CDisplayManager* m_DisplayManager; // 0x7C
};
static_assert(
    offsetof(CGameScriptInterface_IsConversationActive_Field04Overlay, m_DisplayManager) == 0x7C);
}

void __thiscall CGameScriptInterface::IsConversationActive(std::int32_t) const
{
    const auto* const self =
        reinterpret_cast<const CGameScriptInterface_IsConversationActive_SelfOverlay*>(this);
    const auto* const field04 =
        reinterpret_cast<const CGameScriptInterface_IsConversationActive_Field04Overlay*>(
            self->m_Field04);

    field04->m_DisplayManager->CopyBackBufferToTexture();
}