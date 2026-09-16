#include "FableAPI.h"
#include "GameInterface.h"
#include <sol/sol.hpp>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <vector>

static float threshold;
DWORD g_fableBase = reinterpret_cast<DWORD>(&threshold) - (0x013AC858 - 0x400000);
static CScriptThing actor{}, hero{}, otherHero{};
static CGameScriptInterfaceBase game{};
static CScriptThing* heroResult;
static unsigned expectedBits;
static bool answer, failHero, failDistance;
static std::vector<std::string> events;
static bool allyMode;
static int heroIndex, allyIndex, throwAllyAt=-1;
static CScriptThing* allyHeroes[2];
static void check(bool value) { if (!value) throw std::runtime_error("approach check failed"); }
static CScriptThing* __fastcall getHero(CGameScriptInterfaceBase* value, void*) {
    check(value==&game); events.push_back("hero");
    if(allyMode) {check(heroIndex<2);return allyHeroes[heroIndex++];}
    threshold=99.0f;
    if(failHero) throw std::runtime_error("HERO");
    return heroResult;
}
static bool __fastcall distance(const CScriptThing* first, const CScriptThing* second, float value) {
    unsigned bits; std::memcpy(&bits,&value,4);
    check(first==&actor && second==heroResult && bits==expectedBits);
    events.push_back("distance");
    if(failDistance) throw std::runtime_error("DISTANCE");
    return answer;
}
tGetHero GetHero_API = reinterpret_cast<tGetHero>(&getHero);
tIsDistanceBetweenThingsUnder IsDistanceBetweenThingsUnder_API = &distance;
static void __fastcall setAlly(CGameScriptInterfaceBase* value, void*, const CScriptThing* first, const CScriptThing* second) {
    check(value==&game && allyIndex<2);
    check(first==(allyIndex==0?&actor:allyHeroes[1]) && second==(allyIndex==0?allyHeroes[0]:&actor));
    events.push_back("ally");
    if(allyIndex++==throwAllyAt)throw std::runtime_error("ALLY");
}
tEntitySetThingAsAllyOfThing EntitySetThingAsAllyOfThing_API = reinterpret_cast<tEntitySetThingAsAllyOfThing>(&setAlly);

struct Scope {
    CGameScriptInterfaceBase* m_game=&game;
    bool closed=false;
    void CheckOpen() { if(closed)throw std::runtime_error("CLOSED"); }
#include "retail_barrel_approach.inc"
};

int main() {
    try {
        static_assert(sizeof(void*)==4);
        Scope scope; sol::state lua; lua.open_libraries(sol::lib::base,sol::lib::string);
        lua.set_function("approach",[&](){return scope.IsHeroWithinBarrelApproachDistance(&actor);});
        int cases=0;
        for(unsigned bits:{0x00000000u,0x80000000u,0x40D12345u,0x7FC12345u})
            for(bool populated:{false,true}) for(bool result:{false,true}) {
                expectedBits=bits;std::memcpy(&threshold,&bits,4);heroResult=populated?&hero:nullptr;
                events.clear();answer=result;lua["expected"]=result;
                lua.script("assert(approach()==expected)");
                check(events==std::vector<std::string>{"hero","distance"});++cases;
            }
        for(bool heroFailure:{false,true}) {
            expectedBits=0x3F800000;std::memcpy(&threshold,&expectedBits,4);
            events.clear();failHero=heroFailure;failDistance=!heroFailure;
            lua["expectedError"]=heroFailure?"HERO":"DISTANCE";
            lua.script("local ok,err=pcall(approach);assert(not ok and string.find(err,expectedError,1,true))");
            check(events==(heroFailure?std::vector<std::string>{"hero"}:std::vector<std::string>{"hero","distance"}));++cases;
        }
        failHero=failDistance=false;events.clear();scope.closed=true;
        lua.script("local ok,err=pcall(approach);assert(not ok and string.find(err,'CLOSED',1,true))");
        check(events.empty());++cases;
        scope.closed=false;allyMode=true;
        lua.set_function("allies",[&](){scope.SetBarrelManHeroAllies(&actor);});
        for(CScriptThing* first:{static_cast<CScriptThing*>(nullptr),&hero,&otherHero})
            for(CScriptThing* second:{static_cast<CScriptThing*>(nullptr),&hero,&otherHero}) {
                allyHeroes[0]=first;allyHeroes[1]=second;heroIndex=allyIndex=0;events.clear();
                lua.script("allies()");
                check(heroIndex==2 && allyIndex==2 && events==std::vector<std::string>{"hero","ally","hero","ally"});++cases;
            }
        for(int failure:{0,1}) {
            heroIndex=allyIndex=0;events.clear();throwAllyAt=failure;
            lua.script("local ok,err=pcall(allies);assert(not ok and string.find(err,'ALLY',1,true))");
            check(heroIndex==failure+1 && allyIndex==failure+1);++cases;
        }
        std::cout<<"PASS: "<<cases<<" approach policies; live threshold before fresh borrowed hero, null forwarding, ABI and Lua result/errors\n";
        return 0;
    } catch(const std::exception& error) {std::cerr<<error.what()<<'\n';return 1;}
}
