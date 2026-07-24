#include <cstddef>
#include <cstdint>

class C3DVector;
class CGameScriptInterface;
class CScriptThing;
class CTCBase;
class CTCHeroOpinionDeedLog;
class CFaction;
class CThing;
class CThingAICreature;

enum EOpinionDeedType : std::int32_t;
enum EOpinionPostDeedType : std::int32_t;
enum ETCInterfaceType : std::int32_t;

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
    CKeyPair<TKey, TValue>* __thiscall LowerBound(const TKey* pKey);
};

class CThingWithOpinionInterfaces
{
public:
    C3DVector* __thiscall DoSizeof() const;
};

class CTCHeroOpinionDeedLog
{
public:
    long __thiscall PostDeed(
        EOpinionDeedType deedType,
        const C3DVector* pPosition,
        CThing* pThing,
        CThingAICreature* pCreature,
        EOpinionPostDeedType postDeedType,
        bool bParam6,
        bool bParam7,
        CFaction* pFaction,
        bool bParam9,
        long param10);
};

class CGameScriptInterface
{
public:
    long __thiscall EntityPostOpinionDeedKeepSearchingForWitnesses(
        const CScriptThing& entity,
        EOpinionDeedType deedType,
        const CScriptThing& witness) const;
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CScriptThing_VTable_Overlay
{
    std::byte m_Pad00[0x2C];
    CThingWithOpinionInterfaces* (__thiscall* GetThing)(const CScriptThing* pScriptThing); // 0x2C
};

static_assert(offsetof(CScriptThing_VTable_Overlay, GetThing) == 0x2C);

struct CScriptThing_Overlay
{
    const CScriptThing_VTable_Overlay* m_pVTable; // 0x00
};

static_assert(offsetof(CScriptThing_Overlay, m_pVTable) == 0x00);

struct CThingWithOpinionInterfaces_Overlay
{
    std::byte m_Pad00[0x28];
    std::uint32_t m_Flags28;         // 0x28
    std::byte m_Pad2C[0x18];
    TCInterfaceMap m_InterfaceMap44; // 0x44
    CKeyPair<ETCInterfaceType, CTCBase*>* m_InterfaceMapEnd; // 0x48
};

static_assert(offsetof(CThingWithOpinionInterfaces_Overlay, m_Flags28) == 0x28);
static_assert(offsetof(CThingWithOpinionInterfaces_Overlay, m_InterfaceMap44) == 0x44);
static_assert(offsetof(CThingWithOpinionInterfaces_Overlay, m_InterfaceMapEnd) == 0x48);
}

long __thiscall CGameScriptInterface::EntityPostOpinionDeedKeepSearchingForWitnesses(
    const CScriptThing& entity,
    EOpinionDeedType deedType,
    const CScriptThing& witness) const
{
    const auto* const entityOverlay = reinterpret_cast<const CScriptThing_Overlay*>(&entity);
    CThingWithOpinionInterfaces* const pEntityThing = entityOverlay->m_pVTable->GetThing(&entity);

    if (pEntityThing == nullptr)
    {
        return -1;
    }

    auto* const pEntityThingOverlay =
        reinterpret_cast<CThingWithOpinionInterfaces_Overlay*>(pEntityThing);
    if ((pEntityThingOverlay->m_Flags28 & 0x00020000U) == 0)
    {
        return -1;
    }

    ETCInterfaceType interfaceType = static_cast<ETCInterfaceType>(0x51);
    CKeyPair<ETCInterfaceType, CTCBase*>* pEntry =
        pEntityThingOverlay->m_InterfaceMap44.LowerBound(&interfaceType);
    const EOpinionDeedType originalDeedType = deedType;

    if ((pEntry == pEntityThingOverlay->m_InterfaceMapEnd) ||
        (static_cast<std::int32_t>(interfaceType) < static_cast<std::int32_t>(pEntry->m_Key)))
    {
        pEntry = pEntityThingOverlay->m_InterfaceMapEnd;
    }

    auto* const pDeedLog = reinterpret_cast<CTCHeroOpinionDeedLog*>(pEntry->m_Value);

    bool bKeepSearchingForWitnesses = false;
    if ((static_cast<std::int32_t>(deedType) >= 3) && (static_cast<std::int32_t>(deedType) <= 10))
    {
        bKeepSearchingForWitnesses = true;
    }

    long result = 0;
    const bool bParam9 = false;
    CFaction* const pFaction = nullptr;
    const bool bParam7 = true;
    const EOpinionPostDeedType postDeedType = static_cast<EOpinionPostDeedType>(7);
    CThingAICreature* const pCreature = nullptr;
    const bool bParam6 = bKeepSearchingForWitnesses;

    const auto* const witnessOverlay = reinterpret_cast<const CScriptThing_Overlay*>(&witness);
    CThing* const pWitnessThing =
        reinterpret_cast<CThing*>(witnessOverlay->m_pVTable->GetThing(&witness));
    C3DVector* const pPosition = pEntityThing->DoSizeof();

    result = pDeedLog->PostDeed(
        originalDeedType,
        pPosition,
        pWitnessThing,
        pCreature,
        postDeedType,
        bParam6,
        bParam7,
        pFaction,
        bParam9,
        result);
    return result;
}