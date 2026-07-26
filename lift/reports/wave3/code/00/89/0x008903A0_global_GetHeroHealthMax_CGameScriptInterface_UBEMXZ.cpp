#include <cstddef>
#include <cstdint>

class CGameScriptInterface;

class CGameScriptInterface
{
public:
    float __thiscall GetHeroHealthMax() const;
};

namespace
{
struct CGameScriptInterface_GetHeroHealthMax_SelfOverlay
{
    std::byte m_Pad00[0x14];
    void* m_Field14; // 0x14
};
static_assert(
    offsetof(CGameScriptInterface_GetHeroHealthMax_SelfOverlay, m_Field14) == 0x14);

struct CGameScriptInterface_GetHeroHealthMax_TargetOverlay
{
    std::byte m_Pad00[0x91];
    std::uint8_t m_Flags91; // 0x91
    std::byte m_Pad92[0x1E];
    float m_HealthMaxB0; // 0xB0
};
static_assert(
    offsetof(CGameScriptInterface_GetHeroHealthMax_TargetOverlay, m_Flags91) == 0x91);
static_assert(
    offsetof(CGameScriptInterface_GetHeroHealthMax_TargetOverlay, m_HealthMaxB0) == 0xB0);

using CGameScriptInterface_GetHeroHealthMax_Helper00449970Fn =
    void* (__thiscall*)(void*);
using CGameScriptInterface_GetHeroHealthMax_Helper00487DC0Fn =
    CGameScriptInterface_GetHeroHealthMax_TargetOverlay* (__thiscall*)(void*);

auto* const CGameScriptInterface_GetHeroHealthMax_Helper00449970 =
    reinterpret_cast<CGameScriptInterface_GetHeroHealthMax_Helper00449970Fn>(0x00449970);
auto* const CGameScriptInterface_GetHeroHealthMax_Helper00487DC0 =
    reinterpret_cast<CGameScriptInterface_GetHeroHealthMax_Helper00487DC0Fn>(0x00487DC0);

inline constexpr float kDefaultHeroHealthMax_0122DEDC =
    *reinterpret_cast<const float*>(0x0122DEDC);
}

float __thiscall CGameScriptInterface::GetHeroHealthMax() const
{
    const auto* const self =
        reinterpret_cast<const CGameScriptInterface_GetHeroHealthMax_SelfOverlay*>(this);

    auto* const target =
        CGameScriptInterface_GetHeroHealthMax_Helper00487DC0(
            CGameScriptInterface_GetHeroHealthMax_Helper00449970(self->m_Field14));

    if ((target != nullptr) && ((target->m_Flags91 & 0x01) == 0))
    {
        return target->m_HealthMaxB0;
    }

    return kDefaultHeroHealthMax_0122DEDC;
}