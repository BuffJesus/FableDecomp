#include <cstddef>

class CGameScriptInterface
{
public:
    void __thiscall SetAllowScreenFadingIfAlreadyFaded(
        bool allowScreenFadingIfAlreadyFaded);
};

namespace
{
struct CGameScriptInterface_SetAllowScreenFadingIfAlreadyFaded_SelfOverlay
{
    std::byte m_Pad00[0x2D];
    bool m_AllowScreenFadingIfAlreadyFaded; // 0x2D
};
static_assert(
    offsetof(
        CGameScriptInterface_SetAllowScreenFadingIfAlreadyFaded_SelfOverlay,
        m_AllowScreenFadingIfAlreadyFaded) == 0x2D);
}

void __thiscall CGameScriptInterface::SetAllowScreenFadingIfAlreadyFaded(
    bool allowScreenFadingIfAlreadyFaded)
{
    auto* const pSelf =
        reinterpret_cast<CGameScriptInterface_SetAllowScreenFadingIfAlreadyFaded_SelfOverlay*>(this);
    pSelf->m_AllowScreenFadingIfAlreadyFaded = allowScreenFadingIfAlreadyFaded;
}