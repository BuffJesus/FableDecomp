// Unapplied methods inside LuaRetailResources. Do not filter empty Things.
struct DeadFatherText {
    CCharString value{};
    explicit DeadFatherText(const char* text) { CCharString_Construct_Literal(&value,text,-1); }
    ~DeadFatherText() { CCharString_Destroy(&value); }
    DeadFatherText(const DeadFatherText&)=delete;
};
struct DeadFatherThing {
    CScriptThing value{};
    bool constructed=false;
    ~DeadFatherThing() { if(constructed) RetailThing_Destroy_API(&value); }
    DeadFatherThing()=default;
    DeadFatherThing(const DeadFatherThing&)=delete;
};
void InitializeDeadFatherActor(CScriptThing* actor) {
    CheckOpen();
    if(!actor || !g_pCScriptThingVTable || !MiniMapAddMarker_API || !SetIsPushableByHero_API) throw std::runtime_error("DeadFather Init APIs unavailable");
    {
        DeadFatherText icon("HUD_ORB_QUEST_CORE");
        MiniMapAddMarker_API(m_game,actor,&icon.value);
    }
    CScriptThing copy=*actor;copy.pVTable=g_pCScriptThingVTable;
    if(copy.pImp.Info) ++copy.pImp.Info->RefCount;
    SetIsPushableByHero_API(m_game,copy,false); // Callee owns/destroys the copied reference.
}
void PlaceDeadFatherAtMarker(CScriptThing* actor) {
    CheckOpen();
    if(!GetThingWithScriptName_ByName_API || !RetailThing_Destroy_API || !EntityTeleportToThing_API || !EntitySetFacingAngle_API)
        throw std::runtime_error("DeadFather placement APIs unavailable");
    {
        DeadFatherText key("MK_OVID_DAD");
        DeadFatherThing marker;
        auto* target=GetThingWithScriptName_ByName_API(m_game,&marker.value,&key.value);
        marker.constructed=true;
        EntityTeleportToThing_API(m_game,actor,target,false);
    }
    {
        DeadFatherText key("MK_OVID_DAD");
        DeadFatherThing marker;
        auto* target=GetThingWithScriptName_ByName_API(m_game,&marker.value,&key.value);
        marker.constructed=true;
        auto* table=reinterpret_cast<CScriptThingVTable*>(target->pVTable);
        if(!table || !table->GetAngleXY) throw std::runtime_error("DeadFather angle getter unavailable");
        float angle=table->GetAngleXY(target);
        EntitySetFacingAngle_API(m_game,actor,angle,true);
    }
}
void PlayDeadFatherPose(unsigned control) {
    auto& e=Get(control,Kind::Resource);
    DeadFatherText key("CS_DEAD_DAD");
    const unsigned char flag5=*ASLR<const volatile unsigned char*>(0x01375748);
    auto* expert=e.resource.pImp.Data;
    if(!expert) return;
    auto* table=reinterpret_cast<void**>(expert->pVTable);
    if(!table || !table[0x50/4]) throw std::runtime_error("DeadFather looping-animation slot unavailable");
    using RawLoop=void(__thiscall*)(void*,const CCharString*,int,bool,bool,bool,bool,unsigned char,bool,bool);
    reinterpret_cast<RawLoop>(table[0x50/4])(expert,&key.value,-1,false,true,false,true,flag5,false,false);
}
void RemoveDeadFatherMarker(CScriptThing* actor) {
    CheckOpen();
    if(!MiniMapRemoveMarker_API) throw std::runtime_error("DeadFather marker API unavailable");
    MiniMapRemoveMarker_API(m_game,actor);
}
