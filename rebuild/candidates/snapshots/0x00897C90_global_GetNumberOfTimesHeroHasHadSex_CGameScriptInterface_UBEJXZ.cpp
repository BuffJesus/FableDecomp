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
    long __thiscall GetNumberOfTimesHeroHasHadSex() const;
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CGameScriptInterface_GetNumberOfTimesHeroHasHadSex_SelfOverlay
{
    std::byte m_Pad00[0x14];
    void* m_Field14; // 0x14
};

static_assert(
    offsetof(
        CGameScriptInterface_GetNumberOfTimesHeroHasHadSex_SelfOverlay,
        m_Field14) == 0x14);

struct CGameScriptInterface_GetNumberOfTimesHeroHasHadSex_TCInterfaceEntryOverlay
{
    std::int32_t m_Key;    // 0x00
    CTCBase* m_pInterface; // 0x04
};

static_assert(
    offsetof(
        CGameScriptInterface_GetNumberOfTimesHeroHasHadSex_TCInterfaceEntryOverlay,
        m_Key) == 0x00);
static_assert(
    offsetof(
        CGameScriptInterface_GetNumberOfTimesHeroHasHadSex_TCInterfaceEntryOverlay,
        m_pInterface) == 0x04);

struct CGameScriptInterface_GetNumberOfTimesHeroHasHadSex_TargetOverlay
{
    std::byte m_Pad00[0x20];
    std::uint8_t m_Flags20; // 0x20
    std::byte m_Pad21[0x23];
    std::byte m_TCInterfaceMapStorage[0x04]; // 0x44
    CGameScriptInterface_GetNumberOfTimesHeroHasHadSex_TCInterfaceEntryOverlay*
        m_pTCInterfaceMapEnd; // 0x48
    std::byte m_Pad4C[0x45];
    std::uint8_t m_Flags91; // 0x91
};

static_assert(
    offsetof(
        CGameScriptInterface_GetNumberOfTimesHeroHasHadSex_TargetOverlay,
        m_Flags20) == 0x20);
static_assert(
    offsetof(
        CGameScriptInterface_GetNumberOfTimesHeroHasHadSex_TargetOverlay,
        m_TCInterfaceMapStorage) == 0x44);
static_assert(
    offsetof(
        CGameScriptInterface_GetNumberOfTimesHeroHasHadSex_TargetOverlay,
        m_pTCInterfaceMapEnd) == 0x48);
static_assert(
    offsetof(
        CGameScriptInterface_GetNumberOfTimesHeroHasHadSex_TargetOverlay,
        m_Flags91) == 0x91);

struct CGameScriptInterface_GetNumberOfTimesHeroHasHadSex_HeroStatsOverlay
{
    std::byte m_Pad00[0x154];
    long m_Field154; // 0x154
};

static_assert(
    offsetof(
        CGameScriptInterface_GetNumberOfTimesHeroHasHadSex_HeroStatsOverlay,
        m_Field154) == 0x154);

using Fn_00449970 = void* (__thiscall*)(void*);
using Fn_00487DC0 =
    CGameScriptInterface_GetNumberOfTimesHeroHasHadSex_TargetOverlay* (__thiscall*)(void*);

inline constexpr Fn_00449970 CALL_00449970 =
    reinterpret_cast<Fn_00449970>(0x00449970);
inline constexpr Fn_00487DC0 CALL_00487DC0 =
    reinterpret_cast<Fn_00487DC0>(0x00487DC0);
}

long __thiscall CGameScriptInterface::GetNumberOfTimesHeroHasHadSex() const
{
    const auto* const self =
        reinterpret_cast<
            const CGameScriptInterface_GetNumberOfTimesHeroHasHadSex_SelfOverlay*>(this);
    auto* const target =
        CALL_00487DC0(CALL_00449970(self->m_Field14));

    if ((target != nullptr) &&
        ((target->m_Flags91 & 0x01) == 0) &&
        ((target->m_Flags20 & 0x10) != 0))
    {
        ETCInterfaceType searchKey = ETCInterfaceType_HeroStats;

        auto* pEntry =
            reinterpret_cast<
                CGameScriptInterface_GetNumberOfTimesHeroHasHadSex_TCInterfaceEntryOverlay*>(
                TCInterfaceMap::LowerBound(
                    reinterpret_cast<TCInterfaceMap*>(target->m_TCInterfaceMapStorage),
                    &searchKey));

        if ((pEntry == target->m_pTCInterfaceMapEnd) ||
            (ETCInterfaceType_HeroStats < pEntry->m_Key))
        {
            pEntry = target->m_pTCInterfaceMapEnd;
        }

        return reinterpret_cast<
                   CGameScriptInterface_GetNumberOfTimesHeroHasHadSex_HeroStatsOverlay*>(
                   pEntry->m_pInterface)
            ->m_Field154;
    }

    return 0;
}