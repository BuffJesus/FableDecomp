    int GetBarrelWatchTimer(int id) {
        CheckOpen();
        auto* game=*ASLR<CGameScriptInterfaceBase**>(0x0143E8F8);
        if(!game)throw std::runtime_error("Retail global timer interface unavailable");
        auto** table=*reinterpret_cast<void***>(game);
        if(!table || !table[0x168/sizeof(void*)])throw std::runtime_error("Retail timer get slot unavailable");
        return reinterpret_cast<tGetTimer>(table[0x168/sizeof(void*)])(game,id);
    }
    bool IsOwnedThingPositionOnScreen(unsigned id) {
        auto& marker=Get(id,Kind::Thing).thing;
        if(!IsCameraPosOnScreen_API)throw std::runtime_error("Retail camera position API unavailable");
        auto* implementation=reinterpret_cast<CGameScriptThing*>(marker.pImp.Data);
        const C3DVector* position;
        if(implementation) {
            auto* table=reinterpret_cast<CGameScriptThingVTable*>(implementation->pVTable);
            if(!table || !table->GetPos)throw std::runtime_error("Retail marker position slot unavailable");
            position=table->GetPos(implementation);
        } else position=ASLR<const C3DVector*>(0x0143E8E0);
        return IsCameraPosOnScreen_API(m_game,position);
    }
    void TeleportActorToOwnedThing(CScriptThing* actor,unsigned id) {
        auto& marker=Get(id,Kind::Thing).thing;
        if(!EntityTeleportToThing_API)throw std::runtime_error("Retail teleport API unavailable");
        EntityTeleportToThing_API(m_game,actor,&marker,false);
    }
