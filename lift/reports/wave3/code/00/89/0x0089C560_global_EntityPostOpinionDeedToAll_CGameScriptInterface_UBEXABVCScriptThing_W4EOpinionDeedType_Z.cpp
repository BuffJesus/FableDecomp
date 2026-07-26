#include <cstddef>
#include <cstdint>

class C3DVector;
class CCharString;
class CGameScriptInterface;
class CScriptThing;
class CTCBase;
class CTCParamsBase;
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
    CKeyPair<TKey, TValue>* m_pBegin; // 0x00
    CKeyPair<TKey, TValue>* m_pEnd;   // 0x04

    CKeyPair<TKey, TValue>* __thiscall LowerBound(const TKey* pKey);
};

using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

static_assert(offsetof(TCInterfaceMap, m_pBegin) == 0x00);
static_assert(offsetof(TCInterfaceMap, m_pEnd) == 0x04);

class CCharString
{
public:
    CCharString(const char* pString, int length);
    ~CCharString();
};

class CThing
{
public:
    C3DVector* __thiscall DoSizeof() const;
    CTCBase* __thiscall AddTC(const CCharString& tcName, bool enable, CTCParamsBase* pParams);
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
        bool arg6,
        bool arg7,
        CFaction* pFaction,
        bool arg9,
        long arg10);
};

class CGameScriptInterface
{
public:
    void __thiscall EntityPostOpinionDeedToAll(
        const CScriptThing& entity,
        EOpinionDeedType deedType) const;
};

namespace
{
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
    std::uint32_t m_Flags; // 0x28
    std::byte m_Pad2C[0x18];
    TCInterfaceMap m_TCMap; // 0x44
};

static_assert(offsetof(CThingOpinionOverlay, m_Flags) == 0x28);
static_assert(offsetof(CThingOpinionOverlay, m_TCMap) == 0x44);

using OpinionSourceHelper_t = void(__thiscall*)(CTCBase* pThis, std::uint32_t arg0, float arg1);

inline constexpr OpinionSourceHelper_t OpinionSourceHelper =
    reinterpret_cast<OpinionSourceHelper_t>(0x007712F0);
}

void __thiscall CGameScriptInterface::EntityPostOpinionDeedToAll(
    const CScriptThing& entity,
    EOpinionDeedType deedType) const
{
    const auto* const entityOverlay = reinterpret_cast<const CScriptThingOverlay*>(&entity);
    CThing* const pThing = entityOverlay->m_pVTable->GetThing(&entity);

    if (pThing == nullptr)
    {
        return;
    }

    auto* const thingOverlay = reinterpret_cast<CThingOpinionOverlay*>(pThing);

    if ((thingOverlay->m_Flags & 0x00020000U) != 0)
    {
        ETCInterfaceType interfaceType = static_cast<ETCInterfaceType>(0x51);
        CKeyPair<ETCInterfaceType, CTCBase*>* pEntry =
            thingOverlay->m_TCMap.LowerBound(&interfaceType);
        const EOpinionDeedType originalDeedType = deedType;

        if ((pEntry == thingOverlay->m_TCMap.m_pEnd) ||
            (static_cast<std::int32_t>(interfaceType) <
             static_cast<std::int32_t>(pEntry->m_Key)))
        {
            pEntry = thingOverlay->m_TCMap.m_pEnd;
        }

        auto* const pDeedLog = reinterpret_cast<CTCHeroOpinionDeedLog*>(pEntry->m_Value);

        bool arg6 = true;
        if ((static_cast<std::int32_t>(deedType) < 3) ||
            (static_cast<std::int32_t>(originalDeedType) > 10))
        {
            arg6 = false;
        }

        pDeedLog->PostDeed(
            originalDeedType,
            pThing->DoSizeof(),
            nullptr,
            nullptr,
            static_cast<EOpinionPostDeedType>(7),
            arg6,
            false,
            nullptr,
            false,
            0);
        return;
    }

    if ((thingOverlay->m_Flags & 0x00010000U) != 0)
    {
        ETCInterfaceType interfaceType = static_cast<ETCInterfaceType>(0x50);
        CKeyPair<ETCInterfaceType, CTCBase*>* pEntry =
            thingOverlay->m_TCMap.LowerBound(&interfaceType);

        if ((pEntry == thingOverlay->m_TCMap.m_pEnd) ||
            (static_cast<std::int32_t>(interfaceType) <
             static_cast<std::int32_t>(pEntry->m_Key)))
        {
            pEntry = thingOverlay->m_TCMap.m_pEnd;
        }

        OpinionSourceHelper(pEntry->m_Value, static_cast<std::uint32_t>(interfaceType), 5.0f);
        return;
    }

    CTCBase* pOpinionSource;
    {
        CCharString tcName("CTCOpinionSource", -1);
        pOpinionSource = pThing->AddTC(tcName, false, nullptr);
    }

    OpinionSourceHelper(pOpinionSource, static_cast<std::uint32_t>(deedType), 5.0f);
}