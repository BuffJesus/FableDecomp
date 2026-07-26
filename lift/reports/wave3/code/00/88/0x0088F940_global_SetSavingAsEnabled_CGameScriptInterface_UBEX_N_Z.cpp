#include <cstddef>

class CGameScriptInterface
{
public:
    void __thiscall SetSavingAsEnabled(bool enabled) const;
};

namespace
{
struct CGameScriptInterface_SetSavingAsEnabled_SelfOverlay
{
    std::byte m_Pad00[0x04];
    void* m_Field04; // 0x04
};
static_assert(
    offsetof(
        CGameScriptInterface_SetSavingAsEnabled_SelfOverlay,
        m_Field04) == 0x04);

struct CGameScriptInterface_SetSavingAsEnabled_Field04Overlay
{
    std::byte m_Pad00[0xD7];
    bool m_FieldD7; // 0xD7
};
static_assert(
    offsetof(
        CGameScriptInterface_SetSavingAsEnabled_Field04Overlay,
        m_FieldD7) == 0xD7);
}

void __thiscall CGameScriptInterface::SetSavingAsEnabled(bool enabled) const
{
    auto* const pSelf =
        reinterpret_cast<const CGameScriptInterface_SetSavingAsEnabled_SelfOverlay*>(this);
    auto* const pField04 =
        reinterpret_cast<CGameScriptInterface_SetSavingAsEnabled_Field04Overlay*>(
            pSelf->m_Field04);
    pField04->m_FieldD7 = enabled;
}