#include <cstddef>
#include <cstdint>

class CGameScriptInterface;
class CTCBase;
class CTCHeroExperience;

enum ETCInterfaceType : std::int32_t;
enum EHeroTrainableStatType : std::int32_t;

template <typename TKey, typename TValue>
class CKeyPairCompareLess;

template <typename TKey, typename TValue, typename TCompare>
class CVectorMap
{
public:
    void* __thiscall LowerBound(const TKey* pKey);
};

class CTCHeroExperience
{
public:
    long __thiscall GetTrainableStatMaxLevel(EHeroTrainableStatType statType) const;
};

class CGameScriptInterface
{
public:
    long __thiscall GetHeroStatMax(EHeroTrainableStatType statType) const;
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CGameScriptInterface_GetHeroStatMax_SelfOverlay
{
    std::byte m_Pad00[0x14];
    void* m_Field14; // 0x14
};
static_assert(
    offsetof(CGameScriptInterface_GetHeroStatMax_SelfOverlay, m_Field14) == 0x14);

struct CGameScriptInterface_GetHeroStatMax_TCInterfaceEntryOverlay
{
    std::int32_t m_Key;    // 0x00
    CTCBase* m_pInterface; // 0x04
};
static_assert(
    offsetof(CGameScriptInterface_GetHeroStatMax_TCInterfaceEntryOverlay, m_Key) == 0x00);
static_assert(
    offsetof(
        CGameScriptInterface_GetHeroStatMax_TCInterfaceEntryOverlay,
        m_pInterface) == 0x04);
static_assert(
    sizeof(CGameScriptInterface_GetHeroStatMax_TCInterfaceEntryOverlay) == 0x08);

struct CGameScriptInterface_GetHeroStatMax_TargetOverlay
{
    std::byte m_Pad00[0x2C];
    std::uint32_t m_Flags2C; // 0x2C
    std::byte m_Pad30[0x14];
    TCInterfaceMap m_TCInterfaceMap; // 0x44
    CGameScriptInterface_GetHeroStatMax_TCInterfaceEntryOverlay* m_pTCInterfaceMapEnd; // 0x48
    std::byte m_Pad4C[0x45];
    std::uint8_t m_Flags91; // 0x91
};
static_assert(
    offsetof(CGameScriptInterface_GetHeroStatMax_TargetOverlay, m_Flags2C) == 0x2C);
static_assert(
    offsetof(
        CGameScriptInterface_GetHeroStatMax_TargetOverlay,
        m_TCInterfaceMap) == 0x44);
static_assert(
    offsetof(
        CGameScriptInterface_GetHeroStatMax_TargetOverlay,
        m_pTCInterfaceMapEnd) == 0x48);
static_assert(
    offsetof(CGameScriptInterface_GetHeroStatMax_TargetOverlay, m_Flags91) == 0x91);

using Fn_00449970 = void* (__thiscall*)(void*);
using Fn_00487DC0 =
    CGameScriptInterface_GetHeroStatMax_TargetOverlay* (__thiscall*)(void*);
using GetTrainableStatMaxLevelFn =
    long (__thiscall*)(const void*, EHeroTrainableStatType);

inline constexpr Fn_00449970 CALL_00449970 =
    reinterpret_cast<Fn_00449970>(0x00449970);
inline constexpr Fn_00487DC0 CALL_00487DC0 =
    reinterpret_cast<Fn_00487DC0>(0x00487DC0);
inline constexpr GetTrainableStatMaxLevelFn CALL_GetTrainableStatMaxLevel =
    reinterpret_cast<GetTrainableStatMaxLevelFn>(0x006d6710);
} // namespace

long __thiscall CGameScriptInterface::GetHeroStatMax(
    EHeroTrainableStatType statType) const
{
    const auto* const self =
        reinterpret_cast<const CGameScriptInterface_GetHeroStatMax_SelfOverlay*>(this);
    auto* const target = CALL_00487DC0(CALL_00449970(self->m_Field14));

    if ((target != nullptr) && ((target->m_Flags91 & 0x01) == 0))
    {
        if ((target->m_Flags2C & 0x00000100) != 0)
        {
            std::int32_t interfaceTypeValue = 0x68;

            auto* pEntry =
                reinterpret_cast<
                    CGameScriptInterface_GetHeroStatMax_TCInterfaceEntryOverlay*>(
                    target->m_TCInterfaceMap.LowerBound(
                        reinterpret_cast<const ETCInterfaceType*>(&interfaceTypeValue)));

            if ((pEntry == target->m_pTCInterfaceMapEnd) || (0x68 < pEntry->m_Key))
            {
                pEntry = target->m_pTCInterfaceMapEnd;
            }

            return reinterpret_cast<const CTCHeroExperience*>(pEntry->m_pInterface)
                ->GetTrainableStatMaxLevel(statType);
        }

        const auto rawStatType =
            static_cast<std::uint32_t>(static_cast<std::int32_t>(statType));
        return CALL_GetTrainableStatMaxLevel(
            reinterpret_cast<const void*>(static_cast<std::uintptr_t>(rawStatType)),
            static_cast<EHeroTrainableStatType>(rawStatType));
    }

    return -1;
}