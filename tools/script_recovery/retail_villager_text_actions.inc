    void AddVillagerAmbientText(int conversation,unsigned text,CScriptThing* actor) {
        auto& key=Get(text,Kind::Text).text;
        if(!GetHero_API || !AddLineToConversation_API)
            throw std::runtime_error("Retail Villager ambient conversation unavailable");
        auto* hero=GetHero_API(m_game);
        AddLineToConversation_API(m_game,conversation,&key,false,actor,hero);
    }
    int StartVillagerTalkConversation(CScriptThing* actor) {
        CheckOpen();
        if(!GetHero_API || !EntitySetFacingAngleTowardsThing_API ||
                !AddNewConversation_API || !AddPersonToConversation_API)
            throw std::runtime_error("Retail Villager conversation setup unavailable");
        auto* facingHero=GetHero_API(m_game);
        EntitySetFacingAngleTowardsThing_API(m_game,actor,facingHero,false);
        auto conversation=AddNewConversation_API(m_game,actor,false,false);
        auto* participant=GetHero_API(m_game);
        AddPersonToConversation_API(m_game,conversation,participant);
        return conversation;
    }
    void AssignVillagerSuffix(unsigned text,bool male) {
        auto& suffix=Get(text,Kind::Text).text;
        if(!CCharString_AssignLiteral_API)throw std::runtime_error("Retail text assignment unavailable");
        CCharString_AssignLiteral_API(&suffix,male?"_MALE":"_FEMALE");
    }
    void AddVillagerTalkLine(int conversation,unsigned text,CScriptThing* actor,bool hit) {
        auto& suffix=Get(text,Kind::Text).text;
        if(!GetHero_API || !AddLineToConversation_API || !CCharString_Destroy)
            throw std::runtime_error("Retail Villager conversation APIs unavailable");
        auto* hero=GetHero_API(m_game);
        CCharString prefix{},result{};bool prefixLive=false,resultLive=false;
        try {
            using Literal=CCharString*(__thiscall*)(CCharString*,const char*,int);
            auto* left=ASLR<Literal>(0x0099EBF0)(&prefix,hit?
                "TEXT_QST_048_VILLAGER_DONE_BAD_DEEDS":"TEXT_QST_048_VILLAGER_SPOKEN_TO",-1);
            prefixLive=true;
            using Concat=CCharString*(__fastcall*)(CCharString*,const CCharString*,const CCharString*);
            auto* line=ASLR<Concat>(0x0099F570)(&result,left,&suffix);resultLive=true;
            AddLineToConversation_API(m_game,conversation,line,false,actor,hero);
            resultLive=false;CCharString_Destroy(&result);
            prefixLive=false;CCharString_Destroy(&prefix);
        } catch(...) {
            if(resultLive)try {resultLive=false;CCharString_Destroy(&result);}catch(...) {}
            if(prefixLive)try {prefixLive=false;CCharString_Destroy(&prefix);}catch(...) {}
            throw;
        }
    }
