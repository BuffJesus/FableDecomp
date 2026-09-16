#include "LuaRetailResources.h"
#include <cstring>
#include <iostream>
#include <vector>

DWORD g_fableBase=0;
static CGameScriptInterfaceBase game{},timerGame{};
static CScriptThing actor{},heroes[2]{};
static C3DVector positions[2]{};
static int heroIndex,positionIndex,watchId,watchValue=45;
static bool nullPosition,answer,failMove;
static void* expectedResource;
static std::vector<std::string> events;
static void check(bool value){if(!value)throw std::runtime_error("movement check failed");}
static CScriptThing* __fastcall getHero(CGameScriptInterfaceBase* receiver,void*) {
    check(receiver==&game && heroIndex<2);events.push_back("hero");return &heroes[heroIndex++];
}
static const C3DVector* __fastcall position(CScriptThing* receiver,void*) {
    check(receiver==&heroes[positionIndex] && positionIndex<2);events.push_back("position");
    return nullPosition?(++positionIndex,nullptr):&positions[positionIndex++];
}
static bool __fastcall distance(const CScriptThing* receiver,const C3DVector* value,float limit) {
    check(receiver==&actor && value==(nullPosition?nullptr:&positions[0]) && limit==4.0f);
    events.push_back("distance");return answer;
}
static void __fastcall move(void* receiver,void*,const C3DVector* value,float radius,int kind,bool a,bool b) {
    check(receiver==expectedResource && value==(nullPosition?nullptr:&positions[1]) && radius==2.0f && kind==1 && !a && b);
    events.push_back("move");if(failMove)throw std::runtime_error("MOVE");
}
static void __fastcall setTimer(CGameScriptInterfaceBase* receiver,void*,int id,int value) {
    check(receiver==&timerGame && id==watchId && value==watchValue);events.push_back("watch");
}
tGetHero GetHero_API=reinterpret_cast<tGetHero>(&getHero);
struct Scope {
    CGameScriptInterfaceBase* m_game=&game;
    bool closed=false;
    enum class Kind {Resource};
    struct Entry {CScriptGameResourceObjectScriptedThingBase resource{};} entry;
    void CheckOpen(){if(closed)throw std::runtime_error("CLOSED");}
    Entry& Get(unsigned id,Kind){CheckOpen();check(id==1);return entry;}
#include "retail_barrel_movement.inc"
};
static void trampoline(DWORD address,void* target) {
    auto* code=ASLR<unsigned char*>(address);code[0]=0xE9;
    auto delta=reinterpret_cast<DWORD>(target)-(reinterpret_cast<DWORD>(code)+5);
    std::memcpy(code+1,&delta,4);FlushInstructionCache(GetCurrentProcess(),code,5);
}
int main(){
    try {
        static_assert(sizeof(void*)==4);
        auto* memory=VirtualAlloc(nullptr,0x1100000,MEM_RESERVE|MEM_COMMIT,PAGE_EXECUTE_READWRITE);
        check(memory!=nullptr);g_fableBase=reinterpret_cast<DWORD>(memory);
        trampoline(0xCBE45C,reinterpret_cast<void*>(&distance));trampoline(0x7E72F0,reinterpret_cast<void*>(&move));
        *ASLR<CGameScriptInterfaceBase**>(0x143E8F8)=&timerGame;
        void* timerTable[0x168/4]{};timerTable[0x164/4]=reinterpret_cast<void*>(&setTimer);
        *reinterpret_cast<void***>(&timerGame)=timerTable;
        CScriptThingVTable table{};table.GetPos=reinterpret_cast<tCScriptThing_GetPos>(&position);
        for(auto& hero:heroes)hero.pVTable=reinterpret_cast<void**>(&table);
        Scope scope;expectedResource=&scope.entry.resource;
        sol::state lua;lua.open_libraries(sol::lib::base,sol::lib::string);
        lua.set_function("far",[&](){return scope.IsBarrelManFarFromHero(&actor);});
        lua.set_function("move",[&](){scope.MoveBarrelManToHero(1);});
        lua.set_function("watch",[&](int id){scope.SetBarrelWatchTimer(id);});
        lua.set_function("reset",[&](int id){scope.ResetBarrelWatchTimer(id);});
        int cases=0;
        for(bool empty:{false,true})for(bool nullValue:{false,true})for(bool result:{false,true}) {
            scope.entry.resource.pImp.Data=reinterpret_cast<decltype(scope.entry.resource.pImp.Data)>(empty?0:1);
            heroIndex=positionIndex=0;events.clear();nullPosition=nullValue;answer=result;lua["expected"]=result;
            lua.script("assert(far()==expected);move()");
            check(events==std::vector<std::string>{"hero","position","distance","hero","position","move"});++cases;
        }
        for(int id:{0,-1,73}) {watchId=id;events.clear();lua["id"]=id;lua.script("watch(id)");check(events==std::vector<std::string>{"watch"});++cases;}
        watchValue=0;
        for(int id:{0,-1,73}) {watchId=id;events.clear();lua["id"]=id;lua.script("reset(id)");check(events==std::vector<std::string>{"watch"});++cases;}
        heroIndex=positionIndex=0;events.clear();failMove=true;
        lua.script("far(); local ok,err=pcall(move);assert(not ok and string.find(err,'MOVE',1,true))");++cases;
        scope.closed=true;events.clear();
        lua.script("local ok,err=pcall(far);assert(not ok and string.find(err,'CLOSED',1,true))");check(events.empty());++cases;
        std::cout<<"PASS: "<<cases<<" movement/watch policies; exact borrowed pointers, fresh hero per call, resource receiver and native ABI\n";
        VirtualFree(memory,0,MEM_RELEASE);return 0;
    }catch(const std::exception& error){std::cerr<<error.what()<<'\n';return 1;}
}
