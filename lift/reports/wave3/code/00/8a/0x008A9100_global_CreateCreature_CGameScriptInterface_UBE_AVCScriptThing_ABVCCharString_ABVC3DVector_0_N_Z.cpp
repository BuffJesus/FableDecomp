namespace
{
    struct CGameScriptInterface_Overlay
    {
        void* vfptr;                           // 0x00
        std::byte pad_04[0x10 - 0x04];
        CDefinitionManager* definitionManager; // 0x10
        std::byte pad_14[0x4C - 0x14];
        int pendingCallbackCount;              // 0x4C
    };
    static_assert(offsetof(CGameScriptInterface_Overlay, definitionManager) == 0x10);
    static_assert(offsetof(CGameScriptInterface_Overlay, pendingCallbackCount) == 0x4C);

    struct CThingPhysicalSwitchDef_Overlay
    {
        void* vfptr;                    // 0x00
        int refCount;                   // 0x04
        std::byte pad_08[0x11C - 0x08];
        int field_11C;                  // 0x11C
    };
    static_assert(offsetof(CThingPhysicalSwitchDef_Overlay, field_11C) == 0x11C);

    struct CScriptThing_Overlay
    {
        void* vfptr;                    // 0x00
        std::uint32_t field_04;         // 0x04
        int* sharedState;               // 0x08
    };
    static_assert(offsetof(CScriptThing_Overlay, field_04) == 0x04);
    static_assert(offsetof(CScriptThing_Overlay, sharedState) == 0x08);

    struct CThingAICreatureInit_Overlay
    {
        std::uint32_t field_00;         // 0x00
        std::uint32_t field_04;         // 0x04
        std::byte field_08[sizeof(CCharString)]; // 0x08
        std::byte field_0C[sizeof(CCharString)]; // 0x0C
        bool field_10;                  // 0x10
        bool field_11;                  // 0x11
        bool field_12;                  // 0x12
        bool field_13;                  // 0x13
        std::uint32_t field_14;         // 0x14
        std::uint32_t field_18;         // 0x18
    };
    static_assert(offsetof(CThingAICreatureInit_Overlay, field_08) == 0x08);
    static_assert(offsetof(CThingAICreatureInit_Overlay, field_0C) == 0x0C);
    static_assert(offsetof(CThingAICreatureInit_Overlay, field_10) == 0x10);
    static_assert(offsetof(CThingAICreatureInit_Overlay, field_14) == 0x14);
    static_assert(offsetof(CThingAICreatureInit_Overlay, field_18) == 0x18);

    using MorphPair = std::pair<EHeroMorphType, CParticleMorphs::CEntry>;
    using PredicateAndInput =
        NScript::CPredicate_And<
            NScript::CPredicate_And<
                NScript::CPredicate_And<NScript::CIsThingAlive, NScript::CIsThingAlive>,
                NScript::CIsThingAlive>,
            NScript::CIsThingAlive>;

    // These declarations intentionally model the observed ABI/dataflow at the call sites.
    // They do not claim a higher-level source signature beyond what the binary proves.
    extern CScriptThing* __thiscall GFPredicateAnd_Abi(
        PredicateAndInput* self,
        CScriptThing* returnStorage,
        NScript::CIsThingAlive* edxPredicateSeed);

    extern void __thiscall MorphPairConsVal_Abi(
        std::allocator<MorphPair>* self,
        const MorphPair* edxValue,
        const MorphPair* registerValue);

    static void ReleaseScriptThingSharedState(int* sharedState)
    {
        if (sharedState != nullptr)
        {
            --sharedState[0];
            if (sharedState[0] == 0)
            {
                reinterpret_cast<void(__thiscall*)(int*)>(sharedState[1])(sharedState);
                operator_delete(sharedState);
            }
        }
    }

    static void ReleasePhysicalSwitchDef(CThingPhysicalSwitchDef_Overlay* def)
    {
        if (def != nullptr)
        {
            int* const refCount = reinterpret_cast<int*>(reinterpret_cast<std::byte*>(def) + 0x04);
            --(*refCount);
            if (*refCount == 0)
            {
                reinterpret_cast<void(__thiscall*)(CThingPhysicalSwitchDef_Overlay*)>(
                    (*reinterpret_cast<void***>(def))[1])(def);
            }
        }
    }
}

CScriptThing* __thiscall CGameScriptInterface::CreateCreature(
    CScriptThing* result,
    const CCharString& definitionName,
    const C3DVector& position,
    const CCharString& scriptName,
    bool setHealFollowersAction) const
{
    const auto* const self = reinterpret_cast<const CGameScriptInterface_Overlay*>(this);

    const long defIndex =
        CDefinitionManager::GetDefGlobalIndexFromName(self->definitionManager, &definitionName);

    CThingPhysicalSwitchDef_Overlay* switchDef = nullptr;

    CScriptThing_Overlay local_14c;
    NHeroInformationScreens::CBase::CBase(reinterpret_cast<CBase*>(&local_14c));
    local_14c.vfptr = &PTR__scalar_deleting_destructor__01238c8c;
    local_14c.field_04 = 0;
    local_14c.sharedState = nullptr;

    if (CDefinitionManager::GetDef<CThingPhysicalSwitchDef>(
            self->definitionManager,
            defIndex,
            reinterpret_cast<CDefPointer<CThingPhysicalSwitchDef const>*>(&switchDef)))
    {
        CCharString local_140("", -1);
        CCharString local_150_string("", -1);

        CThingAICreatureInit_Overlay init{};
        auto* const initString_08 = reinterpret_cast<CCharString*>(&init.field_08);
        auto* const initString_0C = reinterpret_cast<CCharString*>(&init.field_0C);

        CCharString::CCharString(initString_08, &local_150_string);
        CCharString::CCharString(initString_0C, &local_140);
        init.field_10 = false;
        init.field_11 = true;
        init.field_12 = true;
        init.field_13 = false;
        init.field_14 = 0;
        init.field_18 = 0;

        const MorphPair* regMorph0 = nullptr;
        const MorphPair* regMorph1 = nullptr;
        MorphPairConsVal_Abi(
            reinterpret_cast<std::allocator<MorphPair>*>(&local_150_string),
            regMorph0,
            regMorph1);
        MorphPairConsVal_Abi(
            reinterpret_cast<std::allocator<MorphPair>*>(&local_140),
            regMorph0,
            regMorph1);

        CCharString::operator=(initString_08, &scriptName);

        CThingAICreature* const creature =
            CThingAICreature::Create(
                defIndex,
                &position,
                switchDef->field_11C,
                reinterpret_cast<CThingAICreatureInit*>(&init));

        CScriptThing_Overlay local_13c;
        NScript::CIsThingAlive* predicateSeed = nullptr;
        CScriptThing* const predicateThing =
            GFPredicateAnd_Abi(
                reinterpret_cast<PredicateAndInput*>(creature),
                reinterpret_cast<CScriptThing*>(&local_13c),
                predicateSeed);

        CScriptThing::operator=(reinterpret_cast<CScriptThing*>(&local_14c), predicateThing);

        local_13c.vfptr = &PTR__scalar_deleting_destructor__01238c8c;
        if (local_13c.sharedState != nullptr)
        {
            ReleaseScriptThingSharedState(local_13c.sharedState);
        }
        local_13c.field_04 = 0;
        local_13c.sharedState = nullptr;
        NHeroInformationScreens::CBase::CBase(reinterpret_cast<CBase*>(&local_13c));

        if (setHealFollowersAction)
        {
            alignas(CCombatAction_Guildmaster_HealFollowersInto) std::byte actionStorage[276];

            CCreatureActionBase* const action =
                reinterpret_cast<CCreatureActionBase*>(
                    CCombatAction_Guildmaster_HealFollowersInto::
                        CCombatAction_Guildmaster_HealFollowersInto(
                            reinterpret_cast<CCombatAction_Guildmaster_HealFollowersInto*>(actionStorage),
                            static_cast<CThingCreatureBase*>(creature)));

            CThingCreatureBase::SetCurrentAction(
                static_cast<CThingCreatureBase*>(creature),
                action);

            CTCLightningOrb::~CTCLightningOrb(
                reinterpret_cast<CTCLightningOrb*>(actionStorage));
        }

        int remaining = self->pendingCallbackCount;
        if (remaining > 0)
        {
            do
            {
                reinterpret_cast<void(__thiscall*)(const CGameScriptInterface*)>(
                    (*reinterpret_cast<void***>(const_cast<CGameScriptInterface*>(this)))[7])(this);
                --remaining;
            } while (remaining != 0);
        }

        const MorphPair* regMorph2 = nullptr;
        const MorphPair* regMorph3 = nullptr;
        MorphPairConsVal_Abi(
            reinterpret_cast<std::allocator<MorphPair>*>(initString_0C),
            regMorph2,
            regMorph3);
        MorphPairConsVal_Abi(
            reinterpret_cast<std::allocator<MorphPair>*>(initString_08),
            regMorph2,
            regMorph3);
    }

    auto* const out = reinterpret_cast<CScriptThing_Overlay*>(result);
    out->vfptr = &PTR__scalar_deleting_destructor__01238c8c;
    out->field_04 = local_14c.field_04;
    out->sharedState = local_14c.sharedState;
    if (out->sharedState != nullptr)
    {
        ++out->sharedState[0];
    }

    ReleasePhysicalSwitchDef(switchDef);

    local_14c.vfptr = &PTR__scalar_deleting_destructor__01238c8c;
    if (local_14c.sharedState != nullptr)
    {
        ReleaseScriptThingSharedState(local_14c.sharedState);
    }
    local_14c.field_04 = 0;
    local_14c.sharedState = nullptr;
    NHeroInformationScreens::CBase::CBase(reinterpret_cast<CBase*>(&local_14c));

    return result;
}