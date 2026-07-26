#include <cstddef>
#include <cstdint>

class CGameScriptInterface;
class CScriptThing;
class CTCBase;
class CTCHeroOpinionDeedLog;
class CThing;

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

class CTCHeroOpinionDeedLog
{
public:
    void __thiscall RemoveDeedStillSearchingForWitnesses(long deedId);
};

class CGameScriptInterface
{
public:
    void __thiscall RemoveOpinionDeedStillSearchingForWitnesses(
        const CScriptThing& entity,
        long deedId) const;
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CScriptThingVTableOverlay
{
    std::byte m_Pad00[0x2C];
    CThing* (__thiscall* GetThing)(const CScriptThing* pThis); // 0x2C
};

static_assert(offsetof(CScriptThingVTableOverlay, GetThing) == 0x2C);

struct CScriptThingOverlay
{
    const CScriptThingVTableOverlay* m_pVTable; // 0x00
};

static_assert(offsetof(CScriptThingOverlay, m_pVTable) == 0x00);

struct CThingOpinionOverlay
{
    std::byte m_Pad00[0x28];
    std::uint32_t m_Flags28; // 0x28
    std::byte m_Pad2C[0x18];
    TCInterfaceMap m_InterfaceMap44; // 0x44
};

static_assert(offsetof(CThingOpinionOverlay, m_Flags28) == 0x28);
static_assert(offsetof(CThingOpinionOverlay, m_InterfaceMap44) == 0x44);

constexpr ETCInterfaceType kHeroOpinionLogInterfaceType =
    static_cast<ETCInterfaceType>(0x51);
constexpr std::uint32_t kHasHeroOpinionLogFlag = 0x00020000;
} // namespace

void __thiscall CGameScriptInterface::RemoveOpinionDeedStillSearchingForWitnesses(
    const CScriptThing& entity,
    long deedId) const
{
    const auto* const entityOverlay = reinterpret_cast<const CScriptThingOverlay*>(&entity);
    CThing* const pThing = entityOverlay->m_pVTable->GetThing(&entity);

    if ((pThing != nullptr) &&
        ((reinterpret_cast<const CThingOpinionOverlay*>(pThing)->m_Flags28 &
          kHasHeroOpinionLogFlag) != 0))
    {
        auto* const thingOverlay = reinterpret_cast<CThingOpinionOverlay*>(pThing);
        ETCInterfaceType interfaceType = kHeroOpinionLogInterfaceType;
        CKeyPair<ETCInterfaceType, CTCBase*>* pEntry =
            thingOverlay->m_InterfaceMap44.LowerBound(&interfaceType);

        if ((pEntry == thingOverlay->m_InterfaceMap44.m_pEnd) ||
            (kHeroOpinionLogInterfaceType < pEntry->m_Key))
        {
            pEntry = thingOverlay->m_InterfaceMap44.m_pEnd;
        }

        reinterpret_cast<CTCHeroOpinionDeedLog*>(pEntry->m_Value)
            ->RemoveDeedStillSearchingForWitnesses(deedId);
    }
}