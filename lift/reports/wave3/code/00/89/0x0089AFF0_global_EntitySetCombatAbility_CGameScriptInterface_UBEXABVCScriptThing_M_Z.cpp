#include <cstddef>
#include <cstdint>

class CGameScriptInterface;
class CScriptThing;
class CTCBase;
class CTCCombat;
class CThingCreatureBase;

enum ETCInterfaceType : std::int32_t
{
};

template <typename TKey, typename TValue>
class CKeyPairCompareLess;

template <typename TKey, typename TValue>
struct CKeyPair
{
    TKey m_Key;     // 0x00
    TValue m_Value; // 0x04
};

static_assert(offsetof(CKeyPair<ETCInterfaceType, CTCBase*>, m_Key) == 0x00);
static_assert(offsetof(CKeyPair<ETCInterfaceType, CTCBase*>, m_Value) == 0x04);
static_assert(sizeof(CKeyPair<ETCInterfaceType, CTCBase*>) == 0x08);

template <typename TKey, typename TValue, typename TCompare>
class CVectorMap
{
public:
    CKeyPair<TKey, TValue>* m_pBegin; // 0x00
    CKeyPair<TKey, TValue>* m_pEnd;   // 0x04

    CKeyPair<TKey, TValue>* __thiscall LowerBound(const TKey* pKey);
};

static_assert(
    offsetof(CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>, m_pBegin) == 0x00);
static_assert(
    offsetof(CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>, m_pEnd) == 0x04);

class CTCCombat
{
public:
    void __thiscall SetBlockPercentageChance(float chance, bool unkFlag);
};

class CGameScriptInterface
{
public:
    void __thiscall EntitySetCombatAbility(const CScriptThing& entity, float chance) const;
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CScriptThingVTableOverlay
{
    std::byte m_Pad00[0x2C];
    CThingCreatureBase* (__thiscall* GetThing)(const CScriptThing* pThis); // 0x2C
};

static_assert(offsetof(CScriptThingVTableOverlay, GetThing) == 0x2C);

struct CScriptThingOverlay
{
    const CScriptThingVTableOverlay* m_pVTable; // 0x00
};

static_assert(offsetof(CScriptThingOverlay, m_pVTable) == 0x00);

struct CThingCreatureBaseOverlay
{
    std::byte m_Pad00[0x24];
    std::uint32_t m_Flags24; // 0x24
    std::byte m_Pad28[0x1C];
    TCInterfaceMap m_InterfaceMap44; // 0x44
    std::byte m_Pad4C[0x45];
    std::uint8_t m_Flags91; // 0x91
};

static_assert(offsetof(CThingCreatureBaseOverlay, m_Flags24) == 0x24);
static_assert(offsetof(CThingCreatureBaseOverlay, m_InterfaceMap44) == 0x44);
static_assert(offsetof(CThingCreatureBaseOverlay, m_Flags91) == 0x91);

constexpr std::uint32_t kHasCombatInterfaceFlag = 0x00010000;
}

void __thiscall
CGameScriptInterface::EntitySetCombatAbility(const CScriptThing& entity, float chance) const
{
    const auto* const entityOverlay = reinterpret_cast<const CScriptThingOverlay*>(&entity);
    auto* const pThing = entityOverlay->m_pVTable->GetThing(&entity);

    if ((pThing != nullptr) &&
        ((reinterpret_cast<const CThingCreatureBaseOverlay*>(pThing)->m_Flags91 & 0x01) == 0) &&
        ((reinterpret_cast<const CThingCreatureBaseOverlay*>(pThing)->m_Flags24 & kHasCombatInterfaceFlag) != 0))
    {
        auto* const pThingOverlay = reinterpret_cast<CThingCreatureBaseOverlay*>(pThing);
        ETCInterfaceType interfaceType = static_cast<ETCInterfaceType>(0x30);
        CKeyPair<ETCInterfaceType, CTCBase*>* pEntry =
            pThingOverlay->m_InterfaceMap44.LowerBound(&interfaceType);

        if ((pEntry == pThingOverlay->m_InterfaceMap44.m_pEnd) ||
            (static_cast<std::int32_t>(interfaceType) <
             static_cast<std::int32_t>(pEntry->m_Key)))
        {
            pEntry = pThingOverlay->m_InterfaceMap44.m_pEnd;
        }

        reinterpret_cast<CTCCombat*>(pEntry->m_Value)->SetBlockPercentageChance(chance, true);
    }
}