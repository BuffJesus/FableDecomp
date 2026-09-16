#include "FableAPI.h"
#include "GameInterface.h"
#include <sol/sol.hpp>
#include <iostream>
#include <vector>
#include <stdexcept>

// Minimal host shell; the five implementation bodies are extracted unchanged.
class LuaQuestState {
public:
    CGameScriptInterfaceBase* m_pGameInterface;
    void Log(const char*) {}
    void SetCreatureBrain(CScriptThing*,const std::string&);
    void SetWanderCentrePoint(CScriptThing*,sol::table);
    void SetWanderMinDistance(CScriptThing*,float);
    void SetWanderMaxDistance(CScriptThing*,float);
    void SetScriptingStateGroup(CScriptThing*,int);
};
#include "barrel_man_setup_runtime_bodies.inc"
static void check(bool value) {if(!value)throw std::runtime_error("barrel setup check failed");}
static CGameScriptInterfaceBase game{};
static CScriptThing actor{};
static CCPPointerInfo info{};
static C3DVector home{};
static CCharString* live=nullptr;
static std::vector<std::string> events;
static void __fastcall construct(CCharString* value,void*,const char* text,int length) {
    check(!live && std::string(text)=="BRAIN_PASSIVE_OVERRIDE" && length==-1);
    live=value;value->pStringData=reinterpret_cast<void*>(1);events.push_back("construct");
}
static void __fastcall destroy(CCharString* value,void*) {
    check(value==live);live=nullptr;events.push_back("destroy");
}
static void __fastcall brain(CGameScriptInterfaceBase* actual,void*,const CScriptThing* target,const CCharString* name) {
    check(actual==&game && target==&actor && name==live);events.push_back("brain");
}
static void consume(CGameScriptInterfaceBase* actual,const CScriptThing& copy,const char* name) {
    check(actual==&game && &copy!=&actor && !live);
    check(copy.pVTable==actor.pVTable && copy.pImp.Data==actor.pImp.Data && copy.pImp.Info==actor.pImp.Info);
    if(copy.pImp.Info) {check(copy.pImp.Info->RefCount==8);--copy.pImp.Info->RefCount;}
    events.push_back(name);
}
static void __fastcall center(CGameScriptInterfaceBase* actual,void*,CScriptThing copy,C3DVector position) {
    check(position.x==home.x && position.y==home.y && position.z==home.z);consume(actual,copy,"center");
}
static void __fastcall minimum(CGameScriptInterfaceBase* actual,void*,CScriptThing copy,float distance) {
    check(distance==0.0f);consume(actual,copy,"minimum");
}
static void __fastcall maximum(CGameScriptInterfaceBase* actual,void*,CScriptThing copy,float distance) {
    check(distance==1.0f);consume(actual,copy,"maximum");
}
static void __fastcall group(CGameScriptInterfaceBase* actual,void*,CScriptThing copy,EScriptingStateGroups value) {
    check(static_cast<int>(value)==4);consume(actual,copy,"group");
}
tCCharString_Constructor_Literal CCharString_Construct_Literal=reinterpret_cast<tCCharString_Constructor_Literal>(&construct);
tCCharString_Destructor CCharString_Destroy=reinterpret_cast<tCCharString_Destructor>(&destroy);
tSetCreatureBrain SetCreatureBrain_API=reinterpret_cast<tSetCreatureBrain>(&brain);
tSetWanderCentrePoint SetWanderCentrePoint_API=reinterpret_cast<tSetWanderCentrePoint>(&center);
tSetWanderMinDistance SetWanderMinDistance_API=reinterpret_cast<tSetWanderMinDistance>(&minimum);
tSetWanderMaxDistance SetWanderMaxDistance_API=reinterpret_cast<tSetWanderMaxDistance>(&maximum);
tSetScriptingStateGroup SetScriptingStateGroup_API=reinterpret_cast<tSetScriptingStateGroup>(&group);
int main() {
    try {
        static_assert(sizeof(void*)==4);static_assert(sizeof(CScriptThing)==12);
        unsigned cases=0;
        for(bool retained:{false,true})for(bool empty:{false,true})for(float x:{1.0f,-10.5f,0.0f}) {
            actor.pVTable=reinterpret_cast<void**>(0x1238C8C);
            actor.pImp.Data=empty?nullptr:reinterpret_cast<decltype(actor.pImp.Data)>(0x123456);
            actor.pImp.Info=retained?&info:nullptr;info.RefCount=7;home={x,2.0f,45.25f};events.clear();
            sol::state lua;lua.open_libraries(sol::lib::base);
            auto type=lua.new_usertype<LuaQuestState>("SetupQuest",sol::no_constructor);
            type["SetCreatureBrain"]=&LuaQuestState::SetCreatureBrain;
            type["SetWanderCentrePoint"]=&LuaQuestState::SetWanderCentrePoint;
            type["SetWanderMinDistance"]=&LuaQuestState::SetWanderMinDistance;
            type["SetWanderMaxDistance"]=&LuaQuestState::SetWanderMaxDistance;
            type["SetScriptingStateGroup"]=&LuaQuestState::SetScriptingStateGroup;
            auto thing=lua.new_usertype<CScriptThing>("SetupThing",sol::no_constructor);
            thing["GetHomePos"]=[&](CScriptThing& value) {
                check(&value==&actor && !live);events.push_back("home");
                return lua.create_table_with("x",home.x,"y",home.y,"z",home.z);
            };
            LuaQuestState quest{&game};lua["quest"]=&quest;lua["me"]=&actor;
            lua.script_file("barrel_man_setup.lua");
            check(!live && info.RefCount==7);
            check(events==std::vector<std::string>{"construct","brain","destroy","home","center","minimum","maximum","group"});
            ++cases;
        }
        std::cout<<"PASS: "<<cases<<" real-Lua setup cases, unchanged wrapper bodies, brain string scope and four consumed actor copies\n";
        return 0;
    } catch(const std::exception& error) {std::cerr<<error.what()<<'\n';return 1;}
}
