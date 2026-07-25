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

class CGameScriptInterface
{
public:
    void __thiscall SetTrapAsActive(const CScriptThing& thing, bool active) const;
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct SetTrapAsActive_TargetOverlay
{
    std::byte m_Pad00[0x2C];
    std::uint32_t m_Flags2C; // 0x2C
    std::byte m_Pad30[0x14];
    TCInterfaceMap m_InterfaceMap44; // 0x44
};

static_assert(offsetof(SetTrapAsActive_TargetOverlay, m_Flags2C) == 0x2C);
static_assert(offsetof(SetTrapAsActive_TargetOverlay, m_InterfaceMap44) == 0x44);

using ResolveTargetFn = void* (__thiscall*)(const CGameScriptInterface*);
using TrapSetActiveFn = void(__thiscall*)(CTCBase* pThis);
using TrapSetInactiveFn = void(__thiscall*)(CTCBase* pThis);

inline constexpr TrapSetActiveFn TrapSetActive_00787C07 =
    reinterpret_cast<TrapSetActiveFn>(0x00787C07);
inline constexpr TrapSetInactiveFn TrapSetInactive_00787BFE =
    reinterpret_cast<TrapSetInactiveFn>(0x00787BFE);
} // namespace

void __thiscall CGameScriptInterface::SetTrapAsActive(const CScriptThing& thing, bool active) const
{
    (void)thing;

    auto* const vtable = *reinterpret_cast<void* const* const*>(this);
    auto* const pTarget = reinterpret_cast<SetTrapAsActive_TargetOverlay*>(
        reinterpret_cast<ResolveTargetFn>(vtable[0x2C / sizeof(void*)])(this));

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

        if (active)
        {
            TrapSetActive_00787C07(pEntry->m_Value);
            return;
        }

        TrapSetInactive_00787BFE(pEntry->m_Value);
    }
}