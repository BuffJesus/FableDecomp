    // Staged LuaRetailResources method. Native DBB613..DBB6B9 keeps the
    // talker's CString alive until after the optional teddy CString is destroyed.
    bool BullyTalkedWithTeddy(CScriptThing* actor) {
        CheckOpen();
        if (!actor || !actor->pVTable || !GetHero_API || !IsObjectInThingsPossession_API)
            throw std::runtime_error("Bully talk predicate APIs unavailable");
        auto* table = reinterpret_cast<CScriptThingVTable*>(actor->pVTable);
        if (!table->MsgIsTalkedToBy) throw std::runtime_error("Bully talk slot unavailable");
        FableString who("SCRIPT_NAME_HERO");
        if (!table->MsgIsTalkedToBy(actor, who.get())) return false;
        FableString teddy("OBJECT_TEDDY_BEAR_UNGIVEABLE");
        auto* hero = GetHero_API(m_game);
        return IsObjectInThingsPossession_API(m_game, teddy.get(), hero);
    }
