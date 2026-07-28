#include <cstddef>
#include <cstdint>
#include <memory>
#include <type_traits>
#include <utility>

struct CGameScriptInterface_Overlay {
    std::byte pad_00[0x10];
    CDefinitionManager* definitionManager;
    std::byte pad_14[0x4C - 0x14];
    int postCreateCallbackCount;
};
static_assert(offsetof(CGameScriptInterface_Overlay, definitionManager) == 0x10);
static_assert(offsetof(CGameScriptInterface_Overlay, postCreateCallbackCount) == 0x4C);

struct CThingPhysicalSwitchDef_Overlay {
    void** vftable;
    int refCount;
    std::byte pad_08[0x11C - 0x08];
    int createArg;
};
static_assert(offsetof(CThingPhysicalSwitchDef_Overlay, createArg) == 0x11C);

struct CScriptThing_Local {
    void** vftable;
    std::uint32_t field_04;
    int* refObject;
};
static_assert(offsetof(CScriptThing_Local, field_04) == 0x04);
static_assert(offsetof(CScriptThing_Local, refObject) == 0x08);

struct CThingAICreatureInit_Overlay {
    std::uint32_t local_130;
    std::uint32_t local_12c;
    CCharString local_128;
    CCharString local_124;
    std::uint8_t local_120;
    std::uint8_t local_11f;
    std::uint8_t local_11e;
    std::uint8_t local_11d;
    std::uint32_t local_11c;
    std::uint32_t local_118;
};
static_assert(offsetof(CThingAICreatureInit_Overlay, local_130) == 0x00);
static_assert(offsetof(CThingAICreatureInit_Overlay, local_12c) == 0x04);
static_assert(offsetof(CThingAICreatureInit_Overlay, local_128) == 0x08);
static_assert(offsetof(CThingAICreatureInit_Overlay, local_124) == 0x0C);
static_assert(offsetof(CThingAICreatureInit_Overlay, local_120) == 0x10);
static_assert(offsetof(CThingAICreatureInit_Overlay, local_11c) == 0x14);
static_assert(offsetof(CThingAICreatureInit_Overlay, local_118) == 0x18);

using MorphPair = std::pair<EHeroMorphType, CParticleMorphs::CEntry>;
using MorphAllocator = std::allocator<MorphPair>;
using CCharStringSlot = std::aligned_storage_t<sizeof(CCharString), alignof(CCharString)>;

union Local150Slot {
    CThingPhysicalSwitchDef_Overlay* def;
    CCharStringSlot stringSlot;
    MorphAllocator allocatorSlot;
};

struct CCombatAction_Guildmaster_HealFollowersInto_StackOverlay {
    std::byte storage[0x114];
};
static_assert(sizeof(CCombatAction_Guildmaster_HealFollowersInto_StackOverlay) == 0x114);

CScriptThing CGameScriptInterface::CreateCreature(
    const CCharString& definitionName,
    const C3DVector& position,
    const CCharString& scriptName,
    bool setHealFollowersIntoAction) const
{
    const auto* const self = reinterpret_cast<const CGameScriptInterface_Overlay*>(this);

    MorphPair* extraout_EDX = nullptr;
    MorphPair* extraout_EDX_00 = nullptr;
    NScript::CIsThingAlive* extraout_EDX_01 = nullptr;
    MorphPair* extraout_EDX_02 = nullptr;
    MorphPair* extraout_EDX_03 = nullptr;
    MorphPair* extraout_EDX_04 = nullptr;
    MorphPair* extraout_EDX_05 = nullptr;
    MorphPair* unaff_EDI = nullptr;

    const long defIndex =
        CDefinitionManager::GetDefGlobalIndexFromName(self->definitionManager, &definitionName);

    Local150Slot local_150{};
    CScriptThing_Local local_14c;
    NHeroInformationScreens::CBase::CBase(reinterpret_cast<NHeroInformationScreens::CBase*>(&local_14c));
    local_14c.vftable = &PTR__scalar_deleting_destructor__01238c8c;
    local_14c.field_04 = 0;
    local_14c.refObject = nullptr;

    if (CDefinitionManager::GetDef<CThingPhysicalSwitchDef>(
            self->definitionManager,
            defIndex,
            reinterpret_cast<CDefPointer<CThingPhysicalSwitchDef const>*>(&local_150.def))) {
        CCharStringSlot local_140;
        CScriptThing_Local local_13c;
        std::uint32_t local_130 = 0;
        std::uint32_t local_12c = 0;
        CCharStringSlot local_128;
        CCharStringSlot local_124;
        std::uint8_t local_120 = 0;
        std::uint8_t local_11f = 1;
        std::uint8_t local_11e = 1;
        std::uint8_t local_11d = 0;
        std::uint32_t local_11c = 0;
        std::uint32_t local_118 = 0;
        CCombatAction_Guildmaster_HealFollowersInto_StackOverlay aCStack_114;

        CCharString::CCharString(reinterpret_cast<CCharString*>(&local_140), "", -1);
        CCharString::CCharString(reinterpret_cast<CCharString*>(&local_150.stringSlot), "", -1);
        CCharString::CCharString(
            reinterpret_cast<CCharString*>(&local_128),
            reinterpret_cast<CCharString*>(&local_150.stringSlot));
        CCharString::CCharString(
            reinterpret_cast<CCharString*>(&local_124),
            reinterpret_cast<CCharString*>(&local_140));

        std::_Cons_val<MorphAllocator, MorphPair, MorphPair const&>(
            reinterpret_cast<MorphAllocator*>(&local_150.allocatorSlot),
            extraout_EDX,
            *unaff_EDI);
        std::_Cons_val<MorphAllocator, MorphPair, MorphPair const&>(
            reinterpret_cast<MorphAllocator*>(&local_140),
            extraout_EDX_00,
            *unaff_EDI);

        CCharString::operator=(
            reinterpret_cast<CCharString*>(&local_128),
            &scriptName);

        auto* const creature = CThingAICreature::Create(
            defIndex,
            &position,
            local_150.def->createArg,
            reinterpret_cast<CThingAICreatureInit*>(&local_130));

        auto* const predicateResult = reinterpret_cast<CScriptThing*>(
            NScript::GFPredicateAnd<
                NScript::CPredicate_And<
                    NScript::CPredicate_And<
                        NScript::CPredicate_And<NScript::CIsThingAlive, NScript::CIsThingAlive>,
                        NScript::CIsThingAlive>,
                    NScript::CIsThingAlive>,
                NScript::CIsThingAlive>(
                reinterpret_cast<
                    NScript::CPredicate_And<
                        NScript::CPredicate_And<
                            NScript::CPredicate_And<NScript::CIsThingAlive, NScript::CIsThingAlive>,
                            NScript::CIsThingAlive>,
                        NScript::CIsThingAlive>*>(creature),
                extraout_EDX_01));

        CScriptThing::operator=(reinterpret_cast<CScriptThing*>(&local_14c), predicateResult);

        local_13c.vftable = &PTR__scalar_deleting_destructor__01238c8c;
        if (local_13c.refObject != nullptr) {
            *local_13c.refObject = *local_13c.refObject - 1;
            if (*local_13c.refObject == 0) {
                reinterpret_cast<void(__thiscall*)(int*)>(local_13c.refObject[1])(local_13c.refObject);
                operator delete(local_13c.refObject);
            }
        }
        local_13c.field_04 = 0;
        local_13c.refObject = nullptr;
        NHeroInformationScreens::CBase::CBase(reinterpret_cast<NHeroInformationScreens::CBase*>(&local_13c));

        MorphPair* ppVar7 = extraout_EDX_02;
        if (setHealFollowersIntoAction) {
            auto* const action = reinterpret_cast<CCreatureActionBase*>(
                CCombatAction_Guildmaster_HealFollowersInto::CCombatAction_Guildmaster_HealFollowersInto(
                    reinterpret_cast<CCombatAction_Guildmaster_HealFollowersInto*>(&aCStack_114),
                    reinterpret_cast<CThingCreatureBase*>(creature)));
            CThingCreatureBase::SetCurrentAction(
                reinterpret_cast<CThingCreatureBase*>(creature),
                action);
            CTCLightningOrb::~CTCLightningOrb(reinterpret_cast<CTCLightningOrb*>(&aCStack_114));
            ppVar7 = extraout_EDX_03;
        }

        int iVar8 = self->postCreateCallbackCount;
        if (0 < iVar8) {
            do {
                reinterpret_cast<void(__thiscall*)(const CGameScriptInterface*)>(
                    (*reinterpret_cast<void***>(const_cast<CGameScriptInterface*>(this)))[0x1C / sizeof(void*)])(this);
                iVar8 = iVar8 - 1;
                ppVar7 = extraout_EDX_04;
            } while (iVar8 != 0);
        }

        std::_Cons_val<MorphAllocator, MorphPair, MorphPair const&>(
            reinterpret_cast<MorphAllocator*>(&local_124),
            ppVar7,
            *unaff_EDI);
        std::_Cons_val<MorphAllocator, MorphPair, MorphPair const&>(
            reinterpret_cast<MorphAllocator*>(&local_128),
            extraout_EDX_05,
            *unaff_EDI);
    }

    CScriptThing returnValue;
    auto* const out = reinterpret_cast<CScriptThing_Local*>(&returnValue);
    out->vftable = &PTR__scalar_deleting_destructor__01238c8c;
    out->field_04 = local_14c.field_04;
    out->refObject = local_14c.refObject;
    if (local_14c.refObject != nullptr) {
        *local_14c.refObject = *local_14c.refObject + 1;
    }

    if (local_150.def != nullptr) {
        int* const refCount = reinterpret_cast<int*>(reinterpret_cast<std::byte*>(local_150.def) + 0x04);
        *refCount = *refCount - 1;
        if (*refCount == 0) {
            reinterpret_cast<void(__thiscall*)(CThingPhysicalSwitchDef_Overlay*)>(local_150.def->vftable[1])(local_150.def);
        }
    }

    local_14c.vftable = &PTR__scalar_deleting_destructor__01238c8c;
    if (local_14c.refObject != nullptr) {
        *local_14c.refObject = *local_14c.refObject - 1;
        if (*local_14c.refObject == 0) {
            reinterpret_cast<void(__thiscall*)(int*)>(local_14c.refObject[1])(local_14c.refObject);
            operator delete(local_14c.refObject);
        }
    }
    local_14c.field_04 = 0;
    local_14c.refObject = nullptr;
    NHeroInformationScreens::CBase::CBase(reinterpret_cast<NHeroInformationScreens::CBase*>(&local_14c));

    return returnValue;
}