#include <cstddef>
#include <cstdint>

class CGameScriptInterface;
class CScriptThing;
class CTCBase;
class CTCWife;
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

class CTCWife
{
public:
    bool __thiscall IsWillingToMarry();
};

class CGameScriptInterface
{
public:
    bool __thiscall IsEntityMarriable(const CScriptThing& entity) const;
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

struct MarriableThingOverlay
{
    std::byte m_Pad00[0x10];
    std::uint8_t m_Field10; // 0x10
    std::byte m_Pad11[0x27];
    std::uint32_t m_Flags38; // 0x38
    std::byte m_Pad3C[0x08];
    TCInterfaceMap m_InterfaceMap44; // 0x44
    std::byte m_Pad4C[0x45];
    std::uint8_t m_Flags91; // 0x91
};

static_assert(offsetof(MarriableThingOverlay, m_Field10) == 0x10);
static_assert(offsetof(MarriableThingOverlay, m_Flags38) == 0x38);
static_assert(offsetof(MarriableThingOverlay, m_InterfaceMap44) == 0x44);
static_assert(offsetof(MarriableThingOverlay, m_Flags91) == 0x91);
}

bool __thiscall CGameScriptInterface::IsEntityMarriable(const CScriptThing& entity) const
{
    const auto* const entityOverlay = reinterpret_cast<const CScriptThingOverlay*>(&entity);
    auto* const pThing =
        reinterpret_cast<MarriableThingOverlay*>(entityOverlay->m_pVTable->GetThing(&entity));

    if ((pThing != nullptr) &&
        ((pThing->m_Flags91 & 0x01) == 0) &&
        (pThing->m_Field10 == 0x01) &&
        ((pThing->m_Flags38 & 0x00800000) != 0))
    {
        ETCInterfaceType wifeInterfaceType = static_cast<ETCInterfaceType>(0xD7);
        CKeyPair<ETCInterfaceType, CTCBase*>* pWifeEntry =
            pThing->m_InterfaceMap44.LowerBound(&wifeInterfaceType);

        if ((pWifeEntry == pThing->m_InterfaceMap44.m_pEnd) ||
            (static_cast<ETCInterfaceType>(0xD7) < pWifeEntry->m_Key))
        {
            pWifeEntry = pThing->m_InterfaceMap44.m_pEnd;
        }

        return reinterpret_cast<CTCWife*>(pWifeEntry->m_Value)->IsWillingToMarry();
    }

    return false;
}