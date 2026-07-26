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
    CKeyPair<TKey, TValue>* m_pBegin; // 0x00
    CKeyPair<TKey, TValue>* m_pEnd;   // 0x04

    CKeyPair<TKey, TValue>* __thiscall LowerBound(const TKey* pKey);
};

using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

static_assert(offsetof(TCInterfaceMap, m_pBegin) == 0x00);
static_assert(offsetof(TCInterfaceMap, m_pEnd) == 0x04);

class CThing
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
    void __thiscall EntityPostOpinionDeedToRecipientVillage(
        const CScriptThing& entity,
        EOpinionDeedType deedType,
        const CScriptThing& recipient) const;
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
    std::uint32_t m_Flags28; // 0x28
    std::byte m_Pad2C[0x18];
    TCInterfaceMap m_InterfaceMap44; // 0x44
};

static_assert(offsetof(CThingOpinionOverlay, m_Flags28) == 0x28);
static_assert(offsetof(CThingOpinionOverlay, m_InterfaceMap44) == 0x44);

struct CRecipientThingOverlay
{
    std::byte m_Pad00[0x10];
    std::uint8_t m_Field10; // 0x10
    std::byte m_Pad11[0x18];
    std::uint32_t m_Flags28; // 0x28
};

static_assert(offsetof(CRecipientThingOverlay, m_Field10) == 0x10);
static_assert(offsetof(CRecipientThingOverlay, m_Flags28) == 0x28);
}

void __thiscall CGameScriptInterface::EntityPostOpinionDeedToRecipientVillage(
    const CScriptThing& entity,
    EOpinionDeedType deedType,
    const CScriptThing& recipient) const
{
    const auto* const entityOverlay = reinterpret_cast<const CScriptThingOverlay*>(&entity);
    CThing* const pEntityThing = entityOverlay->m_pVTable->GetThing(&entity);

    const auto* const recipientOverlay = reinterpret_cast<const CScriptThingOverlay*>(&recipient);
    CThing* const pRecipientThing = recipientOverlay->m_pVTable->GetThing(&recipient);

    if ((pEntityThing != nullptr) && (pRecipientThing != nullptr))
    {
        const auto* const pRecipientThingOverlay =
            reinterpret_cast<const CRecipientThingOverlay*>(pRecipientThing);
        auto* const pEntityThingOverlay = reinterpret_cast<CThingOpinionOverlay*>(pEntityThing);

        if ((pRecipientThingOverlay->m_Field10 == 1) &&
            (static_cast<std::int8_t>(pRecipientThingOverlay->m_Flags28 >> 8) < 0) &&
            ((pEntityThingOverlay->m_Flags28 & 0x00020000U) != 0))
        {
            ETCInterfaceType interfaceType = static_cast<ETCInterfaceType>(0x51);
            CKeyPair<ETCInterfaceType, CTCBase*>* pEntry =
                pEntityThingOverlay->m_InterfaceMap44.LowerBound(&interfaceType);
            const EOpinionDeedType originalDeedType = deedType;

            if ((pEntry == pEntityThingOverlay->m_InterfaceMap44.m_pEnd) ||
                (static_cast<ETCInterfaceType>(0x51) < pEntry->m_Key))
            {
                pEntry = pEntityThingOverlay->m_InterfaceMap44.m_pEnd;
            }

            auto* const pDeedLog = reinterpret_cast<CTCHeroOpinionDeedLog*>(pEntry->m_Value);

            bool bParam6 = true;
            if ((static_cast<std::int32_t>(deedType) < 3) ||
                (10 < static_cast<std::int32_t>(deedType)))
            {
                bParam6 = false;
            }

            pDeedLog->PostDeed(
                originalDeedType,
                pEntityThing->DoSizeof(),
                pRecipientThing,
                reinterpret_cast<CThingAICreature*>(pRecipientThing),
                static_cast<EOpinionPostDeedType>(4),
                bParam6,
                false,
                nullptr,
                false,
                0);
        }
    }
}