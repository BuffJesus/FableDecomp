#include "LuaRetailResources.h"
#include <iostream>
#include <vector>

static CGameScriptInterfaceBase game{};
static CScriptThing actor{},heroes[3]{};
static CScriptThing* borrowed[3];
static unsigned nextHero;
static bool visible,nearby;
static std::vector<std::string> events;
static void check(bool value){if(!value)throw std::runtime_error("return encounter check failed");}
static CScriptThing* __fastcall hero(CGameScriptInterfaceBase* receiver,void*){
    check(receiver==&game && nextHero<3);events.push_back("hero");return borrowed[nextHero++];
}
static void __fastcall face(CGameScriptInterfaceBase* receiver,void*,const CScriptThing* moving,const CScriptThing* target,bool snap){
    check(receiver==&game && moving==&actor && target==borrowed[0] && !snap);events.push_back("face");
}
static bool __fastcall seen(CGameScriptInterfaceBase* receiver,void*,const CScriptThing* target,const CScriptThing* observer){
    check(receiver==&game && target==borrowed[1] && observer==&actor);events.push_back("seen");return visible;
}
static bool __fastcall distance(const CScriptThing* first,const CScriptThing* second,float limit){
    check(first==&actor && second==borrowed[2] && limit==10.0f);events.push_back("distance");return nearby;
}
tGetHero GetHero_API=reinterpret_cast<tGetHero>(&hero);
tEntitySetFacingAngleTowardsThing EntitySetFacingAngleTowardsThing_API=reinterpret_cast<tEntitySetFacingAngleTowardsThing>(&face);
tCanThingBe_Seen_ByOtherThing CanThingBe_Seen_ByOtherThing_API=reinterpret_cast<tCanThingBe_Seen_ByOtherThing>(&seen);
tIsDistanceBetweenThingsUnder IsDistanceBetweenThingsUnder_API=reinterpret_cast<tIsDistanceBetweenThingsUnder>(&distance);
struct Scope{
    CGameScriptInterfaceBase* m_game=&game;
    bool closed=false;
    void CheckOpen(){if(closed)throw std::runtime_error("CLOSED");}
#include "retail_barrel_return_encounter.inc"
};
int main(){try{
    static_assert(sizeof(void*)==4);
    Scope scope;sol::state lua;lua.open_libraries(sol::lib::base,sol::lib::string);
    lua.set_function("face",[&](){scope.FaceBarrelManTowardsHero(&actor);});
    lua.set_function("thank",[&](){return scope.ShouldBarrelManThankHero(&actor);});
    int cases=0;
    for(unsigned mask=0;mask<8;++mask)for(bool see:{false,true})for(bool within:{false,true}){
        for(unsigned i=0;i<3;++i)borrowed[i]=(mask&(1u<<i))?nullptr:&heroes[i];
        nextHero=0;events.clear();visible=see;nearby=within;lua["expected"]=see||within;
        lua.script("face();assert(thank()==expected)");
        check(events==(see?std::vector<std::string>{"hero","face","hero","seen"}:
            std::vector<std::string>{"hero","face","hero","seen","hero","distance"}));
        check(nextHero==(see?2u:3u));++cases;
    }
    // A successful visibility branch must not require or invoke the distance API.
    IsDistanceBetweenThingsUnder_API=nullptr;visible=true;nextHero=0;events.clear();
    lua.script("face();assert(thank())");check(nextHero==2);++cases;
    scope.closed=true;events.clear();
    lua.script("for _,f in ipairs({face,thank}) do local ok,e=pcall(f);assert(not ok and string.find(e,'CLOSED',1,true)) end");
    check(events.empty());cases+=2;
    std::cout<<"PASS: "<<cases<<" return encounter policies; borrowed hero identities, native argument direction, short circuit and closed scope\n";
    return 0;
}catch(const std::exception& error){std::cerr<<error.what()<<'\n';return 1;}}
