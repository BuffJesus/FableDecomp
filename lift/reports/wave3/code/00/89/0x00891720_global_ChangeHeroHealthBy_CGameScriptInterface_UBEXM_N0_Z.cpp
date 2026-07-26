#include <cstddef>
#include <cstdint>

class CBaseIntelligentPointer;
class CBaseObject;
class CCreatureActionBase;
class CGameScriptInterface;
class CThingCreatureBase;

class CBaseIntelligentPointer
{
public:
    static CBaseObject* __thiscall GetPItem(CBaseIntelligentPointer* pThis);
};

class CThingCreatureBase
{
public:
    void __thiscall SetCurrentAction(CCreatureActionBase* pAction);
};

class CGameScriptInterface
{
public:
    void __thiscall ChangeHeroHealthBy(float amount, bool, bool applyHealthChange) const;
};

namespace
{
struct CGameScriptInterface_ChangeHeroHealthBy_SelfOverlay
{
    std::byte m_Pad00[0x14];
    void* m_Field14; // 0x14
};
static_assert(offsetof(CGameScriptInterface_ChangeHeroHealthBy_SelfOverlay, m_Field14) == 0x14);

struct CGameScriptInterface_ChangeHeroHealthBy_HelperResultOverlay
{
    std::byte m_Pad00[0x34];
    CBaseIntelligentPointer m_Item; // 0x34
};
static_assert(offsetof(CGameScriptInterface_ChangeHeroHealthBy_HelperResultOverlay, m_Item) == 0x34);

struct CGameScriptInterface_ChangeHeroHealthBy_TargetOverlay
{
    void* m_pVftable;              // 0x00
    std::byte m_Pad04[0x8D];
    std::uint8_t m_Flags91;        // 0x91
    std::byte m_Pad92[0x22];
    float m_HealthB4;              // 0xB4
};
static_assert(offsetof(CGameScriptInterface_ChangeHeroHealthBy_TargetOverlay, m_Flags91) == 0x91);
static_assert(offsetof(CGameScriptInterface_ChangeHeroHealthBy_TargetOverlay, m_HealthB4) == 0xB4);

struct CGameScriptInterface_ChangeHeroHealthBy_StabbedToDeathStorage
{
    std::byte m_Bytes[0xA8];
};
static_assert(sizeof(CGameScriptInterface_ChangeHeroHealthBy_StabbedToDeathStorage) == 0xA8);

using CGameScriptInterface_ChangeHeroHealthBy_Helper0040D959Fn =
    void*(__thiscall*)(void*);
using CGameScriptInterface_ChangeHeroHealthBy_RawHealthChangeSlotFn =
    void(__thiscall*)(CBaseObject*, float, bool);
using CGameScriptInterface_ChangeHeroHealthBy_StabbedToDeathCtorFn =
    CCreatureActionBase*(__thiscall*)(void*, CThingCreatureBase*, bool);
using CGameScriptInterface_ChangeHeroHealthBy_DestroyTempActionFn =
    void(__thiscall*)(void*);
using CGameScriptInterface_ChangeHeroHealthBy_GlobalNegativeAmountFn =
    void(__thiscall*)(void*, float, bool);

auto* const CGameScriptInterface_ChangeHeroHealthBy_Helper0040D959 =
    reinterpret_cast<CGameScriptInterface_ChangeHeroHealthBy_Helper0040D959Fn>(0x0040D959);
auto* const CGameScriptInterface_ChangeHeroHealthBy_StabbedToDeathCtor =
    reinterpret_cast<CGameScriptInterface_ChangeHeroHealthBy_StabbedToDeathCtorFn>(0x008ED650);
auto* const CGameScriptInterface_ChangeHeroHealthBy_DestroyTempAction =
    reinterpret_cast<CGameScriptInterface_ChangeHeroHealthBy_DestroyTempActionFn>(0x00693EF0);
auto* const CGameScriptInterface_ChangeHeroHealthBy_GlobalNegativeAmount =
    reinterpret_cast<CGameScriptInterface_ChangeHeroHealthBy_GlobalNegativeAmountFn>(0x00437940);

auto* const g_CGameScriptInterface_ChangeHeroHealthBy_DeathThreshold =
    reinterpret_cast<const float*>(0x0129BA3C);
auto* const g_CGameScriptInterface_ChangeHeroHealthBy_NegativeAmountThreshold =
    reinterpret_cast<const float*>(0x0122DEDC);
auto* const g_CGameScriptInterface_ChangeHeroHealthBy_Global =
    reinterpret_cast<void**>(0x013B8790);
}

void __thiscall CGameScriptInterface::ChangeHeroHealthBy(
    float amount,
    bool,
    bool applyHealthChange) const
{
    const auto* const self =
        reinterpret_cast<const CGameScriptInterface_ChangeHeroHealthBy_SelfOverlay*>(this);
    auto* const helperResult =
        reinterpret_cast<CGameScriptInterface_ChangeHeroHealthBy_HelperResultOverlay*>(
            CGameScriptInterface_ChangeHeroHealthBy_Helper0040D959(self->m_Field14));

    if (helperResult == nullptr)
    {
        return;
    }

    auto* const target = CBaseIntelligentPointer::GetPItem(&helperResult->m_Item);
    if (target == nullptr)
    {
        return;
    }

    auto* const targetOverlay =
        reinterpret_cast<CGameScriptInterface_ChangeHeroHealthBy_TargetOverlay*>(target);
    if ((targetOverlay->m_Flags91 & 0x01) != 0)
    {
        return;
    }

    auto* const vftable = *reinterpret_cast<void***>(target);
    reinterpret_cast<CGameScriptInterface_ChangeHeroHealthBy_RawHealthChangeSlotFn>(vftable[0x3B])(
        target,
        amount,
        applyHealthChange);

    if (applyHealthChange && (targetOverlay->m_HealthB4 < *g_CGameScriptInterface_ChangeHeroHealthBy_DeathThreshold))
    {
        CGameScriptInterface_ChangeHeroHealthBy_StabbedToDeathStorage actionStorage{};
        auto* const pAction = CGameScriptInterface_ChangeHeroHealthBy_StabbedToDeathCtor(
            &actionStorage,
            reinterpret_cast<CThingCreatureBase*>(target),
            false);
        reinterpret_cast<CThingCreatureBase*>(target)->SetCurrentAction(pAction);
        CGameScriptInterface_ChangeHeroHealthBy_DestroyTempAction(&actionStorage);
    }

    if (applyHealthChange && (amount < *g_CGameScriptInterface_ChangeHeroHealthBy_NegativeAmountThreshold))
    {
        CGameScriptInterface_ChangeHeroHealthBy_GlobalNegativeAmount(
            *g_CGameScriptInterface_ChangeHeroHealthBy_Global,
            -amount,
            true);
    }
}