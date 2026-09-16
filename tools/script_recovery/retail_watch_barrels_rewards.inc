    void RewardRemainingBarrel() {
        CheckOpen();
        if (!CCharString_Construct_Literal || !CCharString_Destroy || !GetThingWithScriptName_ByName_API ||
            !AddItemToContainer_API || !RetailThing_Destroy_API)
            throw std::runtime_error("Barrel reward APIs unavailable");
        CCharString name{},item{};
        CScriptThing barrel{};
        bool nameLive=false,itemLive=false,barrelLive=false;
        auto cleanup=[&]() {
            if(itemLive){itemLive=false;try {CCharString_Destroy(&item);}catch(...) {}}
            if(nameLive){nameLive=false;try {CCharString_Destroy(&name);}catch(...) {}}
            if(barrelLive){barrelLive=false;try {RetailThing_Destroy_API(&barrel);}catch(...) {}}
        };
        try {
            CCharString_Construct_Literal(&name,"NOVI_Barrel",-1);nameLive=true;
            // Native uses the output local, not the returned alias.
            GetThingWithScriptName_ByName_API(m_game,&barrel,&name);barrelLive=true;
            nameLive=false;CCharString_Destroy(&name);
            CCharString_Construct_Literal(&item,"OBJECT_GOLD_1",-1);itemLive=true;
            AddItemToContainer_API(m_game,&barrel,&item);
            itemLive=false;CCharString_Destroy(&item);
            barrelLive=false;RetailThing_Destroy_API(&barrel);
        } catch(...) {cleanup();throw;}
    }
    void SpawnBarrelBeetle(sol::table position) {
        CheckOpen();
        if (!CCharString_Construct_Literal || !CCharString_Destroy || !CreateCreature_API ||
            !EntitySetMaxHealth_API || !RetailThing_Destroy_API)
            throw std::runtime_error("Barrel beetle APIs unavailable");
        C3DVector point={position["x"].get<float>(),position["y"].get<float>(),position["z"].get<float>()};
        CCharString script{},definition{};
        CScriptThing beetle{};
        bool scriptLive=false,definitionLive=false,beetleLive=false;
        auto cleanup=[&]() {
            if(definitionLive){definitionLive=false;try {CCharString_Destroy(&definition);}catch(...) {}}
            if(scriptLive){scriptLive=false;try {CCharString_Destroy(&script);}catch(...) {}}
            if(beetleLive){beetleLive=false;try {RetailThing_Destroy_API(&beetle);}catch(...) {}}
        };
        try {
            CCharString_Construct_Literal(&script,"NOVI_CreatedBeetle",-1);scriptLive=true;
            CCharString_Construct_Literal(&definition,"CREATURE_OAKVALE_STAG_BEETLE",-1);definitionLive=true;
            // Native ignores the returned alias, then closes both names before health.
            CreateCreature_API(m_game,&beetle,&definition,&point,&script,false);beetleLive=true;
            definitionLive=false;CCharString_Destroy(&definition);
            scriptLive=false;CCharString_Destroy(&script);
            EntitySetMaxHealth_API(m_game,&beetle,2.0f,true);
            beetleLive=false;RetailThing_Destroy_API(&beetle);
        } catch(...) {cleanup();throw;}
    }
