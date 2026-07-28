#include <cstddef>
#include <cstdint>

namespace
{
    struct CThingPhysicalSwitchDef_Overlay
    {
        void** vftable;
        std::int32_t refCount;
        std::byte pad_0008[0x11C - 0x08];
        std::int32_t field_11C;
    };
    static_assert(offsetof(CThingPhysicalSwitchDef_Overlay, refCount) == 0x04);
    static_assert(offsetof(CThingPhysicalSwitchDef_Overlay, field_11C) == 0x11C);

    struct CThingAICreatureInit_Overlay
    {
        std::int32_t field_00;
        std::int32_t field_04;
        CCharString field_08;
        CCharString field_18;
        std::uint8_t field_28;
        std::uint8_t field_29;
        std::uint8_t field_2A;
        std::uint8_t field_2B;
        std::int32_t field_2C;
        std::int32_t field_30;
    };
    static_assert(offsetof(CThingAICreatureInit_Overlay, field_08) == 0x08);
    static_assert(offsetof(CThingAICreatureInit_Overlay, field_18) == 0x18);
    static_assert(offsetof(CThingAICreatureInit_Overlay, field_28) == 0x28);
    static_assert(offsetof(CThingAICreatureInit_Overlay, field_2C) == 0x2C);
    static_assert(offsetof(CThingAICreatureInit_Overlay, field_30) == 0x30);
    static_assert(sizeof(CThingAICreatureInit_Overlay) == 0x34);

    struct CScriptThingLocal_Overlay
    {
        void** vftable;
        std::int32_t field_04;
        std::int32_t* shared;
    };
    static_assert(offsetof(CScriptThingLocal_Overlay, field_04) == 0x04);
    static_assert(offsetof(CScriptThingLocal_Overlay, shared) == 0x08);
    static_assert(sizeof(CScriptThingLocal_Overlay) == 0x0C);

    struct CScriptThingTempCleanup_Overlay
    {
        void** vftable;
        std::int32_t field_04;
        std::int32_t* shared;
    };
    static_assert(offsetof(CScriptThingTempCleanup_Overlay, field_04) == 0x04);
    static_assert(offsetof(CScriptThingTempCleanup_Overlay, shared) == 0x08);
    static_assert(sizeof(CScriptThingTempCleanup_Overlay) == 0x0C);

    struct CGameScriptInterface_Overlay
    {
        void** vftable;
        std::byte pad_0004[0x10 - 0x04];
        CDefinitionManager* definitionManager;
        std::byte pad_0014[0x4C - 0x14];
        std::int32_t createCreatureCallbackCount;
    };
    static_assert(offsetof(CGameScriptInterface_Overlay, definitionManager) == 0x10);
    static_assert(offsetof(CGameScriptInterface_Overlay, createCreatureCallbackCount) == 0x4C);
}

CScriptThing* __thiscall CGameScriptInterface::CreateCreature(
    CScriptThing* result,
    const CCharString& definitionName,
    const C3DVector& position,
    const CCharString& scriptName,
    bool startHealingFollowers) const
{
    const auto* const self = reinterpret_cast<const CGameScriptInterface_Overlay*>(this);

    std::pair<EHeroMorphType, CParticleMorphs::CEntry>* extraout_EDX = nullptr;
    std::pair<EHeroMorphType, CParticleMorphs::CEntry>* extraout_EDX_00 = nullptr;
    NScript::CIsThingAlive* extraout_EDX_01 = nullptr;
    std::pair<EHeroMorphType, CParticleMorphs::CEntry>* extraout_EDX_02 = nullptr;
    std::pair<EHeroMorphType, CParticleMorphs::CEntry>* extraout_EDX_03 = nullptr;
    std::pair<EHeroMorphType, CParticleMorphs::CEntry>* extraout_EDX_04 = nullptr;
    std::pair<EHeroMorphType, CParticleMorphs::CEntry>* ppVar7 = nullptr;
    std::pair<EHeroMorphType, CParticleMorphs::CEntry>* extraout_EDX_05 = nullptr;
    std::pair<EHeroMorphType, CParticleMorphs::CEntry>* unaff_EDI = nullptr;

    const long definitionIndex =
        CDefinitionManager::GetDefGlobalIndexFromName(self->definitionManager, &definitionName);

    CScriptThingLocal_Overlay localResult;
    NHeroInformationScreens::CBase::CBase(reinterpret_cast<CBase*>(&localResult));
    localResult.vftable = reinterpret_cast<void**>(&PTR__scalar_deleting_destructor__01238c8c);
    localResult.field_04 = 0;
    localResult.shared = nullptr;

    CThingPhysicalSwitchDef_Overlay* creatureDef = nullptr;
    if (CDefinitionManager::GetDef<CThingPhysicalSwitchDef>(
            self->definitionManager,
            definitionIndex,
            reinterpret_cast<CDefPointer<const CThingPhysicalSwitchDef>*>(&creatureDef)))
    {
        CCharString emptyName("", -1);
        alignas(CCharString) std::byte reusedCharStringStorage[sizeof(CCharString)];
        auto* const reusedCharString =
            reinterpret_cast<CCharString*>(reusedCharStringStorage);

        CThingAICreatureInit_Overlay creatureInit;
        alignas(CCombatAction_Guildmaster_HealFollowersInto)
            std::byte actionStorage[sizeof(CCombatAction_Guildmaster_HealFollowersInto)];
        CScriptThingTempCleanup_Overlay predicateTempCleanup;

        CCharString::CCharString(reusedCharString, "", -1);
        creatureInit.field_00 = 0;
        creatureInit.field_04 = 0;
        CCharString::CCharString(&creatureInit.field_08, reusedCharString);
        CCharString::CCharString(&creatureInit.field_18, &emptyName);
        creatureInit.field_28 = 0;
        creatureInit.field_29 = 1;
        creatureInit.field_2A = 1;
        creatureInit.field_2B = 0;
        creatureInit.field_2C = 0;
        creatureInit.field_30 = 0;

        std::_Cons_val<
            std::allocator<std::pair<EHeroMorphType, CParticleMorphs::CEntry>>,
            std::pair<EHeroMorphType, CParticleMorphs::CEntry>,
            std::pair<EHeroMorphType, CParticleMorphs::CEntry> const&>(
            reinterpret_cast<std::allocator<std::pair<EHeroMorphType, CParticleMorphs::CEntry>>*>(
                reusedCharString),
            extraout_EDX,
            unaff_EDI);
        std::_Cons_val<
            std::allocator<std::pair<EHeroMorphType, CParticleMorphs::CEntry>>,
            std::pair<EHeroMorphType, CParticleMorphs::CEntry>,
            std::pair<EHeroMorphType, CParticleMorphs::CEntry> const&>(
            reinterpret_cast<std::allocator<std::pair<EHeroMorphType, CParticleMorphs::CEntry>>*>(
                &emptyName),
            extraout_EDX_00,
            unaff_EDI);

        CCharString::operator=(&creatureInit.field_08, &scriptName);

        CThingAICreature* const creature =
            CThingAICreature::Create(
                definitionIndex,
                &position,
                creatureDef->field_11C,
                reinterpret_cast<CThingAICreatureInit*>(&creatureInit));

        CScriptThing::operator=(
            reinterpret_cast<CScriptThing*>(&localResult),
            reinterpret_cast<CScriptThing*>(
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
                    extraout_EDX_01)));

        predicateTempCleanup.vftable =
            reinterpret_cast<void**>(&PTR__scalar_deleting_destructor__01238c8c);
        if (predicateTempCleanup.shared != nullptr)
        {
            --(*predicateTempCleanup.shared);
            if (*predicateTempCleanup.shared == 0)
            {
                reinterpret_cast<void(__thiscall*)(std::int32_t*)>(
                    predicateTempCleanup.shared[1])(predicateTempCleanup.shared);
                operator_delete(predicateTempCleanup.shared);
            }
        }
        predicateTempCleanup.field_04 = 0;
        predicateTempCleanup.shared = nullptr;
        NHeroInformationScreens::CBase::CBase(
            reinterpret_cast<CBase*>(&predicateTempCleanup));

        ppVar7 = extraout_EDX_02;
        if (startHealingFollowers)
        {
            CCreatureActionBase* const action =
                reinterpret_cast<CCreatureActionBase*>(
                    CCombatAction_Guildmaster_HealFollowersInto::
                        CCombatAction_Guildmaster_HealFollowersInto(
                            reinterpret_cast<CCombatAction_Guildmaster_HealFollowersInto*>(
                                actionStorage),
                            static_cast<CThingCreatureBase*>(creature)));
            CThingCreatureBase::SetCurrentAction(
                static_cast<CThingCreatureBase*>(creature),
                action);
            CTCLightningOrb::~CTCLightningOrb(
                reinterpret_cast<CTCLightningOrb*>(actionStorage));
            ppVar7 = extraout_EDX_03;
        }

        std::int32_t remainingCallbacks = self->createCreatureCallbackCount;
        if (remainingCallbacks > 0)
        {
            do
            {
                reinterpret_cast<void(__thiscall*)(const CGameScriptInterface*)>(
                    (*reinterpret_cast<void***>(
                        const_cast<CGameScriptInterface*>(this)))[0x1C / sizeof(void*)])(
                    this);
                --remainingCallbacks;
                ppVar7 = extraout_EDX_04;
            } while (remainingCallbacks != 0);
        }

        std::_Cons_val<
            std::allocator<std::pair<EHeroMorphType, CParticleMorphs::CEntry>>,
            std::pair<EHeroMorphType, CParticleMorphs::CEntry>,
            std::pair<EHeroMorphType, CParticleMorphs::CEntry> const&>(
            reinterpret_cast<std::allocator<std::pair<EHeroMorphType, CParticleMorphs::CEntry>>*>(
                &creatureInit.field_18),
            ppVar7,
            unaff_EDI);
        std::_Cons_val<
            std::allocator<std::pair<EHeroMorphType, CParticleMorphs::CEntry>>,
            std::pair<EHeroMorphType, CParticleMorphs::CEntry>,
            std::pair<EHeroMorphType, CParticleMorphs::CEntry> const&>(
            reinterpret_cast<std::allocator<std::pair<EHeroMorphType, CParticleMorphs::CEntry>>*>(
                &creatureInit.field_08),
            extraout_EDX_05,
            unaff_EDI);
    }

    auto* const out = reinterpret_cast<CScriptThingLocal_Overlay*>(result);
    out->vftable = reinterpret_cast<void**>(&PTR__scalar_deleting_destructor__01238c8c);
    out->field_04 = localResult.field_04;
    out->shared = localResult.shared;
    if (out->shared != nullptr)
    {
        ++(*out->shared);
    }

    if (creatureDef != nullptr)
    {
        std::int32_t* const defRefCount =
            reinterpret_cast<std::int32_t*>(creatureDef) + 1;
        --(*defRefCount);
        if (*defRefCount == 0)
        {
            reinterpret_cast<void(__thiscall*)(CThingPhysicalSwitchDef_Overlay*)>(
                (*reinterpret_cast<void***>(creatureDef))[1])(creatureDef);
        }
    }

    localResult.vftable = reinterpret_cast<void**>(&PTR__scalar_deleting_destructor__01238c8c);
    if (localResult.shared != nullptr)
    {
        --(*localResult.shared);
        if (*localResult.shared == 0)
        {
            reinterpret_cast<void(__thiscall*)(std::int32_t*)>(
                localResult.shared[1])(localResult.shared);
            operator_delete(localResult.shared);
        }
    }
    localResult.field_04 = 0;
    localResult.shared = nullptr;
    NHeroInformationScreens::CBase::CBase(reinterpret_cast<CBase*>(&localResult));

    return result;
}