#define main TheresaReferenceMain
#include "theresa_scope_runtime_harness.cpp"
#undef main
static int objectiveStep=0,objectiveFailure=0,objectiveAlias=0,objectiveCloses=0;
static bool objectiveCleanupFailure=false;
static CCharString returnedAlias{};
static std::vector<CCharString*> objectiveKeys;
static CCharString* ownedName=nullptr;
static void step(const std::string& name) {
    events.push_back(name);++objectiveStep;
    if(objectiveStep==objectiveFailure)throw std::runtime_error("PRIMARY");
}
static void __fastcall objectiveConstruct(CCharString* out,void*,const char* text,int length) {
    check(length==-1 && objectiveKeys.size()<3);
    check(std::string(text)==(objectiveKeys.size()==2?"TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_01":""));
    step("key"+std::to_string(objectiveKeys.size()));objectiveKeys.push_back(out);
}
static CCharString* __fastcall objectiveGet(CGameScriptInterfaceBase* self,void*,CCharString* out) {
    check(self==&game && objectiveKeys.size()==3 && !ownedName);
    step("get");ownedName=out;
    return objectiveAlias==0?out:objectiveAlias==1?&returnedAlias:nullptr;
}
static void __fastcall objectiveSet(CGameScriptInterfaceBase* self,void*,const CCharString* name,
    const CCharString* objective,const CCharString* region1,const CCharString* region2) {
    check(self==&game && ownedName && objectiveKeys.size()==3);
    check(name==(objectiveAlias==0?ownedName:objectiveAlias==1?&returnedAlias:nullptr));
    check(objective==objectiveKeys[2] && region1==objectiveKeys[1] && region2==objectiveKeys[0]);step("set");
}
static void __fastcall objectiveDestroy(CCharString* key,void*) {
    if(ownedName){check(key==ownedName && key!=&returnedAlias);ownedName=nullptr;events.emplace_back("name.close");}
    else {check(!objectiveKeys.empty() && key==objectiveKeys.back());events.emplace_back("key.close"+std::to_string(objectiveKeys.size()-1));objectiveKeys.pop_back();}
    ++objectiveCloses;if(objectiveCleanupFailure)throw std::runtime_error("CLEANUP");
}
decltype(GetActiveQuestName_API) GetActiveQuestName_API=reinterpret_cast<decltype(GetActiveQuestName_API)>(&objectiveGet);
decltype(SetQuestCardObjective_API) SetQuestCardObjective_API=reinterpret_cast<decltype(SetQuestCardObjective_API)>(&objectiveSet);

int main(){try{
    CCharString_Construct_Literal=reinterpret_cast<decltype(CCharString_Construct_Literal)>(&objectiveConstruct);
    CCharString_Destroy=reinterpret_cast<decltype(CCharString_Destroy)>(&objectiveDestroy);
    sol::state lua;lua.open_libraries(sol::lib::base,sol::lib::string);
    auto type=lua.new_usertype<LuaRetailResources>("ObjectiveResources",sol::no_constructor);
    type["SetInitialOakvaleObjective"]=&LuaRetailResources::SetInitialOakvaleObjective;
    type["Close"]=&LuaRetailResources::Close;
    int cases=0;
    for(int alias:{0,1,2})for(int failure:{0,1,2,3,4,5})for(bool cleanupFailure:{false,true}) {
        objectiveAlias=alias;objectiveFailure=failure;objectiveCleanupFailure=cleanupFailure;
        objectiveStep=objectiveCloses=0;objectiveKeys.clear();ownedName=nullptr;events.clear();
        {LuaRetailResources scope(&game);lua["scope"]=&scope;lua["failure"]=failure;lua["cleanupFailure"]=cleanupFailure;
            lua.script(R"(
                local ok,error=pcall(function()scope:SetInitialOakvaleObjective()end)
                if failure~=0 then assert(not ok and string.find(error,'PRIMARY',1,true))
                elseif cleanupFailure then assert(not ok and string.find(error,'CLEANUP',1,true))
                else assert(ok) end
                scope:Close();scope:Close()
                assert(not pcall(function()scope:SetInitialOakvaleObjective()end))
            )");
        }
        check(!ownedName && objectiveKeys.empty());
        check(objectiveCloses==(failure==0?4:failure<=4?failure-1:4));
        if(failure==0)check(events==std::vector<std::string>{"key0","key1","key2","get","set","name.close","key.close2","key.close1","key.close0"});
        ++cases;
    }
    lua["scope"]=sol::nil;
    std::cout<<"PASS: "<<cases<<" objective string lifetime/alias/error policies\n";return 0;
}catch(const std::exception& error){std::cerr<<error.what()<<'\n';return 1;}}
