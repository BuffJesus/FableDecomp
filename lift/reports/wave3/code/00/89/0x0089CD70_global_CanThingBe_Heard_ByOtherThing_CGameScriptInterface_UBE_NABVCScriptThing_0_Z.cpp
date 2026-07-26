#include <cstddef>
#include <cstdint>

struct C3DVector
{
    float x;
    float y;
    float z;
};

class CScriptThing;
class CGameScriptInterface;

enum class ETCInterfaceType : std::int32_t
{
};

struct CTCBaseOverlay
{
    std::byte pad_0000[0x32];
    std::uint8_t field_0032;
};

static_assert(offsetof(CTCBaseOverlay, field_0032) == 0x32);

struct TCInterfaceMapEntry
{
    std::int32_t key;
    CTCBaseOverlay* value;
};

static_assert(offsetof(TCInterfaceMapEntry, key) == 0x00);
static_assert(offsetof(TCInterfaceMapEntry, value) == 0x04);

struct TCInterfaceMapOverlay
{
    TCInterfaceMapEntry* begin;
    TCInterfaceMapEntry* end;
};

static_assert(offsetof(TCInterfaceMapOverlay, begin) == 0x00);
static_assert(offsetof(TCInterfaceMapOverlay, end) == 0x04);

struct ScriptThingImplOverlay
{
    std::byte pad_0000[0x24];
    std::uint32_t flags_0024;
    std::byte pad_0028[0x1C];
    TCInterfaceMapOverlay interfaces_0044;
};

static_assert(offsetof(ScriptThingImplOverlay, flags_0024) == 0x24);
static_assert(offsetof(ScriptThingImplOverlay, interfaces_0044) == 0x44);

class CGameScriptInterface
{
public:
    bool __thiscall CanThingBe_Heard_ByOtherThing(
        const CScriptThing& thing,
        const CScriptThing& otherThing) const;
};

bool __thiscall CGameScriptInterface::CanThingBe_Heard_ByOtherThing(
    const CScriptThing& thing,
    const CScriptThing& otherThing) const
{
    using ScriptThingBoolVFunc = bool(__thiscall*)(const CScriptThing*);
    using ScriptThingGetImplVFunc = ScriptThingImplOverlay*(__thiscall*)(const CScriptThing*);
    using LowerBoundFunc = TCInterfaceMapEntry*(__thiscall*)(TCInterfaceMapOverlay*, const ETCInterfaceType*);
    using InterfaceGetThingFunc = void*(__thiscall*)(CTCBaseOverlay*);
    using InterfaceGetRangeFunc = float(__thiscall*)(CTCBaseOverlay*);
    using ImplGetPositionFunc = const C3DVector*(__thiscall*)(ScriptThingImplOverlay*);

    auto* const thingVftable =
        *reinterpret_cast<void* const* const*>(&thing);
    auto* const otherThingVftable =
        *reinterpret_cast<void* const* const*>(&otherThing);

    const auto canUseThing =
        reinterpret_cast<ScriptThingBoolVFunc>(thingVftable[0x12C / sizeof(void*)]);
    if (!canUseThing(&thing))
    {
        return false;
    }

    const auto canUseOtherThing =
        reinterpret_cast<ScriptThingBoolVFunc>(otherThingVftable[0x12C / sizeof(void*)]);
    if (!canUseOtherThing(&otherThing))
    {
        return false;
    }

    const auto getImpl =
        reinterpret_cast<ScriptThingGetImplVFunc>(thingVftable[0x2C / sizeof(void*)]);

    ScriptThingImplOverlay* const otherThingImpl =
        reinterpret_cast<ScriptThingGetImplVFunc>(otherThingVftable[0x2C / sizeof(void*)])(&otherThing);
    ScriptThingImplOverlay* const thingImpl = getImpl(&thing);

    if ((otherThingImpl->flags_0024 & 0x00100000U) == 0)
    {
        return false;
    }

    static const auto lowerBound =
        reinterpret_cast<LowerBoundFunc>(0x0040F020);
    static const auto sub_007446A0 =
        reinterpret_cast<InterfaceGetThingFunc>(0x007446A0);
    static const auto sub_00744440 =
        reinterpret_cast<InterfaceGetRangeFunc>(0x00744440);
    static const auto sub_004C73D0 =
        reinterpret_cast<ImplGetPositionFunc>(0x004C73D0);

    const ETCInterfaceType interfaceType = static_cast<ETCInterfaceType>(0x34);
    TCInterfaceMapEntry* interfaceIt =
        lowerBound(&otherThingImpl->interfaces_0044, &interfaceType);

    if (interfaceIt == otherThingImpl->interfaces_0044.end || 0x34 < interfaceIt->key)
    {
        interfaceIt = otherThingImpl->interfaces_0044.end;
    }

    CTCBaseOverlay* const interfaceObject = interfaceIt->value;
    void* const linkedThing = sub_007446A0(interfaceObject);
    if (linkedThing != nullptr && linkedThing == thingImpl)
    {
        return interfaceObject->field_0032 != 0;
    }

    const float hearingRange = sub_00744440(interfaceObject);
    const C3DVector* const otherThingPosition = sub_004C73D0(otherThingImpl);
    const C3DVector* const thingPosition = sub_004C73D0(thingImpl);

    return
        (thingPosition->x - otherThingPosition->x) * (thingPosition->x - otherThingPosition->x) +
        (thingPosition->y - otherThingPosition->y) * (thingPosition->y - otherThingPosition->y) +
        (thingPosition->z - otherThingPosition->z) * (thingPosition->z - otherThingPosition->z) <=
        hearingRange * hearingRange;
}