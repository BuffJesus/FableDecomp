#include <cstddef>
#include <cstdint>

class CGameScriptInterface
{
public:
    std::uint32_t __thiscall IsTeleportingActive() const;
};

namespace
{
struct TeleportingStateOverlay
{
    std::byte pad_00[0xD5];
    std::uint8_t field_D5;
};
static_assert(offsetof(TeleportingStateOverlay, field_D5) == 0xD5);

struct CGameScriptInterfaceTeleportingOverlay
{
    std::byte pad_00[0x4];
    TeleportingStateOverlay* field_4;
};
static_assert(offsetof(CGameScriptInterfaceTeleportingOverlay, field_4) == 0x4);
}

std::uint32_t __thiscall CGameScriptInterface::IsTeleportingActive() const
{
    const auto* const self =
        reinterpret_cast<const CGameScriptInterfaceTeleportingOverlay*>(this);
    const auto* const teleporting_state = self->field_4;

    return (static_cast<std::uint32_t>(
                reinterpret_cast<std::uintptr_t>(teleporting_state)) &
            0xFFFFFF00u) |
           static_cast<std::uint32_t>(teleporting_state->field_D5);
}