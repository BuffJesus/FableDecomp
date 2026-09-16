    void FaceBarrelManTowardsHero(CScriptThing* actor) {
        CheckOpen();
        if (!m_game || !GetHero_API || !EntitySetFacingAngleTowardsThing_API)
            throw std::runtime_error("Retail Barrel facing APIs unavailable");
        auto* hero=GetHero_API(m_game);
        EntitySetFacingAngleTowardsThing_API(m_game,actor,hero,false);
    }
    bool ShouldBarrelManThankHero(CScriptThing* actor) {
        CheckOpen();
        if (!m_game || !GetHero_API || !CanThingBe_Seen_ByOtherThing_API)
            throw std::runtime_error("Retail Barrel visibility APIs unavailable");
        auto* hero=GetHero_API(m_game);
        if (CanThingBe_Seen_ByOtherThing_API(m_game,hero,actor)) return true;
        if (!IsDistanceBetweenThingsUnder_API)
            throw std::runtime_error("Retail Barrel distance API unavailable");
        hero=GetHero_API(m_game);
        return IsDistanceBetweenThingsUnder_API(actor,hero,10.0f);
    }
