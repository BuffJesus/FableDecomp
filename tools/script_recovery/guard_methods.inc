// Unapplied Guard capabilities. No acquisition/wait policy inside these methods.
struct GuardNativeText {
    CCharString value{};
    explicit GuardNativeText(const char* text) { CCharString_Construct_Literal(&value,text,-1); }
    ~GuardNativeText() { CCharString_Destroy(&value); }
    GuardNativeText(const GuardNativeText&)=delete;
};
float ReadGuardAlertRange(int index) {
    CheckOpen();
    const unsigned address=0x013AC844u+static_cast<unsigned>(index)*4u;
    return *ASLR<const volatile float*>(address);
}
float ReadGuardLectureRange() { CheckOpen();return *ASLR<const volatile float*>(0x013AC840); }
void FollowThing(unsigned id, CScriptThing* target, float distance, bool flag) {
    auto* expert=Get(id,Kind::Resource).resource.pImp.Data;
    if (!expert) return;
    auto* table=reinterpret_cast<CScriptGameResourceObjectScriptedThingBaseVTable*>(expert->pVTable);
    if (!table || !table->FollowThing) throw std::runtime_error("Retail FollowThing slot unavailable");
    table->FollowThing(expert,target,distance,flag);
}
void GuardFaceHero(CScriptThing* actor, bool snap, bool reverse) {
    CheckOpen();
    if (!GetHero_API || !EntitySetFacingAngleTowardsThing_API) throw std::runtime_error("Retail Guard facing APIs unavailable");
    auto* hero=GetHero_API(m_game);
    EntitySetFacingAngleTowardsThing_API(m_game,reverse?hero:actor,reverse?actor:hero,snap);
}
void AddRawConversationPerson(int conversation, CScriptThing* actor) {
    CheckOpen();
    if (!AddPersonToConversation_API) throw std::runtime_error("Retail conversation person API unavailable");
    AddPersonToConversation_API(m_game,conversation,actor);
}
void GuardAddHeroConversationLine(int conversation,const std::string& key,CScriptThing* actor) {
    CheckOpen();
    if (!CCharString_Construct_Literal || !CCharString_Destroy || !GetHero_API || !AddLineToConversation_API)
        throw std::runtime_error("Retail Guard conversation APIs unavailable");
    GuardNativeText text(key.c_str());
    auto* hero=GetHero_API(m_game);
    AddLineToConversation_API(m_game,conversation,&text.value,false,actor,hero);
}
void SetRawCutsceneBehaviour(CScriptThing* actor,int behavior) {
    CheckOpen();
    if (!EntitySetCutsceneBehaviour_API) throw std::runtime_error("Retail cutscene behavior API unavailable");
    EntitySetCutsceneBehaviour_API(m_game,actor,static_cast<ECutsceneBehaviour>(behavior));
}
void InitializeGuardActor(CScriptThing* actor) {
    CheckOpen();
    if (!actor || !actor->pVTable || !g_pCScriptThingVTable || !EntitySetAsDamageable_API || !EntitySetAsKillable_API ||
        !EntitySetAsToAddToComboMultiplierWhenHit_API || !SetWanderCentrePoint_API || !SetWanderMinDistance_API ||
        !SetWanderMaxDistance_API || !SetScriptingStateGroup_API || !EntitySheatheWeapons_API)
        throw std::runtime_error("Retail Guard Init APIs unavailable");
    auto* table=reinterpret_cast<CScriptThingVTable*>(actor->pVTable);
    if (!table->GetHomePos) throw std::runtime_error("Retail Guard home getter unavailable");
    EntitySetAsDamageable_API(m_game,actor,false);
    EntitySetAsKillable_API(m_game,actor,false,false);
    EntitySetAsToAddToComboMultiplierWhenHit_API(m_game,actor,false);
    C3DVector home;
    table->GetHomePos(actor,&home); // Original output storage; returned pointer is unused.
    auto argument=[&]() {
        CScriptThing value=*actor;
        value.pVTable=g_pCScriptThingVTable;
        if (value.pImp.Info) ++value.pImp.Info->RefCount;
        return value;
    };
    // Each original native by-value callee consumes one retained reference.
    // No extra caller-side destructor, shared owner, or IsNull filtering.
    SetWanderCentrePoint_API(m_game,argument(),home);
    SetWanderMinDistance_API(m_game,argument(),0.0f);
    SetWanderMaxDistance_API(m_game,argument(),6.0f);
    SetScriptingStateGroup_API(m_game,argument(),static_cast<EScriptingStateGroups>(4));
    EntitySheatheWeapons_API(m_game,actor,false);
}
