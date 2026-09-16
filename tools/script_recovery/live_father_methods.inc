// Unapplied LiveFather capabilities; shared Text kind preserves existing NewText().
struct LiveFatherText {
    CCharString value{};
    explicit LiveFatherText(const char* text) { CCharString_Construct_Literal(&value,text,-1); }
    ~LiveFatherText() { CCharString_Destroy(&value); }
    LiveFatherText(const LiveFatherText&)=delete;
};
unsigned NewLiteralText(const std::string& text) {
    auto& e=Add(Kind::Text);
    CCharString_Construct_Literal(&e.text,text.c_str(),-1);
    e.live=true;
    return static_cast<unsigned>(m_entries.size());
}
void DestroyText(unsigned id) { Destroy(Get(id,Kind::Text)); }
void GiveRawHeroGold(int amount) {
    CheckOpen();
    if(!GiveHeroGold_API) throw std::runtime_error("LiveFather gold API unavailable");
    GiveHeroGold_API(m_game,amount);
}
bool LiveFatherHeroHasChocolate() {
    CheckOpen();
    if(!GetHero_API || !IsObjectInThingsPossession_API) throw std::runtime_error("LiveFather possession API unavailable");
    LiveFatherText text("OBJECT_CHOCOLATE_BOX_UNGIVEABLE");
    auto* hero=GetHero_API(m_game);
    return IsObjectInThingsPossession_API(m_game,&text.value,hero);
}
void SetActiveQuestObjective(const std::string& key) {
    CheckOpen();
    if(!GetActiveQuestName_API || !SetQuestCardObjective_API) throw std::runtime_error("LiveFather objective APIs unavailable");
    LiveFatherText region2("");
    LiveFatherText region1("");
    LiveFatherText objective(key.c_str());
    CCharString active{};
    // Successful hidden-output construction is a required engine boundary.
    auto* result=GetActiveQuestName_API(m_game,&active);
    try { SetQuestCardObjective_API(m_game,result,&objective.value,&region1.value,&region2.value); }
    catch(...) { CCharString_Destroy(&active);throw; }
    CCharString_Destroy(&active);
}
void DisplayRawGameInfo(const std::string& key) {
    CheckOpen();
    if(!DisplayGameInfo_API) throw std::runtime_error("LiveFather game info API unavailable");
    LiveFatherText text(key.c_str());
    DisplayGameInfo_API(m_game,&text.value);
}
int AddLiveFatherGoodDeedCounter(unsigned key) {
    auto& e=Get(key,Kind::Text);
    if(!AddQuestInfoCounter_ByValue_API) throw std::runtime_error("LiveFather counter API unavailable");
    return AddQuestInfoCounter_ByValue_API(m_game,&e.text,0,1.0f);
}
