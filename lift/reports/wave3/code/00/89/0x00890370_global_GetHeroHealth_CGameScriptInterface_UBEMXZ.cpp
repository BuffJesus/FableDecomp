#include <cstddef>
#include <cstdint>

class CGameScriptInterface;

class CGameScriptInterface
{
public:
    float __thiscall GetHeroHealth() const;
};

namespace
{
struct CGameScriptInterface_GetHeroHealth_SelfOverlay
{
    std::byte m_Pad00[0x14];
    void* m_Field14; // 0x14
};
static_assert(
    offsetof(CGameScriptInterface_GetHeroHealth_SelfOverlay, m_Field14) == 0x14);

struct CGameScriptInterface_GetHeroHealth_TargetOverlay
{
    std::byte m_Pad00[0x91];
    std::uint8_t m_Flags91; // 0x91
    std::byte m_Pad92[0x22];
    float m_HealthB4; // 0xB4
};
static_assert(
    offsetof(CGameScriptInterface_GetHeroHealth_TargetOverlay, m_Flags91) == 0x91);
static_assert(
    offsetof(CGameScriptInterface_GetHeroHealth_TargetOverlay, m_HealthB4) == 0xB4);

using CGameScriptInterface_GetHeroHealth_Helper00449970Fn =
    void* (__thiscall*)(void*);
using CGameScriptInterface_GetHeroHealth_Helper00487DB0Fn =
    CGameScriptInterface_GetHeroHealth_TargetOverlay* (__thiscall*)(void*);

auto* const CGameScriptInterface_GetHeroHealth_Helper00449970 =
    reinterpret_cast<CGameScriptInterface_GetHeroHealth_Helper00449970Fn>(0x00449970);
auto* const CGameScriptInterface_GetHeroHealth_Helper00487DB0 =
    reinterpret_cast<CGameScriptInterface_GetHeroHealth_Helper00487DB0Fn>(0x00487DB0);

inline constexpr float kDefaultHeroHealth_0122DEDC =
    *reinterpret_cast<const float*>(0x0122DEDC);
}

float __thiscall CGameScriptInterface::GetHeroHealth() const
{
    const auto* const self =
        reinterpret_cast<const CGameScriptInterface_GetHeroHealth_SelfOverlay*>(this);

    auto* const target =
        CGameScriptInterface_GetHeroHealth_Helper00487DB0(
            CGameScriptInterface_GetHeroHealth_Helper00449970(self->m_Field14));

    if ((target != nullptr) && ((target->m_Flags91 & 0x01) == 0))
    {
        return target->m_HealthB4;
    }

    return kDefaultHeroHealth_0122DEDC;
}