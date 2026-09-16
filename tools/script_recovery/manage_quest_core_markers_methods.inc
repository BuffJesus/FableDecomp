void AddCoreQuestMarker(unsigned actor) {
    auto& thing=Get(actor,Kind::Thing).thing;
    if(!MiniMapAddMarker_API) throw std::runtime_error("Core quest marker API unavailable");
    struct Key {
        CCharString value{};
        Key(){CCharString_Construct_Literal(&value,"HUD_ORB_QUEST_CORE",-1);}
        ~Key(){CCharString_Destroy(&value);}
    } key;
    MiniMapAddMarker_API(m_game,&thing,&key.value);
}
void RemoveCoreQuestMarker(unsigned actor) {
    auto& thing=Get(actor,Kind::Thing).thing;
    if(!MiniMapRemoveMarker_API) throw std::runtime_error("Core quest marker API unavailable");
    MiniMapRemoveMarker_API(m_game,&thing);
}
