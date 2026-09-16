// The key must exist before the second GetHero and survive AddLine.
void AddWifeWhereHusbandConversation(CScriptThing* actor) {
    CheckOpen();
    if(!AddNewConversation_API || !GetHero_API || !AddPersonToConversation_API || !AddLineToConversation_API)
        throw std::runtime_error("Wife conversation API unavailable");
    const int conversation=AddNewConversation_API(m_game,actor,false,false);
    AddPersonToConversation_API(m_game,conversation,GetHero_API(m_game));
    struct Key {
        CCharString value{};
        Key(){CCharString_Construct_Literal(&value,"TEXT_QST_048_AFFAIR_WIFE_WHERES_HUSBAND",-1);}
        ~Key(){CCharString_Destroy(&value);}
    } key;
    auto* hero=GetHero_API(m_game);
    AddLineToConversation_API(m_game,conversation,&key.value,false,actor,hero);
}
