    static void** PostAttackTable(CGameScriptInterfaceBase* game) {
        if (!game || !*reinterpret_cast<void***>(game))
            throw std::runtime_error("Post-attack interface unavailable");
        return *reinterpret_cast<void***>(game);
    }
    template<class T> static T PostAttackMethod(void** table, unsigned slot) {
        if (!table || !table[slot/4]) throw std::runtime_error("Post-attack method unavailable");
        return reinterpret_cast<T>(table[slot/4]);
    }
    template<class Action> bool WithPostAttackLookup(const char* name, Action action) {
        CheckOpen();
        if (!CCharString_Construct_Literal || !CCharString_Destroy || !RetailThing_Destroy_API)
            throw std::runtime_error("Post-attack ownership APIs unavailable");
        CCharString key{};
        CScriptThing output{};
        bool keyLive=false, outputLive=false;
        try {
            CCharString_Construct_Literal(&key,name,-1);keyLive=true;
            auto** table=PostAttackTable(m_game);
            auto* returned=PostAttackMethod<tGetThingWithScriptName1>(table,0x120)(m_game,&output,&key);
            outputLive=true;
            const bool result=action(returned,table);
            outputLive=false;RetailThing_Destroy_API(&output);
            keyLive=false;CCharString_Destroy(&key);
            return result;
        } catch (...) {
            // A throwing cleanup cannot replace the first failure or run twice.
            if(outputLive){outputLive=false;try{RetailThing_Destroy_API(&output);}catch(...) {}}
            if(keyLive){keyLive=false;try{CCharString_Destroy(&key);}catch(...) {}}
            throw;
        }
    }
    bool PostAttackStartIsAlive() {
        return WithPostAttackLookup("M_PostAttackStart",[](CScriptThing* returned,void**) {
            if(!returned)throw std::runtime_error("Post-attack alive receiver unavailable");
            auto* table=reinterpret_cast<CScriptThingVTable*>(returned->pVTable);
            if(!table || !table->IsAlive)throw std::runtime_error("Post-attack alive method unavailable");
            return table->IsAlive(returned);
        });
    }
    void TeleportToPostAttackStart() {
        WithPostAttackLookup("M_PostAttackStart",[&](CScriptThing* returned,void** table) {
            auto* hero=PostAttackMethod<tGetHero>(PostAttackTable(m_game),0x118)(m_game);
            // Retail retains the table pointer, but loads its slot after GetHero.
            PostAttackMethod<tEntityTeleportToThing>(table,0x760)(m_game,hero,returned,false);
            return true;
        });
    }
    void SetPostAttackVillageLimbo(bool value) {
        WithPostAttackLookup("V_OakVale",[&](CScriptThing* returned,void** table) {
            PostAttackMethod<tSetVillageLimbo>(table,0x6e0)(m_game,returned,value);
            return true;
        });
    }
    bool PostAttackHeroNearTrigger(unsigned id) {
        auto& trigger=Get(id,Kind::Thing).thing;
        if(!IsDistanceBetweenThingsUnder_API)throw std::runtime_error("Post-attack distance API unavailable");
        auto* hero=PostAttackMethod<tGetHero>(PostAttackTable(m_game),0x118)(m_game);
        return IsDistanceBetweenThingsUnder_API(hero,&trigger,5.0f);
    }
