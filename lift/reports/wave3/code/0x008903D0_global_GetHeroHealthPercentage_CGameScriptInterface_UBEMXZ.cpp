#include <cstddef>
#include <cstdint>

class CGameScriptInterface;

class CGameScriptInterface
{
public:
    float __thiscall GetHeroHealthPercentage() const;
};

namespace
{
struct CGameScriptInterface_GetHeroHealthPercentage_SelfOverlay
{
    std::byte m_Pad00[0x14];
    void* m_Field14; // 0x14
};
static_assert(
    offsetof(CGameScriptInterface_GetHeroHealthPercentage_SelfOverlay, m_Field14) == 0x14);

struct CGameScriptInterface_GetHeroHealthPercentage_TargetOverlay
{
    std::byte m_Pad00[0x91];
    std::uint8_t m_Flags91; // 0x91
};
static_assert(
    offsetof(CGameScriptInterface_GetHeroHealthPercentage_TargetOverlay, m_Flags91) == 0x91);

using CGameScriptInterface_GetHeroHealthPercentage_Helper00449970Fn =
    void* (__thiscall*)(void*);
using CGameScriptInterface_GetHeroHealthPercentage_Helper00487DC0Fn =
    CGameScriptInterface_GetHeroHealthPercentage_TargetOverlay* (__thiscall*)(void*);
using CGameScriptInterface_GetHeroHealthPercentage_GetNormalisedTimeFn =
    float (__thiscall*)(CGameScriptInterface_GetHeroHealthPercentage_TargetOverlay*);

auto* const CGameScriptInterface_GetHeroHealthPercentage_Helper00449970 =
    reinterpret_cast<CGameScriptInterface_GetHeroHealthPercentage_Helper00449970Fn>(0x00449970);
auto* const CGameScriptInterface_GetHeroHealthPercentage_Helper00487DC0 =
    reinterpret_cast<CGameScriptInterface_GetHeroHealthPercentage_Helper00487DC0Fn>(0x00487DC0);
auto* const CGameScriptInterface_GetHeroHealthPercentage_GetNormalisedTime =
    reinterpret_cast<CGameScriptInterface_GetHeroHealthPercentage_GetNormalisedTimeFn>(0x006A4E41);

inline constexpr float kDefaultHeroHealthPercentage_0122DEDC =
    *reinterpret_cast<const float*>(0x0122DEDC);
}

float __thiscall CGameScriptInterface::GetHeroHealthPercentage() const
{
    const auto* const self =
        reinterpret_cast<const CGameScriptInterface_GetHeroHealthPercentage_SelfOverlay*>(this);

    auto* const target =
        CGameScriptInterface_GetHeroHealthPercentage_Helper00487DC0(
            CGameScriptInterface_GetHeroHealthPercentage_Helper00449970(self->m_Field14));

    if ((target != nullptr) && ((target->m_Flags91 & 0x01) == 0))
    {
        return CGameScriptInterface_GetHeroHealthPercentage_GetNormalisedTime(target);
    }

    return kDefaultHeroHealthPercentage_0122DEDC;
}