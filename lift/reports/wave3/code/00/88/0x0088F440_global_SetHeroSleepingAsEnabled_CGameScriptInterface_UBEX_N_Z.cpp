#include <cstddef>
#include <cstdint>

class CGameScriptInterface
{
public:
    void __thiscall SetHeroSleepingAsEnabled(std::uint8_t isEnabledRaw);

private:
    std::uint8_t m_Pad0[0x4];
    void* m_InterfaceContext;
};

struct HeroSleepingTargetOverlay
{
    std::uint8_t pad_0[0xD9];
    std::uint8_t sleepingEnabled;
};

static_assert(offsetof(CGameScriptInterface, m_InterfaceContext) == 0x4);
static_assert(offsetof(HeroSleepingTargetOverlay, sleepingEnabled) == 0xD9);

void __thiscall CGameScriptInterface::SetHeroSleepingAsEnabled(std::uint8_t isEnabledRaw)
{
    auto* const target =
        static_cast<HeroSleepingTargetOverlay*>(m_InterfaceContext);
    target->sleepingEnabled = isEnabledRaw;
}