#include <cstddef>
#include <cstdint>

class CGameScriptInterface
{
public:
    void __thiscall SetExperienceSpendingAsEnabled(bool enabled);
};

namespace
{
struct CGameScriptInterfaceOverlay
{
    std::byte pad_0[0x4];
    void* field_4;
};
static_assert(offsetof(CGameScriptInterfaceOverlay, field_4) == 0x4);

struct ExperienceSpendingOverlay
{
    std::byte pad_0[0xD6];
    std::uint8_t field_D6;
};
static_assert(offsetof(ExperienceSpendingOverlay, field_D6) == 0xD6);
}

void __thiscall CGameScriptInterface::SetExperienceSpendingAsEnabled(bool enabled)
{
    auto& self = *reinterpret_cast<CGameScriptInterfaceOverlay*>(this);
    auto& target = *reinterpret_cast<ExperienceSpendingOverlay*>(self.field_4);
    target.field_D6 = enabled;
}