#include <cstddef>
#include <cstdint>

class CGameScriptInterface;
class CTCBase;
class CTCCarrying;
class CThing;

enum ETCInterfaceType : std::int32_t;

template <typename TKey, typename TValue>
class CKeyPairCompareLess;

template <typename TKey, typename TValue, typename TCompare>
class CVectorMap
{
public:
    void* __thiscall LowerBound(const TKey* pKey);
};

class CTCCarrying
{
public:
    CThing* __thiscall GetThingInPrimarySlot();
};

class CGameScriptInterface
{
public:
    bool __thiscall IsPlayerWieldingWeapon() const;
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CGameScriptInterface_IsPlayerWieldingWeapon_SelfOverlay
{
    std::byte m_Pad00[0x14];
    void* m_Field14; // 0x14
};
static_assert(
    offsetof(CGameScriptInterface_IsPlayerWieldingWeapon_SelfOverlay, m_Field14) == 0x14);

struct CGameScriptInterface_IsPlayerWieldingWeapon_TCInterfaceEntryOverlay
{
    std::int32_t m_Key;    // 0x00
    CTCBase* m_pInterface; // 0x04
};
static_assert(
    offsetof(
        CGameScriptInterface_IsPlayerWieldingWeapon_TCInterfaceEntryOverlay,
        m_pInterface) == 0x04);

struct CGameScriptInterface_IsPlayerWieldingWeapon_TargetOverlay
{
    std::byte m_Pad00[0x28];
    std::uint8_t m_Flags28; // 0x28
    std::byte m_Pad29[0x1B];
    std::byte m_TCInterfaceMapStorage[0x04]; // 0x44
    CGameScriptInterface_IsPlayerWieldingWeapon_TCInterfaceEntryOverlay*
        m_pTCInterfaceMapEnd; // 0x48
    std::byte m_Pad4C[0x45];
    std::uint8_t m_Flags91; // 0x91
};
static_assert(
    offsetof(CGameScriptInterface_IsPlayerWieldingWeapon_TargetOverlay, m_Flags28) == 0x28);
static_assert(
    offsetof(
        CGameScriptInterface_IsPlayerWieldingWeapon_TargetOverlay,
        m_TCInterfaceMapStorage) == 0x44);
static_assert(
    offsetof(
        CGameScriptInterface_IsPlayerWieldingWeapon_TargetOverlay,
        m_pTCInterfaceMapEnd) == 0x48);
static_assert(
    offsetof(CGameScriptInterface_IsPlayerWieldingWeapon_TargetOverlay, m_Flags91) == 0x91);

struct CThing_IsPlayerWieldingWeapon_Overlay
{
    std::byte m_Pad00[0x28];
    std::uint32_t m_Flags28; // 0x28
};
static_assert(offsetof(CThing_IsPlayerWieldingWeapon_Overlay, m_Flags28) == 0x28);

using Fn_00449970 = void* (__thiscall*)(void*);
using Fn_00487DC0 =
    CGameScriptInterface_IsPlayerWieldingWeapon_TargetOverlay* (__thiscall*)(void*);

inline constexpr Fn_00449970 CALL_00449970 =
    reinterpret_cast<Fn_00449970>(0x00449970);
inline constexpr Fn_00487DC0 CALL_00487DC0 =
    reinterpret_cast<Fn_00487DC0>(0x00487DC0);
}

bool __thiscall CGameScriptInterface::IsPlayerWieldingWeapon() const
{
    const auto* const self =
        reinterpret_cast<const CGameScriptInterface_IsPlayerWieldingWeapon_SelfOverlay*>(this);
    auto* const target =
        CALL_00487DC0(CALL_00449970(self->m_Field14));

    if ((target == nullptr) ||
        ((target->m_Flags91 & 0x01) != 0) ||
        ((target->m_Flags28 & 0x40) == 0))
    {
        return false;
    }

    ETCInterfaceType searchKey = static_cast<ETCInterfaceType>(0x46);

    auto* pEntry =
        reinterpret_cast<CGameScriptInterface_IsPlayerWieldingWeapon_TCInterfaceEntryOverlay*>(
            reinterpret_cast<TCInterfaceMap*>(target->m_TCInterfaceMapStorage)
                ->LowerBound(&searchKey));

    if ((pEntry == target->m_pTCInterfaceMapEnd) || (0x46 < pEntry->m_Key))
    {
        pEntry = target->m_pTCInterfaceMapEnd;
    }

    CThing* const pThing =
        reinterpret_cast<CTCCarrying*>(pEntry->m_pInterface)->GetThingInPrimarySlot();

    if ((pThing != nullptr) &&
        ((reinterpret_cast<const CThing_IsPlayerWieldingWeapon_Overlay*>(pThing)->m_Flags28 &
          0x00000100) != 0))
    {
        return true;
    }

    return false;
}