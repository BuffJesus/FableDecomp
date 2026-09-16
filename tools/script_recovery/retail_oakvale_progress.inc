    void SetOakvaleProgressObjective(bool hasGold) {
        CheckOpen();
        if(!CCharString_Construct_Literal || !CCharString_Destroy)
            throw std::runtime_error("Oakvale objective strings unavailable");
        CCharString region2{},region1{},objective{},active{};
        bool r2=false,r1=false,obj=false,name=false;
        try {
            CCharString_Construct_Literal(&region2,"",-1);r2=true;
            CCharString_Construct_Literal(&region1,"",-1);r1=true;
            CCharString_Construct_Literal(&objective,hasGold?"TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_03":"TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_06",-1);obj=true;
            auto** table=PostAttackTable(m_game);
            auto* returned=PostAttackMethod<tGetActiveQuestName>(table,0xa3c)(m_game,&active);name=true;
            PostAttackMethod<tSetQuestCardObjective>(table,0x4a0)(m_game,returned,&objective,&region1,&region2);
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
