    void InitializeVillager(CScriptThing* actor) {
        CheckOpen();
        if(!EntitySetAsDamageable_API || !EntitySetAsKillable_API ||
                !EntitySetAsToAddToComboMultiplierWhenHit_API || !GetHero_API || !EntitySetThingAsAllyOfThing_API)
            throw std::runtime_error("Retail Villager initialization unavailable");
        EntitySetAsDamageable_API(m_game,actor,false);
        EntitySetAsKillable_API(m_game,actor,false,false);
        EntitySetAsToAddToComboMultiplierWhenHit_API(m_game,actor,false);
        auto* hero=GetHero_API(m_game);
        EntitySetThingAsAllyOfThing_API(m_game,actor,hero);
    }
    bool WasVillagerHit(CScriptThing* actor) {
        CheckOpen();
        if(!actor || !actor->pVTable || !CCharString_Construct_Literal || !CCharString_Destroy)
            throw std::runtime_error("Retail Villager hit query unavailable");
        auto* table=reinterpret_cast<CScriptThingVTable*>(actor->pVTable);
        CCharString keys[3]{};unsigned live=0;
        auto construct=[&]() {auto* key=&keys[live];CCharString_Construct_Literal(key,"SCRIPT_NAME_HERO",-1);++live;return key;};
        auto cleanup=[&]() {while(live){--live;CCharString_Destroy(&keys[live]);}};
        bool hit;
        try {
            hit=table->MsgIsHitBy(actor,construct());
            if(!hit && table->MsgIsHitByAnySpecialAbilityFrom(actor,construct()))
                hit=!table->MsgIsHitBySpecialAbilityFrom(actor,static_cast<EHeroAbility>(14),construct());
        } catch(...) {while(live)try {--live;CCharString_Destroy(&keys[live]);}catch(...) {} throw;}
        try {cleanup();}catch(...) {while(live)try {--live;CCharString_Destroy(&keys[live]);}catch(...) {} throw;}
        return hit;
    }
    bool WasVillagerTalkedTo(CScriptThing* actor) {
        CheckOpen();
        if(!actor || !actor->pVTable || !CCharString_Construct_Literal || !CCharString_Destroy)
            throw std::runtime_error("Retail Villager talk query unavailable");
        auto* table=reinterpret_cast<CScriptThingVTable*>(actor->pVTable);
        CCharString key{};CCharString_Construct_Literal(&key,"SCRIPT_NAME_HERO",-1);
        bool talked;
        try {talked=table->MsgIsTalkedToBy(actor,&key);}
        catch(...) {try {CCharString_Destroy(&key);}catch(...) {} throw;}
        CCharString_Destroy(&key);return talked;
    }
    void SetVillagerHeroAllies(CScriptThing* actor) { SetBarrelManHeroAllies(actor); }
