    bool TryAcquirePostAttackHero(unsigned id, int priority) {
        CheckOpen();
        auto& resource = Get(id, Kind::Resource).resource;
        if (!GetHero_API || !StartScriptingEntity_API)
            throw std::runtime_error("Retail post-attack Hero control unavailable");
        auto* hero = GetHero_API(m_game);
        // Retail forwards an empty Hero and ignores a failed acquisition result.
        return StartScriptingEntity_API(m_game, hero, &resource,
                                       static_cast<EScriptAIPriority>(priority));
    }
