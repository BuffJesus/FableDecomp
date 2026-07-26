#include <cstddef>
#include <cstdint>

class CGameScriptInterface
{
public:
    std::uint32_t __thiscall IsSavingEnabled() const;
};

namespace
{
struct SavingEnabledStateOverlay
{
    std::byte pad_00[0xD7];
    std::uint8_t field_D7;
};
static_assert(offsetof(SavingEnabledStateOverlay, field_D7) == 0xD7);

struct CGameScriptInterfaceSavingOverlay
{
    std::byte pad_00[0x4];
    std::uint32_t field_4;
};
static_assert(offsetof(CGameScriptInterfaceSavingOverlay, field_4) == 0x4);
}

std::uint32_t __thiscall CGameScriptInterface::IsSavingEnabled() const
{
    const auto* const self =
        reinterpret_cast<const CGameScriptInterfaceSavingOverlay*>(this);
    const std::uint32_t state_address = self->field_4;
    const auto* const state =
        reinterpret_cast<const SavingEnabledStateOverlay*>(state_address);

    return (state_address & 0xFFFFFF00u) | state->field_D7;
}