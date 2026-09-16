// Proposal: exact, bounded native returned-Thing lifetimes, without Lua GC ownership.
namespace {
struct ScytheReturnedThing {
    CScriptThing value{};
    ScytheReturnedThing() { value.pVTable=g_pCScriptThingVTable; }
    ~ScytheReturnedThing() { RetailThing_Destroy_API(&value); }
    ScytheReturnedThing(const ScytheReturnedThing&)=delete;
    ScytheReturnedThing& operator=(const ScytheReturnedThing&)=delete;
};
}
void LuaQuestState::SpawnScytheAndRemoveMarker(CScriptThing* marker) {
    if(!m_pGameInterface || !marker || !marker->pVTable || !CreateCreature_API ||
       !RemoveThing_API || !RetailThing_Destroy_API || !g_pCScriptThingVTable)
        throw std::runtime_error("Scythe spawn APIs unavailable");
    auto* table=reinterpret_cast<CScriptThingVTable*>(marker->pVTable);
    if(!table->GetPos) throw std::runtime_error("Scythe marker position unavailable");
    ScytheReturnedThing spawned;
    {
        FableString script("ScytheNearOracle"), definition("CREATURE_RIVAL_HERO_SCYTHE");
        const C3DVector* position=table->GetPos(marker);
        CreateCreature_API(m_pGameInterface,&spawned.value,definition,position,script,false);
    } // E29FA7/E29FB0: definition and script strings die before marker removal.
    RemoveThing_API(m_pGameInterface,marker,false,true);
    // E29FC5..E2A00A: release output after removal, also when creation is empty.
}
void LuaQuestState::FaceThingByScriptName(CScriptThing* actor, const std::string& name, bool snap) {
    if(!m_pGameInterface || !actor || !GetThingWithScriptName_ByName_API ||
       !EntitySetFacingAngleTowardsThing_API || !RetailThing_Destroy_API || !g_pCScriptThingVTable)
        throw std::runtime_error("Scythe facing APIs unavailable");
    FableString key(name.c_str());
    ScytheReturnedThing target;
    GetThingWithScriptName_ByName_API(m_pGameInterface,&target.value,key);
    EntitySetFacingAngleTowardsThing_API(m_pGameInterface,actor,&target.value,snap);
    // E2A44F cleanup precedes RegisterTimer E2A466; preserve an empty Thing too.
}
