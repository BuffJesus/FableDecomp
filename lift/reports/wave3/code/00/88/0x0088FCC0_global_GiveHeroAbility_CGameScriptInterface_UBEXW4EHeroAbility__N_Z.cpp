#include <cstddef>
#include <cstdint>

class CThingPlayerCreature;

enum EHeroAbility : std::int32_t;

class CGameScriptInterface
{
public:
    void __thiscall GiveHeroAbility(EHeroAbility ability, bool enabled) const;
};

class CThingPlayerCreature
{
public:
    void __thiscall LearnAbility(
        EHeroAbility ability,
        bool enabled,
        int slot,
        bool updateUI);
};

namespace
{
struct CGameScriptInterface_GiveHeroAbility_SelfOverlay
{
    std::byte m_Pad00[0x14];
    void* m_Field14; // 0x14
};
static_assert(
    offsetof(CGameScriptInterface_GiveHeroAbility_SelfOverlay, m_Field14) == 0x14);

struct CThingPlayerCreature_GiveHeroAbility_Overlay
{
    std::byte m_Pad00[0x91];
    std::uint8_t m_Flags91; // 0x91
};
static_assert(
    offsetof(CThingPlayerCreature_GiveHeroAbility_Overlay, m_Flags91) == 0x91);

using Fn_00449970 = void* (__thiscall*)(void*);
using Fn_00487DC0 = CThingPlayerCreature* (__thiscall*)(void*);

inline constexpr Fn_00449970 CALL_00449970 =
    reinterpret_cast<Fn_00449970>(0x00449970);
inline constexpr Fn_00487DC0 CALL_00487DC0 =
    reinterpret_cast<Fn_00487DC0>(0x00487DC0);
} // namespace

void __thiscall CGameScriptInterface::GiveHeroAbility(
    EHeroAbility ability,
    bool enabled) const
{
    const auto* const self =
        reinterpret_cast<const CGameScriptInterface_GiveHeroAbility_SelfOverlay*>(this);
    auto* const playerCreature = CALL_00487DC0(CALL_00449970(self->m_Field14));

    if (playerCreature != nullptr)
    {
        const auto* const playerCreatureOverlay =
            reinterpret_cast<const CThingPlayerCreature_GiveHeroAbility_Overlay*>(
                playerCreature);

        if ((playerCreatureOverlay->m_Flags91 & 0x01) == 0)
        {
            playerCreature->LearnAbility(ability, enabled, -1, true);
        }
    }
}