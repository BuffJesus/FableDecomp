#include <cstddef>
#include <cstdint>

namespace
{
struct CGameScriptInterface_FadeScreenOut_TargetOverlay
{
    std::byte pad_00[0xBC];
    std::uint8_t field_0BC;
};
static_assert(offsetof(CGameScriptInterface_FadeScreenOut_TargetOverlay, field_0BC) == 0xBC);

struct CGameScriptInterface_FadeScreenOut_Overlay
{
    std::byte pad_00[0x0C];
    CGameScriptInterface_FadeScreenOut_TargetOverlay* field_00C;
    std::byte pad_10[0x1D];
    std::uint8_t field_02D;
};
static_assert(offsetof(CGameScriptInterface_FadeScreenOut_Overlay, field_00C) == 0x0C);
static_assert(offsetof(CGameScriptInterface_FadeScreenOut_Overlay, field_02D) == 0x2D);

static_assert(sizeof(CRGBColour) == 4);
}

bool CGameScriptInterface::FadeScreenOut(float param_1, float param_2, CRGBColour colour)
{
    const auto* const overlay =
        reinterpret_cast<const CGameScriptInterface_FadeScreenOut_Overlay*>(this);

    if ((overlay->field_00C->field_0BC != 0) && (overlay->field_02D == 0)) {
        return false;
    }

    CRGBColour localColour;
    const auto* const sourceBytes = reinterpret_cast<const std::uint8_t*>(&colour);
    auto* const localBytes = reinterpret_cast<std::uint8_t*>(&localColour);

    localBytes[3] = sourceBytes[3];
    localBytes[2] = sourceBytes[2];
    localBytes[0] = sourceBytes[0];
    localBytes[1] = sourceBytes[1];

    NGameText::ConstructString(this, param_1, param_2, localColour);
    return true;
}