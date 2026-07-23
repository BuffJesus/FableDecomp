#include <cstddef>

class CTCCreditsUI
{
public:
    static void __thiscall Deactivate(CTCCreditsUI* creditsUI);
};

class CGameScriptInterface
{
public:
    void __thiscall EndLetterBox();
};

namespace
{
struct CGameScriptInterfaceOverlay
{
    std::byte pad00[0x0C];
    CTCCreditsUI* creditsUI_0c; // 0x0C
};

static_assert(offsetof(CGameScriptInterfaceOverlay, creditsUI_0c) == 0x0C);
}

void __thiscall CGameScriptInterface::EndLetterBox()
{
    auto* const overlay = reinterpret_cast<CGameScriptInterfaceOverlay*>(this);
    CTCCreditsUI::Deactivate(overlay->creditsUI_0c);
}