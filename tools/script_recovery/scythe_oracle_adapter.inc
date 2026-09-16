// Proposal only. Include after LuaQuestState and LuaEntityAPI declarations.
// Caller owns the movie, pause, and Scythe control. Hero is a fresh native local.
void LuaQuestState::RunScytheOracleCutscene(CScriptThing* scythe, sol::this_state state) {
    lua_State* luaState = state;
    const auto movie = m_movieHandlesByLuaState.find(luaState);
    auto* control = scythe ? LuaEntityAPI::FindControlHandle(scythe) : nullptr;
    if (movie == m_movieHandlesByLuaState.end() || !movie->second || !control)
        throw std::runtime_error("Scythe cutscene requires caller movie and Scythe control");
    if (!m_pGameInterface || !GetHero_API || !StartScriptingEntity_API ||
        !Game_malloc || !Game_free || !CBaseObject_Construct_API ||
        !g_pCScriptGameResourceObjectScriptedThingBaseVTable ||
        !CSGROSTB_Destroy_API || !CBaseObject_Assign_API ||
        !StdMap_Construct_API || !StdMap_Destroy_API || !StdMap_OperatorBracket_API ||
        !StdMap_String_Construct_API || !StdMap_String_Destroy_API ||
        !RunCutsceneMacro_Func || !FixMovieSequenceCamera_API)
        throw std::runtime_error("Scythe cutscene APIs unavailable");

    void* actors = Game_malloc(0x24);
    void* inputs = Game_malloc(0x24);
    if (!actors || !inputs) {
        if (actors) Game_free(actors);
        if (inputs) Game_free(inputs);
        throw std::bad_alloc();
    }
    StdMap_Construct_API(actors);
    StdMap_String_Construct_API(inputs); // A null pointer has different retail semantics.
    CScriptGameResourceObjectScriptedThingBase hero{};
    CBaseObject_Construct_API(&hero);
    hero.pVTable = g_pCScriptGameResourceObjectScriptedThingBaseVTable;
    hero.pImp.Data = nullptr;
    hero.pImp.Info = nullptr;
    bool cameraFixed = false;
    auto cleanup = [&]() {
        if (cameraFixed) FixMovieSequenceCamera_API(m_pGameInterface, false);
        StdMap_String_Destroy_API(inputs);
        Game_free(inputs);
        StdMap_Destroy_API(actors);
        Game_free(actors);
        CSGROSTB_Destroy_API(&hero);
    };
    try {
        FableString scytheKey("SCYTHE");
        CBaseObject_Assign_API(StdMap_OperatorBracket_API(actors, scytheKey), control);
        // E2A93E: one attempt at priority 4, AL ignored even on failure.
        // Do not reuse a cached Hero handle, retry, yield, or test termination here.
        (void)StartScriptingEntity_API(m_pGameInterface, GetHero_API(m_pGameInterface),
                                      &hero, static_cast<EScriptAIPriority>(4));
        FableString heroKey("HERO");
        CBaseObject_Assign_API(StdMap_OperatorBracket_API(actors, heroKey), &hero);
        FixMovieSequenceCamera_API(m_pGameInterface, true);
        cameraFixed = true;
        FableString macro("CS_ORACLE_AWAKENS");
        RunCutsceneMacro_Func(macro, actors, nullptr, inputs, false, true);
    } catch (...) {
        cleanup();
        throw;
    }
    cleanup();
}
