#define main resource_smoke_main
#include "runtime_checks/retail_resources_smoke.cpp"
#undef main
static std::map<CScriptThing*,std::string> actors;
static std::vector<std::string> trace;
static unsigned populatedMask=0, lookupIndex=0;
static std::string failAt;
static CGameScriptInterfaceBase* game=reinterpret_cast<CGameScriptInterfaceBase*>(1);
static void __fastcall keyCtor(CCharString* p,void*,const char* text,int n){check(n==-1);stringCtor(p,nullptr,text,n);trace.push_back("key.new:"+std::string(text));}
static void __fastcall keyDtor(CCharString* p,void*){trace.push_back("key.destroy:"+strings.at(p));stringDtor(p,nullptr);}
static CScriptThing* __fastcall lookup(CGameScriptInterfaceBase* self,void*,CScriptThing* output,const CCharString* key){
    check(self==game);const auto name=strings.at(const_cast<CCharString*>(key));actors[output]=name;
    output->pImp.Data=(populatedMask&(1u<<lookupIndex++))?reinterpret_cast<decltype(output->pImp.Data)>(1):nullptr;output->pImp.Info=nullptr;
    trace.push_back("lookup:"+name);return output;
}
static void __fastcall destroyThing(CScriptThing* output,void*){check(actors.count(output)==1);trace.push_back("destroy:"+actors.at(output));actors.erase(output);}
static void __fastcall add(CGameScriptInterfaceBase* self,void*,const CScriptThing* actor,const CCharString* key){
    check(self==game&&strings.size()==1&&strings.at(const_cast<CCharString*>(key))=="HUD_ORB_QUEST_CORE");
    const auto name=actors.at(const_cast<CScriptThing*>(actor));trace.push_back("add:"+name);if(failAt=="add:"+name)throw std::runtime_error("MARKER_FAULT");
}
static void __fastcall removeMarker(CGameScriptInterfaceBase* self,void*,const CScriptThing* actor){
    check(self==game&&strings.empty());const auto name=actors.at(const_cast<CScriptThing*>(actor));trace.push_back("remove:"+name);if(failAt=="remove:"+name)throw std::runtime_error("MARKER_FAULT");
}
tMiniMapAddMarker MiniMapAddMarker_API=reinterpret_cast<tMiniMapAddMarker>(&add);
tMiniMapRemoveMarker MiniMapRemoveMarker_API=reinterpret_cast<tMiniMapRemoveMarker>(&removeMarker);
int main(){try{
    CCharString_Construct_Literal=reinterpret_cast<decltype(CCharString_Construct_Literal)>(&keyCtor);CCharString_Destroy=reinterpret_cast<decltype(CCharString_Destroy)>(&keyDtor);
    GetThingWithScriptName_ByName_API=reinterpret_cast<tGetThingWithScriptName1>(&lookup);RetailThing_Destroy_API=reinterpret_cast<tRetailThingDestroy>(&destroyThing);
    unsigned cases=0;
    for(unsigned mask=0;mask<8;++mask)for(const char* fault:{"","add:NOVI_LiveFather","remove:NOVI_LiveFather","add:NOVI_BookTrader","remove:NOVI_BookTrader","add:NOVI_Theresa","remove:NOVI_Theresa","frame"})for(int cancel:{1,99}){
        populatedMask=mask;lookupIndex=0;trace.clear();failAt=fault;sol::state lua;lua.open_libraries(sol::lib::base);RegisterRetailResources(lua);
        auto q=lua.create_table();q["WithRetailResources"]=[&](sol::object,sol::protected_function body){WithRetailResources(game,body);};
        int query=0,goldPoll=0;
        q["GetHeroGold"]=[&](sol::object){return goldPoll++==0?2:3;};q["IsHeroControlledByPlayer"]=[](sol::object){return true;};
        q["NewScriptFrame"]=[&](sol::object){if(failAt=="frame")throw std::runtime_error("MARKER_FAULT");};
        q["IsActiveThreadTerminating"]=[&](sol::object){return ++query>=cancel;};q["DisplayTutorial"]=[](sol::object,int id){check(id==19);return true;};
        q["MsgIsTutorialClickedPast"]=[](sol::object){return true;};q["GetStateBool"]=[](sol::object,const std::string& key){check(key=="GivenSweets"||key=="GivenTheresaChocs");return true;};
        lua["quest"]=q;check(lua.safe_script_file("../CANDIDATE.lua",sol::script_pass_on_error).valid());
        auto result=lua.safe_script("ManageQuestCoreMarkers(quest)",sol::script_pass_on_error);
        const bool reachedFault=failAt=="frame"||failAt=="add:NOVI_LiveFather"||(cancel==99&&!failAt.empty());check(result.valid()==!reachedFault);
        if(!result.valid()){sol::error e=result;check(std::string(e.what()).find("MARKER_FAULT")!=std::string::npos);}
        check(actors.empty()&&strings.empty()&&lookupIndex==3);std::vector<std::string> closed;
        for(const auto& event:trace)if(event.rfind("destroy:",0)==0)closed.push_back(event);
        check(closed==std::vector<std::string>{"destroy:NOVI_Theresa","destroy:NOVI_BookTrader","destroy:NOVI_LiveFather"});
        if(cancel==1)for(const auto& event:trace)check(event.rfind("remove:",0)!=0);
        ++cases;
    }
    printf("Core marker actual-FSE/x86/real-Lua: %u full-helper policies passed\n",cases);return 0;
}catch(const std::exception& e){fprintf(stderr,"%s\n",e.what());return 1;}}

