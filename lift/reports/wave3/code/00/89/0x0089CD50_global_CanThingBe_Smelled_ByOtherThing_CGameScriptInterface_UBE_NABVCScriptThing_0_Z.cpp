#include <cstdint>

class CScriptThing;

class CGameScriptInterface
{
public:
    void __thiscall CanThingBe_Smelled_ByOtherThing(
        const CScriptThing& thing,
        const CScriptThing& otherThing) const;

    void __thiscall CanThingBe_Sensed_ByOtherThing(
        const CScriptThing& thing,
        const CScriptThing& otherThing,
        std::int32_t senseType) const;
};

void __thiscall CGameScriptInterface::CanThingBe_Smelled_ByOtherThing(
    const CScriptThing& thing,
    const CScriptThing& otherThing) const
{
    CanThingBe_Sensed_ByOtherThing(thing, otherThing, 3);
    return;
}