#include <cstddef>
#include <cstdint>

class CEnvironment
{
public:
    void __thiscall TransitionToDefaultThemeAllChannels(std::int32_t unknown, float blendTime);
};

class CGameScriptInterface
{
public:
    void __thiscall ResetToDefaultTheme(float blendTime) const;
};

namespace
{
struct CGameScriptInterface_ResetToDefaultTheme_SelfOverlay
{
    std::byte m_Pad00[0x04];
    void* m_Field04; // 0x04
};
static_assert(
    offsetof(CGameScriptInterface_ResetToDefaultTheme_SelfOverlay, m_Field04) == 0x04);

struct CGameScriptInterface_ResetToDefaultTheme_Field04Overlay
{
    std::byte m_Pad00[0x18];
    CEnvironment* m_pEnvironment; // 0x18
};
static_assert(
    offsetof(CGameScriptInterface_ResetToDefaultTheme_Field04Overlay, m_pEnvironment) == 0x18);
}

void __thiscall CGameScriptInterface::ResetToDefaultTheme(float blendTime) const
{
    const auto* const self =
        reinterpret_cast<const CGameScriptInterface_ResetToDefaultTheme_SelfOverlay*>(this);
    const auto* const field04 =
        reinterpret_cast<const CGameScriptInterface_ResetToDefaultTheme_Field04Overlay*>(
            self->m_Field04);

    field04->m_pEnvironment->TransitionToDefaultThemeAllChannels(1, blendTime);
}