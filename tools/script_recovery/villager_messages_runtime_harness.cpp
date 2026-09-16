#include "LuaRetailResources.h"
#include <iostream>
#include <vector>
DWORD g_fableBase=0;
static CScriptThing actor{};
static CScriptThing heroValue{};
static CGameScriptInterfaceBase game{};
static bool nullHero;
static std::vector<CCharString*> live;
static std::vector<std::string> events;
static bool directHit,anyAbility,excludedAbility,talked,empty,failCleanup;
static int failQuery;
static void check(bool ok){if(!ok)throw std::runtime_error("Villager message check failed");}
static void __fastcall construct(CCharString* out,void*,const char* text,int length){
    check(std::string(text)=="SCRIPT_NAME_HERO" && length==-1);live.push_back(out);
    out->pStringData=reinterpret_cast<decltype(out->pStringData)>(empty?0:1);events.emplace_back("construct");
}
static void __fastcall destroy(CCharString* out,void*){
    check(!live.empty() && live.back()==out);live.pop_back();events.emplace_back("destroy");if(failCleanup)throw std::runtime_error("CLEANUP");
}
static bool query(CScriptThing* self,const CCharString* key,int index,bool value){
    check(self==&actor && !live.empty() && key==live.back());events.emplace_back("query"+std::to_string(index));
    if(failQuery==index)throw std::runtime_error("QUERY");return value;
}
static bool __fastcall direct(CScriptThing* self,void*,const CCharString* key){return query(self,key,1,directHit);}
static bool __fastcall special(CScriptThing* self,void*,const CCharString* key){return query(self,key,2,anyAbility);}
static bool __fastcall excluded(CScriptThing* self,void*,EHeroAbility ability,const CCharString* key){check(static_cast<int>(ability)==14);return query(self,key,3,excludedAbility);}
static bool __fastcall talk(CScriptThing* self,void*,const CCharString* key){return query(self,key,4,talked);}
static void __fastcall damage(CGameScriptInterfaceBase* self,void*,const CScriptThing* target,bool flag){check(self==&game && target==&actor && !flag);events.emplace_back("damage");}
static void __fastcall kill(CGameScriptInterfaceBase* self,void*,const CScriptThing* target,bool first,bool second){check(self==&game && target==&actor && !first && !second);events.emplace_back("kill");}
static void __fastcall combo(CGameScriptInterfaceBase* self,void*,const CScriptThing* target,bool flag){check(self==&game && target==&actor && !flag);events.emplace_back("combo");}
static CScriptThing* __fastcall getHero(CGameScriptInterfaceBase* self,void*){check(self==&game);events.emplace_back("hero");return nullHero?nullptr:&heroValue;}
static void __fastcall ally(CGameScriptInterfaceBase* self,void*,const CScriptThing* first,const CScriptThing* second){check(self==&game && first==&actor && second==(nullHero?nullptr:&heroValue));events.emplace_back("ally");}
tEntitySetAsDamageable EntitySetAsDamageable_API=reinterpret_cast<tEntitySetAsDamageable>(&damage);
tEntitySetAsKillable EntitySetAsKillable_API=reinterpret_cast<tEntitySetAsKillable>(&kill);
tEntitySetAsToAddToComboMultiplierWhenHit EntitySetAsToAddToComboMultiplierWhenHit_API=reinterpret_cast<tEntitySetAsToAddToComboMultiplierWhenHit>(&combo);
tGetHero GetHero_API=reinterpret_cast<tGetHero>(&getHero);
tEntitySetThingAsAllyOfThing EntitySetThingAsAllyOfThing_API=reinterpret_cast<tEntitySetThingAsAllyOfThing>(&ally);
tCCharString_Constructor_Literal CCharString_Construct_Literal=reinterpret_cast<tCCharString_Constructor_Literal>(&construct);
tCCharString_Destructor CCharString_Destroy=reinterpret_cast<tCCharString_Destructor>(&destroy);
struct Scope {
    CGameScriptInterfaceBase* m_game=&game;
    bool closed=false;
    void CheckOpen(){if(closed)throw std::runtime_error("CLOSED");}
    void SetBarrelManHeroAllies(CScriptThing*){}
#include "retail_villager_messages.inc"
};
int main(){try{
    CScriptThingVTable table{};auto** slots=reinterpret_cast<void**>(&table);
    slots[0x54/4]=reinterpret_cast<void*>(&direct);slots[0xA8/4]=reinterpret_cast<void*>(&special);
    slots[0xA4/4]=reinterpret_cast<void*>(&excluded);slots[0x6C/4]=reinterpret_cast<void*>(&talk);
    *reinterpret_cast<void***>(&actor)=reinterpret_cast<void**>(&table);
    sol::state lua;lua.open_libraries(sol::lib::base,sol::lib::string);Scope scope;
    auto type=lua.new_usertype<Scope>("Scope",sol::no_constructor);
    type["WasVillagerHit"]=&Scope::WasVillagerHit;type["WasVillagerTalkedTo"]=&Scope::WasVillagerTalkedTo;
    type["InitializeVillager"]=&Scope::InitializeVillager;
    lua["resources"]=&scope;lua["actor"]=&actor;int cases=0;
    for(bool d:{false,true})for(bool a:{false,true})for(bool e:{false,true})for(bool nil:{false,true}){
        directHit=d;anyAbility=a;excludedAbility=e;empty=nil;events.clear();lua["expected"]=d || (a && !e);
        lua.script("assert(resources:WasVillagerHit(actor)==expected)");check(live.empty());
        std::vector<std::string> expected{"construct","query1"};int count=1;
        if(!d){expected.emplace_back("construct");expected.emplace_back("query2");++count;if(a){expected.emplace_back("construct");expected.emplace_back("query3");++count;}}
        for(int i=0;i<count;++i)expected.emplace_back("destroy");check(events==expected);++cases;
    }
    for(bool value:{false,true})for(bool nil:{false,true}){
        talked=value;empty=nil;events.clear();lua["expected"]=value;lua.script("assert(resources:WasVillagerTalkedTo(actor)==expected)");
        check(live.empty() && events==std::vector<std::string>{"construct","query4","destroy"});++cases;
    }
    directHit=false;anyAbility=true;
    for(int failure:{1,2,3,4})for(bool cleanup:{false,true}){
        failQuery=failure;failCleanup=cleanup;lua["talk"]=failure==4;events.clear();
        lua.script("local ok,e=pcall(function() if talk then resources:WasVillagerTalkedTo(actor) else resources:WasVillagerHit(actor) end end);assert(not ok and string.find(e,'QUERY',1,true))");
        check(live.empty());++cases;
    }
    failQuery=0;failCleanup=true;lua.script("local ok,e=pcall(function() resources:WasVillagerHit(actor) end);assert(not ok and string.find(e,'CLEANUP',1,true))");check(live.empty());++cases;
    for(bool emptyHero:{false,true}){
        nullHero=emptyHero;events.clear();lua.script("resources:InitializeVillager(actor)");
        check(events==std::vector<std::string>{"damage","kill","combo","hero","ally"});++cases;
    }
    scope.closed=true;events.clear();lua.script("local ok=pcall(function() resources:WasVillagerHit(actor) end);assert(not ok)");check(events.empty());++cases;
    std::cout<<"PASS: "<<cases<<" message result/short-circuit/string-cleanup/error policies\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
