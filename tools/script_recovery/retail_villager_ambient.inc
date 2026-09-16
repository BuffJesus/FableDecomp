    bool ShouldVillagerStartAmbientConversation(CScriptThing* actor,int timer) {
        CheckOpen();
        auto* timers=*ASLR<CGameScriptInterfaceBase**>(0x0143E8F8);
        if(!timers)throw std::runtime_error("Retail Villager timer interface unavailable");
        auto** table=*reinterpret_cast<void***>(timers);
        if(!table || !table[0x168/4])throw std::runtime_error("Retail Villager timer query unavailable");
        if(reinterpret_cast<tGetTimer>(table[0x168/4])(timers,timer)!=0)return false;
        using Random=int(__cdecl*)();
        if(ASLR<Random>(0x00BFEB16)()%100!=0)return false;
        if(!GetHero_API || !IsDistanceBetweenThingsUnder_API)
            throw std::runtime_error("Retail Villager proximity APIs unavailable");
        auto* hero=GetHero_API(m_game);
        return IsDistanceBetweenThingsUnder_API(actor,hero,5.0f);
    }
    void SetVillagerAmbientTimer(int timer,int value) {
        CheckOpen();
        auto* timers=*ASLR<CGameScriptInterfaceBase**>(0x0143E8F8);
        if(!timers)throw std::runtime_error("Retail Villager timer interface unavailable");
        auto** table=*reinterpret_cast<void***>(timers);
        if(!table || !table[0x164/4])throw std::runtime_error("Retail Villager timer update unavailable");
        reinterpret_cast<tSetTimer>(table[0x164/4])(timers,timer,value);
    }
    int StartVillagerAmbientConversation(CScriptThing* actor,int timer) {
        SetVillagerAmbientTimer(timer,3);
        if(!AddNewConversation_API || !GetHero_API || !AddPersonToConversation_API)
            throw std::runtime_error("Retail Villager ambient setup unavailable");
        auto conversation=AddNewConversation_API(m_game,actor,false,false);
        auto* hero=GetHero_API(m_game);
        AddPersonToConversation_API(m_game,conversation,hero);
        return conversation;
    }
