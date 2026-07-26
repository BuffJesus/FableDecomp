#include <cstddef>
#include <cstdint>

class CGameScriptInterface
{
public:
    std::uint32_t __thiscall IsRadialBlurFadeActive() const;
};

struct RadialBlurFadeFlagOverlay
{
    std::byte pad_00[0x78];
    std::uint8_t field_78;
};
static_assert(offsetof(RadialBlurFadeFlagOverlay, field_78) == 0x78);

struct RadialBlurFadeStateOverlay
{
    std::byte pad_00[0x1964];
    RadialBlurFadeFlagOverlay* field_1964;
};
static_assert(offsetof(RadialBlurFadeStateOverlay, field_1964) == 0x1964);

struct RadialBlurRootOverlay
{
    std::byte pad_00[0x18];
    RadialBlurFadeStateOverlay* field_18;
};
static_assert(offsetof(RadialBlurRootOverlay, field_18) == 0x18);

struct CGameScriptInterfaceOverlay
{
    std::byte pad_00[0x4];
    std::uint32_t field_4_raw;
};
static_assert(offsetof(CGameScriptInterfaceOverlay, field_4_raw) == 0x4);

std::uint32_t CGameScriptInterface::IsRadialBlurFadeActive() const
{
    const auto* const self = reinterpret_cast<const CGameScriptInterfaceOverlay*>(this);
    const auto field_4 = self->field_4_raw;
    const auto* const root = reinterpret_cast<const RadialBlurRootOverlay*>(field_4);
    const auto* const state = root->field_18;
    const auto* const fade_flag = state->field_1964;

    return (field_4 & 0xFFFFFF00u) | fade_flag->field_78;
}