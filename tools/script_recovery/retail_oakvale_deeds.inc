    void ApplyOakvaleDeedMorality(bool good) {
        CheckOpen();
        auto* definitions=*ASLR<const unsigned char**>(0x0143E90C);
        if(!definitions)throw std::runtime_error("Oakvale script definition unavailable");
        const float amount=*reinterpret_cast<const volatile float*>(definitions+0xd64);
        auto** table=PostAttackTable(m_game);
        PostAttackMethod<tGiveHeroMorality>(table,0x270)(m_game,good?amount:-amount);
    }
    void SetOakvaleDeedObjective() {
        CheckOpen();
        if(!CCharString_Construct_Literal || !CCharString_Destroy || !GetActiveQuestName_API || !SetQuestCardObjective_API)
            throw std::runtime_error("Oakvale deed objective APIs unavailable");
        CCharString region2{},region1{},objective{},active{};
        bool r2=false,r1=false,obj=false,name=false;
        try {
            CCharString_Construct_Literal(&region2,"",-1);r2=true;
            CCharString_Construct_Literal(&region1,"",-1);r1=true;
            CCharString_Construct_Literal(&objective,"TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_02",-1);obj=true;
            auto* returned=GetActiveQuestName_API(m_game,&active);name=true;
            SetQuestCardObjective_API(m_game,returned,&objective,&region1,&region2);
            name=false;CCharString_Destroy(&active);
            obj=false;CCharString_Destroy(&objective);
            r1=false;CCharString_Destroy(&region1);
            r2=false;CCharString_Destroy(&region2);
        }catch(...){
            if(name){name=false;try{CCharString_Destroy(&active);}catch(...) {}}
            if(obj){obj=false;try{CCharString_Destroy(&objective);}catch(...) {}}
            if(r1){r1=false;try{CCharString_Destroy(&region1);}catch(...) {}}
            if(r2){r2=false;try{CCharString_Destroy(&region2);}catch(...) {}}
            throw;
        }
    }
