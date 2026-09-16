// Unapplied quest method. Native EC4FB2/4FCF outputs are borrowed, not copied.
void LuaQuestState::TargetRockTrollAtHero(CScriptThing* troll) {
    if (!m_pGameInterface || !troll || !GetHero_API ||
        !EntityForceToLookAtThing_API || !GiveThingBestEnemyTarget_API)
        throw std::runtime_error("Rock Troll targeting APIs unavailable");
    auto* hero = GetHero_API(m_pGameInterface);
    EntityForceToLookAtThing_API(m_pGameInterface, troll, hero);
    hero = GetHero_API(m_pGameInterface);
    GiveThingBestEnemyTarget_API(m_pGameInterface, troll, hero);
}
