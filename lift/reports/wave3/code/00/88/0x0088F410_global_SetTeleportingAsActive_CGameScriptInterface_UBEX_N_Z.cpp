#include <cstddef>

class CGameScriptInterface
{
public:
    void __thiscall SetTeleportingAsActive(bool active) const;
};

namespace
{
struct CGameScriptInterface_SetTeleportingAsActive_SelfOverlay
{
    std::byte m_Pad00[0x04];
    void* m_Field04; // 0x04
};
static_assert(
    offsetof(
        CGameScriptInterface_SetTeleportingAsActive_SelfOverlay,
        m_Field04) == 0x04);

struct CGameScriptInterface_SetTeleportingAsActive_Field04Overlay
{
    std::byte m_Pad00[0xD5];
    bool m_FieldD5; // 0xD5
};
static_assert(
    offsetof(
        CGameScriptInterface_SetTeleportingAsActive_Field04Overlay,
        m_FieldD5) == 0xD5);
}

void __thiscall CGameScriptInterface::SetTeleportingAsActive(bool active) const
{
    static_cast<CGameScriptInterface_SetTeleportingAsActive_Field04Overlay*>(
        static_cast<CGameScriptInterface_SetTeleportingAsActive_SelfOverlay*>(
            const_cast<CGameScriptInterface*>(this))
            ->m_Field04)
        ->m_FieldD5 = active;
}