#include <cstddef>
#include <cstdint>

class CGameScriptInterface;
class CTCBase;

enum ETCInterfaceType : std::int32_t;

template <typename TKey, typename TValue>
class CKeyPairCompareLess;

template <typename TKey, typename TValue, typename TCompare>
class CVectorMap
{
public:
    void* __thiscall LowerBound(const TKey* pKey);
};

class CGameScriptInterface
{
public:
    void __thiscall ResetPlayerCreatureCombatMultiplier() const;
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CGameScriptInterface_ResetPlayerCreatureCombatMultiplier_SelfOverlay
{
    std::byte m_Pad00[0x14];
    void* m_Field14; // 0x14
};
static_assert(
    offsetof(
        CGameScriptInterface_ResetPlayerCreatureCombatMultiplier_SelfOverlay,
        m_Field14) == 0x14);

struct CGameScriptInterface_ResetPlayerCreatureCombatMultiplier_TCInterfaceEntryOverlay
{
    std::int32_t m_Key;    // 0x00
    CTCBase* m_pInterface; // 0x04
};
static_assert(
    offsetof(
        CGameScriptInterface_ResetPlayerCreatureCombatMultiplier_TCInterfaceEntryOverlay,
        m_Key) == 0x00);
static_assert(
    offsetof(
        CGameScriptInterface_ResetPlayerCreatureCombatMultiplier_TCInterfaceEntryOverlay,
        m_pInterface) == 0x04);

struct CGameScriptInterface_ResetPlayerCreatureCombatMultiplier_TargetOverlay
{
    std::byte m_Pad00[0x2C];
    std::uint32_t m_Flags2C; // 0x2C
    std::byte m_Pad30[0x14];
    std::byte m_TCInterfaceMapStorage[0x04]; // 0x44
    CGameScriptInterface_ResetPlayerCreatureCombatMultiplier_TCInterfaceEntryOverlay*
        m_pTCInterfaceMapEnd; // 0x48
    std::byte m_Pad4C[0x45];
    std::uint8_t m_Flags91; // 0x91
};
static_assert(
    offsetof(
        CGameScriptInterface_ResetPlayerCreatureCombatMultiplier_TargetOverlay,
        m_Flags2C) == 0x2C);
static_assert(
    offsetof(
        CGameScriptInterface_ResetPlayerCreatureCombatMultiplier_TargetOverlay,
        m_TCInterfaceMapStorage) == 0x44);
static_assert(
    offsetof(
        CGameScriptInterface_ResetPlayerCreatureCombatMultiplier_TargetOverlay,
        m_pTCInterfaceMapEnd) == 0x48);
static_assert(
    offsetof(
        CGameScriptInterface_ResetPlayerCreatureCombatMultiplier_TargetOverlay,
        m_Flags91) == 0x91);

using CGameScriptInterface_ResetPlayerCreatureCombatMultiplier_Helper00449970Fn =
    void* (__thiscall*)(void*);
using CGameScriptInterface_ResetPlayerCreatureCombatMultiplier_Helper00487DC0Fn =
    CGameScriptInterface_ResetPlayerCreatureCombatMultiplier_TargetOverlay* (__thiscall*)(void*);
using CGameScriptInterface_ResetPlayerCreatureCombatMultiplier_Call006D8768Fn =
    void (__thiscall*)(CTCBase*);

inline constexpr auto CGameScriptInterface_ResetPlayerCreatureCombatMultiplier_Helper00449970 =
    reinterpret_cast<CGameScriptInterface_ResetPlayerCreatureCombatMultiplier_Helper00449970Fn>(
        0x00449970);
inline constexpr auto CGameScriptInterface_ResetPlayerCreatureCombatMultiplier_Helper00487DC0 =
    reinterpret_cast<CGameScriptInterface_ResetPlayerCreatureCombatMultiplier_Helper00487DC0Fn>(
        0x00487DC0);
inline constexpr auto CGameScriptInterface_ResetPlayerCreatureCombatMultiplier_Call006D8768 =
    reinterpret_cast<CGameScriptInterface_ResetPlayerCreatureCombatMultiplier_Call006D8768Fn>(
        0x006D8768);
}

void __thiscall CGameScriptInterface::ResetPlayerCreatureCombatMultiplier() const
{
    const auto* const self =
        reinterpret_cast<
            const CGameScriptInterface_ResetPlayerCreatureCombatMultiplier_SelfOverlay*>(this);
    auto* const target =
        CGameScriptInterface_ResetPlayerCreatureCombatMultiplier_Helper00487DC0(
            CGameScriptInterface_ResetPlayerCreatureCombatMultiplier_Helper00449970(
                self->m_Field14));

    if ((target != nullptr) &&
        ((target->m_Flags91 & 0x01) == 0) &&
        ((target->m_Flags2C & 0x00000100) != 0))
    {
        std::int32_t interfaceTypeValue = 0x68;

        auto* pEntry =
            reinterpret_cast<
                CGameScriptInterface_ResetPlayerCreatureCombatMultiplier_TCInterfaceEntryOverlay*>(
                reinterpret_cast<TCInterfaceMap*>(target->m_TCInterfaceMapStorage)
                    ->LowerBound(
                        reinterpret_cast<const ETCInterfaceType*>(&interfaceTypeValue)));

        if ((pEntry == target->m_pTCInterfaceMapEnd) || (0x68 < pEntry->m_Key))
        {
            pEntry = target->m_pTCInterfaceMapEnd;
        }

        CGameScriptInterface_ResetPlayerCreatureCombatMultiplier_Call006D8768(
            pEntry->m_pInterface);
    }
}