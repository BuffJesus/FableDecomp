#include <cstddef>
#include <cstdint>

struct CCharString;
struct CThingPlayerCreature;

struct CGameScriptInterface_Overlay_14 {
    std::byte pad_00[0x14];
    void* field_14;
};
static_assert(offsetof(CGameScriptInterface_Overlay_14, field_14) == 0x14);

struct CThingPlayerCreature_Overlay_91 {
    std::byte pad_00[0x91];
    std::uint8_t flags_91;
};
static_assert(offsetof(CThingPlayerCreature_Overlay_91, flags_91) == 0x91);

using Fn_00449970 = CThingPlayerCreature* (__thiscall*)(void*);
using Fn_00487DC0 = CThingPlayerCreature* (__thiscall*)(CThingPlayerCreature*);

static constexpr Fn_00449970 sub_00449970 =
    reinterpret_cast<Fn_00449970>(0x00449970);
static constexpr Fn_00487DC0 sub_00487DC0 =
    reinterpret_cast<Fn_00487DC0>(0x00487DC0);

void CGameScriptInterface::GiveHeroExpression(
    const CCharString& expression,
    long value,
    bool immediate) const
{
    auto* const self = reinterpret_cast<const CGameScriptInterface_Overlay_14*>(this);

    CThingPlayerCreature* creature = sub_00449970(self->field_14);
    creature = sub_00487DC0(creature);

    if (creature == nullptr) {
        return;
    }

    const auto* const creatureOverlay =
        reinterpret_cast<const CThingPlayerCreature_Overlay_91*>(creature);
    if ((creatureOverlay->flags_91 & 0x01) != 0) {
        return;
    }

    creature->LearnExpression(expression, value, immediate);
}