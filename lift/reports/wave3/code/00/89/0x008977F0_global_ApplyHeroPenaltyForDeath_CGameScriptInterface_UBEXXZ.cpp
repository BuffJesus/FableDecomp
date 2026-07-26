#include <cstddef>
#include <cstdint>

class CGameScriptInterface;
class CTCBase;
class CTCHeroStats;
class CTCHeroExperience;

enum ETCInterfaceType : int;

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
    void __thiscall ApplyPenaltyForDeath();
};

class CTCHeroExperience
{
public:
    void __thiscall ApplyPenaltyForDeath();
};

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CGameScriptInterface_ApplyHeroPenaltyForDeath_SelfOverlay
{
    std::byte m_Pad00[0x14];
    void* m_Field14; // 0x14
};
static_assert(offsetof(CGameScriptInterface_ApplyHeroPenaltyForDeath_SelfOverlay, m_Field14) == 0x14);

struct CGameScriptInterface_ApplyHeroPenaltyForDeath_TCInterfaceEntryOverlay
{
    std::int32_t m_Key;      // 0x00
    CTCBase* m_pInterface;   // 0x04
};
static_assert(offsetof(CGameScriptInterface_ApplyHeroPenaltyForDeath_TCInterfaceEntryOverlay, m_pInterface) == 0x04);

struct CGameScriptInterface_ApplyHeroPenaltyForDeath_TargetOverlay
{
    std::byte m_Pad00[0x20];
    std::uint8_t m_Flags20; // 0x20
    std::byte m_Pad21[0x0B];
    std::uint32_t m_Flags2C; // 0x2C
    std::byte m_Pad30[0x14];
    std::byte m_TCInterfaceMapStorage[0x04]; // 0x44
    CGameScriptInterface_ApplyHeroPenaltyForDeath_TCInterfaceEntryOverlay* m_pTCInterfaceMapEnd; // 0x48
    std::byte m_Pad4C[0x45];
    std::uint8_t m_Flags91; // 0x91
};
static_assert(offsetof(CGameScriptInterface_ApplyHeroPenaltyForDeath_TargetOverlay, m_Flags20) == 0x20);
static_assert(offsetof(CGameScriptInterface_ApplyHeroPenaltyForDeath_TargetOverlay, m_Flags2C) == 0x2C);
static_assert(offsetof(CGameScriptInterface_ApplyHeroPenaltyForDeath_TargetOverlay, m_TCInterfaceMapStorage) == 0x44);
static_assert(offsetof(CGameScriptInterface_ApplyHeroPenaltyForDeath_TargetOverlay, m_pTCInterfaceMapEnd) == 0x48);
static_assert(offsetof(CGameScriptInterface_ApplyHeroPenaltyForDeath_TargetOverlay, m_Flags91) == 0x91);

using CGameScriptInterface_ApplyHeroPenaltyForDeath_Helper00449970Fn =
    void*(__thiscall*)(void*);
using CGameScriptInterface_ApplyHeroPenaltyForDeath_Helper00487DC0Fn =
    CGameScriptInterface_ApplyHeroPenaltyForDeath_TargetOverlay*(__thiscall*)(void*);

auto* const CGameScriptInterface_ApplyHeroPenaltyForDeath_Helper00449970 =
    reinterpret_cast<CGameScriptInterface_ApplyHeroPenaltyForDeath_Helper00449970Fn>(0x00449970);
auto* const CGameScriptInterface_ApplyHeroPenaltyForDeath_Helper00487DC0 =
    reinterpret_cast<CGameScriptInterface_ApplyHeroPenaltyForDeath_Helper00487DC0Fn>(0x00487DC0);
}

void __thiscall CGameScriptInterface::ApplyHeroPenaltyForDeath() const
{
    auto* const self =
        reinterpret_cast<const CGameScriptInterface_ApplyHeroPenaltyForDeath_SelfOverlay*>(this);

    auto* const helperResult =
        CGameScriptInterface_ApplyHeroPenaltyForDeath_Helper00449970(self->m_Field14);
    auto* const target =
        CGameScriptInterface_ApplyHeroPenaltyForDeath_Helper00487DC0(helperResult);

    if ((target != nullptr) && ((target->m_Flags91 & 0x01) == 0))
    {
        if ((target->m_Flags20 & 0x10) != 0)
        {
            std::int32_t interfaceTypeValue = 0x04;

            auto* pEntry =
                reinterpret_cast<CGameScriptInterface_ApplyHeroPenaltyForDeath_TCInterfaceEntryOverlay*>(
                    TCInterfaceMap::LowerBound(
                        reinterpret_cast<TCInterfaceMap*>(target->m_TCInterfaceMapStorage),
                        reinterpret_cast<const ETCInterfaceType*>(&interfaceTypeValue)));

            if ((pEntry == target->m_pTCInterfaceMapEnd) || (pEntry->m_Key > 0x04))
            {
                pEntry = target->m_pTCInterfaceMapEnd;
            }

            reinterpret_cast<CTCHeroStats*>(pEntry->m_pInterface)->ApplyPenaltyForDeath();
        }

        if ((target->m_Flags2C & 0x00000100) != 0)
        {
            std::int32_t interfaceTypeValue = 0x68;

            auto* pEntry =
                reinterpret_cast<CGameScriptInterface_ApplyHeroPenaltyForDeath_TCInterfaceEntryOverlay*>(
                    TCInterfaceMap::LowerBound(
                        reinterpret_cast<TCInterfaceMap*>(target->m_TCInterfaceMapStorage),
                        reinterpret_cast<const ETCInterfaceType*>(&interfaceTypeValue)));

            if ((pEntry == target->m_pTCInterfaceMapEnd) || (pEntry->m_Key > 0x68))
            {
                pEntry = target->m_pTCInterfaceMapEnd;
            }

            reinterpret_cast<CTCHeroExperience*>(pEntry->m_pInterface)->ApplyPenaltyForDeath();
        }
    }
}