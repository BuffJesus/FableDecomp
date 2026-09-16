// Unapplied TeddyGirl adapters. All object ownership stays in this scope.
struct TeddyGirlText {
    CCharString value{};
    explicit TeddyGirlText(const char* text) { CCharString_Construct_Literal(&value,text,-1); }
    ~TeddyGirlText() { CCharString_Destroy(&value); }
    TeddyGirlText(const TeddyGirlText&)=delete;
};
unsigned NewMovie() {
    CheckOpen();
    for (const auto& existing:m_entries) if(existing->live && existing->kind==Kind::Movie)
        throw std::runtime_error("Retail scope already owns a movie");
    auto& e=Add(Kind::Movie);
    CBaseObject_Construct_API(&e.movie);
    e.movie.pVTable=g_pMovieObjectVTable;
    e.movie.pImp.Data=nullptr;e.movie.pImp.Info=nullptr;e.live=true;
    return static_cast<unsigned>(m_entries.size());
}
void StartOwnedMovie(unsigned id,const std::string& name) {
    auto& e=Get(id,Kind::Movie);
    TeddyGirlText key(name.c_str());
    StartMovieSequence_API(m_game,&key.value,&e.movie);
}
void InitializeTeddyGirlActor(CScriptThing* actor) {
    CheckOpen();
    if (!EntitySetAsDamageable_API || !EntitySetAsKillable_API || !EntitySetAsToAddToComboMultiplierWhenHit_API || !SetThingHasInformation_API)
        throw std::runtime_error("TeddyGirl Init APIs unavailable");
    EntitySetAsDamageable_API(m_game,actor,false);
    EntitySetAsKillable_API(m_game,actor,false,false);
    EntitySetAsToAddToComboMultiplierWhenHit_API(m_game,actor,false);
    SetThingHasInformation_API(m_game,actor,false,true,false);
}
void TakeTeddyFromHero() {
    CheckOpen();
    if (!TakeObjectFromHero_API) throw std::runtime_error("Teddy take API unavailable");
    TeddyGirlText text("OBJECT_TEDDY_BEAR_UNGIVEABLE");
    TakeObjectFromHero_API(m_game,&text.value);
}
void ClearRawInformation(CScriptThing* actor) {
    CheckOpen();
    if (!ClearThingHasInformation_API) throw std::runtime_error("Teddy information API unavailable");
    ClearThingHasInformation_API(m_game,actor);
}
bool IsTalkedToByHero(CScriptThing* actor) {
    CheckOpen();
    if (!actor || !actor->pVTable) throw std::runtime_error("Teddy talk actor unavailable");
    auto* table=reinterpret_cast<CScriptThingVTable*>(actor->pVTable);
    if (!table->MsgIsTalkedToBy) throw std::runtime_error("Teddy talk slot unavailable");
    TeddyGirlText who("SCRIPT_NAME_HERO");
    return table->MsgIsTalkedToBy(actor,&who.value);
}
bool TeddyGirlTalkedWithTeddy(CScriptThing* actor) {
    CheckOpen();
    if (!actor || !actor->pVTable || !GetHero_API || !IsObjectInThingsPossession_API) throw std::runtime_error("Teddy talk APIs unavailable");
    auto* table=reinterpret_cast<CScriptThingVTable*>(actor->pVTable);
    if (!table->MsgIsTalkedToBy) throw std::runtime_error("Teddy talk slot unavailable");
    TeddyGirlText who("SCRIPT_NAME_HERO");
    if (!table->MsgIsTalkedToBy(actor,&who.value)) return false;
    TeddyGirlText teddy("OBJECT_TEDDY_BEAR_UNGIVEABLE");
    auto* hero=GetHero_API(m_game);
    return IsObjectInThingsPossession_API(m_game,&teddy.value,hero);
}
void RemoveRawThing(CScriptThing* actor,bool first,bool second) {
    CheckOpen();
    if (!RemoveThing_API) throw std::runtime_error("Teddy removal API unavailable");
    RemoveThing_API(m_game,actor,first,second);
}
void GiveTeddyGirlQuestion() {
    CheckOpen();
    if (!GiveHeroYesNoQuestion_API) throw std::runtime_error("Teddy question API unavailable");
    TeddyGirlText empty("");
    TeddyGirlText no("TEXT_OBJECT_HERO_ANSWER_NO");
    TeddyGirlText yes("TEXT_OBJECT_HERO_ANSWER_YES");
    TeddyGirlText question("TEXT_QST_048_GIVE_TEDDY_TO_GIRL");
    GiveHeroYesNoQuestion_API(m_game,&question.value,&yes.value,&no.value,&empty.value,true);
}
void TeddyGirlAddHeroLine(int conversation,CScriptThing* actor,const std::string& key) {
    CheckOpen();
    if (!GetHero_API || !AddLineToConversation_API) throw std::runtime_error("Teddy conversation APIs unavailable");
    TeddyGirlText text(key.c_str());
    auto* hero=GetHero_API(m_game);
    AddLineToConversation_API(m_game,conversation,&text.value,false,actor,hero);
}
bool IsActorPositionOnScreen(CScriptThing* actor) {
    CheckOpen();
    if (!actor || !actor->pVTable || !IsCameraPosOnScreen_API) throw std::runtime_error("Teddy position API unavailable");
    auto* table=reinterpret_cast<CScriptThingVTable*>(actor->pVTable);
    if (!table->GetPos) throw std::runtime_error("Teddy position getter unavailable");
    return IsCameraPosOnScreen_API(m_game,table->GetPos(actor));
}
bool IsDistanceUnderThing(CScriptThing* actor,unsigned target,float distance) {
    auto& thing=Get(target,Kind::Thing).thing;
    using Function=bool(__fastcall*)(CScriptThing*,const CScriptThing*,float);
    return ASLR<Function>(0x00CBE2FF)(actor,&thing,distance);
}
bool IsDistanceOver(CScriptThing* actor,CScriptThing* target,float distance) {
    CheckOpen();
    using Function=bool(__fastcall*)(CScriptThing*,const CScriptThing*,float);
    return ASLR<Function>(0x00CBE3EA)(actor,target,distance);
}
void TeddyGirlMoveToThing(unsigned resource,unsigned target,float radius,int moveType,
        sol::object wait,bool avoid,bool ignore,bool face) {
    if (wait.valid() && wait.get_type()!=sol::type::nil) throw std::runtime_error("Teddy movement requires native null wait control");
    auto* expert=Get(resource,Kind::Resource).resource.pImp.Data;
    auto& thing=Get(target,Kind::Thing).thing;
    if (!expert) return;
    auto* function=reinterpret_cast<void**>(expert->pVTable)[5];
    if (!function) throw std::runtime_error("Teddy movement expert slot unavailable");
    reinterpret_cast<tCSGrost_MoveToThing>(function)(expert,&thing,radius,static_cast<EScriptEntityMoveType>(moveType),nullptr,avoid,ignore,face);
}
