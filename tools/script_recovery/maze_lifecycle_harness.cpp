// Compile actual quest-owned storage and real Lua userdata lifetime behavior.
#define main resource_smoke_main
#include "runtime_checks/retail_resources_smoke.cpp"
#undef main
#include "LuaQuestState.h"
LuaQuestState::LuaQuestState(LuaQuestHost* parent,CGameScriptInterfaceBase* game)
    :m_pParentHost(parent),m_pGameInterface(game){}
int main(){
    try{
        unsigned oldDestroyed=0,newDestroyed=0;
        sol::state entity;entity.open_libraries(sol::lib::base);
        entity.new_usertype<CScriptThing>("Thing",sol::no_constructor);
        RegisterRetailFlags(entity);
        auto type=entity.new_usertype<LuaQuestState>("Quest",sol::no_constructor);
        type["GetRetainedRetailThing"]=&LuaQuestState::GetRetainedRetailThing;
        type["RetailFlags"]=&LuaQuestState::RetailFlags;
        auto quest=std::make_unique<LuaQuestState>(nullptr,reinterpret_cast<CGameScriptInterfaceBase*>(1));
        entity["quest"]=quest.get();
        quest->RetainRetailThing("MazeResearch",std::shared_ptr<CScriptThing>(new CScriptThing{},[&](CScriptThing* p){++oldDestroyed;delete p;}));
        entity.script("sword=quest:GetRetainedRetailThing('MazeResearch'); flags=quest:RetailFlags('MazeResearch'); flags:Set('UNLIMBO',false)");
        quest->RetainRetailThing("MazeResearch",std::shared_ptr<CScriptThing>(new CScriptThing{},[&](CScriptThing* p){++newDestroyed;delete p;}));
        // Unlike native parent assignment, the old wrapper is not destroyed.
        check(oldDestroyed==0);
        entity.script("sword=nil");
        // Neither nil nor scope exit promises collection. Force it only in this
        // diagnostic test, never in the candidate implementation.
        entity.collect_garbage();check(oldDestroyed==1&&newDestroyed==0);
        entity.script("sword=quest:GetRetainedRetailThing('MazeResearch'); quest=nil");
        quest.reset();
        check(newDestroyed==0&&boolDestroys==0);
        entity.script("assert(not flags:Get('UNLIMBO')); sword=nil; flags=nil");
        entity.collect_garbage();check(newDestroyed==1&&boolDestroys==1);
        std::cout<<"PASS: confirmed host ownership gaps: old Sword survives replacement; Lua Sword and flags survive parent teardown until final userdata collection\n";
        return 0;
    }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}
}
