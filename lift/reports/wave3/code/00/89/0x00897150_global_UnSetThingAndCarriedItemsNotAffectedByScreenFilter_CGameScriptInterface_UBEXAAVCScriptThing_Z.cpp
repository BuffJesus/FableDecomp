#include <cstddef>
#include <cstdint>

class CScriptThing;

enum ETCInterfaceType : std::int32_t
{
    ETCInterfaceType_ThingAndCarriedItemsNotAffectedByScreenFilter = 0x46,
};

class CTCBase;

template <typename K, typename V, typename Compare>
class CVectorMap
{
public:
    using value_type = std::int32_t[2];

    value_type* LowerBound(const K& key);
};

template <typename K, typename V>
class CKeyPairCompareLess;

struct CThingScreenFilterOverlay
{
    std::byte m_Pad00[0x1C];
    std::uint8_t m_Enabled; // 0x1C
};
static_assert(offsetof(CThingScreenFilterOverlay, m_Enabled) == 0x1C);

struct CThingFlagsOverlay
{
    std::byte m_Pad00[0x28];
    std::uint8_t m_Flags; // 0x28
};
static_assert(offsetof(CThingFlagsOverlay, m_Flags) == 0x28);

struct CThingInterfaceMapOverlay
{
    std::byte m_Pad00[0x44];
    CVectorMap<
        ETCInterfaceType,
        CTCBase*,
        CKeyPairCompareLess<ETCInterfaceType, CTCBase*>> m_InterfaceMap; // 0x44
};
static_assert(offsetof(CThingInterfaceMapOverlay, m_InterfaceMap) == 0x44);

class CGameScriptInterface
{
public:
    virtual void UnSetThingAndCarriedItemsNotAffectedByScreenFilter(CScriptThing& thing) const;
};

void CGameScriptInterface::UnSetThingAndCarriedItemsNotAffectedByScreenFilter(CScriptThing& thing) const
{
    using GetThingFn = int* (__thiscall*)(CScriptThing*);

    void** const vtable = *reinterpret_cast<void***>(&thing);
    auto* const thingObject =
        reinterpret_cast<CThingInterfaceMapOverlay*>(
            reinterpret_cast<GetThingFn>(vtable[0x2C / sizeof(void*)])(&thing));

    if ((reinterpret_cast<CThingFlagsOverlay*>(thingObject)->m_Flags & 0x40) != 0)
    {
        const ETCInterfaceType interfaceType =
            ETCInterfaceType_ThingAndCarriedItemsNotAffectedByScreenFilter;

        auto* interfaceEntry = thingObject->m_InterfaceMap.LowerBound(interfaceType);
        auto* const endEntry = *reinterpret_cast<decltype(interfaceEntry)*>(
            reinterpret_cast<std::byte*>(&thingObject->m_InterfaceMap) + 0x04);

        if ((interfaceEntry == endEntry) || (0x46 < (*interfaceEntry)[0]))
        {
            interfaceEntry = endEntry;
        }

        reinterpret_cast<CThingScreenFilterOverlay*>(
            reinterpret_cast<void*>((*interfaceEntry)[1]))->m_Enabled = 0;
    }
}