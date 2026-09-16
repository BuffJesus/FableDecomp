#define main resource_smoke_main
#include "runtime_checks/retail_resources_smoke.cpp"
#undef main
#define private public
#include "maze_entry_host.h"
#undef private
#include <fstream>
class LuaQuestState {};
DWORD g_fableBase=0x400000;
static bool terminated,flag,closeInFrame;
static unsigned entryQueries,bodyQueries,frames,consumers,logs;
static LuaQuestHost* activeHost;
std::string GetLogFilePath(){return "";}
static bool __fastcall entryTerm(void*,void*){++entryQueries;return terminated;}
tIsActiveThreadTerminating_Quest IsActiveThreadTerminating_Quest_API=reinterpret_cast<tIsActiveThreadTerminating_Quest>(&entryTerm);
tIsActiveThreadTerminating IsActiveThreadTerminating_API=reinterpret_cast<tIsActiveThreadTerminating>(&entryTerm);
static void __fastcall spawnConstruct(CSpawnedFunc*,void*,const CCharString* name,int zero){
    check(strings.at(const_cast<CCharString*>(name))=="UnLimboSword"&&zero==0);
}
static void __fastcall addSpawn(CScriptBase_Retail*,void*,CSpawnedFunc* f,const CCharString* region){
    check(strings.at(const_cast<CCharString*>(region)).empty());
    check(activeHost->m_threads.at(activeHost->m_threadCount-1).functionName=="UnLimboSword");
    std::free(f);
}
tCSpawnedFunc_Constructor CSpawnedFunc_Construct=reinterpret_cast<tCSpawnedFunc_Constructor>(&spawnConstruct);
tAddSpawnedFunction AddSpawnedFunction_func=reinterpret_cast<tAddSpawnedFunction>(&addSpawn);
constexpr int MAX_QUEST_THREADS=20;
using tThreadRunner=void(LuaQuestHost::*)();
tThreadRunner g_threadRunnerPool[MAX_QUEST_THREADS]={&LuaQuestHost::ThreadRunner<0>,&LuaQuestHost::ThreadRunner<1>};
#include "maze_entry_methods.inc"
LuaQuestHost::LuaQuestHost(CScriptDataBase*,CGameScriptInterfaceBase* game,const std::string& name)
 :pInterface(game),m_scriptName(name),m_pQuestState(std::make_unique<LuaQuestState>()),
 m_pLuaState(std::make_unique<sol::state>()),m_env(*m_pLuaState,sol::create,m_pLuaState->globals()){}
LuaQuestHost::~LuaQuestHost(){
    for(auto& item:m_threads)if(item.second.registryRef!=LUA_NOREF)luaL_unref(m_pLuaState->lua_state(),LUA_REGISTRYINDEX,item.second.registryRef);
}
static void configure(LuaQuestHost& host){
    activeHost=&host;auto& lua=*host.m_pLuaState;lua.open_libraries(sol::lib::base);
    auto type=lua.new_usertype<LuaQuestState>("Quest",sol::no_constructor);
    sol::table flags=lua.create_table();flags["Get"]=[] (sol::table,const std::string& name){check(name=="UNLIMBO");return flag;};
    type["RetailFlags"]=[flags](LuaQuestState&,const std::string& name){check(name=="MazeResearch");return flags;};
    type["IsActiveThreadTerminating"]=[](LuaQuestState&){++bodyQueries;return terminated;};
    type["NewScriptFrame"]=[](LuaQuestState&){
        ++frames;flag=true;
        if(closeInFrame){activeHost->BeginNativeEntryTeardown();check(!activeHost->NativeEntryCallbacksDrained());terminated=true;}
    };
    type["UnlimboMazeSword"]=[](LuaQuestState&){++consumers;};
    std::ifstream file("MazeResearch.UNLIMBO.lua");check(file.good());
    std::string helper((std::istreambuf_iterator<char>(file)),std::istreambuf_iterator<char>());
    lua.script(helper,host.m_env);
}
int main(){
    try{
        for(bool native:{false,true})for(bool initialFlag:{false,true})for(bool cancelled:{false,true}){
            entryQueries=bodyQueries=frames=consumers=0;flag=initialFlag;terminated=cancelled;closeInFrame=false;
            LuaQuestHost host(nullptr,reinterpret_cast<CGameScriptInterfaceBase*>(1),"MazeResearch");configure(host);
            if(native)host.CreateNativeEntryThread("UnLimboSword","",{});else host.CreateThread("UnLimboSword","",{});
            check(host.m_threads.at(0).nativeEntry==native);
            host.ThreadRunner<0>();
            check(entryQueries==(native?0u:cancelled?1u:2u));
            check(frames==((!native&&cancelled)||initialFlag?0u:1u));
            check(consumers==(cancelled?0u:1u));
            check(host.m_activeNativeEntries==0);
        }
        entryQueries=bodyQueries=frames=consumers=0;flag=false;terminated=false;closeInFrame=true;
        LuaQuestHost host(nullptr,reinterpret_cast<CGameScriptInterfaceBase*>(1),"MazeResearch");configure(host);
        host.CreateNativeEntryThread("UnLimboSword","",{});host.ThreadRunner<0>();
        check(frames==1&&consumers==0&&host.NativeEntryCallbacksDrained());
        host.ThreadRunner<0>();check(frames==1);
        auto count=host.m_threadCount;host.CreateNativeEntryThread("UnLimboSword","",{});check(host.m_threadCount==count);
        // Existing default creation/guard behavior remains independent of opt-in closing.
        host.CreateThread("UnLimboSword","",{});check(host.m_threadCount==count+1&&!host.m_threads.at(1).nativeEntry);
        host.ThreadRunner<1>();check(entryQueries==1);
        {
            LuaQuestHost errors(nullptr,reinterpret_cast<CGameScriptInterfaceBase*>(1),"MazeResearch");configure(errors);
            errors.m_pLuaState->script("function UnLimboSword(quest) error('BODY') end",errors.m_env);
            errors.CreateNativeEntryThread("UnLimboSword","",{});errors.ThreadRunner<0>();
            check(errors.m_activeNativeEntries==0);
            errors.BeginNativeEntryTeardown();check(errors.NativeEntryCallbacksDrained());
        }
        std::cout<<"PASS: actual host CreateThread/ThreadRunner default guards unchanged; native false flag yields before cancellation; true flag does not yield; per-thread policy set before scheduling; closing blocks new native entries and active counter unwinds\n";return 0;
    }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}
}
