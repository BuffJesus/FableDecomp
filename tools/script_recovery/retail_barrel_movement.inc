    const C3DVector* BorrowedBarrelHeroPosition() {
        CheckOpen();
        if(!GetHero_API)throw std::runtime_error("Retail hero API unavailable");
        auto* hero=GetHero_API(m_game);
        if(!hero || !hero->pVTable)throw std::runtime_error("Retail hero position requires a native hero");
        auto* table=reinterpret_cast<CScriptThingVTable*>(hero->pVTable);
        if(!table->GetPos)throw std::runtime_error("Retail hero position slot unavailable");
        return table->GetPos(hero);
    }
    bool IsBarrelManFarFromHero(CScriptThing* actor) {
        const auto* position=BorrowedBarrelHeroPosition();
        using Distance=bool(__fastcall*)(const CScriptThing*,const C3DVector*,float);
        return ASLR<Distance>(0x00CBE45C)(actor,position,4.0f);
    }
    void MoveBarrelManToHero(unsigned id) {
        const auto* position=BorrowedBarrelHeroPosition();
        auto& resource=Get(id,Kind::Resource).resource;
        using Move=void(__thiscall*)(void*,const C3DVector*,float,int,bool,bool);
        ASLR<Move>(0x007E72F0)(&resource,position,2.0f,1,false,true);
    }
    void SetBarrelWatchTimer(int id) {
        CheckOpen();
        auto* game=*ASLR<CGameScriptInterfaceBase**>(0x0143E8F8);
        if(!game)throw std::runtime_error("Retail global timer interface unavailable");
        auto** table=*reinterpret_cast<void***>(game);
        if(!table || !table[0x164/sizeof(void*)])throw std::runtime_error("Retail timer set slot unavailable");
        reinterpret_cast<tSetTimer>(table[0x164/sizeof(void*)])(game,id,45);
    }
    void ResetBarrelWatchTimer(int id) {
        CheckOpen();
        auto* game=*ASLR<CGameScriptInterfaceBase**>(0x0143E8F8);
        if(!game)throw std::runtime_error("Retail global timer interface unavailable");
        auto** table=*reinterpret_cast<void***>(game);
        if(!table || !table[0x164/sizeof(void*)])throw std::runtime_error("Retail timer set slot unavailable");
        reinterpret_cast<tSetTimer>(table[0x164/sizeof(void*)])(game,id,0);
    }
