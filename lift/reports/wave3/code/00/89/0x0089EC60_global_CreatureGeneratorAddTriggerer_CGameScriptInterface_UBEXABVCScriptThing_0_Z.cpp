#include <cstddef>
#include <cstdint>

class CGameScriptInterface;
class CScriptThing;
class CThing;
class CTCBase;
class CTCShop;

using ETCInterfaceType = int;

template <typename TKey, typename TValue>
class CKeyPairCompareLess;

template <typename TKey, typename TValue, typename TCompare>
class CVectorMap
{
public:
    struct Entry
    {
        TKey first;
        TValue second;
    };

    std::byte m_Pad00[0x04];
    Entry* m_End; // 0x04

    Entry* __thiscall LowerBound(const TKey* key);
};

class CTCShop
{
public:
    CThing* __thiscall AddShopper(CThing* shopper);
};

class CGameScriptInterface
{
public:
    void __thiscall CreatureGeneratorAddTriggerer(
        const CScriptThing& creatureGenerator,
        const CScriptThing& triggerer) const;
};

namespace
{
using TCInterfaceMap =
    CVectorMap<
        ETCInterfaceType,
        CTCBase*,
        CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CScriptThing_VTable_Overlay
{
    std::byte m_Pad00[0x2C];
    CThing* (__thiscall* GetThing)(const CScriptThing* scriptThing);
};
static_assert(offsetof(CScriptThing_VTable_Overlay, GetThing) == 0x2C);

struct CScriptThing_Overlay
{
    const CScriptThing_VTable_Overlay* m_pVTable;
};
static_assert(offsetof(CScriptThing_Overlay, m_pVTable) == 0x00);

struct TCInterfaceEntryOverlay
{
    ETCInterfaceType m_Key; // 0x00
    CTCBase* m_pValue;      // 0x04
};
static_assert(offsetof(TCInterfaceEntryOverlay, m_Key) == 0x00);
static_assert(offsetof(TCInterfaceEntryOverlay, m_pValue) == 0x04);

struct ThingOverlay
{
    std::byte m_Pad00[0x24];
    std::uint32_t m_Flags24; // 0x24
    std::byte m_Pad28[0x1C];
    TCInterfaceMap m_TCInterfaceMap; // 0x44
    std::byte m_Pad4C[0x20];
    std::uint8_t m_Flags6C; // 0x6C
};
static_assert(offsetof(ThingOverlay, m_Flags24) == 0x24);
static_assert(offsetof(ThingOverlay, m_TCInterfaceMap) == 0x44);
static_assert(offsetof(ThingOverlay, m_Flags6C) == 0x6C);
}

void __thiscall CGameScriptInterface::CreatureGeneratorAddTriggerer(
    const CScriptThing& creatureGenerator,
    const CScriptThing& triggerer) const
{
    const auto* const creatureGeneratorScriptThing =
        reinterpret_cast<const CScriptThing_Overlay*>(&creatureGenerator);
    CThing* const creatureGeneratorThing =
        creatureGeneratorScriptThing->m_pVTable->GetThing(&creatureGenerator);

    const auto* const triggererScriptThing =
        reinterpret_cast<const CScriptThing_Overlay*>(&triggerer);
    CThing* const originalTriggererThing =
        triggererScriptThing->m_pVTable->GetThing(&triggerer);

    CThing* resultThing = originalTriggererThing;

    auto* const creatureGeneratorThingOverlay =
        reinterpret_cast<ThingOverlay*>(creatureGeneratorThing);

    if ((creatureGeneratorThingOverlay->m_Flags24 & 0x20000000) != 0)
    {
        ETCInterfaceType interfaceType = 0x3D;

        auto* pEntry = reinterpret_cast<TCInterfaceEntryOverlay*>(
            creatureGeneratorThingOverlay->m_TCInterfaceMap.LowerBound(&interfaceType));

        if ((pEntry == creatureGeneratorThingOverlay->m_TCInterfaceMap.m_End) ||
            (0x3D < pEntry->m_Key))
        {
            pEntry = reinterpret_cast<TCInterfaceEntryOverlay*>(
                creatureGeneratorThingOverlay->m_TCInterfaceMap.m_End);
        }

        CTCBase* const interfaceObject = pEntry->m_pValue;
        resultThing = reinterpret_cast<CThing*>(interfaceObject);

        if ((reinterpret_cast<const ThingOverlay*>(originalTriggererThing)->m_Flags6C & 0x08) != 0)
        {
            resultThing =
                reinterpret_cast<CTCShop*>(interfaceObject)->AddShopper(originalTriggererThing);
        }
    }

    static_cast<void>(resultThing);
}