#include <cstddef>
#include <cstdint>

struct CScriptThing;
struct CTCBase;

enum ETCInterfaceType : int;

template <class K, class V, class Compare>
class CVectorMap;

template <class K, class V>
struct CKeyPairCompareLess;

class CTCVillage
{
public:
    void SetVillageLimbo(std::uint8_t enable);
};

class CGameScriptInterface
{
public:
    virtual void SetVillageLimbo(const CScriptThing& thing, bool enable) const;
};

struct CTCEntryOverlay
{
    ETCInterfaceType key;
    CTCBase* value;
};

static_assert(sizeof(CTCEntryOverlay) == 0x8);
static_assert(offsetof(CTCEntryOverlay, key) == 0x0);
static_assert(offsetof(CTCEntryOverlay, value) == 0x4);

struct CVectorMapVillageOverlay
{
    std::byte pad_00[0x4];
    CTCEntryOverlay* end;
};

static_assert(offsetof(CVectorMapVillageOverlay, end) == 0x4);

struct ScriptTargetOverlay
{
    std::byte pad_00[0x24];
    std::uint8_t flags24;
    std::byte pad_25[0x44 - 0x25];
    CVectorMapVillageOverlay interfaceMap;
    std::byte pad_4C[0x91 - 0x4C];
    std::uint8_t flags91;
};

static_assert(offsetof(ScriptTargetOverlay, flags24) == 0x24);
static_assert(offsetof(ScriptTargetOverlay, interfaceMap) == 0x44);
static_assert(offsetof(ScriptTargetOverlay, flags91) == 0x91);

void CGameScriptInterface::SetVillageLimbo(const CScriptThing& thing, bool enable) const
{
    using RawThingSlot2CFn = ScriptTargetOverlay* (__thiscall*)(const void*);

    const void* const thingObject = &thing;
    void** const thingVftable = *reinterpret_cast<void***>(const_cast<void*>(thingObject));
    ScriptTargetOverlay* const target =
        reinterpret_cast<RawThingSlot2CFn>(thingVftable[0x2C / sizeof(void*)])(thingObject);

    if ((target != nullptr) &&
        ((target->flags91 & 0x01) == 0) &&
        ((target->flags24 & 0x04) != 0))
    {
        ETCInterfaceType key = static_cast<ETCInterfaceType>(0x22);
        CTCEntryOverlay* entry = reinterpret_cast<CTCEntryOverlay*>(
            CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>::LowerBound(
                reinterpret_cast<CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>*>(
                    &target->interfaceMap),
                &key));

        if ((entry == target->interfaceMap.end) || (0x22 < static_cast<int>(entry->key)))
        {
            entry = target->interfaceMap.end;
        }

        reinterpret_cast<CTCVillage*>(entry[1 - 1].value)->SetVillageLimbo(
            *reinterpret_cast<const std::uint8_t*>(&enable));
    }
}