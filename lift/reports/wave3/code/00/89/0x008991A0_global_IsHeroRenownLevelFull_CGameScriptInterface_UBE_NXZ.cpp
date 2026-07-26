#include <cstddef>
#include <cstdint>

enum ETCInterfaceType : std::int32_t
{
    ETCInterfaceType_HeroStats = 4,
};

class CGameScriptInterface;
class CTCBase;
class CTCHeroStats;

template <typename TKey, typename TValue>
class CKeyPairCompareLess;

template <typename TKey, typename TValue, typename TCompare>
class CVectorMap
{
public:
    static void* __thiscall LowerBound(CVectorMap* pThis, const TKey* pKey);
};

class CTCHeroStats
{
public:
    float __thiscall GetPercentageThroughRenownLevel();
};

class CGameScriptInterface
{
public:
    bool __thiscall IsHeroRenownLevelFull() const;
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CGameScriptInterface_IsHeroRenownLevelFull_SelfOverlay
{
    std::byte m_Pad00[0x14];
    void* m_Field14; // 0x14
};
static_assert(
    offsetof(CGameScriptInterface_IsHeroRenownLevelFull_SelfOverlay, m_Field14) == 0x14);

struct CGameScriptInterface_IsHeroRenownLevelFull_TCInterfaceEntryOverlay
{
    std::int32_t m_Key;    // 0x00
    CTCBase* m_pInterface; // 0x04
};
static_assert(
    offsetof(
        CGameScriptInterface_IsHeroRenownLevelFull_TCInterfaceEntryOverlay,
        m_Key) == 0x00);
static_assert(
    offsetof(
        CGameScriptInterface_IsHeroRenownLevelFull_TCInterfaceEntryOverlay,
        m_pInterface) == 0x04);

struct CGameScriptInterface_IsHeroRenownLevelFull_TargetOverlay
{
    std::byte m_Pad00[0x20];
    std::uint8_t m_Flags20; // 0x20
    std::byte m_Pad21[0x23];
    std::byte m_TCInterfaceMapStorage[0x04]; // 0x44
    CGameScriptInterface_IsHeroRenownLevelFull_TCInterfaceEntryOverlay* m_pTCInterfaceMapEnd; // 0x48
    std::byte m_Pad4C[0x45];
    std::uint8_t m_Flags91; // 0x91
};
static_assert(
    offsetof(CGameScriptInterface_IsHeroRenownLevelFull_TargetOverlay, m_Flags20) == 0x20);
static_assert(
    offsetof(
        CGameScriptInterface_IsHeroRenownLevelFull_TargetOverlay,
        m_TCInterfaceMapStorage) == 0x44);
static_assert(
    offsetof(
        CGameScriptInterface_IsHeroRenownLevelFull_TargetOverlay,
        m_pTCInterfaceMapEnd) == 0x48);
static_assert(
    offsetof(CGameScriptInterface_IsHeroRenownLevelFull_TargetOverlay, m_Flags91) == 0x91);

using CGameScriptInterface_IsHeroRenownLevelFull_Helper00449970Fn =
    void* (__thiscall*)(void*);
using CGameScriptInterface_IsHeroRenownLevelFull_Helper00487DC0Fn =
    CGameScriptInterface_IsHeroRenownLevelFull_TargetOverlay* (__thiscall*)(void*);

inline constexpr auto CGameScriptInterface_IsHeroRenownLevelFull_Helper00449970 =
    reinterpret_cast<CGameScriptInterface_IsHeroRenownLevelFull_Helper00449970Fn>(0x00449970);
inline constexpr auto CGameScriptInterface_IsHeroRenownLevelFull_Helper00487DC0 =
    reinterpret_cast<CGameScriptInterface_IsHeroRenownLevelFull_Helper00487DC0Fn>(0x00487DC0);

inline constexpr auto DAT_0122DED8 = reinterpret_cast<const float*>(0x0122DED8);
inline constexpr auto DAT_0129BA3C = reinterpret_cast<const float*>(0x0129BA3C);
}

bool __thiscall CGameScriptInterface::IsHeroRenownLevelFull() const
{
    auto* const savedThis = this;
    const auto* const self =
        reinterpret_cast<const CGameScriptInterface_IsHeroRenownLevelFull_SelfOverlay*>(this);
    auto* const target =
        CGameScriptInterface_IsHeroRenownLevelFull_Helper00487DC0(
            CGameScriptInterface_IsHeroRenownLevelFull_Helper00449970(self->m_Field14));

    if ((target != nullptr) && ((target->m_Flags91 & 0x01) == 0))
    {
        CTCHeroStats* heroStats;

        if ((target->m_Flags20 & 0x10) != 0)
        {
            ETCInterfaceType interfaceType = ETCInterfaceType_HeroStats;

            auto* pEntry =
                reinterpret_cast<CGameScriptInterface_IsHeroRenownLevelFull_TCInterfaceEntryOverlay*>(
                    TCInterfaceMap::LowerBound(
                        reinterpret_cast<TCInterfaceMap*>(target->m_TCInterfaceMapStorage),
                        &interfaceType));

            if ((pEntry == target->m_pTCInterfaceMapEnd) || (ETCInterfaceType_HeroStats < pEntry->m_Key))
            {
                pEntry = target->m_pTCInterfaceMapEnd;
            }

            heroStats = reinterpret_cast<CTCHeroStats*>(pEntry->m_pInterface);
        }
        else
        {
            heroStats =
                reinterpret_cast<CTCHeroStats*>(const_cast<CGameScriptInterface*>(savedThis));
        }

        return heroStats->GetPercentageThroughRenownLevel() > (*DAT_0122DED8 - *DAT_0129BA3C);
    }

    return false;
}