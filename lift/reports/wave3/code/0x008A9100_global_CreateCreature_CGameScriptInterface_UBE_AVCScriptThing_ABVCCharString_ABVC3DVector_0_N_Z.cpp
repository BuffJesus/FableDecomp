extern void* PTR__scalar_deleting_destructor__01238c8c;

struct CGameScriptInterface_008A9100_Overlay
{
    void** vftable;
    std::byte pad_0004[0x10 - 0x04];
    CDefinitionManager* definitionManager; // 0x10
    std::byte pad_0014[0x4C - 0x14];
    int createCreatureCallbackCount; // 0x4C
};

static_assert(offsetof(CGameScriptInterface_008A9100_Overlay, definitionManager) == 0x10);
static_assert(offsetof(CGameScriptInterface_008A9100_Overlay, createCreatureCallbackCount) == 0x4C);

struct CThingPhysicalSwitchDef_008A9100_Overlay
{
    void** vftable;          // 0x00
    int refCount;            // 0x04
    std::byte pad_0008[0x11C - 0x08];
    int field_11C;           // 0x11C
};

static_assert(offsetof(CThingPhysicalSwitchDef_008A9100_Overlay, refCount) == 0x04);
static_assert(offsetof(CThingPhysicalSwitchDef_008A9100_Overlay, field_11C) == 0x11C);

struct CDefPointer_008A9100_Overlay
{
    CThingPhysicalSwitchDef_008A9100_Overlay* ptr;
};

static_assert(offsetof(CDefPointer_008A9100_Overlay, ptr) == 0x00);

struct CScriptThing_008A9100_Overlay
{
    void* vftable;           // 0x00
    std::uint32_t field_04;  // 0x04
    int* sharedState;        // 0x08
};

static_assert(offsetof(CScriptThing_008A9100_Overlay, vftable) == 0x00);
static_assert(offsetof(CScriptThing_008A9100_Overlay, field_04) == 0x04);
static_assert(offsetof(CScriptThing_008A9100_Overlay, sharedState) == 0x08);

struct CThingAICreatureInit_008A9100_Overlay
{
    std::uint32_t field_00;  // 0x00
    std::uint32_t field_04;  // 0x04
    CCharString field_08;    // 0x08
    CCharString field_0C;    // 0x0C
    bool field_10;           // 0x10
    bool field_11;           // 0x11
    bool field_12;           // 0x12
    bool field_13;           // 0x13
    std::uint32_t field_14;  // 0x14
    std::uint32_t field_18;  // 0x18
    std::vector<std::pair<EHeroMorphType, CParticleMorphs::CEntry>> field_1C; // 0x1C
    std::vector<std::pair<EHeroMorphType, CParticleMorphs::CEntry>> field_28; // 0x28
};

static_assert(offsetof(CThingAICreatureInit_008A9100_Overlay, field_00) == 0x00);
static_assert(offsetof(CThingAICreatureInit_008A9100_Overlay, field_04) == 0x04);
static_assert(offsetof(CThingAICreatureInit_008A9100_Overlay, field_08) == 0x08);
static_assert(offsetof(CThingAICreatureInit_008A9100_Overlay, field_0C) == 0x0C);
static_assert(offsetof(CThingAICreatureInit_008A9100_Overlay, field_10) == 0x10);
static_assert(offsetof(CThingAICreatureInit_008A9100_Overlay, field_11) == 0x11);
static_assert(offsetof(CThingAICreatureInit_008A9100_Overlay, field_12) == 0x12);
static_assert(offsetof(CThingAICreatureInit_008A9100_Overlay, field_13) == 0x13);
static_assert(offsetof(CThingAICreatureInit_008A9100_Overlay, field_14) == 0x14);
static_assert(offsetof(CThingAICreatureInit_008A9100_Overlay, field_18) == 0x18);
static_assert(offsetof(CThingAICreatureInit_008A9100_Overlay, field_1C) == 0x1C);
static_assert(offsetof(CThingAICreatureInit_008A9100_Overlay, field_28) == 0x28);

static void ReleaseScriptThingSharedState_008A9100(CScriptThing_008A9100_Overlay& thing)
{
    if (thing.sharedState != nullptr)
    {
        *thing.sharedState = *thing.sharedState - 1;
        if (*thing.sharedState == 0)
        {
            reinterpret_cast<void (*)()>(thing.sharedState[1])();
            operator delete(thing.sharedState);
        }
    }
}

CScriptThing* __thiscall CGameScriptInterface::CreateCreature(
    CScriptThing* outResult,
    const CCharString& definitionName,
    const C3DVector& position,
    const CCharString& scriptName,
    bool startHealFollowersIntoAction) const
{
    const auto* const self = reinterpret_cast<const CGameScriptInterface_008A9100_Overlay*>(this);
    const long defGlobalIndex =
        CDefinitionManager::GetDefGlobalIndexFromName(self->definitionManager, &definitionName);

    alignas(CDefPointer<CThingPhysicalSwitchDef const>) std::byte switchDefStorage[sizeof(CDefPointer<CThingPhysicalSwitchDef const>)];
    auto& switchDef = *reinterpret_cast<CDefPointer_008A9100_Overlay*>(switchDefStorage);

    alignas(CScriptThing) std::byte localResultStorage[sizeof(CScriptThing)];
    auto& localResult = *reinterpret_cast<CScriptThing_008A9100_Overlay*>(localResultStorage);

    NHeroInformationScreens::CBase::CBase(reinterpret_cast<NHeroInformationScreens::CBase*>(&localResult));
    localResult.vftable = &PTR__scalar_deleting_destructor__01238c8c;
    localResult.field_04 = 0;
    localResult.sharedState = nullptr;

    switchDef.ptr = nullptr;

    if (CDefinitionManager::GetDef<CThingPhysicalSwitchDef>(
            self->definitionManager,
            defGlobalIndex,
            reinterpret_cast<CDefPointer<CThingPhysicalSwitchDef const>*>(&switchDef)))
    {
        CCharString local_140("", -1);
        CCharString local_150("", -1);

        alignas(CThingAICreatureInit_008A9100_Overlay) std::byte initStorage[sizeof(CThingAICreatureInit_008A9100_Overlay)];
        auto& init = *reinterpret_cast<CThingAICreatureInit_008A9100_Overlay*>(initStorage);

        init.field_00 = 0;
        init.field_04 = 0;
        std::construct_at(&init.field_08, local_150);
        std::construct_at(&init.field_0C, local_140);
        init.field_10 = false;
        init.field_11 = true;
        init.field_12 = true;
        init.field_13 = false;
        init.field_14 = 0;
        init.field_18 = 0;
        std::construct_at(&init.field_1C);
        std::construct_at(&init.field_28);
        CCharString::operator=(&init.field_08, &scriptName);

        CThingAICreature* const creature = CThingAICreature::Create(
            defGlobalIndex,
            &position,
            switchDef.ptr->field_11C,
            reinterpret_cast<CThingAICreatureInit*>(&init));

        alignas(CScriptThing) std::byte predicateStorage[sizeof(CScriptThing)];
        auto& predicateThing = *reinterpret_cast<CScriptThing_008A9100_Overlay*>(predicateStorage);

        CScriptThing* const predicateResult =
            NScript::GFPredicateAnd<
                NScript::CPredicate_And<
                    NScript::CPredicate_And<
                        NScript::CPredicate_And<
                            NScript::CIsThingAlive,
                            NScript::CIsThingAlive>,
                        NScript::CIsThingAlive>,
                    NScript::CIsThingAlive>,
                NScript::CIsThingAlive>(
                    reinterpret_cast<decltype(reinterpret_cast<
                        NScript::CPredicate_And<
                            NScript::CPredicate_And<
                                NScript::CPredicate_And<
                                    NScript::CIsThingAlive,
                                    NScript::CIsThingAlive>,
                                NScript::CIsThingAlive>,
                            NScript::CIsThingAlive>*>(nullptr))>(creature));

        CScriptThing::operator=(
            reinterpret_cast<CScriptThing*>(&localResult),
            predicateResult);

        predicateThing.vftable = &PTR__scalar_deleting_destructor__01238c8c;
        ReleaseScriptThingSharedState_008A9100(predicateThing);
        predicateThing.field_04 = 0;
        predicateThing.sharedState = nullptr;
        NHeroInformationScreens::CBase::CBase(reinterpret_cast<NHeroInformationScreens::CBase*>(&predicateThing));

        if (startHealFollowersIntoAction)
        {
            alignas(CCombatAction_Guildmaster_HealFollowersInto) std::byte actionStorage[sizeof(CCombatAction_Guildmaster_HealFollowersInto)];
            auto* const action =
                CCombatAction_Guildmaster_HealFollowersInto::CCombatAction_Guildmaster_HealFollowersInto(
                    reinterpret_cast<CCombatAction_Guildmaster_HealFollowersInto*>(actionStorage),
                    static_cast<CThingCreatureBase*>(creature));

            CThingCreatureBase::SetCurrentAction(
                static_cast<CThingCreatureBase*>(creature),
                static_cast<CCreatureActionBase*>(action));

            CTCLightningOrb::~CTCLightningOrb(reinterpret_cast<CTCLightningOrb*>(action));
        }

        int remainingCallbacks = self->createCreatureCallbackCount;
        if (0 < remainingCallbacks)
        {
            do
            {
                reinterpret_cast<void (*)(const CGameScriptInterface*)>(
                    self->vftable[0x1C / sizeof(void*)])(this);
                remainingCallbacks = remainingCallbacks - 1;
            } while (remainingCallbacks != 0);
        }

        std::destroy_at(&init.field_28);
        std::destroy_at(&init.field_1C);
        std::destroy_at(&init.field_0C);
        std::destroy_at(&init.field_08);
    }

    auto& dst = *reinterpret_cast<CScriptThing_008A9100_Overlay*>(outResult);
    dst.vftable = &PTR__scalar_deleting_destructor__01238c8c;
    dst.field_04 = localResult.field_04;
    dst.sharedState = localResult.sharedState;
    if (dst.sharedState != nullptr)
    {
        *dst.sharedState = *dst.sharedState + 1;
    }

    if (switchDef.ptr != nullptr)
    {
        int* const refCount = &switchDef.ptr->refCount;
        *refCount = *refCount - 1;
        if (*refCount == 0)
        {
            reinterpret_cast<void (*)(CThingPhysicalSwitchDef_008A9100_Overlay*)>(
                switchDef.ptr->vftable[1])(switchDef.ptr);
        }
    }

    localResult.vftable = &PTR__scalar_deleting_destructor__01238c8c;
    ReleaseScriptThingSharedState_008A9100(localResult);
    localResult.field_04 = 0;
    localResult.sharedState = nullptr;
    NHeroInformationScreens::CBase::CBase(reinterpret_cast<NHeroInformationScreens::CBase*>(&localResult));

    return outResult;
}