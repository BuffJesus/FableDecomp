    // Include in LuaRetailResources public section. Native EC43EB..EC4460.
    void CreateCreatureAtThingPosition(unsigned markerId,const std::string& definitionName,
                                      const std::string& scriptName,bool flag) {
        auto& entry=Get(markerId,Kind::Thing);
        if(!CreateCreature_API || !RetailThing_Destroy_API || !g_pCScriptThingVTable)
            throw std::runtime_error("Retail marker spawn APIs unavailable");
        FableString script(scriptName.c_str());
        FableString definition(definitionName.c_str());
        const C3DVector* position;
        auto* data=reinterpret_cast<CGameScriptThing*>(entry.thing.pImp.Data);
        if(data) {
            auto** table=*reinterpret_cast<void***>(data);
            auto getPosition=reinterpret_cast<tCGameScriptThing_GetPos>(table[6]);
            position=getPosition(data);
        } else {
            position=ASLR<const C3DVector*>(0x0143E8E0);
        }
        struct Output {
            CScriptThing thing{};
            Output(){thing.pVTable=g_pCScriptThingVTable;}
            ~Output(){RetailThing_Destroy_API(&thing);}
        } output;
        CreateCreature_API(m_game,&output.thing,definition,position,script,flag);
        // Output -> definition -> script. Retained marker remains live in scope.
    }
