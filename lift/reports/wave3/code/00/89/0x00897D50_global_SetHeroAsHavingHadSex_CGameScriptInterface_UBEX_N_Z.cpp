#include <cstddef>
#include <cstdint>

class CGameScriptInterface;
class CTCBase;

enum ETCInterfaceType : std::int32_t
{
    ETCInterfaceType_HeroStats = 4,
};

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
    void __thiscall SetHeroAsHavingHadSex(bool hasHadSex) const;
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CGameScriptInterface_SetHeroAsHavingHadSex_SelfOverlay
{
    std::byte m_Pad00[0x14];
    void* m_Field14; // 0x14
};

static_assert(
    offsetof(
        CGameScriptInterface_SetHeroAsHavingHadSex_SelfOverlay,
        m_Field14) == 0x14);

struct CGameScriptInterface_SetHeroAsHavingHadSex_TCInterfaceEntryOverlay
{
    std::int32_t m_Key;    // 0x00
    CTCBase* m_pInterface; // 0x04
};

static_assert(
    offsetof(
        CGameScriptInterface_SetHeroAsHavingHadSex_TCInterfaceEntryOverlay,
        m_Key) == 0x00);
static_assert(
    offsetof(
        CGameScriptInterface_SetHeroAsHavingHadSex_TCInterfaceEntryOverlay,
        m_pInterface) == 0x04);

struct CGameScriptInterface_SetHeroAsHavingHadSex_TargetOverlay
{
    std::byte m_Pad00[0x20];
    std::uint8_t m_Flags20; // 0x20
    std::byte m_Pad21[0x23];
    std::byte m_TCInterfaceMapStorage[0x04]; // 0x44
    CGameScriptInterface_SetHeroAsHavingHadSex_TCInterfaceEntryOverlay*
        m_pTCInterfaceMapEnd; // 0x48
    std::byte m_Pad4C[0x45];
    std::uint8_t m_Flags91; // 0x91
};

static_assert(
    offsetof(
        CGameScriptInterface_SetHeroAsHavingHadSex_TargetOverlay,
        m_Flags20) == 0x20);
static_assert(
    offsetof(
        CGameScriptInterface_SetHeroAsHavingHadSex_TargetOverlay,
        m_TCInterfaceMapStorage) == 0x44);
static_assert(
    offsetof(
        CGameScriptInterface_SetHeroAsHavingHadSex_TargetOverlay,
        m_pTCInterfaceMapEnd) == 0x48);
static_assert(
    offsetof(
        CGameScriptInterface_SetHeroAsHavingHadSex_TargetOverlay,
        m_Flags91) == 0x91);

struct CGameScriptInterface_SetHeroAsHavingHadSex_HeroStatsOverlay
{
    std::byte m_Pad00[0x158];
    std::uint8_t m_Field158; // 0x158
};

static_assert(
    offsetof(
        CGameScriptInterface_SetHeroAsHavingHadSex_HeroStatsOverlay,
        m_Field158) == 0x158);

using Fn_00449970 = void* (__thiscall*)(void*);
using Fn_00487DC0 =
    CGameScriptInterface_SetHeroAsHavingHadSex_TargetOverlay* (__thiscall*)(void*);

inline constexpr Fn_00449970 CALL_00449970 =
    reinterpret_cast<Fn_00449970>(0x00449970);
inline constexpr Fn_00487DC0 CALL_00487DC0 =
    reinterpret_cast<Fn_00487DC0>(0x00487DC0);
}

void __thiscall CGameScriptInterface::SetHeroAsHavingHadSex(bool hasHadSex) const
{
    const auto* const self =
        reinterpret_cast<
            const CGameScriptInterface_SetHeroAsHavingHadSex_SelfOverlay*>(this);
    auto* const target =
        CALL_00487DC0(CALL_00449970(self->m_Field14));

    if ((target != nullptr) &&
        ((target->m_Flags91 & 0x01) == 0) &&
        ((target->m_Flags20 & 0x10) != 0))
    {
        ETCInterfaceType interfaceType = ETCInterfaceType_HeroStats;

        auto* pEntry =
            reinterpret_cast<
                CGameScriptInterface_SetHeroAsHavingHadSex_TCInterfaceEntryOverlay*>(
                TCInterfaceMap::LowerBound(
                    reinterpret_cast<TCInterfaceMap*>(target->m_TCInterfaceMapStorage),
                    &interfaceType));

        if ((pEntry == target->m_pTCInterfaceMapEnd) ||
            (ETCInterfaceType_HeroStats < pEntry->m_Key))
        {
            pEntry = target->m_pTCInterfaceMapEnd;
        }

        reinterpret_cast<
            CGameScriptInterface_SetHeroAsHavingHadSex_HeroStatsOverlay*>(
            pEntry->m_pInterface)
            ->m_Field158 = static_cast<std::uint8_t>(hasHadSex);
    }
}