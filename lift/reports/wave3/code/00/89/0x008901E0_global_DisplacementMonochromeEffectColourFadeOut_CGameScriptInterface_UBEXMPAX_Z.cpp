#include <cstddef>

class CGameScriptInterface;
class CXMVPlaybackInfo;

class CGameScriptInterface
{
public:
    void __thiscall DisplacementMonochromeEffectColourFadeOut(
        float fadeOutTime,
        void* pFadeOutContext) const;
};

namespace
{
struct CGameScriptInterface_DisplacementMonochromeEffectColourFadeOut_SelfOverlay
{
    std::byte m_Pad00[0x04];
    void* m_Field04; // 0x04
};
static_assert(
    offsetof(
        CGameScriptInterface_DisplacementMonochromeEffectColourFadeOut_SelfOverlay,
        m_Field04) == 0x04);

struct CGameScriptInterface_DisplacementMonochromeEffectColourFadeOut_Field04Overlay
{
    std::byte m_Pad00[0x18];
    void* m_Field18; // 0x18
};
static_assert(
    offsetof(
        CGameScriptInterface_DisplacementMonochromeEffectColourFadeOut_Field04Overlay,
        m_Field18) == 0x18);

struct CGameScriptInterface_DisplacementMonochromeEffectColourFadeOut_Field18Overlay
{
    std::byte m_Pad00[0x1964];
    CXMVPlaybackInfo* m_pPlaybackInfo; // 0x1964
};
static_assert(
    offsetof(
        CGameScriptInterface_DisplacementMonochromeEffectColourFadeOut_Field18Overlay,
        m_pPlaybackInfo) == 0x1964);

// Ghidra identified 0x006B8390 as std::_Cons_val<...>, but this wrapper is a pure tail jump.
// Keep the callee ABI at the raw observed shape instead of inferring a stronger source-level prototype.
using CGameScriptInterface_DisplacementMonochromeEffectColourFadeOut_006B8390Fn =
    void(__thiscall*)(CXMVPlaybackInfo* pThis, float fadeOutTime, void* pFadeOutContext);

auto* const CGameScriptInterface_DisplacementMonochromeEffectColourFadeOut_006B8390 =
    reinterpret_cast<CGameScriptInterface_DisplacementMonochromeEffectColourFadeOut_006B8390Fn>(
        0x006B8390);
}

void __thiscall CGameScriptInterface::DisplacementMonochromeEffectColourFadeOut(
    float fadeOutTime,
    void* pFadeOutContext) const
{
    const auto* const self =
        reinterpret_cast<
            const CGameScriptInterface_DisplacementMonochromeEffectColourFadeOut_SelfOverlay*>(
            this);
    auto* const field04 =
        reinterpret_cast<
            CGameScriptInterface_DisplacementMonochromeEffectColourFadeOut_Field04Overlay*>(
            self->m_Field04);
    auto* const field18 =
        reinterpret_cast<
            CGameScriptInterface_DisplacementMonochromeEffectColourFadeOut_Field18Overlay*>(
            field04->m_Field18);

    CGameScriptInterface_DisplacementMonochromeEffectColourFadeOut_006B8390(
        field18->m_pPlaybackInfo,
        fadeOutTime,
        pFadeOutContext);
}