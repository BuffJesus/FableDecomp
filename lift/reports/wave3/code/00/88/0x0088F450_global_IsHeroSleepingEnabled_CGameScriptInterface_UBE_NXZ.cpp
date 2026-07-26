#include <cstddef>
#include <cstdint>

class CGameScriptInterface
{
public:
    std::uint32_t __thiscall IsHeroSleepingEnabled() const;
};

namespace
{
struct CGameScriptInterface_IsHeroSleepingEnabled_SelfOverlay
{
    std::byte m_Pad00[0x04];
    std::uint32_t m_Field04; // 0x04
};
static_assert(
    offsetof(CGameScriptInterface_IsHeroSleepingEnabled_SelfOverlay, m_Field04) == 0x04);

struct CGameScriptInterface_IsHeroSleepingEnabled_TargetOverlay
{
    std::byte m_Pad00[0xD9];
    std::uint8_t m_FieldD9; // 0xD9
};
static_assert(
    offsetof(CGameScriptInterface_IsHeroSleepingEnabled_TargetOverlay, m_FieldD9) == 0xD9);
}

std::uint32_t __thiscall CGameScriptInterface::IsHeroSleepingEnabled() const
{
    const auto* const self =
        reinterpret_cast<const CGameScriptInterface_IsHeroSleepingEnabled_SelfOverlay*>(this);
    const std::uint32_t value = self->m_Field04;
    const auto* const target =
        reinterpret_cast<const CGameScriptInterface_IsHeroSleepingEnabled_TargetOverlay*>(value);

    return (value & 0xFFFFFF00u) | static_cast<std::uint32_t>(target->m_FieldD9);
}