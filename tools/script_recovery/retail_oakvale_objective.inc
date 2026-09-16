    void SetInitialOakvaleObjective() {
        CheckOpen();
        if (!CCharString_Construct_Literal || !CCharString_Destroy ||
            !GetActiveQuestName_API || !SetQuestCardObjective_API)
            throw std::runtime_error("Retail Oakvale objective APIs unavailable");
        CCharString region2{}, region1{}, objective{}, activeName{};
        bool region2Live=false, region1Live=false, objectiveLive=false, activeLive=false;
        auto cleanup=[&]() {
            if(activeLive){activeLive=false;try{CCharString_Destroy(&activeName);}catch(...) {}}
            if(objectiveLive){objectiveLive=false;try{CCharString_Destroy(&objective);}catch(...) {}}
            if(region1Live){region1Live=false;try{CCharString_Destroy(&region1);}catch(...) {}}
            if(region2Live){region2Live=false;try{CCharString_Destroy(&region2);}catch(...) {}}
        };
        try {
            CCharString_Construct_Literal(&region2,"",-1);region2Live=true;
            CCharString_Construct_Literal(&region1,"",-1);region1Live=true;
            CCharString_Construct_Literal(&objective,"TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_01",-1);objectiveLive=true;
            auto* returnedName=GetActiveQuestName_API(m_game,&activeName);activeLive=true;
            // Native passes the returned pointer, but destroys its owned output.
            SetQuestCardObjective_API(m_game,returnedName,&objective,&region1,&region2);
            activeLive=false;CCharString_Destroy(&activeName);
            objectiveLive=false;CCharString_Destroy(&objective);
            region1Live=false;CCharString_Destroy(&region1);
            region2Live=false;CCharString_Destroy(&region2);
        } catch(...) {cleanup();throw;}
    }
