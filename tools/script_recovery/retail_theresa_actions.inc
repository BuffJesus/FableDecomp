    void SpeakTheresa(unsigned id,const std::string& line) {
        CheckOpen();
        if(!GetHero_API)throw std::runtime_error("Retail Theresa Hero unavailable");
        auto* hero=GetHero_API(m_game);
        auto* expert=Get(id,Kind::Resource).resource.pImp.Data;
        if(!expert)return;
        auto* table=reinterpret_cast<CScriptGameResourceObjectScriptedThingBaseVTable*>(expert->pVTable);
        if(!table || !table->Speak)throw std::runtime_error("Retail Theresa speech unavailable");
        table->Speak(expert,hero,line.c_str(),static_cast<ETextGroupSelectionMethod>(0),false,true,false);
    }
    bool TryAcquireTheresaHero(unsigned id,int priority) {
        CheckOpen();
        auto& resource=Get(id,Kind::Resource).resource;
        if(!GetHero_API || !StartScriptingEntity_API)throw std::runtime_error("Retail Theresa Hero control unavailable");
        auto* hero=GetHero_API(m_game);
        return StartScriptingEntity_API(m_game,hero,&resource,static_cast<EScriptAIPriority>(priority));
    }
    bool DoesTheresaHeroHaveChocolates() {
        CheckOpen();
        if(!GetHero_API || !IsObjectInThingsPossession_API || !CCharString_Construct_Literal || !CCharString_Destroy)
            throw std::runtime_error("Retail Theresa possession query unavailable");
        CCharString key{};
        CCharString_Construct_Literal(&key,"OBJECT_CHOCOLATE_BOX_UNGIVEABLE",-1);
        bool has;
        try {auto* hero=GetHero_API(m_game);has=IsObjectInThingsPossession_API(m_game,&key,hero);}
        catch(...) {try {CCharString_Destroy(&key);}catch(...) {} throw;}
        CCharString_Destroy(&key);return has;
    }
    void TakeTheresaChocolatesAndUpdateObjective() {
        CheckOpen();
        if(!CCharString_Construct_Literal || !CCharString_Destroy || !TakeObjectFromHero_API ||
                !GetActiveQuestName_API || !SetQuestCardObjective_API)
            throw std::runtime_error("Retail Theresa gift update unavailable");
        CCharString item{};
        CCharString_Construct_Literal(&item,"OBJECT_CHOCOLATE_BOX_UNGIVEABLE",-1);
        try {TakeObjectFromHero_API(m_game,&item);}
        catch(...) {try {CCharString_Destroy(&item);}catch(...) {} throw;}
        CCharString_Destroy(&item);
        CCharString keys[4]{};unsigned live=0;
        const char* text[]={"","","TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_05"};
        try {
            for(unsigned i=0;i<3;++i){CCharString_Construct_Literal(&keys[i],text[i],-1);++live;}
            auto* name=GetActiveQuestName_API(m_game,&keys[3]);++live;
            SetQuestCardObjective_API(m_game,name,&keys[2],&keys[1],&keys[0]);
        } catch(...) {while(live)try {CCharString_Destroy(&keys[--live]);}catch(...) {} throw;}
        try {while(live)CCharString_Destroy(&keys[--live]);}
        catch(...) {while(live)try {CCharString_Destroy(&keys[--live]);}catch(...) {} throw;}
    }
    void ClearTheresaInformation(CScriptThing* actor) {
        CheckOpen();
        if(!ClearThingHasInformation_API)throw std::runtime_error("Retail Theresa information API unavailable");
        ClearThingHasInformation_API(m_game,actor);
    }
    bool IsTheresaNearHero(CScriptThing* actor, float distance) {
        CheckOpen();
        if(!GetHero_API || !IsDistanceBetweenThingsUnder_API)
            throw std::runtime_error("Retail Theresa proximity unavailable");
        auto* hero=GetHero_API(m_game);
        return IsDistanceBetweenThingsUnder_API(actor,hero,distance);
    }
    void PlayTheresaSkip(unsigned id) {
        CheckOpen();
        if(!CCharString_Construct_Literal || !CCharString_Destroy)
            throw std::runtime_error("Retail Theresa animation strings unavailable");
        CCharString key{};
        CCharString_Construct_Literal(&key,"SKIP",-1);
        try {
            auto* expert=Get(id,Kind::Resource).resource.pImp.Data;
            if(expert) {
                auto* table=reinterpret_cast<CScriptGameResourceObjectScriptedThingBaseVTable*>(expert->pVTable);
                if(!table || !table->PlayCombatAnimation)
                    throw std::runtime_error("Retail Theresa combat animation unavailable");
                table->PlayCombatAnimation(expert,&key,true,false,false,true,false,false);
            }
        } catch(...) {try {CCharString_Destroy(&key);}catch(...) {} throw;}
        CCharString_Destroy(&key);
    }
    void ShowTheresaChocolateQuestion() {
        CheckOpen();
        if(!CCharString_Construct_Literal || !CCharString_Destroy || !m_game)
            throw std::runtime_error("Retail Theresa question unavailable");
        auto** table=*reinterpret_cast<void***>(m_game);
        using Question=void(__thiscall*)(CGameScriptInterfaceBase*,const CCharString*,const CCharString*,const CCharString*,const CCharString*,bool);
        if(!table || !table[0x1c8/4])throw std::runtime_error("Retail Theresa question API unavailable");
        CCharString keys[4]{};unsigned live=0;
        const char* text[]={"","TEXT_OBJECT_HERO_ANSWER_NO","TEXT_OBJECT_HERO_ANSWER_YES","TEXT_QST_048_GIVE_CHOCOLATE_BOX"};
        try {
            for(unsigned i=0;i<4;++i){CCharString_Construct_Literal(&keys[i],text[i],-1);++live;}
            reinterpret_cast<Question>(table[0x1c8/4])(m_game,&keys[3],&keys[2],&keys[1],&keys[0],true);
        } catch(...) {while(live)try {CCharString_Destroy(&keys[--live]);}catch(...) {} throw;}
        try {while(live)CCharString_Destroy(&keys[--live]);}
        catch(...) {while(live)try {CCharString_Destroy(&keys[--live]);}catch(...) {} throw;}
    }
