    void InitializeBarrelThugActor(CScriptThing* actor) {
        CheckOpen();
        if (!actor || !EntitySetAsDamageable_API || !EntitySetAsKillable_API ||
            !EntitySetAsToAddToComboMultiplierWhenHit_API)
            throw std::runtime_error("BarrelThug Init APIs unavailable");
        EntitySetAsDamageable_API(m_game,actor,false);
        EntitySetAsKillable_API(m_game,actor,false,false);
        EntitySetAsToAddToComboMultiplierWhenHit_API(m_game,actor,false);
    }
    void ResetResource(unsigned id) {
        auto& entry=Get(id,Kind::Resource);
        if (!InitScriptObjectHelper2_API) throw std::runtime_error("Retail resource reset unavailable");
        InitScriptObjectHelper2_API(&entry.resource);
    }
    void SpeakBarrelThug(unsigned id,const std::string& line,int selection) {
        CheckOpen();
        if (!GetHero_API) throw std::runtime_error("BarrelThug Hero unavailable");
        auto* hero=GetHero_API(m_game);
        Speak(id,hero,line,selection,false,true,false);
    }
    void FollowBarrelThugHero(unsigned id) {
        CheckOpen();
        if (!GetHero_API) throw std::runtime_error("BarrelThug Hero unavailable");
        auto* hero=GetHero_API(m_game);
        auto* expert=Get(id,Kind::Resource).resource.pImp.Data;
        if (!expert) return;
        auto* table=reinterpret_cast<CScriptGameResourceObjectScriptedThingBaseVTable*>(expert->pVTable);
        if (!table || !table->FollowThing) throw std::runtime_error("BarrelThug follow unavailable");
        table->FollowThing(expert,hero,1.0f,true);
    }
    void PlaceBarrelThugAtStart(CScriptThing* actor) {
        CheckOpen();
        if (!actor || !CCharString_Construct_Literal || !CCharString_Destroy ||
            !GetThingWithScriptName_ByName_API || !EntityTeleportToThing_API ||
            !RetailThing_Destroy_API || !GetHero_API || !EntitySetFacingAngleTowardsThing_API)
            throw std::runtime_error("BarrelThug placement APIs unavailable");
        CCharString key{};
        CCharString_Construct_Literal(&key,"M_WHouse_ManStart",-1);
        CScriptThing marker{};
        bool markerLive=false;
        try {
            auto* target=GetThingWithScriptName_ByName_API(m_game,&marker,&key);
            markerLive=true;
            EntityTeleportToThing_API(m_game,actor,target,false);
            markerLive=false;RetailThing_Destroy_API(&marker);
        } catch (...) {
            if(markerLive) try {RetailThing_Destroy_API(&marker);} catch (...) {}
            try {CCharString_Destroy(&key);} catch (...) {}
            throw;
        }
        CCharString_Destroy(&key);
        auto* hero=GetHero_API(m_game);
        EntitySetFacingAngleTowardsThing_API(m_game,actor,hero,false);
    }
    int NewBarrelThugRemarkConversation(CScriptThing* actor) {
        CheckOpen();
        if (!actor || !AddNewConversation_API || !GetHero_API || !AddPersonToConversation_API)
            throw std::runtime_error("BarrelThug conversation APIs unavailable");
        int conversation=AddNewConversation_API(m_game,actor,false,false);
        auto* hero=GetHero_API(m_game);
        AddPersonToConversation_API(m_game,conversation,hero);
        return conversation;
    }
    void AddBarrelThugRemark(int conversation,CScriptThing* actor,const std::string& line) {
        CheckOpen();
        if (!actor || !CCharString_Construct_Literal || !CCharString_Destroy || !GetHero_API || !AddLineToConversation_API)
            throw std::runtime_error("BarrelThug remark APIs unavailable");
        CCharString key{};
        CCharString_Construct_Literal(&key,line.c_str(),-1);
        try {
            auto* hero=GetHero_API(m_game);
            AddLineToConversation_API(m_game,conversation,&key,false,actor,hero);
        } catch (...) {try {CCharString_Destroy(&key);} catch (...) {} throw;}
        CCharString_Destroy(&key);
    }
