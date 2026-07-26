#include <cstddef>
#include <cstdint>

class CCharString;
class CGameScriptInterface;
class CTCBase;

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

static_assert(
    offsetof(
        CVectorMap<
            ETCInterfaceType,
            CTCBase*,
            CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>,
        m_pEnd) == 0x04);

class CCharString
{
public:
    CCharString& __thiscall operator=(const CCharString& rhs);
};

class CGameScriptInterface
{
public:
    virtual void __thiscall SetDeathRecoveryMarkerName(const CCharString& name) const;
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct SetDeathRecoveryMarkerName_SelfOverlay
{
    std::byte m_Pad00[0x14];
    void* m_Field14; // 0x14
};

static_assert(offsetof(SetDeathRecoveryMarkerName_SelfOverlay, m_Field14) == 0x14);

struct SetDeathRecoveryMarkerName_TargetOverlay
{
    std::byte m_Pad00[0x20];
    std::uint8_t m_Flags20; // 0x20
    std::byte m_Pad21[0x23];
    TCInterfaceMap m_InterfaceMap44; // 0x44
    std::byte m_Pad4C[0x45];
    std::uint8_t m_Flags91; // 0x91
};

static_assert(offsetof(SetDeathRecoveryMarkerName_TargetOverlay, m_Flags20) == 0x20);
static_assert(offsetof(SetDeathRecoveryMarkerName_TargetOverlay, m_InterfaceMap44) == 0x44);
static_assert(offsetof(SetDeathRecoveryMarkerName_TargetOverlay, m_Flags91) == 0x91);

struct SetDeathRecoveryMarkerName_InterfaceOverlay
{
    std::byte m_Pad00[0x170];
    CCharString m_DeathRecoveryMarkerName; // 0x170
};

static_assert(
    offsetof(
        SetDeathRecoveryMarkerName_InterfaceOverlay,
        m_DeathRecoveryMarkerName) == 0x170);

using Fn_00449970 = void* (__thiscall*)(void*);
using Fn_00487DD0 = SetDeathRecoveryMarkerName_TargetOverlay* (__thiscall*)(void*);

inline constexpr Fn_00449970 CALL_00449970 =
    reinterpret_cast<Fn_00449970>(0x00449970);
inline constexpr Fn_00487DD0 CALL_00487DD0 =
    reinterpret_cast<Fn_00487DD0>(0x00487DD0);
}

void __thiscall CGameScriptInterface::SetDeathRecoveryMarkerName(
    const CCharString& name) const
{
    const auto* const self =
        reinterpret_cast<const SetDeathRecoveryMarkerName_SelfOverlay*>(this);
    auto* const pTarget =
        CALL_00487DD0(CALL_00449970(self->m_Field14));

    if ((pTarget != nullptr) &&
        ((pTarget->m_Flags91 & 0x01) == 0) &&
        ((pTarget->m_Flags20 & 0x10) != 0))
    {
        const ETCInterfaceType searchKey = static_cast<ETCInterfaceType>(4);
        CKeyPair<ETCInterfaceType, CTCBase*>* pEntry =
            pTarget->m_InterfaceMap44.LowerBound(&searchKey);

        if ((pEntry == pTarget->m_InterfaceMap44.m_pEnd) ||
            (static_cast<std::int32_t>(pEntry->m_Key) > 4))
        {
            pEntry = pTarget->m_InterfaceMap44.m_pEnd;
        }

        reinterpret_cast<SetDeathRecoveryMarkerName_InterfaceOverlay*>(pEntry->m_Value)
            ->m_DeathRecoveryMarkerName = name;
    }
}