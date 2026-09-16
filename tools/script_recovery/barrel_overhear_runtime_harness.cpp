#include "LuaRetailResources.h"
#include <cstring>
#include <iostream>
#include <vector>
DWORD g_fableBase=0;
static CGameScriptInterfaceBase game{};
static CScriptThing actor{},heroValue{};
static int randomValue,divisorValue;
static bool nullHero,distanceResult;
static std::vector<std::string> events;
static void check(bool value){if(!value)throw std::runtime_error("overhear check failed");}
static int __cdecl randomBoundary(){
    events.emplace_back("rand");*ASLR<int*>(0x13AC854)=divisorValue;return randomValue;
}
static CScriptThing* __fastcall hero(CGameScriptInterfaceBase* receiver,void*){
    check(receiver==&game);events.emplace_back("hero");return nullHero?nullptr:&heroValue;
}
static bool __fastcall distance(const CScriptThing* first,const CScriptThing* second,float limit){
    check(first==&actor && second==(nullHero?nullptr:&heroValue) && limit==15.0f);
    events.emplace_back("distance");return distanceResult;
}
tGetHero GetHero_API=reinterpret_cast<tGetHero>(&hero);
tIsDistanceBetweenThingsUnder IsDistanceBetweenThingsUnder_API=reinterpret_cast<tIsDistanceBetweenThingsUnder>(&distance);
struct Scope {
    CGameScriptInterfaceBase* m_game=&game;bool closed=false;
    void CheckOpen(){if(closed)throw std::runtime_error("CLOSED");}
#include "retail_barrel_overhear.inc"
};
int main(){try{
    static_assert(sizeof(void*)==4);
    auto* memory=VirtualAlloc(nullptr,0x1100000,MEM_RESERVE|MEM_COMMIT,PAGE_EXECUTE_READWRITE);
    check(memory!=nullptr);g_fableBase=reinterpret_cast<DWORD>(memory);
    auto* code=ASLR<unsigned char*>(0xBFEB16);code[0]=0xE9;
    DWORD delta=reinterpret_cast<DWORD>(&randomBoundary)-(reinterpret_cast<DWORD>(code)+5);
    std::memcpy(code+1,&delta,4);FlushInstructionCache(GetCurrentProcess(),code,5);
    Scope scope;sol::state lua;lua.open_libraries(sol::lib::base,sol::lib::string);
    lua.set_function("overhear",[&](bool heard){return scope.ShouldBarrelOverhear(&actor,heard);});int cases=0;
    for(bool heard:{false,true})for(int value:{-6,-1,0,6})for(int divisor:{-3,2})
    for(bool empty:{false,true})for(bool close:{false,true}){
        randomValue=value;divisorValue=divisor;nullHero=empty;distanceResult=close;events.clear();
        *ASLR<int*>(0x13AC854)=0;lua["heard"]=heard;
        bool passed=!heard || value%divisor==0;lua["expected"]=passed&&close;
        lua.script("assert(overhear(heard)==expected)");
        std::vector<std::string> expected;if(heard)expected.emplace_back("rand");
        if(passed){expected.emplace_back("hero");expected.emplace_back("distance");}
        check(events==expected);++cases;
    }
    for(int mode:{0,1}){
        randomValue=mode?(-2147483647-1):3;divisorValue=mode?-1:0;events.clear();
        lua.script("local ok,e=pcall(overhear,true);assert(not ok and string.find(e,'division fault',1,true))");
        check(events==std::vector<std::string>{"rand"});++cases;
    }
    scope.closed=true;events.clear();
    lua.script("local ok,e=pcall(overhear,true);assert(not ok and string.find(e,'CLOSED',1,true))");
    check(events.empty());++cases;
    std::cout<<"PASS: "<<cases<<" overhear policies; signed remainder, post-rand divisor, empty hero, short circuit and faults\n";
    VirtualFree(memory,0,MEM_RELEASE);return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
