#include "LuaRetailResources.h"
#include <iostream>
#include <vector>

static CGameScriptInterfaceBase game{};
static CScriptThing actor{},hero{},alternateTarget{};
static CCharString* liveKey;
static CScriptThing* liveMarker;
static bool emptyKey,emptyMarker,nullHero,alternateOutput;
static int index,failAt;
static std::vector<std::string> events;
static void check(bool value){if(!value)throw std::runtime_error("departure check failed");}
static void event(const char* name){events.emplace_back(name);if(static_cast<int>(events.size())==failAt)throw std::runtime_error("BODY");}
static void __fastcall construct(CCharString* key,void*,const char* text,int length){
    check(!liveKey && length==-1 && std::string(text)==(index==0?"M_WHouse_GuardPoint":"M_BarrelManHiddenPos"));
    liveKey=key;key->pStringData=reinterpret_cast<decltype(key->pStringData)>(emptyKey?0:1);
    events.emplace_back("string.new");
}
static void __fastcall destroyString(CCharString* key,void*){check(key==liveKey && !liveMarker);liveKey=nullptr;events.emplace_back("string.destroy");++index;}
static CScriptThing* __fastcall lookup(CGameScriptInterfaceBase* receiver,void*,CScriptThing* output,const CCharString* key){
    check(receiver==&game && key==liveKey && !liveMarker);liveMarker=output;
    output->pImp.Data=reinterpret_cast<decltype(output->pImp.Data)>(emptyMarker?0:1);
    event("lookup");return alternateOutput?&alternateTarget:output;
}
static CScriptThing* __fastcall getHero(CGameScriptInterfaceBase* receiver,void*){
    check(receiver==&game && index==0 && liveMarker && liveKey);event("hero");return nullHero?nullptr:&hero;
}
static void __fastcall teleport(CGameScriptInterfaceBase* receiver,void*,const CScriptThing* moving,const CScriptThing* target,bool flag){
    check(receiver==&game && liveKey && liveMarker && !flag);
    check(moving==(index==0?(nullHero?nullptr:&hero):&actor));
    check(target==(alternateOutput?&alternateTarget:liveMarker));event("teleport");
}
static void __fastcall destroyThing(CScriptThing* marker,void*){check(marker==liveMarker && liveKey);liveMarker=nullptr;events.emplace_back("thing.destroy");}
tCCharString_Constructor_Literal CCharString_Construct_Literal=reinterpret_cast<tCCharString_Constructor_Literal>(&construct);
tCCharString_Destructor CCharString_Destroy=reinterpret_cast<tCCharString_Destructor>(&destroyString);
tGetThingWithScriptName1 GetThingWithScriptName_ByName_API=reinterpret_cast<tGetThingWithScriptName1>(&lookup);
tGetHero GetHero_API=reinterpret_cast<tGetHero>(&getHero);
tEntityTeleportToThing EntityTeleportToThing_API=reinterpret_cast<tEntityTeleportToThing>(&teleport);
tRetailThingDestroy RetailThing_Destroy_API=reinterpret_cast<tRetailThingDestroy>(&destroyThing);
void** g_pCScriptThingVTable=reinterpret_cast<void**>(1);
struct Scope {
    CGameScriptInterfaceBase* m_game=&game;
    void CheckOpen(){}
#include "retail_barrel_departure.inc"
};
int main(){try{
    static_assert(sizeof(void*)==4);Scope scope;sol::state lua;lua.open_libraries(sol::lib::base,sol::lib::string);
    lua.set_function("depart",[&](){scope.TeleportBarrelDepartureActors(&actor);});int cases=0;
    for(bool key:{false,true})for(bool marker:{false,true})for(bool heroNull:{false,true})for(bool alias:{false,true}) {
        emptyKey=key;emptyMarker=marker;nullHero=heroNull;alternateOutput=alias;index=0;failAt=0;events.clear();
        lua.script("depart()");check(!liveKey && !liveMarker && index==2);
        check(events==std::vector<std::string>{"string.new","lookup","hero","teleport","thing.destroy","string.destroy",
            "string.new","lookup","teleport","thing.destroy","string.destroy"});++cases;
    }
    for(int failure:{2,3,4,8,9}) {
        index=0;events.clear();failAt=failure;
        lua.script("local ok,err=pcall(depart);assert(not ok and string.find(err,'BODY',1,true))");
        check(!liveKey && !liveMarker && events[events.size()-2]=="thing.destroy" && events.back()=="string.destroy");++cases;
    }
    std::cout<<"PASS: "<<cases<<" departure policies; marker/name nesting, returned target, raw hero, empty wrappers and error cleanup\n";return 0;
}catch(const std::exception& error){std::cerr<<error.what()<<'\n';return 1;}}
