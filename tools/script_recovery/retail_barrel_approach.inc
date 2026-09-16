    // DB60AA..DB60C5: read the live threshold BEFORE querying the borrowed hero.
    // Do not turn an empty hero into an early return or a copied Thing.
    bool IsHeroWithinBarrelApproachDistance(CScriptThing* actor) {
        CheckOpen();
        if (!actor || !m_game || !GetHero_API || !IsDistanceBetweenThingsUnder_API)
            throw std::runtime_error("Retail Barrel approach APIs are unavailable");
        const float distance = *ASLR<const volatile float*>(0x013AC858);
        CScriptThing* hero = GetHero_API(m_game);
        return IsDistanceBetweenThingsUnder_API(actor, hero, distance);
    }
    // DB5EEA..DB5F16: preserve both directed calls and the intervening hero query.
    void SetBarrelManHeroAllies(CScriptThing* actor) {
        CheckOpen();
        if (!actor || !m_game || !GetHero_API || !EntitySetThingAsAllyOfThing_API)
            throw std::runtime_error("Retail Barrel ally APIs are unavailable");
        CScriptThing* hero = GetHero_API(m_game);
        EntitySetThingAsAllyOfThing_API(m_game, actor, hero);
        hero = GetHero_API(m_game);
        EntitySetThingAsAllyOfThing_API(m_game, hero, actor);
    }
