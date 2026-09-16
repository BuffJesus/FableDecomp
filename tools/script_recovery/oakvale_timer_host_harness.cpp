#include "oakvale_timer_host_policy.h"
#include <iostream>
#include <map>
#include <vector>
DWORD g_fableBase=0;
static CGameScriptInterfaceBase game{};
static void* methods[91]{};
static int registrations=0,closed=0,nextId=-2,failRegistration=0;
static std::vector<int> cleanup;
static void check(bool value){if(!value)throw std::runtime_error("timer host check failed");}
static CGameScriptInterfaceBase* current(){return &game;}
static int __fastcall reg(CGameScriptInterfaceBase* self,void*){check(self==&game);if(++registrations==failRegistration)throw std::runtime_error("REGISTER_FAULT");return nextId++;}
static void __fastcall dereg(CGameScriptInterfaceBase* self,void*,int id){check(self==&game);++closed;cleanup.push_back(id);}
class LuaManager {
public:
    std::map<std::string,int> persisted;
    static LuaManager& GetInstance(){static LuaManager instance;return instance;}
    void SetGlobalStateInt(const std::string& key,int value){persisted[key]=value;}
    int GetGlobalStateInt(const std::string& key){return persisted[key];}
};
class GeneratedState {
public:
    NativeQuestTimerOwner m_nativeTimers;
    std::string GetNamespacedKey(const std::string& key){return "path/quest.lua:"+key;}
    int GetStateInt(const std::string& key);
    void SetStateInt(const std::string& key,int value);
};
// Extracted from the staged actual LuaQuestState.cpp; only class name changes.
#include "state_int_methods.inc"
std::vector<NativeQuestSlot> g_questScriptFileNames;
class LuaQuestHost {
public:
    char base=0;
    std::string file;
    NativeQuestLifetime lifetime;
    LuaQuestHost(CScriptDataBase*,CGameScriptInterfaceBase* interface,const std::string& path,NativeQuestLifetime policy):file(path),lifetime(policy){check(interface==&game);}
};
// Actual staged allocator body. Constructor is a recording boundary double.
#include "allocator.inc"
int main(){try{
    methods[0x15c/4]=reinterpret_cast<void*>(&reg);methods[0x160/4]=reinterpret_cast<void*>(&dereg);*reinterpret_cast<void***>(&game)=methods;
    unsigned cases=0;
    for(const char* metadata:{"nil","'NewOakValeIntro'","'BadPolicy'","false","12","{}"}) {
        sol::state lua;lua.open_libraries(sol::lib::base);lua.safe_script(std::string("metadata=")+metadata);bool accepted=true;NativeQuestLifetime policy{};
        try{policy=ParseNativeQuestLifetime(lua["metadata"]);}catch(...){accepted=false;}
        check(accepted==(std::string(metadata)=="nil"||std::string(metadata)=="'NewOakValeIntro'"));
        if(accepted)check(policy==(std::string(metadata)=="nil"?NativeQuestLifetime::None:NativeQuestLifetime::NewOakValeIntro));++cases;
    }
    for(bool first:{false,true})for(bool second:{false,true}) {
        g_questScriptFileNames={{"arbitrary/override.lua",first?NativeQuestLifetime::NewOakValeIntro:NativeQuestLifetime::None},{"other/sorted.lua",second?NativeQuestLifetime::NewOakValeIntro:NativeQuestLifetime::None}};
        auto* a=reinterpret_cast<LuaQuestHost*>(QuestAllocator<0>(nullptr,&game));auto* b=reinterpret_cast<LuaQuestHost*>(QuestAllocator<1>(nullptr,&game));
        check(a->file==g_questScriptFileNames[0].file&&a->lifetime==g_questScriptFileNames[0].lifetime);
        check(b->file==g_questScriptFileNames[1].file&&b->lifetime==g_questScriptFileNames[1].lifetime);check(QuestAllocator<2>(nullptr,&game)==nullptr);
        delete b;delete a;g_questScriptFileNames.resize(1);check(g_questScriptFileNames[0].lifetime==(first?NativeQuestLifetime::NewOakValeIntro:NativeQuestLifetime::None));++cases;
    }
    for(bool optIn:{false,true})for(int first:{-2147483647,-1,0,42})for(bool stale:{false,true}) {
        registrations=closed=0;nextId=first;cleanup.clear();auto& store=LuaManager::GetInstance().persisted;store.clear();
        if(stale){store["path/quest.lua:TalkIntermittentTimer"]=800;store["path/quest.lua:WatchTimer"]=900;}
        {
            GeneratedState state;state.m_nativeTimers.Configure(optIn?NativeQuestLifetime::NewOakValeIntro:NativeQuestLifetime::None,&current);
            check(registrations==(optIn?2:0));
            sol::state lua;lua.open_libraries(sol::lib::base);lua.new_usertype<GeneratedState>("QuestState",sol::no_constructor,"GetStateInt",&GeneratedState::GetStateInt,"SetStateInt",&GeneratedState::SetStateInt);lua["quest"]=&state;
            lua["ambient"]=optIn?first:(stale?800:0);lua["watch"]=optIn?first+1:(stale?900:0);
            check(lua.safe_script("function Init(q) assert(q:GetStateInt('TalkIntermittentTimer')==ambient);assert(q:GetStateInt('WatchTimer')==watch);q:SetStateInt('Other',17) end; Init(quest)",sol::script_pass_on_error).valid());
            check(state.GetStateInt("Other")==17);
            for(const char* field:{"TalkIntermittentTimer","WatchTimer"}) {
                lua["field"]=field;auto result=lua.safe_script("quest:SetStateInt(field,123)",sol::script_pass_on_error);check(result.valid()==!optIn);
                if(optIn){const auto k="path/quest.lua:"+std::string(field);if(stale)check(store.at(k)==(std::string(field)=="WatchTimer"?900:800));else check(store.count(k)==0);}
            }
            bool rejected=false;try{state.m_nativeTimers.Configure(NativeQuestLifetime::None,&current);}catch(...){rejected=true;}check(rejected);
        }
        check(closed==(optIn?2:0));if(optIn)check(cleanup==std::vector<int>{first+1,first});++cases;
    }
    for(int fault:{1,2}) {
        registrations=closed=0;failRegistration=fault;cleanup.clear();bool rejected=false;
        try{GeneratedState state;state.m_nativeTimers.Configure(NativeQuestLifetime::NewOakValeIntro,&current);}catch(const std::exception& e){check(std::string(e.what())=="REGISTER_FAULT");rejected=true;}
        check(rejected&&registrations==fault&&closed==fault-1);++cases;
    }
    std::cout<<"PASS: "<<cases<<" actual-FSE timer policy / real-Lua transient state cases\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
