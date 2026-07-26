#include <cstddef>
#include <cstdint>

class CScriptThing;
class CTCBase;
class CTCCreatureOpinionOfHero;

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

using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

static_assert(offsetof(TCInterfaceMap, m_pBegin) == 0x00);
static_assert(offsetof(TCInterfaceMap, m_pEnd) == 0x04);

class CTCCreatureOpinionOfHero
{
public:
    void __thiscall ClearPersonalityOverride();
};

class CGameScriptInterface
{
public:
    void __thiscall EntityClearPersonalityOverride(const CScriptThing& thing) const;
};

namespace
{
struct EntityClearPersonalityOverride_TargetOverlay
{
    std::byte m_Pad00[0x28];
    std::uint32_t m_Flags28; // 0x28
    std::byte m_Pad2C[0x18];
    TCInterfaceMap m_InterfaceMap; // 0x44
};

static_assert(offsetof(EntityClearPersonalityOverride_TargetOverlay, m_Flags28) == 0x28);
static_assert(offsetof(EntityClearPersonalityOverride_TargetOverlay, m_InterfaceMap) == 0x44);
}

void __thiscall CGameScriptInterface::EntityClearPersonalityOverride(
    const CScriptThing& thing) const
{
    (void)thing;

    using ResolveTargetFn =
        EntityClearPersonalityOverride_TargetOverlay* (__thiscall*)(const CGameScriptInterface*);
    const auto resolveTarget =
        reinterpret_cast<ResolveTargetFn>((*reinterpret_cast<void* const* const*>(this))[0x2C / sizeof(void*)]);

    EntityClearPersonalityOverride_TargetOverlay* const pTarget = resolveTarget(this);
    if ((pTarget != nullptr) && (static_cast<char>(pTarget->m_Flags28 >> 8) < 0))
    {
        ETCInterfaceType interfaceType = static_cast<ETCInterfaceType>(0x4F);
        CKeyPair<ETCInterfaceType, CTCBase*>* pEntry =
            pTarget->m_InterfaceMap.LowerBound(&interfaceType);

        if ((pEntry == pTarget->m_InterfaceMap.m_pEnd) ||
            (0x4F < static_cast<std::int32_t>(pEntry->m_Key)))
        {
            pEntry = pTarget->m_InterfaceMap.m_pEnd;
        }

        reinterpret_cast<CTCCreatureOpinionOfHero*>(pEntry->m_Value)->ClearPersonalityOverride();
    }
}