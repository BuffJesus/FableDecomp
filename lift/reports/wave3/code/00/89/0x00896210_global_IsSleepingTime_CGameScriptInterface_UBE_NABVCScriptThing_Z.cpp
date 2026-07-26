#include <cstddef>
#include <cstdint>

class CScriptThing;
class CTCBase;
class CTCVillage;

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

class CTCVillage
{
public:
    bool __thiscall IsCurfewTime();
};

class CGameScriptInterface
{
public:
    bool __thiscall IsSleepingTime(const CScriptThing& thing) const;
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CGameScriptInterface_IsSleepingTime_TargetOverlay
{
    std::byte m_Pad00[0x24];
    std::uint8_t m_Flags24; // 0x24
    std::byte m_Pad25[0x1F];
    TCInterfaceMap m_InterfaceMap44; // 0x44
    std::byte m_Pad4C[0x45];
    std::uint8_t m_Flags91; // 0x91
};

static_assert(
    offsetof(CGameScriptInterface_IsSleepingTime_TargetOverlay, m_Flags24) == 0x24);
static_assert(
    offsetof(CGameScriptInterface_IsSleepingTime_TargetOverlay, m_InterfaceMap44) == 0x44);
static_assert(
    offsetof(CGameScriptInterface_IsSleepingTime_TargetOverlay, m_Flags91) == 0x91);
}

bool __thiscall CGameScriptInterface::IsSleepingTime(const CScriptThing& thing) const
{
    (void)thing;

    using GetTargetFn =
        CGameScriptInterface_IsSleepingTime_TargetOverlay* (__thiscall*)(const CGameScriptInterface*);
    const auto getTarget =
        reinterpret_cast<GetTargetFn>((*reinterpret_cast<void* const* const*>(this))[0x2C / sizeof(void*)]);

    CGameScriptInterface_IsSleepingTime_TargetOverlay* const pTarget = getTarget(this);
    if ((pTarget != nullptr) &&
        ((pTarget->m_Flags91 & 0x01) == 0) &&
        ((pTarget->m_Flags24 & 0x04) != 0))
    {
        ETCInterfaceType interfaceType = static_cast<ETCInterfaceType>(0x22);
        CKeyPair<ETCInterfaceType, CTCBase*>* pEntry =
            pTarget->m_InterfaceMap44.LowerBound(&interfaceType);

        if ((pEntry == pTarget->m_InterfaceMap44.m_pEnd) ||
            (static_cast<ETCInterfaceType>(0x22) < pEntry->m_Key))
        {
            pEntry = pTarget->m_InterfaceMap44.m_pEnd;
        }

        return reinterpret_cast<CTCVillage*>(pEntry->m_Value)->IsCurfewTime();
    }

    return false;
}