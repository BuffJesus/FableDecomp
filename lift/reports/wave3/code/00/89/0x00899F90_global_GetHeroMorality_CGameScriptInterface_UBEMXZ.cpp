#include <cstddef>
#include <cstdint>

class CGameScriptInterface;
class CTCBase;
class CTCHeroStats;

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

class CTCHeroStats
{
public:
    float __thiscall GetMoralityPercentage();
};

class CGameScriptInterface
{
public:
    float __thiscall GetHeroMorality() const;
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CGameScriptInterface_GetHeroMorality_SelfOverlay
{
    std::byte m_Pad00[0x14];
    void* m_Field14; // 0x14
};
static_assert(
    offsetof(CGameScriptInterface_GetHeroMorality_SelfOverlay, m_Field14) == 0x14);

struct CGameScriptInterface_GetHeroMorality_TCInterfaceEntryOverlay
{
    std::int32_t m_Key;    // 0x00
    CTCBase* m_pInterface; // 0x04
};
static_assert(
    offsetof(
        CGameScriptInterface_GetHeroMorality_TCInterfaceEntryOverlay,
        m_pInterface) == 0x04);

struct CGameScriptInterface_GetHeroMorality_TargetOverlay
{
    std::byte m_Pad00[0x20];
    std::uint8_t m_Flags20; // 0x20
    std::byte m_Pad21[0x23];
    std::byte m_TCInterfaceMapStorage[0x04]; // 0x44
    CGameScriptInterface_GetHeroMorality_TCInterfaceEntryOverlay* m_pTCInterfaceMapEnd; // 0x48
    std::byte m_Pad4C[0x45];
    std::uint8_t m_Flags91; // 0x91
};
static_assert(
    offsetof(CGameScriptInterface_GetHeroMorality_TargetOverlay, m_Flags20) == 0x20);
static_assert(
    offsetof(
        CGameScriptInterface_GetHeroMorality_TargetOverlay,
        m_TCInterfaceMapStorage) == 0x44);
static_assert(
    offsetof(
        CGameScriptInterface_GetHeroMorality_TargetOverlay,
        m_pTCInterfaceMapEnd) == 0x48);
static_assert(
    offsetof(CGameScriptInterface_GetHeroMorality_TargetOverlay, m_Flags91) == 0x91);

using CGameScriptInterface_GetHeroMorality_Helper00449970Fn =
    void* (__thiscall*)(void*);
using CGameScriptInterface_GetHeroMorality_Helper00487DC0Fn =
    CGameScriptInterface_GetHeroMorality_TargetOverlay* (__thiscall*)(void*);

inline constexpr auto CGameScriptInterface_GetHeroMorality_Helper00449970 =
    reinterpret_cast<CGameScriptInterface_GetHeroMorality_Helper00449970Fn>(0x00449970);
inline constexpr auto CGameScriptInterface_GetHeroMorality_Helper00487DC0 =
    reinterpret_cast<CGameScriptInterface_GetHeroMorality_Helper00487DC0Fn>(0x00487DC0);

inline constexpr auto DAT_0122F59C =
    reinterpret_cast<const float*>(0x0122F59C);
}

float __thiscall CGameScriptInterface::GetHeroMorality() const
{
    const auto* const self =
        reinterpret_cast<const CGameScriptInterface_GetHeroMorality_SelfOverlay*>(this);
    auto* const target =
        CGameScriptInterface_GetHeroMorality_Helper00487DC0(
            CGameScriptInterface_GetHeroMorality_Helper00449970(self->m_Field14));

    if ((target != nullptr) && ((target->m_Flags91 & 0x01) == 0))
    {
        if ((target->m_Flags20 & 0x10) != 0)
        {
            ETCInterfaceType searchKey = static_cast<ETCInterfaceType>(4);

            auto* pEntry =
                reinterpret_cast<CGameScriptInterface_GetHeroMorality_TCInterfaceEntryOverlay*>(
                    TCInterfaceMap::LowerBound(
                        reinterpret_cast<TCInterfaceMap*>(target->m_TCInterfaceMapStorage),
                        &searchKey));

            if ((pEntry == target->m_pTCInterfaceMapEnd) || (pEntry->m_Key > 4))
            {
                pEntry = target->m_pTCInterfaceMapEnd;
            }

            return reinterpret_cast<CTCHeroStats*>(pEntry->m_pInterface)->GetMoralityPercentage();
        }

        return reinterpret_cast<CTCHeroStats*>(
                   const_cast<CGameScriptInterface*>(this))
            ->GetMoralityPercentage();
    }

    return *DAT_0122F59C;
}