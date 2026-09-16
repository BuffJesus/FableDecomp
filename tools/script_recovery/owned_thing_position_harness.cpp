#define main resource_smoke_main
#include "runtime_checks/retail_resources_smoke.cpp"
#undef main
DWORD g_fableBase = 0;
tIsDistanceBetweenThingsUnder IsDistanceBetweenThingsUnder_API = nullptr;
tEntitySetFacingAngleTowardsThing EntitySetFacingAngleTowardsThing_API = nullptr;
tAddPersonToConversation AddPersonToConversation_API = nullptr;
tAddLineToConversation AddLineToConversation_API = nullptr;
static C3DVector position{1,2,3}, fallback{4,5,6};
static CGameScriptThing implementation{};
static bool emptyMarker, failQuery;
static unsigned queries, released;
static const C3DVector* __fastcall queryPosition(CGameScriptThing* actor,void*) {
    check(actor==&implementation); ++queries;
    if(failQuery) throw std::runtime_error("position failure");
    return &position;
}
static CScriptThing* __fastcall lookupPosition(CGameScriptInterfaceBase*,void*,CScriptThing* out,const CCharString* name) {
    check(strings.at(const_cast<CCharString*>(name))=="AffairWomanRunOffPoint");
    out->pImp.Data=emptyMarker?nullptr:reinterpret_cast<decltype(out->pImp.Data)>(&implementation);
    return out;
}
static void __fastcall destroyPosition(CScriptThing* value,void*) {++released;value->pImp={};}
int main() {
    try {
        static_assert(sizeof(void*)==4);
        check(resource_smoke_main()==0);
        CGameScriptThingVTable table{};table.GetPos=reinterpret_cast<decltype(table.GetPos)>(&queryPosition);
        implementation.pVTable=reinterpret_cast<void**>(&table);
        GetThingWithScriptName_ByName_API=reinterpret_cast<tGetThingWithScriptName1>(&lookupPosition);
        RetailThing_Destroy_API=reinterpret_cast<tRetailThingDestroy>(&destroyPosition);
        g_fableBase=reinterpret_cast<DWORD>(&fallback)-(0x0143E8E0-0x400000);
        sol::state lua;lua.open_libraries(sol::lib::base);RegisterRetailResources(lua);
        auto scope=std::make_shared<LuaRetailResources>(reinterpret_cast<CGameScriptInterfaceBase*>(1));
        lua["resources"]=scope;
        auto marker=scope->NewThingFromScriptName("AffairWomanRunOffPoint");lua["marker"]=marker;
        lua.script("saved=resources:ThingPosition(marker); assert(saved.x==1 and saved.y==2 and saved.z==3)");
        position.x=9;
        lua.script("assert(saved.x==1 and resources:ThingPosition(marker).x==9)");
        failQuery=true;lua.script("assert(not pcall(function() resources:ThingPosition(marker) end))");failQuery=false;
        check(released==0);scope->DestroyThing(marker);check(released==1);
        lua.script("assert(not pcall(function() resources:ThingPosition(marker) end))");
        emptyMarker=true;marker=scope->NewThingFromScriptName("AffairWomanRunOffPoint");lua["marker"]=marker;
        const auto before=queries;
        lua.script("saved=resources:ThingPosition(marker); assert(saved.x==4)");fallback.x=12;
        lua.script("assert(saved.x==4 and resources:ThingPosition(marker).x==12)");check(queries==before);
        auto resource=scope->NewResource();lua["resource"]=resource;
        lua.script("assert(not pcall(function() resources:ThingPosition(resource) end))");
        scope->ReleaseResource(resource);scope->Close();check(released==2);
        lua.script("assert(not pcall(function() resources:ThingPosition(marker) end))");
        std::cout<<"PASS: owned position binding preserves identity, vector snapshots, live empty fallback, error ownership, kind/stale/closed checks\n";
        return 0;
    } catch(const std::exception& error) {std::cerr<<error.what()<<'\n';return 1;}
}
