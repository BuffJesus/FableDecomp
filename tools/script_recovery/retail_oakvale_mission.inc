    void TurnOakvaleHeroIntoChild() {
        CheckOpen();
        if(!CCharString_Construct_Literal || !CCharString_Destroy || !RetailThing_Destroy_API)
            throw std::runtime_error("Oakvale mission ownership APIs unavailable");
        CCharString key{}; CScriptThing output{}; bool keyLive=false, outputLive=false;
        try {
            CCharString_Construct_Literal(&key,"CREATURE_HERO_CHILD",-1);keyLive=true;
            auto** table=PostAttackTable(m_game);
            auto* hero=PostAttackMethod<tGetHero>(table,0x118)(m_game);
            PostAttackMethod<tTurnCreatureInto>(table,0x178)(m_game,&output,hero,&key);outputLive=true;
            outputLive=false;RetailThing_Destroy_API(&output);
            keyLive=false;CCharString_Destroy(&key);
        }catch(...) {
            if(outputLive){outputLive=false;try{RetailThing_Destroy_API(&output);}catch(...) {}}
            if(keyLive){keyLive=false;try{CCharString_Destroy(&key);}catch(...) {}}
            throw;
        }
    }
    void PrepareOakvaleHouseAndStartScreen() {
        CheckOpen();
        if(!CCharString_Construct_Literal || !CCharString_Destroy || !RetailThing_Destroy_API)
            throw std::runtime_error("Oakvale mission ownership APIs unavailable");
        CCharString key{},active{};CScriptThing house{};
        bool keyLive=false,houseLive=false,activeLive=false;
        try {
            CCharString_Construct_Literal(&key,"HerosOldHouse",-1);keyLive=true;
            PostAttackMethod<tGetThingWithScriptName1>(PostAttackTable(m_game),0x120)(m_game,&house,&key);houseLive=true;
            keyLive=false;CCharString_Destroy(&key);
            // This caller uses its owned output even if lookup returns an alias.
            PostAttackMethod<tOverrideAutomaticHouseLocking>(PostAttackTable(m_game),0x6bc)(m_game,&house,true);
            PostAttackMethod<tOpenHouseDoors>(PostAttackTable(m_game),0x6ac)(m_game,&house);
            auto** table=PostAttackTable(m_game);
            auto* returned=PostAttackMethod<tGetActiveQuestName>(table,0xa3c)(m_game,&active);activeLive=true;
            PostAttackMethod<tKickOffQuestStartScreen>(table,0x4d4)(m_game,returned,false,true);
            activeLive=false;CCharString_Destroy(&active);
            PostAttackMethod<tOverrideMusic>(PostAttackTable(m_game),0xae0)(m_game,static_cast<EMusicSetType>(19),false,true);
            houseLive=false;RetailThing_Destroy_API(&house);
        }catch(...) {
            if(activeLive){activeLive=false;try{CCharString_Destroy(&active);}catch(...) {}}
            if(houseLive){houseLive=false;try{RetailThing_Destroy_API(&house);}catch(...) {}}
            if(keyLive){keyLive=false;try{CCharString_Destroy(&key);}catch(...) {}}
            throw;
        }
    }
    void SetOakvaleHeroKillable(bool value) {
        CheckOpen();auto** table=PostAttackTable(m_game);
        auto* hero=PostAttackMethod<tGetHero>(table,0x118)(m_game);
        PostAttackMethod<tEntitySetAsKillable>(table,0x814)(m_game,hero,value,false);
    }
    void FinishOakvaleActiveQuest(bool deactivate) {
        CheckOpen();
        if(!CCharString_Destroy)throw std::runtime_error("Oakvale mission string destructor unavailable");
        CCharString active{};bool live=false;
        try {
            auto** table=PostAttackTable(m_game);
            auto* returned=PostAttackMethod<tGetActiveQuestName>(table,0xa3c)(m_game,&active);live=true;
            if(deactivate)PostAttackMethod<tDeactivateQuestLater>(table,0x464)(m_game,returned,0);
            else PostAttackMethod<tSetQuestAsCompleted>(table,0x480)(m_game,returned,false,false,false);
            live=false;CCharString_Destroy(&active);
        }catch(...) {
            if(live){live=false;try{CCharString_Destroy(&active);}catch(...) {}}
            throw;
        }
    }
