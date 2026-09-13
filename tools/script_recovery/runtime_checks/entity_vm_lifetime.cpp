// Actual EntityScriptData and manager lifecycle methods; game bindings doubled.
#include "LuaManager.h"
#include "LuaQuestState.h"
#include <iostream>
#include <stdexcept>

std::string g_fseBasePath=".";
std::string GetLogFilePath(){return {};}
static unsigned abandoned=0;
void LuaQuestState::AbandonMovieSequence(lua_State* state){
    if(!state)throw std::runtime_error("missing live VM at abandonment");
    lua_gc(state,LUA_GCCOUNT,0);++abandoned;
}
static void (*testBindings)(sol::state&)=nullptr;
void LuaManager::RegisterBindingsInState(sol::state& lua,LuaQuestHost*,LuaEntityHost*){if(testBindings)testBindings(lua);}
#include "entity-manager-methods.inc"

int main(int argc,char** argv){
    try {
        if(argc!=2)throw std::runtime_error("expected lifecycle scenario");
        const std::string mode=argv[1];
        auto& manager=LuaManager::GetInstance();manager.Initialize();
        auto* host=reinterpret_cast<LuaEntityHost*>(0x1000);
        auto* quest=reinterpret_cast<LuaQuestState*>(0x2000);
        manager.RegisterEntityScriptData(host,"lifetime",quest);
        auto* data=manager.GetEntityScriptData(host);
        data->pLuaState->script("function Main() return 73 end",data->env);
        data->luaMain=data->env["Main"];
        if(data->luaMain().get<int>()!=73)throw std::runtime_error("bad callback");
        std::cout<<"ENTER "<<mode<<std::endl;
        if(mode=="unregister") {
            manager.UnregisterEntityScriptData(host);manager.UnregisterEntityScriptData(host);
            if(abandoned!=1)throw std::runtime_error("abandonment not idempotent");
        } else if(mode=="clear")manager.ClearAllEntityData();
        else if(mode=="shutdown")manager.Shutdown();
        else if(mode=="reinitialize")manager.Reinitialize();
        else if(mode=="replace") {
            manager.RegisterEntityScriptData(host,"replacement",quest);
            data=manager.GetEntityScriptData(host);
            if(data->scriptName!="replacement" || data->env["Main"].valid())throw std::runtime_error("replacement leaked environment");
            manager.UnregisterEntityScriptData(host);
        } else if(mode=="process_exit") {
            std::cout<<"PASS process_exit pending static manager destruction"<<std::endl;
            return 0;
        } else throw std::runtime_error("unknown scenario");
        if(manager.GetEntityScriptData(host))throw std::runtime_error("entity survived cleanup");
        std::cout<<"PASS "<<mode<<std::endl;return 0;
    }catch(const std::exception& e){std::cerr<<e.what()<<std::endl;return 1;}
}
