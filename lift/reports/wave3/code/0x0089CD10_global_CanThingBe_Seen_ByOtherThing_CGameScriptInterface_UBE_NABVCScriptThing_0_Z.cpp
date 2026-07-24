#include <cstdint>

class CScriptThing;

class CGameScriptInterface
{
public:
    bool __thiscall CanThingBe_Seen_ByOtherThing(
        const CScriptThing& thing,
        const CScriptThing& otherThing) const;

    bool __thiscall CanThingBe_Sensed_ByOtherThing(
        const CScriptThing& thing,
        const CScriptThing& otherThing,
        std::int32_t senseType) const;
};

bool __thiscall CGameScriptInterface::CanThingBe_Seen_ByOtherThing(
    const CScriptThing& thing,
    const CScriptThing& otherThing) const
{
    return CanThingBe_Sensed_ByOtherThing(thing, otherThing, 1);
}