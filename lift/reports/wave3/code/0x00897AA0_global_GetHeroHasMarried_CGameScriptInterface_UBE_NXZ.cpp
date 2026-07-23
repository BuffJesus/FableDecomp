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
    static void* __thiscall LowerBound(
        CVectorMap* pThis,
        const TKey* pKey);
};

class CGameScriptInterface
{
public:
    bool __thiscall GetHeroHasMarried() const;
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CGameScriptInterface_GetHeroHasMarried_SelfOverlay
{
    std::byte m_Pad00[0x14];
    void* m_Field14; // 0x14
};
static_assert(
    offsetof(CGameScriptInterface_GetHeroHasMarried_SelfOverlay, m_Field14) == 0x14);

struct CGameScriptInterface_GetHeroHasMarried_TCInterfaceEntryOverlay
{
    std::int32_t m_Key;    // 0x00
    CTCBase* m_pInterface; // 0x04
};
static_assert(
    offsetof(
        CGameScriptInterface_GetHeroHasMarried_TCInterfaceEntryOverlay,
        m_pInterface) == 0x04);

struct CGameScriptInterface_GetHeroHasMarried_TargetOverlay
{
    std::byte m_Pad00[0x20];
    std::uint8_t m_Flags20; // 0x20
    std::byte m_Pad21[0x23];
    std::byte m_TCInterfaceMapStorage[0x04]; // 0x44
    CGameScriptInterface_GetHeroHasMarried_TCInterfaceEntryOverlay* m_pTCInterfaceMapEnd; // 0x48
    std::byte m_Pad4C[0x45];
    std::uint8_t m_Flags91; // 0x91
};
static_assert(
    offsetof(CGameScriptInterface_GetHeroHasMarried_TargetOverlay, m_Flags20) == 0x20);
static_assert(
    offsetof(
        CGameScriptInterface_GetHeroHasMarried_TargetOverlay,
        m_TCInterfaceMapStorage) == 0x44);
static_assert(
    offsetof(
        CGameScriptInterface_GetHeroHasMarried_TargetOverlay,
        m_pTCInterfaceMapEnd) == 0x48);
static_assert(
    offsetof(CGameScriptInterface_GetHeroHasMarried_TargetOverlay, m_Flags91) == 0x91);

struct CGameScriptInterface_GetHeroHasMarried_MarriageStateOverlay
{
    CGameScriptInterface_GetHeroHasMarried_MarriageStateOverlay* m_Field00; // 0x00
};
static_assert(
    offsetof(CGameScriptInterface_GetHeroHasMarried_MarriageStateOverlay, m_Field00) == 0x00);

struct CGameScriptInterface_GetHeroHasMarried_HeroStatsOverlay
{
    std::byte m_Pad00[0x1D8];
    CGameScriptInterface_GetHeroHasMarried_MarriageStateOverlay* m_pMarriageState; // 0x1D8
};
static_assert(
    offsetof(
        CGameScriptInterface_GetHeroHasMarried_HeroStatsOverlay,
        m_pMarriageState) == 0x1D8);

using Fn_00449970 = void* (__thiscall*)(void*);
using Fn_00487DB0 =
    CGameScriptInterface_GetHeroHasMarried_TargetOverlay* (__thiscall*)(void*);

inline constexpr Fn_00449970 CALL_00449970 =
    reinterpret_cast<Fn_00449970>(0x00449970);
inline constexpr Fn_00487DB0 CALL_00487DB0 =
    reinterpret_cast<Fn_00487DB0>(0x00487DB0);
}

bool __thiscall CGameScriptInterface::GetHeroHasMarried() const
{
    const auto* const self =
        reinterpret_cast<const CGameScriptInterface_GetHeroHasMarried_SelfOverlay*>(this);
    auto* const target =
        CALL_00487DB0(CALL_00449970(self->m_Field14));

    if ((target != nullptr) &&
        ((target->m_Flags91 & 0x01) == 0) &&
        ((target->m_Flags20 & 0x10) != 0))
    {
        ETCInterfaceType searchKey = static_cast<ETCInterfaceType>(4);

        auto* pEntry =
            reinterpret_cast<CGameScriptInterface_GetHeroHasMarried_TCInterfaceEntryOverlay*>(
                TCInterfaceMap::LowerBound(
                    reinterpret_cast<TCInterfaceMap*>(target->m_TCInterfaceMapStorage),
                    &searchKey));

        if ((pEntry == target->m_pTCInterfaceMapEnd) || (pEntry->m_Key > 4))
        {
            pEntry = target->m_pTCInterfaceMapEnd;
        }

        auto* const pMarriageState =
            reinterpret_cast<CGameScriptInterface_GetHeroHasMarried_HeroStatsOverlay*>(
                pEntry->m_pInterface)
                ->m_pMarriageState;

        return pMarriageState->m_Field00 != pMarriageState;
    }

    return false;
}