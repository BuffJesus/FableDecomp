using HeroMorphEntry = std::pair<EHeroMorphType, CParticleMorphs::CEntry>;

struct CGameScriptInterface_Overlay
{
    std::byte pad_00[0x10];
    CDefinitionManager* m_pDefinitionManager; // 0x10
    std::byte pad_14[0x38];
    int m_spawnCallbackCount; // 0x4C
};
static_assert(offsetof(CGameScriptInterface_Overlay, m_pDefinitionManager) == 0x10);
static_assert(offsetof(CGameScriptInterface_Overlay, m_spawnCallbackCount) == 0x4C);

struct CThingPhysicalSwitchDef_Overlay
{
    std::byte pad_000[0x11C];
    int field_11C; // 0x11C
};
static_assert(offsetof(CThingPhysicalSwitchDef_Overlay, field_11C) == 0x11C);

struct CThingAICreatureInit_Overlay
{
    int field_00;         // local_130
    int field_04;         // local_12C
    CCharString field_08; // local_128
    CCharString field_0C; // local_124
    bool field_10;        // local_120
    bool field_11;        // local_11F
    bool field_12;        // local_11E
    bool field_13;        // local_11D
    int field_14;         // local_11C
    int field_18;         // local_118
};
static_assert(offsetof(CThingAICreatureInit_Overlay, field_00) == 0x00);
static_assert(offsetof(CThingAICreatureInit_Overlay, field_04) == 0x04);
static_assert(offsetof(CThingAICreatureInit_Overlay, field_08) == 0x08);
static_assert(offsetof(CThingAICreatureInit_Overlay, field_0C) == 0x0C);
static_assert(offsetof(CThingAICreatureInit_Overlay, field_10) == 0x10);
static_assert(offsetof(CThingAICreatureInit_Overlay, field_11) == 0x11);
static_assert(offsetof(CThingAICreatureInit_Overlay, field_12) == 0x12);
static_assert(offsetof(CThingAICreatureInit_Overlay, field_13) == 0x13);
static_assert(offsetof(CThingAICreatureInit_Overlay, field_14) == 0x14);
static_assert(offsetof(CThingAICreatureInit_Overlay, field_18) == 0x18);
static_assert(sizeof(CThingAICreatureInit_Overlay) == 0x1C);

struct CScriptThing_Overlay
{
    void* vfptr;
    std::uint32_t field_04;
    int* ref_ptr;
};
static_assert(offsetof(CScriptThing_Overlay, vfptr) == 0x00);
static_assert(offsetof(CScriptThing_Overlay, field_04) == 0x04);
static_assert(offsetof(CScriptThing_Overlay, ref_ptr) == 0x08);
static_assert(sizeof(CScriptThing_Overlay) == 0x0C);

union Local150_Overlay
{
    int* def_ptr;
    CCharString as_string;
    std::byte raw[0x10];
};
static_assert(offsetof(Local150_Overlay, def_ptr) == 0x00);

CScriptThing* __thiscall CGameScriptInterface::CreateCreature(
    CScriptThing* result,
    const CCharString& definitionName,
    const C3DVector& position,
    const CCharString& scriptName,
    bool startWithHealFollowersAction) const
{
    auto const* const self = reinterpret_cast<CGameScriptInterface_Overlay const*>(this);

    HeroMorphEntry* extraout_EDX = nullptr;
    HeroMorphEntry* extraout_EDX_00 = nullptr;
    NScript::CIsThingAlive* extraout_EDX_01 = nullptr;
    HeroMorphEntry* extraout_EDX_02 = nullptr;
    HeroMorphEntry* extraout_EDX_03 = nullptr;
    HeroMorphEntry* extraout_EDX_04 = nullptr;
    HeroMorphEntry* extraout_EDX_05 = nullptr;
    HeroMorphEntry* ppVar7 = nullptr;
    HeroMorphEntry* unaff_EDI = nullptr;

    const long defGlobalIndex =
        CDefinitionManager::GetDefGlobalIndexFromName(self->m_pDefinitionManager, &definitionName);

    CScriptThing_Overlay local_14c{};
    NHeroInformationScreens::CBase::CBase(reinterpret_cast<CBase*>(&local_14c));
    local_14c.vfptr = &PTR__scalar_deleting_destructor__01238c8c;
    local_14c.field_04 = 0;
    local_14c.ref_ptr = nullptr;

    Local150_Overlay local_150{};
    local_150.def_ptr = nullptr;

    const bool found = CDefinitionManager::GetDef<CThingPhysicalSwitchDef>(
        self->m_pDefinitionManager,
        defGlobalIndex,
        reinterpret_cast<CDefPointer<CThingPhysicalSwitchDef const>*>(&local_150));

    int* piVar2 = local_150.def_ptr;
    if (found)
    {
        alignas(CCharString) std::byte local_140_raw[0x10];
        auto* const local_140 = reinterpret_cast<CCharString*>(local_140_raw);

        CScriptThing_Overlay local_13c{};
        CThingAICreatureInit_Overlay local_130{};

        CCharString::CCharString(local_140, "", -1);
        CCharString::CCharString(reinterpret_cast<CCharString*>(&local_150), "", -1);

        local_130.field_00 = 0;
        local_130.field_04 = 0;
        CCharString::CCharString(&local_130.field_08, reinterpret_cast<CCharString*>(&local_150));
        CCharString::CCharString(&local_130.field_0C, local_140);
        local_130.field_10 = false;
        local_130.field_11 = true;
        local_130.field_12 = true;
        local_130.field_13 = false;
        local_130.field_14 = 0;
        local_130.field_18 = 0;

        std::_Cons_val<
            std::allocator<HeroMorphEntry>,
            HeroMorphEntry,
            HeroMorphEntry const&>(
            reinterpret_cast<std::allocator<HeroMorphEntry>*>(&local_150),
            extraout_EDX,
            unaff_EDI);
        std::_Cons_val<
            std::allocator<HeroMorphEntry>,
            HeroMorphEntry,
            HeroMorphEntry const&>(
            reinterpret_cast<std::allocator<HeroMorphEntry>*>(local_140),
            extraout_EDX_00,
            unaff_EDI);

        CCharString::operator=(&local_130.field_08, &scriptName);

        CThingAICreature* const creature = CThingAICreature::Create(
            defGlobalIndex,
            &position,
            reinterpret_cast<CThingPhysicalSwitchDef_Overlay const*>(piVar2)->field_11C,
            reinterpret_cast<CThingAICreatureInit*>(&local_130));

        CScriptThing* const aliveThing =
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
                    extraout_EDX_01));

        CScriptThing::operator=(reinterpret_cast<CScriptThing*>(&local_14c), aliveThing);

        local_13c.vfptr = &PTR__scalar_deleting_destructor__01238c8c;
        if (local_13c.ref_ptr != nullptr)
        {
            *local_13c.ref_ptr = *local_13c.ref_ptr - 1;
            if (*local_13c.ref_ptr == 0)
            {
                reinterpret_cast<void(__thiscall*)(int*)>(local_13c.ref_ptr[1])(local_13c.ref_ptr);
                operator_delete(local_13c.ref_ptr);
            }
        }
        local_13c.field_04 = 0;
        local_13c.ref_ptr = nullptr;
        NHeroInformationScreens::CBase::CBase(reinterpret_cast<CBase*>(&local_13c));

        ppVar7 = extraout_EDX_02;
        if (startWithHealFollowersAction)
        {
            alignas(CCombatAction_Guildmaster_HealFollowersInto)
                std::byte action_storage[sizeof(CCombatAction_Guildmaster_HealFollowersInto)];

            CCreatureActionBase* const action =
                reinterpret_cast<CCreatureActionBase*>(
                    CCombatAction_Guildmaster_HealFollowersInto::CCombatAction_Guildmaster_HealFollowersInto(
                        reinterpret_cast<CCombatAction_Guildmaster_HealFollowersInto*>(action_storage),
                        reinterpret_cast<CThingCreatureBase*>(creature)));

            CThingCreatureBase::SetCurrentAction(
                reinterpret_cast<CThingCreatureBase*>(creature),
                action);

            CTCLightningOrb::~CTCLightningOrb(reinterpret_cast<CTCLightningOrb*>(action_storage));
            ppVar7 = extraout_EDX_03;
        }

        int iVar8 = self->m_spawnCallbackCount;
        if (0 < iVar8)
        {
            do
            {
                reinterpret_cast<void(__thiscall*)(const CGameScriptInterface*)>(
                    (*reinterpret_cast<void***>(const_cast<CGameScriptInterface*>(this)))[7])(
                    this);
                iVar8 = iVar8 - 1;
                ppVar7 = extraout_EDX_04;
            } while (iVar8 != 0);
        }

        std::_Cons_val<
            std::allocator<HeroMorphEntry>,
            HeroMorphEntry,
            HeroMorphEntry const&>(
            reinterpret_cast<std::allocator<HeroMorphEntry>*>(&local_130.field_0C),
            ppVar7,
            unaff_EDI);
        std::_Cons_val<
            std::allocator<HeroMorphEntry>,
            HeroMorphEntry,
            HeroMorphEntry const&>(
            reinterpret_cast<std::allocator<HeroMorphEntry>*>(&local_130.field_08),
            extraout_EDX_05,
            unaff_EDI);
    }

    auto* const out = reinterpret_cast<CScriptThing_Overlay*>(result);
    out->vfptr = &PTR__scalar_deleting_destructor__01238c8c;
    out->field_04 = local_14c.field_04;
    out->ref_ptr = local_14c.ref_ptr;
    if (local_14c.ref_ptr != nullptr)
    {
        *local_14c.ref_ptr = *local_14c.ref_ptr + 1;
    }

    if (piVar2 != nullptr)
    {
        int* const ref_count = piVar2 + 1;
        *ref_count = *ref_count - 1;
        if (*ref_count == 0)
        {
            reinterpret_cast<void(__thiscall*)(int*)>(
                (*reinterpret_cast<void***>(piVar2))[1])(piVar2);
        }
    }

    local_14c.vfptr = &PTR__scalar_deleting_destructor__01238c8c;
    if (local_14c.ref_ptr != nullptr)
    {
        *local_14c.ref_ptr = *local_14c.ref_ptr - 1;
        if (*local_14c.ref_ptr == 0)
        {
            reinterpret_cast<void(__thiscall*)(int*)>(local_14c.ref_ptr[1])(local_14c.ref_ptr);
            operator_delete(local_14c.ref_ptr);
        }
    }
    local_14c.field_04 = 0;
    local_14c.ref_ptr = nullptr;
    NHeroInformationScreens::CBase::CBase(reinterpret_cast<CBase*>(&local_14c));

    return result;
}