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
    long __thiscall GetHeroRenownLevel() const;
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CGameScriptInterface_GetHeroRenownLevel_SelfOverlay
{
    std::byte m_Pad00[0x14];
    void* m_Field14; // 0x14
    std::byte m_Pad18[0x70 - 0x18];
    long m_RenownLevel; // 0x70
};
static_assert(
    offsetof(
        CGameScriptInterface_GetHeroRenownLevel_SelfOverlay,
        m_Field14) == 0x14);
static_assert(
    offsetof(
        CGameScriptInterface_GetHeroRenownLevel_SelfOverlay,
        m_RenownLevel) == 0x70);

struct CGameScriptInterface_GetHeroRenownLevel_TCInterfaceEntryOverlay
{
    std::int32_t m_Key;    // 0x00
    CTCBase* m_pInterface; // 0x04
};
static_assert(
    offsetof(
        CGameScriptInterface_GetHeroRenownLevel_TCInterfaceEntryOverlay,
        m_Key) == 0x00);
static_assert(
    offsetof(
        CGameScriptInterface_GetHeroRenownLevel_TCInterfaceEntryOverlay,
        m_pInterface) == 0x04);
static_assert(
    sizeof(CGameScriptInterface_GetHeroRenownLevel_TCInterfaceEntryOverlay) == 0x08);

struct CGameScriptInterface_GetHeroRenownLevel_TargetOverlay
{
    std::byte m_Pad00[0x20];
    std::uint8_t m_Flags20; // 0x20
    std::byte m_Pad21[0x23];
    TCInterfaceMap m_TCInterfaceMap; // 0x44
    CGameScriptInterface_GetHeroRenownLevel_TCInterfaceEntryOverlay*
        m_pTCInterfaceMapEnd; // 0x48
    std::byte m_Pad4C[0x45];
    std::uint8_t m_Flags91; // 0x91
};
static_assert(
    offsetof(
        CGameScriptInterface_GetHeroRenownLevel_TargetOverlay,
        m_Flags20) == 0x20);
static_assert(
    offsetof(
        CGameScriptInterface_GetHeroRenownLevel_TargetOverlay,
        m_TCInterfaceMap) == 0x44);
static_assert(
    offsetof(
        CGameScriptInterface_GetHeroRenownLevel_TargetOverlay,
        m_pTCInterfaceMapEnd) == 0x48);
static_assert(
    offsetof(
        CGameScriptInterface_GetHeroRenownLevel_TargetOverlay,
        m_Flags91) == 0x91);

struct CGameScriptInterface_GetHeroRenownLevel_InterfaceOverlay
{
    std::byte m_Pad00[0x70];
    long m_RenownLevel; // 0x70
};
static_assert(
    offsetof(
        CGameScriptInterface_GetHeroRenownLevel_InterfaceOverlay,
        m_RenownLevel) == 0x70);

using Fn_00449970 = void* (__thiscall*)(void*);
using Fn_00487DC0 =
    CGameScriptInterface_GetHeroRenownLevel_TargetOverlay* (__thiscall*)(void*);

inline constexpr Fn_00449970 CALL_00449970 =
    reinterpret_cast<Fn_00449970>(0x00449970);
inline constexpr Fn_00487DC0 CALL_00487DC0 =
    reinterpret_cast<Fn_00487DC0>(0x00487DC0);
} // namespace

long __thiscall CGameScriptInterface::GetHeroRenownLevel() const
{
    const auto* const self =
        reinterpret_cast<
            const CGameScriptInterface_GetHeroRenownLevel_SelfOverlay*>(this);
    auto* const target =
        CALL_00487DC0(CALL_00449970(self->m_Field14));

    if ((target != nullptr) && ((target->m_Flags91 & 0x01) == 0))
    {
        if ((target->m_Flags20 & 0x10) != 0)
        {
            std::int32_t interfaceTypeValue = 0x04;

            auto* pEntry =
                reinterpret_cast<
                    CGameScriptInterface_GetHeroRenownLevel_TCInterfaceEntryOverlay*>(
                    target->m_TCInterfaceMap.LowerBound(
                        reinterpret_cast<const ETCInterfaceType*>(&interfaceTypeValue)));

            if ((pEntry == target->m_pTCInterfaceMapEnd) || (0x04 < pEntry->m_Key))
            {
                pEntry = target->m_pTCInterfaceMapEnd;
            }

            return reinterpret_cast<
                const CGameScriptInterface_GetHeroRenownLevel_InterfaceOverlay*>(
                pEntry->m_pInterface)
                ->m_RenownLevel;
        }

        return self->m_RenownLevel;
    }

    return -1;
}