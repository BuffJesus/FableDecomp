// DB4D70 -> DB4D7D -> DB4D88, with the original unconditional participant call.
int LuaQuestState::StartConversationWithHero(CScriptThing* speaker, bool soundIn2D,
                                            bool playDuringCutscene) {
    if (!m_pGameInterface || !speaker || !AddNewConversation_API || !GetHero_API ||
        !AddPersonToConversation_API)
        throw std::runtime_error("Conversation APIs unavailable");
    int conversation = AddNewConversation_API(m_pGameInterface, speaker, soundIn2D, playDuringCutscene);
    CScriptThing* hero = GetHero_API(m_pGameInterface);
    AddPersonToConversation_API(m_pGameInterface, conversation, hero);
    return conversation;
}

// Proposed scoped binding: DB4E38 -> DB4E42 -> DB4E55 -> DB4E5F.
void LuaQuestState::AddConversationLineToHero(int conversation, const std::string& text,
                                           CScriptThing* speaker, bool subtitle) {
    if (!m_pGameInterface || !speaker || !GetHero_API || !AddLineToConversation_API)
        throw std::runtime_error("Conversation APIs unavailable");
    FableString key(text.c_str());
    // 891CA0 returns interface+0x30, owned by the interface. Do not copy/destroy it.
    CScriptThing* hero = GetHero_API(m_pGameInterface);
    AddLineToConversation_API(m_pGameInterface, conversation, key, subtitle, speaker, hero);
}
