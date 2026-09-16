    // DBA3E3: fresh Hero first, retained M_TriggerOutro second, radius 2.0.
    bool IsTheresaHeroNearTrigger(unsigned id) {
        CheckOpen();
        auto& trigger = Get(id, Kind::Thing).thing;
        if (!GetHero_API || !IsDistanceBetweenThingsUnder_API)
            throw std::runtime_error("Theresa departure distance APIs unavailable");
        auto* hero = GetHero_API(m_game);
        return IsDistanceBetweenThingsUnder_API(hero, &trigger, 2.0f);
    }
    unsigned NewTheresaDepartureTrigger() {
        CheckOpen();
        if (!RetailThing_Destroy_API || !GetThingWithScriptName_ByName_API || !g_pCScriptThingVTable ||
            !CCharString_Construct_Literal || !CCharString_Destroy)
            throw std::runtime_error("Theresa departure lookup APIs unavailable");
        auto& entry = Add(Kind::Thing);
        entry.thing.pVTable = g_pCScriptThingVTable;
        entry.live = true;
        CCharString key{};
        bool constructed = false;
        try {
            CCharString_Construct_Literal(&key,"M_TriggerOutro",-1);
            constructed = true;
            GetThingWithScriptName_ByName_API(m_game,&entry.thing,&key);
            constructed = false;
            CCharString_Destroy(&key);
        } catch (...) {
            if (constructed) { try { CCharString_Destroy(&key); } catch (...) {} }
            Destroy(entry);
            throw;
        }
        return entry.id;
    }
