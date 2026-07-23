#include <cstddef>

class CGameScriptInterface;
class CXMVPlaybackInfo;

class CGameScriptInterface
{
public:
    void __thiscall CancelRadialBlurFade() const;
};

namespace
{
struct CGameScriptInterface_CancelRadialBlurFade_SelfOverlay
{
    std::byte m_Pad00[0x04];
    void* m_Field04; // 0x04
};
static_assert(offsetof(CGameScriptInterface_CancelRadialBlurFade_SelfOverlay, m_Field04) == 0x04);

struct CGameScriptInterface_CancelRadialBlurFade_Field04Overlay
{
    std::byte m_Pad00[0x18];
    void* m_Field18; // 0x18
};
static_assert(offsetof(CGameScriptInterface_CancelRadialBlurFade_Field04Overlay, m_Field18) == 0x18);

struct CGameScriptInterface_CancelRadialBlurFade_Field18Overlay
{
    std::byte m_Pad00[0x1964];
    CXMVPlaybackInfo* m_pPlaybackInfo; // 0x1964
};
static_assert(offsetof(CGameScriptInterface_CancelRadialBlurFade_Field18Overlay, m_pPlaybackInfo) == 0x1964);

using CGameScriptInterface_CancelRadialBlurFade_006B8300Fn =
    void(__thiscall*)(CXMVPlaybackInfo*);

auto* const CGameScriptInterface_CancelRadialBlurFade_006B8300 =
    reinterpret_cast<CGameScriptInterface_CancelRadialBlurFade_006B8300Fn>(0x006B8300);
}

void __thiscall CGameScriptInterface::CancelRadialBlurFade() const
{
    const auto* const self =
        reinterpret_cast<const CGameScriptInterface_CancelRadialBlurFade_SelfOverlay*>(this);
    auto* const field04 =
        reinterpret_cast<CGameScriptInterface_CancelRadialBlurFade_Field04Overlay*>(self->m_Field04);
    auto* const field18 =
        reinterpret_cast<CGameScriptInterface_CancelRadialBlurFade_Field18Overlay*>(field04->m_Field18);

    CGameScriptInterface_CancelRadialBlurFade_006B8300(field18->m_pPlaybackInfo);
}