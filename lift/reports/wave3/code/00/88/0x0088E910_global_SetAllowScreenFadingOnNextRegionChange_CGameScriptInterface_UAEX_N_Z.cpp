#include <cstddef>

class CGameScriptInterface
{
public:
    void __thiscall SetAllowScreenFadingOnNextRegionChange(
        bool allowScreenFadingOnNextRegionChange);
};

namespace
{
struct CGameScriptInterface_SetAllowScreenFadingOnNextRegionChange_SelfOverlay
{
    std::byte m_Pad00[0x04];
    void* m_Field04; // 0x04
};
static_assert(
    offsetof(
        CGameScriptInterface_SetAllowScreenFadingOnNextRegionChange_SelfOverlay,
        m_Field04) == 0x04);

struct CGameScriptInterface_SetAllowScreenFadingOnNextRegionChange_Field04Overlay
{
    std::byte m_Pad00[0xDB];
    bool m_AllowScreenFadingOnNextRegionChange; // 0xDB
};
static_assert(
    offsetof(
        CGameScriptInterface_SetAllowScreenFadingOnNextRegionChange_Field04Overlay,
        m_AllowScreenFadingOnNextRegionChange) == 0xDB);
}

void __thiscall CGameScriptInterface::SetAllowScreenFadingOnNextRegionChange(
    bool allowScreenFadingOnNextRegionChange)
{
    auto* const pSelf =
        reinterpret_cast<CGameScriptInterface_SetAllowScreenFadingOnNextRegionChange_SelfOverlay*>(this);
    auto* const pField04 =
        reinterpret_cast<CGameScriptInterface_SetAllowScreenFadingOnNextRegionChange_Field04Overlay*>(
            pSelf->m_Field04);
    pField04->m_AllowScreenFadingOnNextRegionChange = allowScreenFadingOnNextRegionChange;
}