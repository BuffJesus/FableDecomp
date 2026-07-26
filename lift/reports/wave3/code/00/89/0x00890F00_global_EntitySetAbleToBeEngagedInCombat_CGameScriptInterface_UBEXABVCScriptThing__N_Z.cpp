#include <cstddef>
#include <cstdint>

class CGameScriptInterface;
class CScriptThing;

class CThingCombatOverlay
{
public:
    std::uint8_t m_Pad00[0x6C];        // 0x00
    std::uint8_t m_Byte6C;             // 0x6C
    std::uint8_t m_Pad6D[0x91 - 0x6D]; // 0x6D
    std::uint8_t m_Byte91;             // 0x91
    std::uint8_t m_Pad92[0xBC - 0x92]; // 0x92
    std::uint8_t m_ByteBC;             // 0xBC
};

static_assert(offsetof(CThingCombatOverlay, m_Byte6C) == 0x6C);
static_assert(offsetof(CThingCombatOverlay, m_Byte91) == 0x91);
static_assert(offsetof(CThingCombatOverlay, m_ByteBC) == 0xBC);

void __thiscall CGameScriptInterface::EntitySetAbleToBeEngagedInCombat(
    const CScriptThing& script_thing,
    bool able_to_be_engaged_in_combat) const
{
    using GetThingFn = void* (__thiscall*)(const CScriptThing*);

    auto* const vtable = *reinterpret_cast<void***>(const_cast<CScriptThing*>(&script_thing));
    auto* const thing = static_cast<CThingCombatOverlay*>(
        reinterpret_cast<GetThingFn>(vtable[0x2C / sizeof(void*)])(&script_thing));

    if ((thing != nullptr) &&
        ((thing->m_Byte91 & 0x01) == 0) &&
        ((thing->m_Byte6C & 0x02) != 0))
    {
        thing->m_ByteBC =
            thing->m_ByteBC ^
            (((static_cast<std::uint8_t>(able_to_be_engaged_in_combat) << 5) ^ thing->m_ByteBC) & 0x20);
    }
}