#define main resource_smoke_main
#include "runtime_checks/retail_resources_smoke.cpp"
#undef main
DWORD g_fableBase=0;
static CScriptThing trollThing{},heroThing{};
static CScriptGameResourceObjectScriptedThingBase trollExpert{},heroExpert{};
static std::set<void*> ownedResources;
static bool emptyHero,emptyTroll,emptyMovie,played,termination,continued;
static int failure,termQueries;
static std::vector<std::string> events;
static bool __fastcall acquireActors(CGameScriptInterfaceBase*,void*,const CScriptThing* target,
    CScriptGameResourceObjectScriptedThingBase* resource,EScriptAIPriority priority) {
    check(static_cast<int>(priority)==4 && (target==&trollThing||target==&heroThing));
    bool empty=target==&heroThing?emptyHero:emptyTroll;
    if(!empty){resource->pImp.Data=target==&heroThing?&heroExpert:&trollExpert;refs[resource->pImp.Data]++;}
    ownedResources.insert(resource);return true;
}
static void __fastcall releaseActor(void* pointer,void*) {
    auto* resource=static_cast<CScriptGameResourceObjectScriptedThingBase*>(pointer);
    check(ownedResources.erase(pointer)==1);
    if(resource->pImp.Data){check(refs[resource->pImp.Data]==1);--refs[resource->pImp.Data];}
    events.push_back(ownedResources.empty()?"self.destroy":"hero.destroy");
}
static void __fastcall startExhume(CGameScriptInterfaceBase*,void*,const CCharString* name,CScriptGameResourceObjectMovieBase* movie) {
    check(strings.at(const_cast<CCharString*>(name)).empty() && movies==0 && ownedResources.size()==2);
    movies=1;if(!emptyMovie)movie->pImp.Data=reinterpret_cast<decltype(movie->pImp.Data)>(1);
    events.push_back("movie.start");
}
static void __fastcall stopExhume(CScriptGameResourceObjectMovieBase*,void*) {
    check(movies==1 && !paused && maps.size()==1 && ownedResources.size()==2);movies=0;events.push_back("movie.destroy");
}
static void __fastcall pauseExhume(CGameScriptInterfaceBase*,void*,bool value) {paused=value;events.push_back(value?"pause.true":"pause.false");}
static void __fastcall destroyExhumeMap(void* map,void*) {
    check(movies==0 && !paused && ownedResources.size()==2);events.push_back("map.destroy");mapDtor(map,nullptr);
}
static void __fastcall runExhume(const CCharString* name,void* actorMap,void* flags,void* input,bool setup,bool skippable) {
    check(strings.at(const_cast<CCharString*>(name))=="CS_ROCKTROLL_EXHUME" && !flags && !input && !setup && skippable);
    check(movies==1 && paused && ownedResources.size()==2);
    auto& actors=maps.at(actorMap);check(actors.size()==2);
    check(actors.at("HERO").pImp.Data==(emptyHero?nullptr:&heroExpert));
    check(actors.at("TROLL").pImp.Data==(emptyTroll?nullptr:&trollExpert));
    if(!emptyHero)check(refs[&heroExpert]==2);
    if(!emptyTroll)check(refs[&trollExpert]==2);
    events.push_back("macro");
    if(failure==1)throw std::runtime_error("MACRO");
    if(failure==3)termination=true;
}
class ExhumeQuest {};
int main() {
    try {
        StartScriptingEntity_API=reinterpret_cast<tStartScriptingEntity>(&acquireActors);
        CSGROSTB_Destroy_API=reinterpret_cast<tCSGROSTB_Destructor>(&releaseActor);
        StartMovieSequence_API=reinterpret_cast<tStartMovieSequence>(&startExhume);
        MovieResource_Destroy_API=reinterpret_cast<tMovieResource_Destructor>(&stopExhume);
        PauseAllNonScriptedEntities_API=reinterpret_cast<tPauseAllNonScriptedEntities>(&pauseExhume);
        StdMap_Destroy_API=reinterpret_cast<tStdMap_Destructor>(&destroyExhumeMap);
        RunCutsceneMacro_Func=&runExhume;
        int cases=0;
        for(bool hero:{false,true})for(bool troll:{false,true})for(bool movie:{false,true})for(int mode=0;mode<4;++mode) {
            emptyHero=hero;emptyTroll=troll;emptyMovie=movie;failure=mode;played=termination=continued=false;termQueries=0;events.clear();
            check(strings.empty()&&maps.empty()&&ownedResources.empty()&&movies==0&&!paused);
            sol::state lua;lua.open_libraries(sol::lib::base);RegisterRetailResources(lua);
            auto type=lua.new_usertype<ExhumeQuest>("ExhumeQuest",sol::no_constructor);
            type["GetStateBool"]=[](ExhumeQuest&,const std::string& key){check(key=="PlayedExhumeCutScene");return played;};
            type["SetStateBool"]=[](ExhumeQuest&,const std::string& key,bool value){check(key=="PlayedExhumeCutScene"&&movies==1&&paused);played=value;events.push_back("state");};
            type["IsActiveThreadTerminating"]=[](ExhumeQuest&){++termQueries;return termination;};
            type["GetHero"]=[](ExhumeQuest&){return &heroThing;};
            type["CreateRetainedThingThread"]=[](ExhumeQuest&,const std::string& name,CScriptThing* target,bool flag,const std::string& region){
                check(name=="WatchForRockTrollHit"&&target==&trollThing&&!flag&&region.empty()&&movies==1&&paused);
                events.push_back("thread");if(failure==2)throw std::runtime_error("THREAD");
            };
            type["WithRetailResources"]=[](ExhumeQuest&,sol::protected_function callback){WithRetailResources(reinterpret_cast<CGameScriptInterfaceBase*>(1),callback);};
            ExhumeQuest quest;lua["quest"]=&quest;lua["me"]=&trollThing;
            lua["continue_live"]=[](){check(ownedResources.size()==1&&maps.empty()&&movies==0&&!paused&&played);continued=true;};
            check(lua.safe_script_file("rock_exhumation.lua",sol::script_pass_on_error).valid());
            auto result=lua.safe_script(R"(
                quest:WithRetailResources(function(resources)
                    local self=resources:NewResource()
                    resources:PrepareResource(self)
                    assert(resources:TryAcquire(self,me,4))
                    WithRockTrollExhumationPhase(quest,me,resources,self,continue_live)
                end)
            )",sol::script_pass_on_error);
            bool success=mode==0||mode==3;check(result.valid()==success&&played==success&&continued==success);
            check(termQueries==2 && strings.empty()&&maps.empty()&&ownedResources.empty()&&movies==0&&!paused);
            check(refs[&heroExpert]==0&&refs[&trollExpert]==0);
            std::vector<std::string> suffix={"pause.false","movie.destroy","map.destroy","hero.destroy","self.destroy"};
            check(events.size()>=suffix.size()&&std::equal(suffix.begin(),suffix.end(),events.end()-suffix.size()));
            if(!success){sol::error error=result;check(std::string(error.what()).find(mode==1?"MACRO":"THREAD")!=std::string::npos);}
            ++cases;
        }
        std::cout<<"Rock exhumation x86: "<<cases<<" real-Lua map/movie/resource policies passed\n";return 0;
    }catch(const std::exception& error){std::cerr<<error.what()<<'\n';return 1;}
}
