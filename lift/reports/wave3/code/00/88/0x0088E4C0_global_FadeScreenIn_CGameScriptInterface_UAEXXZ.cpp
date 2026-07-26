#include <cstddef>
#include <cstdint>

class CTCCreditsUI;
class CGameScriptInterface
{
public:
    void __thiscall FadeScreenIn();
};

namespace
{
using FadeScreenInDonorFn = void(__thiscall*)(CGameScriptInterface*);

struct CTCCreditsUIOverlay
{
    std::byte pad00[0xD8];
    std::uint8_t fadeScreenOutFlag_d8; // 0xD8
};

static_assert(offsetof(CTCCreditsUIOverlay, fadeScreenOutFlag_d8) == 0xD8);

struct CGameScriptInterfaceOverlay
{
    std::byte pad00[0x0C];
    CTCCreditsUI* creditsUI_0c; // 0x0C
};

static_assert(offsetof(CGameScriptInterfaceOverlay, creditsUI_0c) == 0x0C);
}

void __thiscall CGameScriptInterface::FadeScreenIn()
{
    auto* const overlay = reinterpret_cast<CGameScriptInterfaceOverlay*>(this);
    auto* const creditsUI = reinterpret_cast<CTCCreditsUIOverlay*>(overlay->creditsUI_0c);

    creditsUI->fadeScreenOutFlag_d8 = 0;
    reinterpret_cast<FadeScreenInDonorFn>(0x006E7270)(this);
}