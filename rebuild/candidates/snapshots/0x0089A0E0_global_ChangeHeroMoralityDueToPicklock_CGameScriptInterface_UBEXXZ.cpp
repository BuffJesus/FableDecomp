#include <cstddef>
#include <cstdint>

class CDefinitionManager;
class CGameScriptInterface;
class CTCBase;
class CTCHeroStats;

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
    void __thiscall SetMorality(int morality);
};

extern "C" CDefinitionManager* GFGetPreMainMemoryUsed();

namespace
{
using TCInterfaceMap =
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>>;

struct CGameScriptInterface_ChangeHeroMoralityDueToPicklock_SelfOverlay
{
    std::byte m_Pad00[0x14];
    void* m_Field14; // 0x14
};
static_assert(offsetof(CGameScriptInterface_ChangeHeroMoralityDueToPicklock_SelfOverlay, m_Field14) == 0x14);

struct CGameScriptInterface_ChangeHeroMoralityDueToPicklock_TCInterfaceEntryOverlay
{
    std::int32_t m_Key;     // 0x00
    CTCBase* m_pInterface;  // 0x04
};
static_assert(offsetof(CGameScriptInterface_ChangeHeroMoralityDueToPicklock_TCInterfaceEntryOverlay, m_pInterface) == 0x04);

struct CGameScriptInterface_ChangeHeroMoralityDueToPicklock_TargetOverlay
{
    std::byte m_Pad00[0x20];
    std::uint8_t m_Flags20; // 0x20
    std::byte m_Pad21[0x23];
    std::byte m_TCInterfaceMapStorage[0x04]; // 0x44
    CGameScriptInterface_ChangeHeroMoralityDueToPicklock_TCInterfaceEntryOverlay* m_pTCInterfaceMapEnd; // 0x48
    std::byte m_Pad4C[0x45];
    std::uint8_t m_Flags91; // 0x91
};
static_assert(offsetof(CGameScriptInterface_ChangeHeroMoralityDueToPicklock_TargetOverlay, m_Flags20) == 0x20);
static_assert(offsetof(CGameScriptInterface_ChangeHeroMoralityDueToPicklock_TargetOverlay, m_TCInterfaceMapStorage) == 0x44);
static_assert(offsetof(CGameScriptInterface_ChangeHeroMoralityDueToPicklock_TargetOverlay, m_pTCInterfaceMapEnd) == 0x48);
static_assert(offsetof(CGameScriptInterface_ChangeHeroMoralityDueToPicklock_TargetOverlay, m_Flags91) == 0x91);

struct CGameScriptInterface_ChangeHeroMoralityDueToPicklock_HeroStatsOverlay
{
    std::byte m_Pad00[0x28];
    std::int32_t m_Field28; // 0x28
};
static_assert(offsetof(CGameScriptInterface_ChangeHeroMoralityDueToPicklock_HeroStatsOverlay, m_Field28) == 0x28);

struct CGameScriptInterface_ChangeHeroMoralityDueToPicklock_DefinitionManagerOverlay
{
    std::byte m_Pad00[0xDC];
    void* m_FieldDC; // 0xDC
};
static_assert(offsetof(CGameScriptInterface_ChangeHeroMoralityDueToPicklock_DefinitionManagerOverlay, m_FieldDC) == 0xDC);

struct CGameScriptInterface_ChangeHeroMoralityDueToPicklock_DefinitionManagerFieldDCOverlay
{
    std::byte m_Pad00[0xE8];
    std::int32_t m_FieldE8; // 0xE8
};
static_assert(offsetof(CGameScriptInterface_ChangeHeroMoralityDueToPicklock_DefinitionManagerFieldDCOverlay, m_FieldE8) == 0xE8);

using CGameScriptInterface_ChangeHeroMoralityDueToPicklock_Helper00449970Fn =
    void*(__thiscall*)(void*);
using CGameScriptInterface_ChangeHeroMoralityDueToPicklock_Helper00487DC0Fn =
    CGameScriptInterface_ChangeHeroMoralityDueToPicklock_TargetOverlay*(__thiscall*)(void*);

auto* const CGameScriptInterface_ChangeHeroMoralityDueToPicklock_Helper00449970 =
    reinterpret_cast<CGameScriptInterface_ChangeHeroMoralityDueToPicklock_Helper00449970Fn>(0x00449970);
auto* const CGameScriptInterface_ChangeHeroMoralityDueToPicklock_Helper00487DC0 =
    reinterpret_cast<CGameScriptInterface_ChangeHeroMoralityDueToPicklock_Helper00487DC0Fn>(0x00487DC0);
}

class CGameScriptInterface
{
public:
    void __thiscall ChangeHeroMoralityDueToPicklock() const;
};

void __thiscall CGameScriptInterface::ChangeHeroMoralityDueToPicklock() const
{
    auto* const self =
        reinterpret_cast<const CGameScriptInterface_ChangeHeroMoralityDueToPicklock_SelfOverlay*>(this);

    auto* const helperResult =
        CGameScriptInterface_ChangeHeroMoralityDueToPicklock_Helper00449970(self->m_Field14);
    auto* const target =
        CGameScriptInterface_ChangeHeroMoralityDueToPicklock_Helper00487DC0(helperResult);

    if ((target != nullptr) &&
        ((target->m_Flags91 & 0x01) == 0) &&
        ((target->m_Flags20 & 0x10) != 0))
    {
        std::int32_t interfaceTypeValue = 0x04;

        auto* pEntry =
            reinterpret_cast<CGameScriptInterface_ChangeHeroMoralityDueToPicklock_TCInterfaceEntryOverlay*>(
                TCInterfaceMap::LowerBound(
                    reinterpret_cast<TCInterfaceMap*>(target->m_TCInterfaceMapStorage),
                    reinterpret_cast<const ETCInterfaceType*>(&interfaceTypeValue)));

        if ((pEntry == target->m_pTCInterfaceMapEnd) || (pEntry->m_Key > 0x04))
        {
            pEntry = target->m_pTCInterfaceMapEnd;
        }

        auto* const heroStats =
            reinterpret_cast<CTCHeroStats*>(pEntry->m_pInterface);
        const auto* const heroStatsOverlay =
            reinterpret_cast<const CGameScriptInterface_ChangeHeroMoralityDueToPicklock_HeroStatsOverlay*>(
                heroStats);

        auto* const definitionManager =
            reinterpret_cast<CGameScriptInterface_ChangeHeroMoralityDueToPicklock_DefinitionManagerOverlay*>(
                GFGetPreMainMemoryUsed());
        auto* const fieldDC =
            reinterpret_cast<
                CGameScriptInterface_ChangeHeroMoralityDueToPicklock_DefinitionManagerFieldDCOverlay*>(
                definitionManager->m_FieldDC);

        heroStats->SetMorality(fieldDC->m_FieldE8 + heroStatsOverlay->m_Field28);
    }
}