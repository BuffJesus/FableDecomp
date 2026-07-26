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

struct CThingAppearanceSeedOverlay
{
    std::byte m_Pad00[0x20];
    std::uint8_t m_Flags20;          // 0x20
    std::byte m_Pad21[0x44 - 0x21];
    TCInterfaceMap m_InterfaceMap44; // 0x44
};

static_assert(offsetof(CThingAppearanceSeedOverlay, m_Flags20) == 0x20);
static_assert(offsetof(CThingAppearanceSeedOverlay, m_InterfaceMap44) == 0x44);

extern "C" void __thiscall AddToInterface(CTCBase* pThis, std::uint32_t appearance_seed);
}

void __thiscall CGameScriptInterface::EntitySetAppearanceSeed(
    const CScriptThing& entity,
    std::uint32_t appearance_seed) const
{
    const auto* const entityOverlay = reinterpret_cast<const CScriptThingOverlay*>(&entity);
    auto* const pThing = static_cast<CThingAppearanceSeedOverlay*>(
        entityOverlay->m_pVTable->GetThing(&entity));

    if ((pThing != nullptr) && ((pThing->m_Flags20 & 0x08) != 0))
    {
        ETCInterfaceType interfaceType = static_cast<ETCInterfaceType>(3);
        CKeyPair<ETCInterfaceType, CTCBase*>* pEntry =
            pThing->m_InterfaceMap44.LowerBound(&interfaceType);

        if ((pEntry == pThing->m_InterfaceMap44.m_pEnd) ||
            (static_cast<ETCInterfaceType>(3) < pEntry->m_Key))
        {
            pEntry = pThing->m_InterfaceMap44.m_pEnd;
        }

        AddToInterface(pEntry->m_Value, appearance_seed);
    }
}