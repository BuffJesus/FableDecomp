// Unapplied proposal: include in LuaRetailResources' public section.
// Requires StringMap entry kind using existing Entry::map and destructor case.
bool TryAcquireThing(unsigned resource, unsigned thing, int priority) {
    auto& output = Get(resource, Kind::Resource);
    auto& target = Get(thing, Kind::Thing);
    return StartScriptingEntity_API(m_game, &target.thing, &output.resource,
        static_cast<EScriptAIPriority>(priority));
}
unsigned NewStringMap() {
    if (!StdMap_String_Construct_API || !StdMap_String_Destroy_API ||
        !StdMap_String_OperatorBracket_API || !CCharString_AssignLiteral_API)
        throw std::runtime_error("Retail string-map APIs unavailable");
    auto& e = Add(Kind::StringMap);
    e.map = Game_malloc(0x24);
    if (!e.map) throw std::bad_alloc();
    StdMap_String_Construct_API(e.map);
    e.live = true;
    return static_cast<unsigned>(m_entries.size());
}
void SetString(unsigned map, const std::string& key, const std::string& value) {
    auto& e = Get(map, Kind::StringMap);
    FableString nativeKey(key.c_str());
    auto* output = StdMap_String_OperatorBracket_API(e.map, nativeKey);
    CCharString_AssignLiteral_API(output, value.c_str());
}
void DestroyStringMap(unsigned id) { Destroy(Get(id, Kind::StringMap)); }
void RunMacroWithStrings(const std::string& name, unsigned actors, unsigned inputs,
                         bool setup, bool skippable) {
    auto& actorMap = Get(actors, Kind::ActorMap);
    auto& stringMap = Get(inputs, Kind::StringMap);
    FableString key(name.c_str());
    RunCutsceneMacro_Func(key, actorMap.map, nullptr, stringMap.map, setup, skippable);
}
void ClearThingHasInformation(unsigned thing) {
    auto& e = Get(thing, Kind::Thing);
    if (!ClearThingHasInformation_API)
        throw std::runtime_error("Retail information API unavailable");
    ClearThingHasInformation_API(m_game, &e.thing); // Preserve empty retained Thing.
}
void SetThingAsAlly(CScriptThing* first, CScriptThing* second) {
    CheckOpen();
    if (!EntitySetThingAsAllyOfThing_API)
        throw std::runtime_error("Retail alliance API unavailable");
    EntitySetThingAsAllyOfThing_API(m_game, first, second);
}
void PlayAnimationWithNativeArgument5(unsigned id, const std::string& name,
        bool b1, bool b2, bool b3, bool b4, bool b6, bool b7) {
    auto& e = Get(id, Kind::Resource);
    // Native constructs even when its expert is empty, before loading the byte.
    FableString animation(name.c_str());
    const auto b5 = *ASLR<const volatile unsigned char*>(0x01375748);
    auto* expert = e.resource.pImp.Data;
    if (!expert) return;
    using RawAnimation = void(__thiscall*)(void*, const CCharString*,
        bool, bool, bool, bool, unsigned char, bool, bool);
    auto* table = reinterpret_cast<void**>(expert->pVTable);
    if (!table || !table[0x48 / 4]) throw std::runtime_error("Retail animation slot unavailable");
    reinterpret_cast<RawAnimation>(table[0x48 / 4])(
        expert, animation.get(), b1, b2, b3, b4, b5, b6, b7);
}
int NewConversation(CScriptThing* actor, bool first, bool second) {
    CheckOpen();
    if (!AddNewConversation_API) throw std::runtime_error("Retail conversation API unavailable");
    return AddNewConversation_API(m_game, actor, first, second);
}
void AddConversationPerson(int conversation, unsigned thing) {
    auto& e = Get(thing, Kind::Thing);
    if (!AddPersonToConversation_API) throw std::runtime_error("Retail conversation person API unavailable");
    AddPersonToConversation_API(m_game, conversation, &e.thing);
}
void AddConversationLine(int conversation, const std::string& text,
        CScriptThing* speaker, unsigned listener, bool flag) {
    auto& e = Get(listener, Kind::Thing);
    if (!AddLineToConversation_API) throw std::runtime_error("Retail conversation line API unavailable");
    FableString key(text.c_str());
    AddLineToConversation_API(m_game, conversation, key, flag, speaker, &e.thing);
}
unsigned NewText() {
    auto& e = Add(Kind::Text);
    using Constructor = CCharString*(__thiscall*)(CCharString*);
    ASLR<Constructor>(0x0099E4B0)(&e.text);
    e.live = true;
    return static_cast<unsigned>(m_entries.size());
}
void FormatBullyIntimidationText(unsigned text, int index) {
    auto& e = Get(text, Kind::Text);
    using Format = void(__cdecl*)(CCharString*, const char*, ...);
    ASLR<Format>(0x0099F1F0)(&e.text, "TEXT_QST_048_BULLY_SCRMSG_INTIMIDATING_%d", index);
}
void DestroyText(unsigned id) { Destroy(Get(id, Kind::Text)); }
void AddConversationText(int conversation, unsigned text, CScriptThing* speaker,
                         unsigned listener, bool flag) {
    auto& key = Get(text, Kind::Text);
    auto& target = Get(listener, Kind::Thing);
    if (!AddLineToConversation_API) throw std::runtime_error("Retail conversation line API unavailable");
    AddLineToConversation_API(m_game, conversation, &key.text, flag, speaker, &target.thing);
}
void FaceRetainedThing(CScriptThing* actor, unsigned target, bool snap) {
    auto& e = Get(target, Kind::Thing);
    if (!EntitySetFacingAngleTowardsThing_API) throw std::runtime_error("Retail facing API unavailable");
    EntitySetFacingAngleTowardsThing_API(m_game, actor, &e.thing, snap);
}
int ReadBullyRandomModulus() { CheckOpen();return *ASLR<const volatile int*>(0x013AC860); }
float ReadBullyProximityRange() { CheckOpen();return *ASLR<const volatile float*>(0x013AC85C); }
bool IsDistanceBetweenThingsUnder(CScriptThing* first, CScriptThing* second, float range) {
    CheckOpen();
    using Distance = bool(__fastcall*)(CScriptThing*, CScriptThing*, float);
    return ASLR<Distance>(0x00CBE2FF)(first, second, range);
}
struct BullyNativeLiteral {
    CCharString value{};
    explicit BullyNativeLiteral(const char* text) { CCharString_Construct_Literal(&value, text, -1); }
    ~BullyNativeLiteral() { CCharString_Destroy(&value); }
    BullyNativeLiteral(const BullyNativeLiteral&) = delete;
    BullyNativeLiteral& operator=(const BullyNativeLiteral&) = delete;
};
void GiveBullyTeddyQuestion() {
    CheckOpen();
    if (!GiveHeroYesNoQuestion_API) throw std::runtime_error("Retail question API unavailable");
    BullyNativeLiteral empty("");
    BullyNativeLiteral no("TEXT_OBJECT_HERO_ANSWER_NO");
    BullyNativeLiteral yes("TEXT_OBJECT_HERO_ANSWER_YES");
    BullyNativeLiteral question("TEXT_QST_048_GIVE_TEDDY_TO_BULLY");
    GiveHeroYesNoQuestion_API(m_game, &question.value, &yes.value, &no.value, &empty.value, true);
}
int AddBullyHealthBar(int current) {
    CheckOpen();
    if (!AddQuestInfoBar_API) throw std::runtime_error("Retail HUD bar API unavailable");
    BullyNativeLiteral empty("");
    BullyNativeLiteral icon("HUD_QUEST_ICON_GRANDSON");
    const CRGBColour green{0,255,0,255},red{0,0,255,255};
    return AddQuestInfoBar_API(m_game, static_cast<float>(current), 0.0f,
        &green, &red, &icon.value, &empty.value, 1.0f);
}
void InitializeBullyActor(CScriptThing* actor) {
    CheckOpen();
    if (!actor || !EntitySetAsDamageable_API || !EntitySetAsKillable_API ||
        !EntitySetAsToAddToComboMultiplierWhenHit_API || !SetThingHasInformation_API ||
        !GetHero_API || !EntitySetThingAsAllyOfThing_API || !SetIsPushableByHero_API || !g_pCScriptThingVTable)
        throw std::runtime_error("Retail Bully Init APIs unavailable");
    EntitySetAsDamageable_API(m_game, actor, false);
    EntitySetAsKillable_API(m_game, actor, false, false);
    EntitySetAsToAddToComboMultiplierWhenHit_API(m_game, actor, false);
    SetThingHasInformation_API(m_game, actor, false, false, false);
    auto* hero = GetHero_API(m_game);
    EntitySetThingAsAllyOfThing_API(m_game, actor, hero);
    CScriptThing argument = *actor;
    argument.pVTable = g_pCScriptThingVTable;
    if (argument.pImp.Info) ++argument.pImp.Info->RefCount;
    // Native by-value callee consumes this one retained reference, including
    // its early-empty path. Do not add a caller-side DestroyThing here.
    SetIsPushableByHero_API(m_game, argument, false);
}
