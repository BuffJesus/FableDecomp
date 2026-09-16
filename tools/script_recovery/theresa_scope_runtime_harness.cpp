#include "LuaRetailResources.h"
#include <iostream>
#include <vector>
#include <string>
DWORD g_fableBase=0;
static CGameScriptInterfaceBase game{};
static CScriptThing guards[2]{};
static std::vector<CCharString*> keys;
static std::vector<std::string> events;
static int failGet=0;
static void check(bool ok){if(!ok)throw std::runtime_error("Theresa scope check failed");}
static void unused(){throw std::runtime_error("Unexpected API call");}
decltype(Game_malloc) Game_malloc=reinterpret_cast<decltype(Game_malloc)>(&unused);
decltype(Game_free) Game_free=reinterpret_cast<decltype(Game_free)>(&unused);
decltype(g_pCScriptGameResourceObjectScriptedThingBaseVTable) g_pCScriptGameResourceObjectScriptedThingBaseVTable=reinterpret_cast<decltype(g_pCScriptGameResourceObjectScriptedThingBaseVTable)>(&unused);
decltype(InitScriptObjectHelper1_API) InitScriptObjectHelper1_API=reinterpret_cast<decltype(InitScriptObjectHelper1_API)>(&unused);
decltype(InitScriptObjectHelper2_API) InitScriptObjectHelper2_API=reinterpret_cast<decltype(InitScriptObjectHelper2_API)>(&unused);
decltype(StdMap_Construct_API) StdMap_Construct_API=reinterpret_cast<decltype(StdMap_Construct_API)>(&unused);
decltype(StdMap_Destroy_API) StdMap_Destroy_API=reinterpret_cast<decltype(StdMap_Destroy_API)>(&unused);
decltype(StdMap_OperatorBracket_API) StdMap_OperatorBracket_API=reinterpret_cast<decltype(StdMap_OperatorBracket_API)>(&unused);
decltype(CBaseObject_Assign_API) CBaseObject_Assign_API=reinterpret_cast<decltype(CBaseObject_Assign_API)>(&unused);
decltype(RunCutsceneMacro_Func) RunCutsceneMacro_Func=reinterpret_cast<decltype(RunCutsceneMacro_Func)>(&unused);
decltype(g_pMovieObjectVTable) g_pMovieObjectVTable=reinterpret_cast<decltype(g_pMovieObjectVTable)>(&unused);
decltype(StartMovieSequence_API) StartMovieSequence_API=reinterpret_cast<decltype(StartMovieSequence_API)>(&unused);
decltype(MovieResource_Destroy_API) MovieResource_Destroy_API=reinterpret_cast<decltype(MovieResource_Destroy_API)>(&unused);
decltype(PauseAllNonScriptedEntities_API) PauseAllNonScriptedEntities_API=reinterpret_cast<decltype(PauseAllNonScriptedEntities_API)>(&unused);
decltype(NewScriptFrame_API) NewScriptFrame_API=reinterpret_cast<decltype(NewScriptFrame_API)>(&unused);

static void __fastcall construct(CCharString* key,void*,const char* text,int length){
    check((std::string(text)=="NOVI_Guard" || std::string(text)=="M_TriggerOutro" || std::string(text)=="SCRIPT_NAME_HERO" || std::string(text)=="OBJECT_CHOCOLATE_BOX_UNGIVEABLE") && length==-1);keys.push_back(key);events.emplace_back("key");
}
static void __fastcall destroyKey(CCharString* key,void*){
    check(!keys.empty() && keys.back()==key);keys.pop_back();events.emplace_back("key.close");
}
static int __fastcall getGuards(CGameScriptInterfaceBase* self,void*,const CCharString* key,RetailTheresaGuardVector::Vector* out){
    check(self==&game && keys.size()==1 && keys.back()==key && out->begin==nullptr && out->end==nullptr && out->capacity==nullptr);
    out->begin=guards;out->end=guards+2;out->capacity=guards+2;events.emplace_back("get");
    if(failGet)throw std::runtime_error("GET");return 2;
}
static void __fastcall removeGuards(CGameScriptInterfaceBase* self,RetailTheresaGuardVector::Vector* out,bool flag){
    check(self==&game && out->begin==guards && !flag);events.emplace_back("remove");
}
static void __fastcall destroyGuards(RetailTheresaGuardVector::Vector* out,void*){
    check(out->begin==guards && out->end==guards+2);events.emplace_back("vector.close");
}
tCCharString_Constructor_Literal CCharString_Construct_Literal=reinterpret_cast<tCCharString_Constructor_Literal>(&construct);
tCCharString_Destructor CCharString_Destroy=reinterpret_cast<tCCharString_Destructor>(&destroyKey);
tGetAllThingsWithScriptName GetAllThingsWithScriptName_API=reinterpret_cast<tGetAllThingsWithScriptName>(&getGuards);
static CScriptGameResourceObjectScriptedThingBase expert{};
static int getterMode=0;
static void __fastcall initializeResource(void*,void*){}
static void __fastcall releaseResource(void*,void*){events.emplace_back("resource.close");}
static void __fastcall releaseThing(CScriptThing*,void*){events.emplace_back("thing.close");}
static bool __fastcall acquire(CGameScriptInterfaceBase*,void*,const CScriptThing*,CScriptGameResourceObjectScriptedThingBase* out,EScriptAIPriority){out->pImp.Data=&expert;return true;}
static CScriptThing* __fastcall getThing(CScriptGameResourceObjectScriptedThingBase*,void*,CScriptThing* out){
    events.emplace_back("thing.get");if(getterMode==1)throw std::runtime_error("GETTER");return out;
}
decltype(CBaseObject_Construct_API) CBaseObject_Construct_API=reinterpret_cast<decltype(CBaseObject_Construct_API)>(&initializeResource);
decltype(CSGROSTB_Destroy_API) CSGROSTB_Destroy_API=reinterpret_cast<decltype(CSGROSTB_Destroy_API)>(&releaseResource);
decltype(StartScriptingEntity_API) StartScriptingEntity_API=reinterpret_cast<decltype(StartScriptingEntity_API)>(&acquire);
decltype(RetailThing_Destroy_API) RetailThing_Destroy_API=reinterpret_cast<decltype(RetailThing_Destroy_API)>(&releaseThing);
decltype(g_pCScriptThingVTable) g_pCScriptThingVTable=reinterpret_cast<decltype(g_pCScriptThingVTable)>(&unused);
static void redirect(DWORD address,void* destination){
    auto* out=ASLR<unsigned char*>(address);out[0]=0xE9;
    *reinterpret_cast<int*>(out+1)=static_cast<int>(reinterpret_cast<unsigned char*>(destination)-(out+5));
    FlushInstructionCache(GetCurrentProcess(),out,5);
}
static int presentedPolls=0;
static bool failPresented=false;
static CCharString* presentedOutput=nullptr;
static CCharString* __fastcall defaultPresented(CCharString* key,void*){
    check(keys.empty());keys.push_back(key);presentedOutput=key;events.emplace_back("presented.new");return key;
}
static bool __fastcall pollPresented(CScriptThing* actor,void*,CCharString* out){
    check(actor==&guards[0] && out==presentedOutput);events.emplace_back("presented.poll");++presentedPolls;
    if(failPresented)throw std::runtime_error("PRESENTED");return presentedPolls==2;
}
static bool __fastcall presentedNotEqual(CCharString* key,void*,const char* text){
    check(key==presentedOutput && std::string(text)=="OBJECT_CHOCOLATE_BOX_UNGIVEABLE");
    events.emplace_back("presented.compare");return presentedPolls!=2;
}
static CScriptThing* retainedTrigger=nullptr;
static bool nullDepartureHero=false,nearDeparture=false,failTriggerLookup=false;
static CScriptThing departureHero{},lookupAlias{};
static CScriptThing* __fastcall lookupTrigger(CGameScriptInterfaceBase* self,void*,CScriptThing* out,const CCharString* key){
    check(self==&game && keys.size()==1 && keys.back()==key);
    retainedTrigger=out;events.emplace_back("trigger.lookup");
    if(failTriggerLookup)throw std::runtime_error("TRIGGER");return &lookupAlias;
}
static CScriptThing* __fastcall departureGetHero(CGameScriptInterfaceBase* self,void*){
    check(self==&game);events.emplace_back("hero");return nullDepartureHero?nullptr:&departureHero;
}
static bool __fastcall departureDistance(const CScriptThing* first,const CScriptThing* second,float distance){
    check(first==(nullDepartureHero?nullptr:&departureHero) && second==retainedTrigger && second!=&lookupAlias && distance==2.0f);
    events.emplace_back("distance");return nearDeparture;
}
decltype(GetThingWithScriptName_ByName_API) GetThingWithScriptName_ByName_API=reinterpret_cast<decltype(GetThingWithScriptName_ByName_API)>(&lookupTrigger);
decltype(GetHero_API) GetHero_API=reinterpret_cast<decltype(GetHero_API)>(&departureGetHero);
decltype(IsDistanceBetweenThingsUnder_API) IsDistanceBetweenThingsUnder_API=&departureDistance;
static bool talkedForOffer=false,hasOfferChocolates=false;
static bool __fastcall talkForOffer(CScriptThing* actor,void*,const CCharString* key){
    check(actor==&guards[0] && keys.size()==1 && keys.back()==key);events.emplace_back("talk.offer");return talkedForOffer;
}
static bool __fastcall offerPossession(CGameScriptInterfaceBase* self,void*,const CCharString* key,const CScriptThing* hero){
    check(self==&game && keys.size()==2 && keys.back()==key && hero==(nullDepartureHero?nullptr:&departureHero));
    events.emplace_back("possession");return hasOfferChocolates;
}
decltype(IsObjectInThingsPossession_API) IsObjectInThingsPossession_API=reinterpret_cast<decltype(IsObjectInThingsPossession_API)>(&offerPossession);
static void initActorCheck(CGameScriptInterfaceBase* self,const CScriptThing* actor){check(self==&game && actor==&guards[0]);}
static void __fastcall initDamage(CGameScriptInterfaceBase* self,void*,const CScriptThing* actor,bool value){initActorCheck(self,actor);check(!value);events.emplace_back("damage");}
static void __fastcall initKill(CGameScriptInterfaceBase* self,void*,const CScriptThing* actor,bool value,bool extra){initActorCheck(self,actor);check(!value&&!extra);events.emplace_back("kill");}
static void __fastcall initCombo(CGameScriptInterfaceBase* self,void*,const CScriptThing* actor,bool value){initActorCheck(self,actor);check(!value);events.emplace_back("combo");}
static void __fastcall initInformation(CGameScriptInterfaceBase* self,void*,const CScriptThing* actor,bool a,bool b,bool c){initActorCheck(self,actor);check(!a&&b&&!c);events.emplace_back("info");}
static void __fastcall initPushable(CGameScriptInterfaceBase* self,void*,CScriptThing copy,bool value){
    check(self==&game && !value && copy.pVTable==guards[0].pVTable && copy.pImp.Data==guards[0].pImp.Data && copy.pImp.Info==guards[0].pImp.Info);
    if(copy.pImp.Info){check(copy.pImp.Info->RefCount==8);--copy.pImp.Info->RefCount;}
    events.emplace_back("pushable");
}
static void __fastcall initMovement(CGameScriptInterfaceBase* self,void*,const CScriptThing* actor,bool value){initActorCheck(self,actor);check(!value);events.emplace_back("movement");}
decltype(EntitySetAsDamageable_API) EntitySetAsDamageable_API=reinterpret_cast<decltype(EntitySetAsDamageable_API)>(&initDamage);
decltype(EntitySetAsKillable_API) EntitySetAsKillable_API=reinterpret_cast<decltype(EntitySetAsKillable_API)>(&initKill);
decltype(EntitySetAsToAddToComboMultiplierWhenHit_API) EntitySetAsToAddToComboMultiplierWhenHit_API=reinterpret_cast<decltype(EntitySetAsToAddToComboMultiplierWhenHit_API)>(&initCombo);
decltype(SetThingHasInformation_API) SetThingHasInformation_API=reinterpret_cast<decltype(SetThingHasInformation_API)>(&initInformation);
decltype(SetIsPushableByHero_API) SetIsPushableByHero_API=reinterpret_cast<decltype(SetIsPushableByHero_API)>(&initPushable);
decltype(EntitySetAsUseMovementInActions_API) EntitySetAsUseMovementInActions_API=reinterpret_cast<decltype(EntitySetAsUseMovementInActions_API)>(&initMovement);
int main(){try{
    auto* image=VirtualAlloc(nullptr,0xA00000,MEM_COMMIT|MEM_RESERVE,PAGE_EXECUTE_READWRITE);check(image!=nullptr);
    g_fableBase=reinterpret_cast<DWORD>(image);
    redirect(0xCBED82,reinterpret_cast<void*>(&removeGuards));redirect(0x8AC970,reinterpret_cast<void*>(&destroyGuards));
    redirect(0x99E4B0,reinterpret_cast<void*>(&defaultPresented));redirect(0x99E960,reinterpret_cast<void*>(&presentedNotEqual));
    sol::state lua;lua.open_libraries(sol::lib::base,sol::lib::string);
    auto r=lua.new_usertype<LuaRetailResources>("ResourceScope",sol::no_constructor);
    r["NewTheresaGuardVector"]=&LuaRetailResources::NewTheresaGuardVector;r["Close"]=&LuaRetailResources::Close;
    r["NewResource"]=&LuaRetailResources::NewResource;r["TryAcquire"]=&LuaRetailResources::TryAcquire;
    r["NewThingFromResource"]=&LuaRetailResources::NewThingFromResource;r["DestroyThing"]=&LuaRetailResources::DestroyThing;
    r["NewPresentedItemOutput"]=&LuaRetailResources::NewPresentedItemOutput;
    r["PollPresentedItem"]=&LuaRetailResources::PollPresentedItem;
    r["PresentedItemMatches"]=&LuaRetailResources::PresentedItemMatches;
    r["DestroyPresentedItemOutput"]=&LuaRetailResources::DestroyPresentedItemOutput;
    r["NewTheresaDepartureTrigger"]=&LuaRetailResources::NewTheresaDepartureTrigger;
    r["IsTheresaHeroNearTrigger"]=&LuaRetailResources::IsTheresaHeroNearTrigger;
    r["TheresaTalkOffersChocolates"]=&LuaRetailResources::TheresaTalkOffersChocolates;
    r["InitializeTheresaActor"]=&LuaRetailResources::InitializeTheresaActor;
    auto g=lua.new_usertype<RetailTheresaGuardVector>("Guards",sol::no_constructor);
    g["RemoveLivingGuards"]=&RetailTheresaGuardVector::RemoveLivingGuards;g["Close"]=&RetailTheresaGuardVector::Close;
    int cases=0;
    for(bool explicitClose:{false,true})for(bool scopeDestroy:{false,true}){
        events.clear();auto scope=std::make_unique<LuaRetailResources>(&game);lua["scope"]=scope.get();lua["explicitClose"]=explicitClose;
        lua.script("saved=scope:NewTheresaGuardVector();saved:RemoveLivingGuards();if explicitClose then saved:Close() end");
        if(scopeDestroy)scope.reset();else {scope->Close();scope->Close();}
        check(events==std::vector<std::string>{"key","get","key.close","remove","vector.close"} && keys.empty());
        lua.script("assert(not pcall(function() saved:RemoveLivingGuards() end));saved:Close();saved=nil;collectgarbage('collect')");
        if(!scopeDestroy){lua.script("assert(not pcall(function() scope:NewTheresaGuardVector() end))");scope.reset();}
        check(events.size()==5);lua["scope"]=sol::nil;++cases;
    }
    events.clear();{
        LuaRetailResources scope(&game);lua["scope"]=&scope;failGet=1;
        lua.script("local ok,e=pcall(function() saved=scope:NewTheresaGuardVector() end);assert(not ok and string.find(e,'GET',1,true))");
        scope.Close();check(keys.empty() && events==std::vector<std::string>{"key","get","key.close","vector.close"});
    }++cases;
    failGet=0;
    CScriptGameResourceObjectScriptedThingBaseVTable expertTable{};
    expert.pVTable=reinterpret_cast<decltype(expert.pVTable)>(&expertTable);
    lua["actor"]=&guards[0];
    for(int mode:{0,1,2}){
        events.clear();getterMode=mode;
        expertTable.GetScriptThing=mode==2?nullptr:reinterpret_cast<decltype(expertTable.GetScriptThing)>(&getThing);
        {LuaRetailResources scope(&game);lua["scope"]=&scope;lua["mode"]=mode;
            lua.script("local control=scope:NewResource();assert(scope:TryAcquire(control,actor,4));local ok,id=pcall(function()return scope:NewThingFromResource(control)end);if mode==0 then assert(ok);scope:DestroyThing(id) else assert(not ok) end");
            check(events==(mode==2?std::vector<std::string>{"thing.close"}:std::vector<std::string>{"thing.get","thing.close"}));
            scope.Close();}
        check(events.back()=="resource.close");check(events.size()==(mode==2?2:3));++cases;
    }
    CScriptThingVTable actorTable{};
    actorTable.MsgIsPresentedWithItem=reinterpret_cast<decltype(actorTable.MsgIsPresentedWithItem)>(&pollPresented);
    guards[0].pVTable=reinterpret_cast<decltype(guards[0].pVTable)>(&actorTable);
    for(bool fail:{false,true}){
        events.clear();presentedPolls=0;failPresented=fail;
        {LuaRetailResources scope(&game);lua["scope"]=&scope;lua["failPresented"]=fail;
            lua.script(R"(
                local first=scope:NewPresentedItemOutput(actor)
                local ok=pcall(function() assert(not scope:PollPresentedItem(first)) end)
                if failPresented then assert(not ok) else
                    assert(ok and not scope:PresentedItemMatches(first,"OBJECT_CHOCOLATE_BOX_UNGIVEABLE"))
                    assert(scope:PollPresentedItem(first))
                    assert(scope:PresentedItemMatches(first,"OBJECT_CHOCOLATE_BOX_UNGIVEABLE"))
                    scope:DestroyPresentedItemOutput(first)
                    local second=scope:NewPresentedItemOutput(actor)
                    assert(second>first)
                    assert(not pcall(function() scope:PollPresentedItem(first) end))
                    assert(not pcall(function() scope:DestroyPresentedItemOutput(first) end))
                end
            )");
            scope.Close();scope.Close();
        }
        check(keys.empty());
        check(events==(fail?std::vector<std::string>{"presented.new","presented.poll","key.close"}:
            std::vector<std::string>{"presented.new","presented.poll","presented.compare","presented.poll","presented.compare","key.close","presented.new","key.close"}));
        ++cases;
    }
    for(bool emptyHero:{false,true})for(bool inRange:{false,true}){
        events.clear();nullDepartureHero=emptyHero;nearDeparture=inRange;
        {LuaRetailResources scope(&game);lua["scope"]=&scope;lua["nearDeparture"]=inRange;
            lua.script(R"(
                local trigger=scope:NewTheresaDepartureTrigger()
                assert(scope:IsTheresaHeroNearTrigger(trigger)==nearDeparture)
                assert(scope:IsTheresaHeroNearTrigger(trigger)==nearDeparture)
                scope:DestroyThing(trigger)
                assert(not pcall(function() scope:IsTheresaHeroNearTrigger(trigger) end))
            )");scope.Close();}
        check(keys.empty() && events==std::vector<std::string>{"key","trigger.lookup","key.close","hero","distance","hero","distance","thing.close"});++cases;
    }
    events.clear();failTriggerLookup=true;
    {LuaRetailResources scope(&game);lua["scope"]=&scope;
        lua.script("local ok,e=pcall(function()scope:NewTheresaDepartureTrigger()end);assert(not ok and string.find(e,'TRIGGER',1,true))");
        check(keys.empty() && events==std::vector<std::string>{"key","trigger.lookup","key.close","thing.close"});
        scope.Close();check(events.size()==4);++cases;
    }
    actorTable.MsgIsTalkedToBy=reinterpret_cast<decltype(actorTable.MsgIsTalkedToBy)>(&talkForOffer);
    for(bool talk:{false,true})for(bool has:{false,true}){
        events.clear();talkedForOffer=talk;hasOfferChocolates=has;
        {LuaRetailResources scope(&game);lua["scope"]=&scope;lua["offers"]=talk&&has;
            lua.script("assert(scope:TheresaTalkOffersChocolates(actor)==offers)");scope.Close();}
        check(keys.empty() && events==(talk?std::vector<std::string>{"key","talk.offer","key","hero","possession","key.close","key.close"}:
            std::vector<std::string>{"key","talk.offer","key.close"}));++cases;
    }
    std::remove_pointer_t<decltype(guards[0].pImp.Info)> initInfo{};initInfo.RefCount=7;
    for(bool populated:{false,true})for(bool counted:{false,true}){
        guards[0].pImp.Data=populated?reinterpret_cast<decltype(guards[0].pImp.Data)>(0x4242):nullptr;
        guards[0].pImp.Info=counted?&initInfo:nullptr;events.clear();
        {LuaRetailResources scope(&game);lua["scope"]=&scope;lua.script("scope:InitializeTheresaActor(actor)");scope.Close();}
        check(initInfo.RefCount==7 && events==std::vector<std::string>{"damage","kill","combo","info","pushable","movement"});++cases;
    }
    guards[0].pImp.Info=nullptr;guards[0].pImp.Data=nullptr;
    lua["scope"]=sol::nil;VirtualFree(image,0,MEM_RELEASE);
    std::cout<<"PASS: "<<cases<<" full staged scope/borrowed-vector policies\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
