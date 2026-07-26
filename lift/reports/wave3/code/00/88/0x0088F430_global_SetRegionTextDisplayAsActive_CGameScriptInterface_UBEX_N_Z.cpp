#include <cstddef>

class CGameScriptInterface
{
public:
    void __thiscall SetRegionTextDisplayAsActive(bool active);
};

namespace
{
struct CGameScriptInterface_SetRegionTextDisplayAsActive_SelfOverlay
{
    std::byte m_Pad00[0x04];
    void* m_Field04; // 0x04
};
static_assert(
    offsetof(
        CGameScriptInterface_SetRegionTextDisplayAsActive_SelfOverlay,
        m_Field04) == 0x04);

struct CGameScriptInterface_SetRegionTextDisplayAsActive_Field04Overlay
{
    std::byte m_Pad00[0xDF];
    bool m_RegionTextDisplayAsActive; // 0xDF
};
static_assert(
    offsetof(
        CGameScriptInterface_SetRegionTextDisplayAsActive_Field04Overlay,
        m_RegionTextDisplayAsActive) == 0xDF);
}

void __thiscall CGameScriptInterface::SetRegionTextDisplayAsActive(bool active)
{
    auto* const pSelf =
        reinterpret_cast<CGameScriptInterface_SetRegionTextDisplayAsActive_SelfOverlay*>(this);
    auto* const pField04 =
        reinterpret_cast<CGameScriptInterface_SetRegionTextDisplayAsActive_Field04Overlay*>(
            pSelf->m_Field04);
    pField04->m_RegionTextDisplayAsActive = active;
}