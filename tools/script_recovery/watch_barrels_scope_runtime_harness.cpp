#define main TheresaReferenceMain
#include "theresa_scope_runtime_harness.cpp"
#undef main
static int snapshotCount=0,refreshResult=0,refreshCalls=0,snapshotCloses=0;
static bool failRefresh=false;
static RetailBarrelWatchSnapshot::Vector* savedVector=nullptr;
static int rewardStep=0,failRewardStep=0;
static bool failRewardCleanup=false;
static CScriptThing* rewardThing=nullptr;
static std::vector<std::string> rewardKeys;
static void rewardEvent(const std::string& name,bool cleanup=false) {
    events.push_back(name);++rewardStep;
    if(rewardStep==failRewardStep)throw std::runtime_error("STEP"+std::to_string(rewardStep));
    if(cleanup && failRewardCleanup)throw std::runtime_error("CLEANUP");
}
static void __fastcall rewardKey(CCharString* key,void*,const char* text,int length) {
    check(length==-1);rewardEvent(std::string("key:")+text);keys.push_back(key);rewardKeys.emplace_back(text);
}
static void __fastcall rewardKeyClose(CCharString* key,void*) {
    check(!keys.empty() && keys.back()==key);keys.pop_back();rewardKeys.pop_back();rewardEvent("key.close",true);
}
static CScriptThing* __fastcall rewardLookup(CGameScriptInterfaceBase* self,void*,CScriptThing* out,const CCharString* key) {
    check(self==&game && keys.size()==1 && keys.back()==key && rewardKeys.back()=="NOVI_Barrel" && !rewardThing);
    rewardEvent("lookup");rewardThing=out;return &lookupAlias;
}
static void __fastcall rewardGold(CGameScriptInterfaceBase* self,void*,const CScriptThing* target,const CCharString* key) {
    check(self==&game && target==rewardThing && target!=&lookupAlias && keys.size()==1 && keys.back()==key && rewardKeys.back()=="OBJECT_GOLD_1");
    rewardEvent("gold");
}
static CScriptThing* __fastcall rewardCreate(CGameScriptInterfaceBase* self,void*,CScriptThing* out,const CCharString* definition,
    const C3DVector* point,const CCharString* script,bool flag) {
    check(self==&game && !rewardThing && keys.size()==2 && keys[0]==script && keys[1]==definition && !flag);
    check(rewardKeys[0]=="NOVI_CreatedBeetle" && rewardKeys[1]=="CREATURE_OAKVALE_STAG_BEETLE");
    check(point->x==1.0f && point->y==-2.0f && point->z==3.5f);
    rewardEvent("create");rewardThing=out;return &lookupAlias;
}
static void __fastcall rewardHealth(CGameScriptInterfaceBase* self,void*,const CScriptThing* target,float health,bool flag) {
    check(self==&game && target==rewardThing && target!=&lookupAlias && keys.empty() && health==2.0f && flag);rewardEvent("health");
}
static void __fastcall rewardThingClose(CScriptThing* thing,void*) {
    check(thing==rewardThing && thing!=&lookupAlias && keys.empty());rewardThing=nullptr;rewardEvent("thing.close",true);
}
decltype(AddItemToContainer_API) AddItemToContainer_API=reinterpret_cast<decltype(AddItemToContainer_API)>(&rewardGold);
decltype(CreateCreature_API) CreateCreature_API=reinterpret_cast<decltype(CreateCreature_API)>(&rewardCreate);
decltype(EntitySetMaxHealth_API) EntitySetMaxHealth_API=reinterpret_cast<decltype(EntitySetMaxHealth_API)>(&rewardHealth);
static void __fastcall snapshotKey(CCharString* key,void*,const char* text,int length) {
    check(std::string(text)=="NOVI_Barrel" && length==-1);keys.push_back(key);events.emplace_back("key");
}
static int __fastcall snapshotGet(CGameScriptInterfaceBase* self,void*,const CCharString* key,RetailBarrelWatchSnapshot::Vector* vector) {
    check(self==&game && keys.size()==1 && keys.back()==key);
    if(refreshCalls==0)check(!vector->begin && !vector->end && !vector->capacity);
    else check(vector==savedVector);
    savedVector=vector;++refreshCalls;
    vector->begin=guards;vector->end=guards+snapshotCount;vector->capacity=guards+2;
    events.emplace_back("refresh");if(failRefresh)throw std::runtime_error("REFRESH");return refreshResult;
}
static void __fastcall snapshotClose(RetailBarrelWatchSnapshot::Vector* vector,void*) {
    check(vector==savedVector && vector->begin==guards && vector->end==guards+snapshotCount && keys.empty());
    ++snapshotCloses;events.emplace_back("snapshot.close");
}
int main(){try {
    auto* image=VirtualAlloc(nullptr,0xA00000,MEM_COMMIT|MEM_RESERVE,PAGE_EXECUTE_READWRITE);check(image!=nullptr);
    g_fableBase=reinterpret_cast<DWORD>(image);redirect(0x8AC970,reinterpret_cast<void*>(&snapshotClose));
    CCharString_Construct_Literal=reinterpret_cast<decltype(CCharString_Construct_Literal)>(&snapshotKey);
    GetAllThingsWithScriptName_API=reinterpret_cast<decltype(GetAllThingsWithScriptName_API)>(&snapshotGet);
    sol::state lua;lua.open_libraries(sol::lib::base,sol::lib::string);
    auto r=lua.new_usertype<LuaRetailResources>("ResourceScope",sol::no_constructor);
    r["NewBarrelWatchSnapshot"]=&LuaRetailResources::NewBarrelWatchSnapshot;
    r["RewardRemainingBarrel"]=&LuaRetailResources::RewardRemainingBarrel;
    r["SpawnBarrelBeetle"]=&LuaRetailResources::SpawnBarrelBeetle;
    auto s=lua.new_usertype<RetailBarrelWatchSnapshot>("RetailBarrelWatchSnapshot",sol::no_constructor);
    s["Refresh"]=&RetailBarrelWatchSnapshot::Refresh;s["Count"]=&RetailBarrelWatchSnapshot::Count;s["Close"]=&RetailBarrelWatchSnapshot::Close;
    int cases=0;
    for(int total:{0,2})for(int result:{0,1,7})for(bool failing:{false,true})for(bool explicitClose:{false,true}) {
        snapshotCount=total;refreshResult=result;failRefresh=failing;refreshCalls=0;snapshotCloses=0;savedVector=nullptr;events.clear();
        auto scope=std::make_unique<LuaRetailResources>(&game);lua["scope"]=scope.get();
        lua["total"]=total;lua["result"]=result;lua["failing"]=failing;lua["explicitClose"]=explicitClose;
        lua.script(R"(
            saved=scope:NewBarrelWatchSnapshot()
            assert(saved:Count()==0)
            local ok,err=pcall(function() assert(saved:Refresh()==result) end)
            if failing then assert(not ok and string.find(err,"REFRESH",1,true)) else
                assert(ok and saved:Count()==total)
                assert(saved:Refresh()==result and saved:Count()==total)
            end
            if explicitClose then saved:Close();saved:Close() end
        )");
        scope->Close();scope->Close();scope.reset();lua["scope"]=sol::nil;
        lua.script("assert(not pcall(function()saved:Refresh()end));assert(not pcall(function()saved:Count()end));saved:Close();saved=nil;collectgarbage('collect')");
        check(keys.empty() && snapshotCloses==1 && refreshCalls==(failing?1:2));
        check(events==(failing?std::vector<std::string>{"key","refresh","key.close","snapshot.close"}:
            std::vector<std::string>{"key","refresh","key.close","key","refresh","key.close","snapshot.close"}));++cases;
    }
    CCharString_Construct_Literal=reinterpret_cast<decltype(CCharString_Construct_Literal)>(&rewardKey);
    CCharString_Destroy=reinterpret_cast<decltype(CCharString_Destroy)>(&rewardKeyClose);
    GetThingWithScriptName_ByName_API=reinterpret_cast<decltype(GetThingWithScriptName_ByName_API)>(&rewardLookup);
    RetailThing_Destroy_API=reinterpret_cast<decltype(RetailThing_Destroy_API)>(&rewardThingClose);
    for(bool beetle:{false,true})for(int fail=0;fail<=7;++fail)for(bool cleanup:{false,true}) {
        rewardStep=0;failRewardStep=fail;failRewardCleanup=cleanup;events.clear();
        LuaRetailResources scope(&game);lua["scope"]=&scope;lua["beetle"]=beetle;
        int firstClose=beetle?4:3;
        std::string expected=fail?"STEP"+std::to_string(fail):"";
        if(cleanup && (fail==0 || fail>firstClose))expected="CLEANUP";
        lua["expected"]=expected;
        lua.script(R"(
            local ok,err=pcall(function()
                if beetle then scope:SpawnBarrelBeetle({x=1,y=-2,z=3.5}) else scope:RewardRemainingBarrel() end
            end)
            if expected=="" then assert(ok) else assert(not ok and string.find(err,expected,1,true)) end
        )");scope.Close();check(keys.empty() && rewardKeys.empty() && !rewardThing);
        if(!fail && !cleanup)check(events==(beetle?
            std::vector<std::string>{"key:NOVI_CreatedBeetle","key:CREATURE_OAKVALE_STAG_BEETLE","create","key.close","key.close","health","thing.close"}:
            std::vector<std::string>{"key:NOVI_Barrel","lookup","key.close","key:OBJECT_GOLD_1","gold","key.close","thing.close"}));
        ++cases;
    }
    lua["scope"]=sol::nil;VirtualFree(image,0,MEM_RELEASE);
    std::cout<<"PASS: "<<cases<<" WatchBarrels full-class snapshot/reward policies\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
