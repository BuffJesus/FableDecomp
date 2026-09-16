    void TeleportBarrelDepartureActors(CScriptThing* actor) {
        CheckOpen();
        if(!actor || !GetHero_API || !GetThingWithScriptName_ByName_API ||
           !EntityTeleportToThing_API || !RetailThing_Destroy_API || !g_pCScriptThingVTable ||
           !CCharString_Construct_Literal || !CCharString_Destroy)
            throw std::runtime_error("Retail Barrel departure APIs unavailable");
        auto teleport=[&](const char* name,bool useHero) {
            CCharString key{};
            CScriptThing marker{};
            bool keyLive=false,markerLive=false;
            try {
                CCharString_Construct_Literal(&key,name,-1);keyLive=true;
                marker.pVTable=g_pCScriptThingVTable;markerLive=true;
                auto* target=GetThingWithScriptName_ByName_API(m_game,&marker,&key);
                auto* moving=useHero?GetHero_API(m_game):actor;
                EntityTeleportToThing_API(m_game,moving,target,false);
                markerLive=false;RetailThing_Destroy_API(&marker);
                keyLive=false;CCharString_Destroy(&key);
            } catch(...) {
                if(markerLive)try {markerLive=false;RetailThing_Destroy_API(&marker);}catch(...) {}
                if(keyLive)try {keyLive=false;CCharString_Destroy(&key);}catch(...) {}
                throw;
            }
        };
        teleport("M_WHouse_GuardPoint",true);
        teleport("M_BarrelManHiddenPos",false);
    }
