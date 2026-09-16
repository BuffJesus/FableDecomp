// Staged only: declare and bind separately on LuaQuestState.
// Native EC4540 uses the bound Thing even when its Data is empty.
void LuaQuestState::InitializeRockTrollMarker(CScriptThing* actor) {
    if (!actor || !m_pGameInterface || !SetThingPersistent_API || !MiniMapAddMarker_API)
        throw std::runtime_error("InitializeRockTrollMarker requires bound actor and APIs");
    SetThingPersistent_API(m_pGameInterface, actor, true);
    FableString marker("HUD_ORB_RED_SMALL");
    MiniMapAddMarker_API(m_pGameInterface, actor, marker.get());
}
