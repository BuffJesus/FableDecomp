#include <cstddef>
#include <cstdint>

class CGameScriptInterface;

class CEnvironment
{
public:
    void __thiscall TransitionToDefaultThemeExternals(
        std::int32_t unknown0,
        std::int32_t unknown1,
        float blendTime);
};

class CGameScriptInterface
{
public:
    void __thiscall ResetToDefaultThemeExternals(float blendTime) const;
};

namespace
{
struct CGameScriptInterface_ResetToDefaultThemeExternals_SelfOverlay
{
    std::byte m_Pad00[0x04];
    void* m_Field04; // 0x04
};
static_assert(
    offsetof(CGameScriptInterface_ResetToDefaultThemeExternals_SelfOverlay, m_Field04) == 0x04);

struct CGameScriptInterface_ResetToDefaultThemeExternals_Field04Overlay
{
    std::byte m_Pad00[0x18];
    CEnvironment* m_pEnvironment; // 0x18
};
static_assert(
    offsetof(CGameScriptInterface_ResetToDefaultThemeExternals_Field04Overlay, m_pEnvironment) == 0x18);
}

void __thiscall CGameScriptInterface::ResetToDefaultThemeExternals(float blendTime) const
{
    const auto* const pSelf =
        reinterpret_cast<const CGameScriptInterface_ResetToDefaultThemeExternals_SelfOverlay*>(this);
    const auto* const pField04 =
        reinterpret_cast<const CGameScriptInterface_ResetToDefaultThemeExternals_Field04Overlay*>(
            pSelf->m_Field04);

    pField04->m_pEnvironment->TransitionToDefaultThemeExternals(0, 1, blendTime);
}