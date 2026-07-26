#include <cstddef>
#include <cstdint>

class CScriptThing;
class CGameScriptInterface;
class CTCBase;

class CTCTrapBase
{
public:
    void __thiscall ManualTrigger();
};

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

static_assert(
    offsetof(CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>, m_pEnd) ==
    0x04);

namespace
{
struct ManuallyTriggerTrap_TargetOverlay
{
    std::byte m_Pad00[0x2C];
    std::uint32_t m_Flags2C; // 0x2C
    std::byte m_Pad30[0x14];
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>> m_InterfaceMap44; // 0x44
};

static_assert(offsetof(ManuallyTriggerTrap_TargetOverlay, m_Flags2C) == 0x2C);
static_assert(offsetof(ManuallyTriggerTrap_TargetOverlay, m_InterfaceMap44) == 0x44);
}

bool __thiscall CGameScriptInterface::ManuallyTriggerTrap(const CScriptThing&) const
{
    using RawSlot2CFn = void* (__thiscall*)(const CGameScriptInterface*);

    auto* const vtable = *reinterpret_cast<void* const* const*>(this);
    auto* const pTarget = reinterpret_cast<ManuallyTriggerTrap_TargetOverlay*>(
        reinterpret_cast<RawSlot2CFn>(vtable[0x2C / sizeof(void*)])(this));

    if ((pTarget != nullptr) && ((pTarget->m_Flags2C & 0x20000000U) != 0))
    {
        const ETCInterfaceType interfaceType = static_cast<ETCInterfaceType>(0x7D);
        CKeyPair<ETCInterfaceType, CTCBase*>* pEntry =
            pTarget->m_InterfaceMap44.LowerBound(&interfaceType);

        if ((pEntry == pTarget->m_InterfaceMap44.m_pEnd) ||
            (static_cast<std::int32_t>(interfaceType) < static_cast<std::int32_t>(pEntry->m_Key)))
        {
            pEntry = pTarget->m_InterfaceMap44.m_pEnd;
        }

        static_cast<CTCTrapBase*>(pEntry->m_Value)->ManualTrigger();
        return true;
    }

    return false;
}