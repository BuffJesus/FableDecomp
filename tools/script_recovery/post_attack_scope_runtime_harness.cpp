#define main TheresaReferenceMain
#include "theresa_scope_runtime_harness.cpp"
#undef main

static bool postEmpty=false,postResult=false,postPopulate=false;
static int postFailure=0,postHeroCalls=0,postAcquireCalls=0,postCloses=0;
static CScriptGameResourceObjectScriptedThingBase* postControl=nullptr;
static CScriptThing* __fastcall postHero(CGameScriptInterfaceBase* self,void*) {
    check(self==&game);++postHeroCalls;events.emplace_back("hero");
    if(postFailure==1)throw std::runtime_error("HERO_FAILURE");
    return postEmpty?nullptr:&departureHero;
}
static bool __fastcall postAcquire(CGameScriptInterfaceBase* self,void*,const CScriptThing* actor,
    CScriptGameResourceObjectScriptedThingBase* resource,EScriptAIPriority priority) {
    check(self==&game && actor==(postEmpty?nullptr:&departureHero) && static_cast<int>(priority)==4);
    if(postControl)check(resource==postControl);
    postControl=resource;++postAcquireCalls;events.emplace_back("acquire");
    resource->pImp.Data=postPopulate?&expert:nullptr;
    if(postFailure==2)throw std::runtime_error("ACQUIRE_FAILURE");
    return postResult;
}
static void __fastcall postClose(CScriptGameResourceObjectScriptedThingBase* resource,void*) {
    if(postControl)check(resource==postControl);
    check(resource->pImp.Data==(postAcquireCalls && postPopulate?&expert:nullptr));
    ++postCloses;events.emplace_back("close");
}

int main(){try{
    GetHero_API=reinterpret_cast<decltype(GetHero_API)>(&postHero);
    StartScriptingEntity_API=reinterpret_cast<decltype(StartScriptingEntity_API)>(&postAcquire);
    CSGROSTB_Destroy_API=reinterpret_cast<decltype(CSGROSTB_Destroy_API)>(&postClose);
    sol::state lua;lua.open_libraries(sol::lib::base,sol::lib::string);
    auto type=lua.new_usertype<LuaRetailResources>("PostAttackResources",sol::no_constructor);
    type["NewResource"]=&LuaRetailResources::NewResource;
    type["TryAcquirePostAttackHero"]=&LuaRetailResources::TryAcquirePostAttackHero;
    type["ReleaseResource"]=&LuaRetailResources::ReleaseResource;
    type["Close"]=&LuaRetailResources::Close;
    int cases=0;
    for(bool empty:{false,true})for(bool result:{false,true})for(bool populate:{false,true})for(int failure:{0,1,2}){
        postEmpty=empty;postResult=result;postPopulate=populate;postFailure=failure;
        postHeroCalls=postAcquireCalls=postCloses=0;postControl=nullptr;events.clear();
        {LuaRetailResources scope(&game);lua["scope"]=&scope;lua["expected"]=result;lua["failure"]=failure;
            lua.script(R"(
                control=scope:NewResource()
                local ok,value=pcall(function() return scope:TryAcquirePostAttackHero(control,4) end)
                if failure==0 then assert(ok and value==expected)
                else assert(not ok and string.find(value,failure==1 and 'HERO_FAILURE' or 'ACQUIRE_FAILURE',1,true)) end
                scope:ReleaseResource(control)
                assert(not pcall(function() scope:TryAcquirePostAttackHero(control,4) end))
                scope:Close();scope:Close()
                assert(not pcall(function() scope:TryAcquirePostAttackHero(control,4) end))
            )");
        }
        check(postHeroCalls==1 && postAcquireCalls==(failure==1?0:1) && postCloses==1);
        check(events==(failure==1?std::vector<std::string>{"hero","close"}:std::vector<std::string>{"hero","acquire","close"}));++cases;
    }
    // A second call must query Hero again rather than retain the first result.
    postFailure=0;postPopulate=false;postResult=false;postEmpty=true;
    postHeroCalls=postAcquireCalls=postCloses=0;postControl=nullptr;events.clear();
    {LuaRetailResources scope(&game);lua["scope"]=&scope;
        lua.script("control=scope:NewResource();assert(not scope:TryAcquirePostAttackHero(control,4))");
        postEmpty=false;postResult=true;
        lua.script("assert(scope:TryAcquirePostAttackHero(control,4));scope:ReleaseResource(control)");scope.Close();
    }
    check(postHeroCalls==2 && postAcquireCalls==2 && postCloses==1);++cases;
    // API availability is checked before querying Hero or altering ownership.
    for(int missing:{1,2}){
        postHeroCalls=postAcquireCalls=postCloses=0;postControl=nullptr;events.clear();
        {LuaRetailResources scope(&game);lua["scope"]=&scope;lua.script("control=scope:NewResource()");
            if(missing==1)GetHero_API=nullptr;else StartScriptingEntity_API=nullptr;
            lua.script("assert(not pcall(function()scope:TryAcquirePostAttackHero(control,4)end));scope:ReleaseResource(control)");
            GetHero_API=reinterpret_cast<decltype(GetHero_API)>(&postHero);
            StartScriptingEntity_API=reinterpret_cast<decltype(StartScriptingEntity_API)>(&postAcquire);
            scope.Close();
        }
        check(postHeroCalls==0 && postAcquireCalls==0 && postCloses==1);++cases;
    }
    lua["scope"]=sol::nil;
    std::cout<<"PASS: "<<cases<<" post-attack Hero acquisition and owner policies\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
