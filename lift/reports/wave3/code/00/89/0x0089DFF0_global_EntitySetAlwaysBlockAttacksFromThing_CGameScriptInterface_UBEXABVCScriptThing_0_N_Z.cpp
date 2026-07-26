#include <cstddef>
#include <cstdint>

class CGameScriptInterface;
class CScriptThing;
class CTCBase;

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

namespace
{
struct CScriptThingVTableOverlay
{
    std::byte m_Pad00[0x2C];
    void* (__thiscall* GetThing)(const CScriptThing* pThis); // 0x2C
};

static_assert(offsetof(CScriptThingVTableOverlay, GetThing) == 0x2C);

struct CScriptThingOverlay
{
    const CScriptThingVTableOverlay* m_pVTable; // 0x00
};

static_assert(offsetof(CScriptThingOverlay, m_pVTable) == 0x00);

using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CThingCombatBlockingOverlay
{
    std::byte m_Pad00[0x24];
    std::uint32_t m_Flags24;          // 0x24
    std::byte m_Pad28[0x1C];
    TCInterfaceMap m_InterfaceMap44;  // 0x44
    std::byte m_Pad4C[0x91 - 0x4C];
    std::uint8_t m_Byte91;            // 0x91
};

static_assert(offsetof(CThingCombatBlockingOverlay, m_Flags24) == 0x24);
static_assert(offsetof(CThingCombatBlockingOverlay, m_InterfaceMap44) == 0x44);
static_assert(offsetof(CThingCombatBlockingOverlay, m_Byte91) == 0x91);

extern "C" void __thiscall CombatBlockingData_AddIfNew(CTCBase* pThis, void* pThing);
extern "C" void __thiscall CombatBlockingData_RemoveAndUpdate(CTCBase* pThis, void* pThing);
}

void __thiscall CGameScriptInterface::EntitySetAlwaysBlockAttacksFromThing(
    const CScriptThing& entity,
    const CScriptThing& other_entity,
    bool always_block) const
{
    const auto* const entityOverlay = reinterpret_cast<const CScriptThingOverlay*>(&entity);
    auto* const entityThing = static_cast<CThingCombatBlockingOverlay*>(
        entityOverlay->m_pVTable->GetThing(&entity));

    const auto* const otherEntityOverlay = reinterpret_cast<const CScriptThingOverlay*>(&other_entity);
    auto* const otherEntityThing = static_cast<CThingCombatBlockingOverlay*>(
        otherEntityOverlay->m_pVTable->GetThing(&other_entity));

    if ((entityThing != nullptr) &&
        ((entityThing->m_Byte91 & 0x01) == 0) &&
        (otherEntityThing != nullptr) &&
        ((otherEntityThing->m_Byte91 & 0x01) == 0) &&
        ((entityThing->m_Flags24 & 0x00010000) != 0))
    {
        ETCInterfaceType interfaceType = static_cast<ETCInterfaceType>(0x30);
        CKeyPair<ETCInterfaceType, CTCBase*>* pEntry =
            entityThing->m_InterfaceMap44.LowerBound(&interfaceType);

        if ((pEntry == entityThing->m_InterfaceMap44.m_pEnd) ||
            (static_cast<ETCInterfaceType>(0x30) < pEntry->m_Key))
        {
            pEntry = entityThing->m_InterfaceMap44.m_pEnd;
        }

        CTCBase* const pCombatBlockingData = pEntry->m_Value;

        if (always_block)
        {
            CombatBlockingData_AddIfNew(
                pCombatBlockingData,
                otherEntityThing);
            return;
        }

        CombatBlockingData_RemoveAndUpdate(
            pCombatBlockingData,
            otherEntityThing);
    }
}