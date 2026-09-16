// Reuse the established full-class API doubles; its main is not run here.
#define main TheresaReferenceMain
#include "theresa_scope_runtime_harness.cpp"
#undef main
static bool emptyHero=false;
static int selectionExpected=0;
static int actionFailure=0;
static bool keyCleanupFailure=false;
static CScriptThing* ownedMarker=nullptr;
static void __fastcall thugKey(CCharString* key,void*,const char* text,int length) {
    check(length==-1 && (std::string(text)=="M_WHouse_ManStart" || std::string(text)=="TEXT_QST_048_BARRELTHUG_SCRMSG_TEMPT_10"));
    check(key->pStringData==nullptr);keys.push_back(key);events.emplace_back("key");
}
static void __fastcall thugKeyClose(CCharString* key,void*) {
    check(!keys.empty() && keys.back()==key);keys.pop_back();events.emplace_back("key.close");
    if(keyCleanupFailure)throw std::runtime_error("KEY_CLEANUP");
}
static CScriptThing* __fastcall thugLookup(CGameScriptInterfaceBase* self,void*,CScriptThing* out,const CCharString* key) {
    check(self==&game && keys.size()==1 && keys.back()==key && ownedMarker==nullptr);
    events.emplace_back("lookup");if(actionFailure==1)throw std::runtime_error("LOOKUP");
    ownedMarker=out;return &lookupAlias;
}
static void __fastcall thugThingClose(CScriptThing* thing,void*) {
    check(thing==ownedMarker && thing!=&lookupAlias && keys.size()==1);
    ownedMarker=nullptr;events.emplace_back("thing.close");
}
static void __fastcall thugTeleport(CGameScriptInterfaceBase* self,void*,const CScriptThing* actor,const CScriptThing* target,bool flag) {
    check(self==&game && actor==&guards[0] && target==&lookupAlias && !flag && ownedMarker && keys.size()==1);
    events.emplace_back("teleport");if(actionFailure==2)throw std::runtime_error("TELEPORT");
}
static void __fastcall thugFace(CGameScriptInterfaceBase* self,void*,const CScriptThing* actor,const CScriptThing* target,bool flag) {
    check(self==&game && actor==&guards[0] && target==(emptyHero?nullptr:&departureHero) && !flag && !ownedMarker && keys.empty());
    events.emplace_back("face");if(actionFailure==3)throw std::runtime_error("FACE");
}
static int __fastcall thugConversation(CGameScriptInterfaceBase* self,void*,const CScriptThing* actor,bool first,bool second) {
    check(self==&game && actor==&guards[0] && !first && !second);events.emplace_back("conversation");return -7;
}
static void __fastcall thugPerson(CGameScriptInterfaceBase* self,void*,int id,const CScriptThing* who) {
    check(self==&game && id==-7 && who==(emptyHero?nullptr:&departureHero));events.emplace_back("person");
}
static void __fastcall thugLine(CGameScriptInterfaceBase* self,void*,int id,const CCharString* key,bool flag,const CScriptThing* speaker,const CScriptThing* listener) {
    check(self==&game && id==-7 && keys.size()==1 && keys.back()==key && !flag && speaker==&guards[0] && listener==(emptyHero?nullptr:&departureHero));
    events.emplace_back("line");if(actionFailure==4)throw std::runtime_error("LINE");
}
decltype(EntityTeleportToThing_API) EntityTeleportToThing_API=reinterpret_cast<decltype(EntityTeleportToThing_API)>(&thugTeleport);
decltype(EntitySetFacingAngleTowardsThing_API) EntitySetFacingAngleTowardsThing_API=reinterpret_cast<decltype(EntitySetFacingAngleTowardsThing_API)>(&thugFace);
decltype(AddNewConversation_API) AddNewConversation_API=reinterpret_cast<decltype(AddNewConversation_API)>(&thugConversation);
decltype(AddPersonToConversation_API) AddPersonToConversation_API=reinterpret_cast<decltype(AddPersonToConversation_API)>(&thugPerson);
decltype(AddLineToConversation_API) AddLineToConversation_API=reinterpret_cast<decltype(AddLineToConversation_API)>(&thugLine);
static void __fastcall resetThug(void* resource,void*) {check(resource!=nullptr);events.emplace_back("reset");}
static CScriptThing* __fastcall thugHero(CGameScriptInterfaceBase* self,void*) {
    check(self==&game);events.emplace_back("hero");
    if(actionFailure==5)throw std::runtime_error("HERO");
    return emptyHero?nullptr:&departureHero;
}
static void __fastcall thugSpeak(CScriptGameResourceObjectScriptedThingBase* self,void*,const CScriptThing* target,
    const char* text,ETextGroupSelectionMethod selection,bool listen,bool sound,bool fade) {
    check(self==&expert && target==(emptyHero?nullptr:&departureHero) && std::string(text)=="TEXT_QST_048_BARRELTHUG_WHY_HIT" &&
        static_cast<int>(selection)==selectionExpected && !listen && sound && !fade);
    events.emplace_back("speak");
}
static void __fastcall thugFollow(CScriptGameResourceObjectScriptedThingBase* self,void*,const CScriptThing* target,float distance,bool flag) {
    check(self==&expert && target==(emptyHero?nullptr:&departureHero) && distance==1.0f && flag);events.emplace_back("follow");
}
int main(){try {
    sol::state lua;lua.open_libraries(sol::lib::base,sol::lib::string);
    auto r=lua.new_usertype<LuaRetailResources>("ResourceScope",sol::no_constructor);
    r["NewResource"]=&LuaRetailResources::NewResource;
    r["TryAcquire"]=&LuaRetailResources::TryAcquire;
    r["ReleaseResource"]=&LuaRetailResources::ReleaseResource;
    r["ResetResource"]=&LuaRetailResources::ResetResource;
    r["InitializeBarrelThugActor"]=&LuaRetailResources::InitializeBarrelThugActor;
    r["SpeakBarrelThug"]=&LuaRetailResources::SpeakBarrelThug;
    r["FollowBarrelThugHero"]=&LuaRetailResources::FollowBarrelThugHero;
    r["PlaceBarrelThugAtStart"]=&LuaRetailResources::PlaceBarrelThugAtStart;
    r["NewBarrelThugRemarkConversation"]=&LuaRetailResources::NewBarrelThugRemarkConversation;
    r["AddBarrelThugRemark"]=&LuaRetailResources::AddBarrelThugRemark;
    lua["actor"]=&guards[0];
    CScriptGameResourceObjectScriptedThingBaseVTable table{};
    table.Speak=reinterpret_cast<decltype(table.Speak)>(&thugSpeak);
    table.FollowThing=reinterpret_cast<decltype(table.FollowThing)>(&thugFollow);
    expert.pVTable=reinterpret_cast<decltype(expert.pVTable)>(&table);
    GetHero_API=reinterpret_cast<decltype(GetHero_API)>(&thugHero);
    InitScriptObjectHelper2_API=reinterpret_cast<decltype(InitScriptObjectHelper2_API)>(&resetThug);
    int cases=0;
    for(bool controlled:{false,true})for(bool nullHero:{false,true})for(int selection:{0,2}) {
        emptyHero=nullHero;selectionExpected=selection;events.clear();
        LuaRetailResources scope(&game);lua["scope"]=&scope;lua["controlled"]=controlled;lua["selection"]=selection;
        lua.script(R"(
            local id=scope:NewResource()
            scope:InitializeBarrelThugActor(actor)
            if controlled then assert(scope:TryAcquire(id,actor,4)) end
            scope:ResetResource(id)
            scope:SpeakBarrelThug(id,"TEXT_QST_048_BARRELTHUG_WHY_HIT",selection)
            scope:FollowBarrelThugHero(id)
            scope:ReleaseResource(id)
            assert(not pcall(function() scope:ResetResource(id) end))
        )");scope.Close();scope.Close();
        check(events==(controlled?std::vector<std::string>{"damage","kill","combo","reset","hero","speak","hero","follow","resource.close"}:
            std::vector<std::string>{"damage","kill","combo","reset","hero","hero","resource.close"}));
        ++cases;
    }
    CCharString_Construct_Literal=reinterpret_cast<decltype(CCharString_Construct_Literal)>(&thugKey);
    CCharString_Destroy=reinterpret_cast<decltype(CCharString_Destroy)>(&thugKeyClose);
    RetailThing_Destroy_API=reinterpret_cast<decltype(RetailThing_Destroy_API)>(&thugThingClose);
    GetThingWithScriptName_ByName_API=reinterpret_cast<decltype(GetThingWithScriptName_ByName_API)>(&thugLookup);
    for(bool nullHero:{false,true})for(int failure:{0,1,2,3})for(bool cleanupFailure:{false,true}) {
        emptyHero=nullHero;actionFailure=failure;keyCleanupFailure=cleanupFailure;events.clear();
        LuaRetailResources scope(&game);lua["scope"]=&scope;
        const char* expected=failure==1?"LOOKUP":failure==2?"TELEPORT":cleanupFailure?"KEY_CLEANUP":failure==3?"FACE":"";
        lua["expected"]=expected;
        lua.script(R"(
            local ok,err=pcall(function() scope:PlaceBarrelThugAtStart(actor) end)
            if expected=="" then assert(ok) else assert(not ok and string.find(err,expected,1,true)) end
        )");scope.Close();check(keys.empty() && !ownedMarker);
        std::vector<std::string> wanted{"key","lookup"};
        if(failure!=1){wanted.emplace_back("teleport");wanted.emplace_back("thing.close");}
        wanted.emplace_back("key.close");
        if(failure!=1 && failure!=2 && !cleanupFailure){wanted.emplace_back("hero");wanted.emplace_back("face");}
        check(events==wanted);++cases;
    }
    for(bool nullHero:{false,true})for(int failure:{0,4,5})for(bool cleanupFailure:{false,true}) {
        emptyHero=nullHero;actionFailure=0;keyCleanupFailure=false;events.clear();
        LuaRetailResources scope(&game);lua["scope"]=&scope;
        lua.script("conversation=scope:NewBarrelThugRemarkConversation(actor);assert(conversation==-7)");
        check(events==std::vector<std::string>{"conversation","hero","person"});events.clear();
        actionFailure=failure;keyCleanupFailure=cleanupFailure;
        lua["expected"]=failure==4?"LINE":failure==5?"HERO":cleanupFailure?"KEY_CLEANUP":"";
        lua.script(R"(
            local ok,err=pcall(function() scope:AddBarrelThugRemark(conversation,actor,"TEXT_QST_048_BARRELTHUG_SCRMSG_TEMPT_10") end)
            if expected=="" then assert(ok) else assert(not ok and string.find(err,expected,1,true)) end
        )");scope.Close();check(keys.empty());
        check(events==(failure==5?std::vector<std::string>{"key","hero","key.close"}:
            std::vector<std::string>{"key","hero","line","key.close"}));++cases;
    }
    lua["scope"]=sol::nil;
    std::cout<<"PASS: "<<cases<<" BarrelThug full-class adapter policies\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
