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

struct CEntityRegionFollowOverlay
{
    std::byte m_Pad00[0x38];
    std::uint32_t m_Flags38;          // 0x38
    std::byte m_Pad3C[0x44 - 0x3C];
    TCInterfaceMap m_InterfaceMap44;  // 0x44
    std::byte m_Pad4C[0x91 - 0x4C];
    std::uint8_t m_Byte91;            // 0x91
};

static_assert(offsetof(CEntityRegionFollowOverlay, m_Flags38) == 0x38);
static_assert(offsetof(CEntityRegionFollowOverlay, m_InterfaceMap44) == 0x44);
static_assert(offsetof(CEntityRegionFollowOverlay, m_Byte91) == 0x91);

struct CTCBaseRegionFollowOverlay
{
    std::byte m_Pad00[0x1C];
    bool m_AbleToRegionFollowWhenMarried; // 0x1C
};

static_assert(offsetof(CTCBaseRegionFollowOverlay, m_AbleToRegionFollowWhenMarried) == 0x1C);
}

void __thiscall CGameScriptInterface::EntitySetAsAbleToRegionFollowWhenMarried(
    const CScriptThing& entity,
    bool able_to_region_follow_when_married) const
{
    const auto* const entityOverlay = reinterpret_cast<const CScriptThingOverlay*>(&entity);
    auto* const pThing = static_cast<CEntityRegionFollowOverlay*>(
        entityOverlay->m_pVTable->GetThing(&entity));

    if ((pThing != nullptr) &&
        ((pThing->m_Byte91 & 0x01) == 0) &&
        ((pThing->m_Flags38 & 0x00800000) != 0))
    {
        ETCInterfaceType interfaceType = static_cast<ETCInterfaceType>(0xD7);
        CKeyPair<ETCInterfaceType, CTCBase*>* pEntry =
            pThing->m_InterfaceMap44.LowerBound(&interfaceType);

        if ((pEntry == pThing->m_InterfaceMap44.m_pEnd) ||
            (static_cast<ETCInterfaceType>(0xD7) < pEntry->m_Key))
        {
            pEntry = pThing->m_InterfaceMap44.m_pEnd;
        }

        reinterpret_cast<CTCBaseRegionFollowOverlay*>(pEntry->m_Value)
            ->m_AbleToRegionFollowWhenMarried = able_to_region_follow_when_married;
    }
}