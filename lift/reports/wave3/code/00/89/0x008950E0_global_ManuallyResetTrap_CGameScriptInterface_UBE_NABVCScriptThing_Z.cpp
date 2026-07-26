#include <cstddef>
#include <cstdint>

class CScriptThing;
class CGameScriptInterface;
class CTCBase;

class CTCTrapBase
{
public:
    void ManualReset();
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

template <typename TKey, typename TValue, typename TCompare>
class CVectorMap
{
public:
    CKeyPair<TKey, TValue>* m_pBegin; // 0x00
    CKeyPair<TKey, TValue>* m_pEnd;   // 0x04

    CKeyPair<TKey, TValue>* LowerBound(const TKey* pKey);
};

static_assert(
    offsetof(CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>, m_pEnd) ==
    0x04);

namespace
{
struct CGameScriptInterface_ManuallyResetTrap_TargetOverlay
{
    std::byte m_Pad00[0x2C];
    std::uint32_t m_Flags2C; // 0x2C
    std::byte m_Pad30[0x14];
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>> m_Interfaces; // 0x44
};

static_assert(offsetof(CGameScriptInterface_ManuallyResetTrap_TargetOverlay, m_Flags2C) == 0x2C);
static_assert(offsetof(CGameScriptInterface_ManuallyResetTrap_TargetOverlay, m_Interfaces) == 0x44);
}

bool __thiscall CGameScriptInterface::ManuallyResetTrap(const CScriptThing&) const
{
    using ResolveTargetFn = void* (__thiscall*)(const CGameScriptInterface*);

    auto* const vtable = *reinterpret_cast<void* const* const*>(this);
    auto* const pTarget = reinterpret_cast<CGameScriptInterface_ManuallyResetTrap_TargetOverlay*>(
        reinterpret_cast<ResolveTargetFn>(vtable[0x2C / sizeof(void*)])(this));

    if ((pTarget != nullptr) && ((pTarget->m_Flags2C & 0x20000000U) != 0))
    {
        const ETCInterfaceType interfaceType = static_cast<ETCInterfaceType>(0x7D);
        auto* pEntry = pTarget->m_Interfaces.LowerBound(&interfaceType);
        if ((pEntry == pTarget->m_Interfaces.m_pEnd) ||
            (static_cast<std::int32_t>(interfaceType) < static_cast<std::int32_t>(pEntry->m_Key)))
        {
            pEntry = pTarget->m_Interfaces.m_pEnd;
        }

        static_cast<CTCTrapBase*>(pEntry->m_Value)->ManualReset();
        return true;
    }

    return false;
}