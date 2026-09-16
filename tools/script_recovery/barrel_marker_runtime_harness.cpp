#include "LuaRetailResources.h"
#include <iostream>
#include <vector>

DWORD g_fableBase=0;
static CGameScriptInterfaceBase game{}, timerGames[2]{};
static CScriptThing actor{};
static CGameScriptThing implementation{};
static C3DVector positionValue{};
static const C3DVector* expectedPosition;
static const CScriptThing* expectedTarget;
static int timerIndex, timerId, timerValue;
static bool visible, nullPosition;
static std::vector<std::string> events;
static void check(bool value){if(!value)throw std::runtime_error("marker check failed");}
static int __fastcall getTimer(CGameScriptInterfaceBase* receiver,void*,int id){
    check(receiver==&timerGames[timerIndex] && id==timerId);events.push_back("timer");return timerValue;
}
static const C3DVector* __fastcall getPosition(CGameScriptThing* receiver,void*){
    check(receiver==&implementation);events.push_back("position");return nullPosition?nullptr:&positionValue;
}
static bool __fastcall camera(CGameScriptInterfaceBase* receiver,void*,const C3DVector* position){
    check(receiver==&game && position==expectedPosition);events.push_back("camera");return visible;
}
static void __fastcall teleport(CGameScriptInterfaceBase* receiver,void*,const CScriptThing* moving,const CScriptThing* target,bool flag){
    check(receiver==&game && moving==&actor && target==expectedTarget && !flag);events.push_back("teleport");
}
tIsCameraPosOnScreen IsCameraPosOnScreen_API=reinterpret_cast<tIsCameraPosOnScreen>(&camera);
tEntityTeleportToThing EntityTeleportToThing_API=reinterpret_cast<tEntityTeleportToThing>(&teleport);
struct Scope {
    CGameScriptInterfaceBase* m_game=&game;
    bool closed=false;
    enum class Kind {Thing};
    struct Entry {CScriptThing thing{};} entries[2];
    void CheckOpen(){if(closed)throw std::runtime_error("CLOSED");}
    Entry& Get(unsigned id,Kind){CheckOpen();check(id==1 || id==2);return entries[id-1];}
#include "retail_barrel_marker_actions.inc"
};
int main(){try{
    static_assert(sizeof(void*)==4);
    auto* memory=VirtualAlloc(nullptr,0x1100000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
    check(memory!=nullptr);g_fableBase=reinterpret_cast<DWORD>(memory);
    void* timerTable[0x16c/4]{};timerTable[0x168/4]=reinterpret_cast<void*>(&getTimer);
    for(auto& timer:timerGames)*reinterpret_cast<void***>(&timer)=timerTable;
    CGameScriptThingVTable table{};table.GetPos=reinterpret_cast<tCGameScriptThing_GetPos>(&getPosition);
    implementation.pVTable=reinterpret_cast<void**>(&table);
    Scope scope;
    sol::state lua;lua.open_libraries(sol::lib::base,sol::lib::string);
    lua.set_function("watch",[&](int id){return scope.GetBarrelWatchTimer(id);});
    lua.set_function("camera",[&](){return scope.IsOwnedThingPositionOnScreen(1);});
    lua.set_function("teleport",[&](unsigned id){scope.TeleportActorToOwnedThing(&actor,id);});
    int cases=0;
    for(int index:{0,1})for(int id:{-1,0,73})for(int value:{-1,14,15,16}){
        timerIndex=index;timerId=id;timerValue=value;events.clear();
        *ASLR<CGameScriptInterfaceBase**>(0x143E8F8)=&timerGames[index];
        lua["id"]=id;lua["expected"]=value;lua.script("assert(watch(id)==expected)");
        check(events==std::vector<std::string>{"timer"});++cases;
    }
    for(bool empty:{false,true})for(bool nullValue:{false,true})for(bool answer:{false,true}){
        scope.entries[0].thing.pImp.Data=reinterpret_cast<decltype(scope.entries[0].thing.pImp.Data)>(empty?nullptr:&implementation);
        nullPosition=nullValue;visible=answer;events.clear();
        expectedPosition=empty?ASLR<const C3DVector*>(0x143E8E0):(nullValue?nullptr:&positionValue);
        expectedTarget=&scope.entries[answer?1:0].thing;lua["expected"]=answer;
        lua.script("local visible=camera();assert(visible==expected);teleport(visible and 2 or 1)");
        check(events==(empty?std::vector<std::string>{"camera","teleport"}:std::vector<std::string>{"position","camera","teleport"}));++cases;
    }
    scope.closed=true;events.clear();
    lua.script("for _,f in ipairs({function()watch(0)end,camera,function()teleport(1)end}) do local ok,e=pcall(f);assert(not ok and string.find(e,'CLOSED',1,true)) end");
    check(events.empty());cases+=3;
    std::cout<<"PASS: "<<cases<<" marker/timer policies; exact position pointer, global timer receiver, selected owned wrapper and closed scope\n";
    VirtualFree(memory,0,MEM_RELEASE);return 0;
}catch(const std::exception& error){std::cerr<<error.what()<<'\n';return 1;}}
