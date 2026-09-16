#define main TheresaReferenceMain
#include "theresa_scope_runtime_harness.cpp"
#undef main

static void worldCheck(bool value,int line){if(!value)throw std::runtime_error("Post-attack assertion at line "+std::to_string(line));}
#define check(value) worldCheck((value),__LINE__)

static void* worldTable[512]{},*worldOther[512]{};
static CScriptThingVTable worldThingTable{};
static CScriptThing worldAlias{},worldHero{};
static CScriptThing* worldOwned=nullptr;
static CCharString* worldKey=nullptr;
static std::string worldName,worldFault;
static int worldAliasMode=0,worldHeroCalls=0;
static bool worldEmpty=false,worldResult=false,worldMutate=false,worldFlag=false,worldCleanupFault=false;
static void worldHit(const char* point){if(worldFault==point)throw std::runtime_error(std::string("WORLD_")+point);}
static void __fastcall worldKeyNew(CCharString* key,void*,const char* name,int n){
    check(!worldKey&&n==-1);events.emplace_back("key.new");worldHit("key.new");worldKey=key;worldName=name;
    key->pStringData=reinterpret_cast<decltype(key->pStringData)>(1);
}
static void __fastcall worldKeyClose(CCharString* key,void*){
    check(key==worldKey&&(!worldOwned||worldName=="MK_OVI_DADTRIGGER"));worldKey=nullptr;key->pStringData=nullptr;events.emplace_back("key.close");worldHit("key.close");
    if(worldCleanupFault)throw std::runtime_error("SECONDARY_KEY");
}
static CScriptThing* __fastcall worldLookup(CGameScriptInterfaceBase* self,void*,CScriptThing* out,const CCharString* key){
    check(self==&game&&key==worldKey&&!worldOwned);events.emplace_back("lookup");worldHit("lookup");
    out->pVTable=reinterpret_cast<void**>(&worldThingTable);out->pImp.Data=nullptr;out->pImp.Info=nullptr;worldOwned=out;
    if(worldMutate){*reinterpret_cast<void***>(&game)=worldOther;worldTable[0x6e0/4]=worldOther[0x6e0/4];}
    return worldAliasMode==2?nullptr:worldAliasMode==1?&worldAlias:out;
}
static void __fastcall worldOutputClose(CScriptThing* out,void*){
    check(out==worldOwned);worldOwned=nullptr;events.emplace_back("output.close");worldHit("output.close");
    if(worldCleanupFault)throw std::runtime_error("SECONDARY_OUTPUT");
}
static void worldReturned(const CScriptThing* thing){check(thing==(worldAliasMode==2?nullptr:worldAliasMode==1?&worldAlias:worldOwned));}
static bool __fastcall worldAlive(CScriptThing* thing,void*){
    check(worldName=="M_PostAttackStart"&&worldKey&&worldOwned);worldReturned(thing);events.emplace_back("alive");worldHit("action");return worldResult;
}
static CScriptThing* __fastcall worldGetHero(CGameScriptInterfaceBase* self,void*){
    check(self==&game);++worldHeroCalls;events.emplace_back("hero");worldHit("hero");
    if(worldMutate)worldTable[0x760/4]=worldOther[0x760/4];return worldEmpty?nullptr:&worldHero;
}
static void worldTeleportCheck(CGameScriptInterfaceBase* self,const CScriptThing* hero,const CScriptThing* target,bool flag,bool changed){
    check(self==&game&&hero==(worldEmpty?nullptr:&worldHero)&&!flag&&worldName=="M_PostAttackStart"&&worldKey&&worldOwned&&changed==worldMutate);
    worldReturned(target);events.emplace_back("teleport");worldHit("action");
}
static void __fastcall worldTeleport(CGameScriptInterfaceBase* self,void*,const CScriptThing* hero,const CScriptThing* target,bool flag){worldTeleportCheck(self,hero,target,flag,false);}
static void __fastcall worldTeleportChanged(CGameScriptInterfaceBase* self,void*,const CScriptThing* hero,const CScriptThing* target,bool flag){worldTeleportCheck(self,hero,target,flag,true);}
static void worldLimboCheck(CGameScriptInterfaceBase* self,const CScriptThing* village,bool flag,bool changed){
    check(self==&game&&flag==worldFlag&&worldName=="V_OakVale"&&worldKey&&worldOwned&&changed==worldMutate);worldReturned(village);events.emplace_back("limbo");worldHit("action");
}
static void __fastcall worldLimbo(CGameScriptInterfaceBase* self,void*,const CScriptThing* village,bool flag){worldLimboCheck(self,village,flag,false);}
static void __fastcall worldLimboChanged(CGameScriptInterfaceBase* self,void*,const CScriptThing* village,bool flag){worldLimboCheck(self,village,flag,true);}
static bool __fastcall worldDistance(const CScriptThing* hero,const CScriptThing* trigger,float distance){
    check(hero==(worldEmpty?nullptr:&worldHero)&&trigger==worldOwned&&distance==5.f&&!worldKey);events.emplace_back("near");worldHit("action");return worldResult;
}
static void worldReset(){
    check(!worldKey&&!worldOwned);events.clear();worldHeroCalls=0;worldCleanupFault=false;
    *reinterpret_cast<void***>(&game)=worldTable;
    worldTable[0x120/4]=reinterpret_cast<void*>(&worldLookup);worldTable[0x118/4]=reinterpret_cast<void*>(&worldGetHero);
    worldTable[0x760/4]=reinterpret_cast<void*>(&worldTeleport);worldTable[0x6e0/4]=reinterpret_cast<void*>(&worldLimbo);
    worldOther[0x118/4]=reinterpret_cast<void*>(&worldGetHero);worldOther[0x760/4]=reinterpret_cast<void*>(&worldTeleportChanged);worldOther[0x6e0/4]=reinterpret_cast<void*>(&worldLimboChanged);
}
int main(){try{
    CCharString_Construct_Literal=reinterpret_cast<decltype(CCharString_Construct_Literal)>(&worldKeyNew);
    CCharString_Destroy=reinterpret_cast<decltype(CCharString_Destroy)>(&worldKeyClose);
    RetailThing_Destroy_API=reinterpret_cast<decltype(RetailThing_Destroy_API)>(&worldOutputClose);
    GetThingWithScriptName_ByName_API=reinterpret_cast<decltype(GetThingWithScriptName_ByName_API)>(&worldLookup);
    IsDistanceBetweenThingsUnder_API=&worldDistance;
    worldThingTable.IsAlive=reinterpret_cast<decltype(worldThingTable.IsAlive)>(&worldAlive);
    worldAlias.pVTable=reinterpret_cast<void**>(&worldThingTable);
    sol::state lua;lua.open_libraries(sol::lib::base,sol::lib::string);
    auto type=lua.new_usertype<LuaRetailResources>("Scope",sol::no_constructor);
    type["PostAttackStartIsAlive"]=&LuaRetailResources::PostAttackStartIsAlive;
    type["TeleportToPostAttackStart"]=&LuaRetailResources::TeleportToPostAttackStart;
    type["SetPostAttackVillageLimbo"]=&LuaRetailResources::SetPostAttackVillageLimbo;
    type["PostAttackHeroNearTrigger"]=&LuaRetailResources::PostAttackHeroNearTrigger;
    unsigned cases=0;
    for(int kind=0;kind<4;++kind)for(int alias=0;alias<3;++alias)for(bool empty:{false,true})for(bool mutate:{false,true})for(bool result:{false,true}){
        if(kind==0&&alias==2)continue;
        for(const std::string fault:{"","key.new","lookup","action","output.close","key.close","hero"}){
            if(fault=="hero"&&kind!=1)continue;
            worldReset();worldAliasMode=alias;worldEmpty=empty;worldMutate=mutate;worldResult=result;worldFlag=kind==2;worldFault=fault;
            {LuaRetailResources scope(&game);lua["scope"]=&scope;lua["expected"]=result;lua["flag"]=worldFlag;
                std::string call=kind==0?"assert(scope:PostAttackStartIsAlive()==expected)":kind==1?"scope:TeleportToPostAttackStart()":"scope:SetPostAttackVillageLimbo(flag)";
                auto outcome=lua.safe_script(call,sol::script_pass_on_error);check(outcome.valid()==fault.empty());
                if(!outcome.valid()){sol::error e=outcome;check(std::string(e.what()).find("WORLD_"+fault)!=std::string::npos);}
                check(!worldKey&&!worldOwned);scope.Close();
                auto before=events.size();check(!lua.safe_script(call,sol::script_pass_on_error).valid());check(events.size()==before);
            }
            std::vector<std::string> expected{"key.new"};
            if(fault!="key.new"){
                expected.push_back("lookup");
                if(fault!="lookup"){
                    if(kind==1)expected.push_back("hero");
                    if(fault!="hero")expected.push_back(kind==0?"alive":kind==1?"teleport":"limbo");
                    expected.push_back("output.close");
                }
                expected.push_back("key.close");
            }
            check(events==expected);++cases;
        }
    }
    // Preserve a primary action failure even when both destructors fail.
    worldReset();worldAliasMode=1;worldFault="action";worldCleanupFault=true;
    {LuaRetailResources scope(&game);lua["scope"]=&scope;auto outcome=lua.safe_script("scope:PostAttackStartIsAlive()",sol::script_pass_on_error);check(!outcome.valid());sol::error e=outcome;check(std::string(e.what()).find("WORLD_action")!=std::string::npos);check(!worldOwned&&!worldKey);}
    ++cases;
    // Retained output, not a returned alias; fresh Hero on every distance query.
    for(bool empty:{false,true})for(bool result:{false,true}){
        worldReset();worldAliasMode=1;worldFault="";worldEmpty=empty;worldResult=result;worldMutate=false;
        {LuaRetailResources scope(&game);unsigned id=scope.NewThingFromScriptName("MK_OVI_DADTRIGGER");
            check(scope.PostAttackHeroNearTrigger(id)==result);check(scope.PostAttackHeroNearTrigger(id)==result);check(worldHeroCalls==2);
            scope.DestroyThing(id);bool rejected=false;try{scope.PostAttackHeroNearTrigger(id);}catch(...){rejected=true;}check(rejected&&worldHeroCalls==2);
        }
        check(!worldOwned&&!worldKey);++cases;
    }
    lua["scope"]=sol::nil;std::cout<<"PASS: "<<cases<<" atomic post-attack scope policies\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
