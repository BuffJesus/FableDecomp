// Actual manager storage, namespace, bool transfer and sol registrations.
// Quest construction and engine persistence are doubled; VMs are actual Lua.
#define main resource_smoke_main
#include "retail_resources_smoke.cpp"
#undef main
#define main entity_vm_lifetime_main
#include "entity_vm_lifetime.cpp"
#undef main
#include "LuaQuestHost.h"

LuaQuestHost::LuaQuestHost(CScriptDataBase*,CGameScriptInterfaceBase*,const std::string& name)
    :m_scriptName(name),m_pQuestState(nullptr),m_pLuaState(nullptr){}
LuaQuestHost::~LuaQuestHost(){}
LuaQuestState::LuaQuestState(LuaQuestHost* parent,CGameScriptInterfaceBase* game)
    :m_pParentHost(parent),m_pGameInterface(game){}
void LuaQuestState::Log(const std::string&){}
#include "quest-state-methods.inc"
static bool reading=false;
static std::map<std::string,bool> saved;
static void* context=reinterpret_cast<void*>(0x7788);
static unsigned transfers=0;
static void __fastcall transferBool(CPersistContext* ctx,void*,const char* name,bool* value,const bool* fallback){
    check(ctx==context && std::string(name)=="PieceOver" && !*fallback);
    if(reading){auto p=saved.find(name);*value=p==saved.end()?*fallback:p->second;}
    else saved[name]=*value;
    ++transfers;
}
tCPersistContext_Transfer_bool CPersistContext_Transfer_bool_API=reinterpret_cast<tCPersistContext_Transfer_bool>(&transferBool);

static void bindState(sol::state& lua){
    auto questState_type=lua.new_usertype<LuaQuestState>("Quest",sol::no_constructor);
#include "quest-state-registration.inc"
}
static void loadRoot(sol::state& lua,LuaQuestState* quest,const std::string& package){
    lua.open_libraries(sol::lib::base,sol::lib::package,sol::lib::table);
    bindState(lua);lua["quest"]=quest;lua["context"]=context;
    lua["package"]["path"]=package+"/?.lua;";
    lua.script_file(package+"/GuardianTrophyDealerInfo/GuardianTrophyDealerInfo.lua");
    lua.script("A=require('GuardianTrophyDealerInfo.runtime_adapter');h=A.new(quest)");
}

int main(int argc,char** argv){
    try {
        check(argc==2);g_fseBasePath=argv[1];testBindings=&bindState;
        auto& manager=LuaManager::GetInstance();manager.Reinitialize();
        LuaQuestHost parent(nullptr,nullptr,"GuardianTrophyDealerInfo/GuardianTrophyDealerInfo");
        LuaQuestHost otherParent(nullptr,nullptr,"MazeResearch/MazeResearch");
        LuaQuestState quest(&parent,nullptr),other(&otherParent,nullptr);
        sol::state root;loadRoot(root,&quest,g_fseBasePath);root.script("Init(quest);assert(not h:piece_over())");
        auto* host=reinterpret_cast<LuaEntityHost*>(0x1000);
        manager.RegisterEntityScriptData(host,"GTDI_Maze",&quest);
        auto* data=manager.GetEntityScriptData(host);
        check(data->pQuestState==&quest && data->pLuaState->lua_state()!=root.lua_state());
        (*data->pLuaState)["quest"]=data->pQuestState;
        data->pLuaState->script("A=require('GuardianTrophyDealerInfo.runtime_adapter');h=A.new(quest);assert(not h:piece_over());h:set_piece_over(true)");
        lua_State* thread=lua_newthread(root.lua_state());int anchor=luaL_ref(root.lua_state(),LUA_REGISTRYINDEX);
        {sol::state_view watcher(thread);watcher.script("assert(h:piece_over())");}
        check(quest.GetStateBool("PieceOver") && !other.GetStateBool("PieceOver"));
        other.SetStateBool("PieceOver",true);quest.SetStateBool("PieceOver",false);
        check(other.GetStateBool("PieceOver") && !quest.GetStateBool("PieceOver"));
        std::cout<<"PASS isolated entity VM and watcher coroutine share only their quest namespace\n";

        data->pLuaState->script("h:set_piece_over(true)");root.script("OnPersist(quest,context)");
        check(saved.at("PieceOver"));manager.UnregisterEntityScriptData(host);
        check(quest.GetStateBool("PieceOver"));
        // New wrappers for the same script see its named state until Init resets it.
        LuaQuestHost reloadedParent(nullptr,nullptr,"GuardianTrophyDealerInfo/GuardianTrophyDealerInfo");
        LuaQuestState reloaded(&reloadedParent,nullptr);
        check(reloaded.GetStateBool("PieceOver"));
        sol::state reload;loadRoot(reload,&reloaded,g_fseBasePath);
        reload.script("Init(quest);assert(not h:piece_over())");
        reading=true;reload.script("OnPersist(quest,context);assert(h:piece_over())");
        check(quest.GetStateBool("PieceOver"));
        std::cout<<"PASS same-name reconstruction resets in Init and restores saved PieceOver\n";

        manager.Reinitialize();check(!quest.GetStateBool("PieceOver") && !other.GetStateBool("PieceOver"));
        reload.script("Init(quest);OnPersist(quest,context);assert(h:piece_over())");
        saved.clear();reload.script("OnPersist(quest,context);assert(not h:piece_over())");
        check(transfers==4);
        manager.SetGlobalStateInt("GuardianTrophyDealerInfo/GuardianTrophyDealerInfo:PieceOver",1);
        check(!reloaded.GetStateBool("PieceOver"));reloaded.SetStateBool("PieceOver",true);
        check(reloaded.GetStateBool("PieceOver"));
        std::cout<<"PASS manager reset, absent saved field and strict bool storage\n";
        luaL_unref(root.lua_state(),LUA_REGISTRYINDEX,anchor);manager.Shutdown();testBindings=nullptr;
        return 0;
    }catch(const std::exception& e){std::cerr<<e.what()<<std::endl;return 1;}
}
