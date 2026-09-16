#include "LuaRetailResources.h"
#include <cstring>
#include <iostream>
#include <vector>
DWORD g_fableBase=0;
static CGameScriptInterfaceBase game{},timerGame{},replacementTimers{};
static CScriptThing actor{},heroValue{};
static int randomValue,timerValue,timerId,setValue=3;
static bool nullHero,nearHero;
static std::vector<std::string> events;
static void check(bool ok){if(!ok)throw std::runtime_error("ambient check failed");}
static int __fastcall getTimer(CGameScriptInterfaceBase* self,void*,int id){check(self==&timerGame && id==timerId);events.emplace_back("get");return timerValue;}
static void __fastcall setTimer(CGameScriptInterfaceBase* self,void*,int id,int value){check(self==&replacementTimers && id==timerId && value==setValue);events.emplace_back("set");}
static int __cdecl randomBoundary(){events.emplace_back("rand");return randomValue;}
static CScriptThing* __fastcall hero(CGameScriptInterfaceBase* self,void*){check(self==&game);events.emplace_back("hero");return nullHero?nullptr:&heroValue;}
static bool __fastcall distance(const CScriptThing* first,const CScriptThing* second,float limit){check(first==&actor && second==(nullHero?nullptr:&heroValue) && limit==5.0f);events.emplace_back("distance");return nearHero;}
static int __fastcall create(CGameScriptInterfaceBase* self,void*,const CScriptThing* speaker,bool first,bool second){check(self==&game && speaker==&actor && !first && !second);events.emplace_back("conversation");return -7;}
static void __fastcall person(CGameScriptInterfaceBase* self,void*,int id,const CScriptThing* target){check(self==&game && id==-7 && target==(nullHero?nullptr:&heroValue));events.emplace_back("person");}
tGetHero GetHero_API=reinterpret_cast<tGetHero>(&hero);
tIsDistanceBetweenThingsUnder IsDistanceBetweenThingsUnder_API=reinterpret_cast<tIsDistanceBetweenThingsUnder>(&distance);
tAddNewConversation AddNewConversation_API=reinterpret_cast<tAddNewConversation>(&create);
tAddPersonToConversation AddPersonToConversation_API=reinterpret_cast<tAddPersonToConversation>(&person);
struct Scope {
    CGameScriptInterfaceBase* m_game=&game;bool closed=false;
    void CheckOpen(){if(closed)throw std::runtime_error("CLOSED");}
#include "retail_villager_ambient.inc"
};
int main(){try{
    auto* memory=VirtualAlloc(nullptr,0x1100000,MEM_RESERVE|MEM_COMMIT,PAGE_EXECUTE_READWRITE);check(memory!=nullptr);g_fableBase=reinterpret_cast<DWORD>(memory);
    auto* code=ASLR<unsigned char*>(0xBFEB16);code[0]=0xE9;DWORD delta=reinterpret_cast<DWORD>(&randomBoundary)-(reinterpret_cast<DWORD>(code)+5);std::memcpy(code+1,&delta,4);FlushInstructionCache(GetCurrentProcess(),code,5);
    void* table[100]{};table[0x168/4]=reinterpret_cast<void*>(&getTimer);table[0x164/4]=reinterpret_cast<void*>(&setTimer);
    *reinterpret_cast<void***>(&timerGame)=table;*reinterpret_cast<void***>(&replacementTimers)=table;
    Scope scope;sol::state lua;lua.open_libraries(sol::lib::base);
    auto type=lua.new_usertype<Scope>("Scope",sol::no_constructor);
    type["ShouldVillagerStartAmbientConversation"]=&Scope::ShouldVillagerStartAmbientConversation;
    type["StartVillagerAmbientConversation"]=&Scope::StartVillagerAmbientConversation;
    type["SetVillagerAmbientTimer"]=&Scope::SetVillagerAmbientTimer;
    lua["resources"]=&scope;lua["actor"]=&actor;int cases=0;
    for(int id:{-1,0,73})for(int timer:{-1,0,1})for(int random:{(-2147483647-1),-100,-1,0,100,2147483647})for(bool empty:{false,true})for(bool close:{false,true}){
        timerId=id;timerValue=timer;randomValue=random;nullHero=empty;nearHero=close;events.clear();lua["id"]=id;
        *ASLR<CGameScriptInterfaceBase**>(0x143E8F8)=&timerGame;
        const bool passed=timer==0 && random%100==0;lua["expected"]=passed&&close;
        lua.script("assert(resources:ShouldVillagerStartAmbientConversation(actor,id)==expected)");
        std::vector<std::string> expected{"get"};if(timer==0)expected.emplace_back("rand");if(passed){expected.emplace_back("hero");expected.emplace_back("distance");}check(events==expected);
        events.clear();*ASLR<CGameScriptInterfaceBase**>(0x143E8F8)=&replacementTimers;
        lua.script("assert(resources:StartVillagerAmbientConversation(actor,id)==-7)");
        check(events==std::vector<std::string>{"set","conversation","hero","person"});++cases;
    }
    setValue=0;events.clear();lua.script("resources:SetVillagerAmbientTimer(id,0)");check(events==std::vector<std::string>{"set"});++cases;
    scope.closed=true;events.clear();lua.script("local ok=pcall(function() resources:ShouldVillagerStartAmbientConversation(actor,id) end);assert(not ok)");check(events.empty());++cases;
    std::cout<<"PASS: "<<cases<<" ambient timer/random/proximity/setup policies\n";VirtualFree(memory,0,MEM_RELEASE);return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
