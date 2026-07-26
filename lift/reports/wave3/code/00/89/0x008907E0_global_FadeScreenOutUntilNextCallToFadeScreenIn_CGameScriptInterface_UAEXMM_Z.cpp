#include <cstddef>
#include <cstdint>

class CTCCreditsUI;
class CGameScriptInterface
{
public:
    void __thiscall FadeScreenOutUntilNextCallToFadeScreenIn(float param_1, float param_2);
};

namespace
{
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

using FadeScreenOutRawThunk = char(__thiscall*)(CGameScriptInterface*, float, float, std::uint32_t);
}

void __thiscall CGameScriptInterface::FadeScreenOutUntilNextCallToFadeScreenIn(float param_1, float param_2)
{
    auto* const vtable = *reinterpret_cast<void***>(this);
    auto* const overlay = reinterpret_cast<CGameScriptInterfaceOverlay*>(this);

    if (reinterpret_cast<FadeScreenOutRawThunk>(vtable[0x5D4 / sizeof(void*)])(this, param_1, param_2, 0xFF000000u) != 0) {
        auto* const creditsUI = reinterpret_cast<CTCCreditsUIOverlay*>(overlay->creditsUI_0c);
        creditsUI->fadeScreenOutFlag_d8 = 1;
    }
}