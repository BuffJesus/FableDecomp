#define main TheresaReferenceMain
#include "theresa_scope_runtime_harness.cpp"
#undef main
#include "mission_native_cases.h"
#include <map>
#include <set>

static void __fastcall unexpectedMissionMap(void*,void*){throw std::runtime_error("unexpected map destructor");}
decltype(StdMap_String_Destroy_API) StdMap_String_Destroy_API=reinterpret_cast<decltype(StdMap_String_Destroy_API)>(&unexpectedMissionMap);

static void missionCheck(bool value,int line){if(!value)throw std::runtime_error("Mission assertion line "+std::to_string(line));}
#define mc(v) missionCheck((v),__LINE__)
static void* mt[1024]{},*mo[1024]{};
static CScriptThing mh{},ma{},*owned=nullptr;
static CCharString sa{};
static std::map<const CCharString*,std::string> live;
static int aliasMode=0;
static bool emptyHero=false,mutateTable=false,secondary=false;
static std::string trace,fault;
static void hit(const std::string& text){if(!trace.empty())trace+='|';trace+=text;if(text.substr(0,text.find(':'))==fault)throw std::runtime_error("MISSION_"+fault);}
static std::string b(bool value){return value?"1":"0";}
static void __fastcall keyNew(CCharString* out,void*,const char* value,int length){mc(length==-1&&!live.count(out));hit("key.new:"+std::string(value));live[out]=value;out->pStringData=reinterpret_cast<decltype(out->pStringData)>(1);}
static void __fastcall keyClose(CCharString* out,void*){mc(live.count(out));auto value=live.at(out);live.erase(out);out->pStringData=nullptr;hit("key.close:"+value);if(secondary)throw std::runtime_error("SECONDARY");}
static void __fastcall outputClose(CScriptThing* out,void*){mc(out==owned);owned=nullptr;hit("output.close");if(secondary)throw std::runtime_error("SECONDARY");}
static CScriptThing* __fastcall hero(CGameScriptInterfaceBase* self,void*){
    mc(self==&game);hit("hero:"+b(emptyHero));if(mutateTable){*reinterpret_cast<void***>(&game)=mo;mt[0x178/4]=mo[0x178/4];mt[0x814/4]=mo[0x814/4];}return emptyHero?nullptr:&mh;
}
template<bool changed> static CScriptThing* __fastcall turn(CGameScriptInterfaceBase* self,void*,CScriptThing* out,const CScriptThing* target,const CCharString* key){
    mc(self==&game&&target==(emptyHero?nullptr:&mh)&&live.at(key)=="CREATURE_HERO_CHILD"&&!owned);hit("turn:"+b(changed));owned=out;return aliasMode==2?nullptr:aliasMode==1?&ma:out;
}
template<bool changed> static CScriptThing* __fastcall lookup(CGameScriptInterfaceBase* self,void*,CScriptThing* out,const CCharString* key){
    mc(self==&game&&live.at(key)=="HerosOldHouse"&&!owned);hit("lookup:"+b(changed));owned=out;if(mutateTable)*reinterpret_cast<void***>(&game)=mo;return aliasMode==2?nullptr:aliasMode==1?&ma:out;
}
template<bool changed> static void __fastcall unlock(CGameScriptInterfaceBase* self,void*,const CScriptThing* house,bool value){mc(self==&game&&house==owned&&owned&&value&&live.empty());hit("unlock:"+b(changed));}
template<bool changed> static void __fastcall doors(CGameScriptInterfaceBase* self,void*,const CScriptThing* house){mc(self==&game&&house==owned&&owned&&live.empty());hit("doors:"+b(changed));}
template<bool changed> static CCharString* __fastcall active(CGameScriptInterfaceBase* self,void*,CCharString* out){
    mc(self==&game&&live.empty());hit("active:"+b(changed));live[out]="active";out->pStringData=reinterpret_cast<decltype(out->pStringData)>(1);
    if(mutateTable){*reinterpret_cast<void***>(&game)=mo;for(unsigned slot:{0x4d4,0x480,0x464})mt[slot/4]=mo[slot/4];}
    return aliasMode==2?nullptr:aliasMode==1?&sa:out;
}
static void checkActive(CGameScriptInterfaceBase* self,const CCharString* name){mc(self==&game&&live.size()==1&&live.begin()->second=="active");mc(name==(aliasMode==2?nullptr:aliasMode==1?&sa:live.begin()->first));}
template<bool changed> static void __fastcall screen(CGameScriptInterfaceBase* self,void*,const CCharString* name,bool a,bool z){checkActive(self,name);mc(!a&&z&&owned);hit("screen:"+std::to_string(aliasMode)+":"+b(changed));}
template<bool changed> static void __fastcall music(CGameScriptInterfaceBase* self,void*,EMusicSetType value,bool a,bool z){mc(self==&game&&int(value)==19&&!a&&z&&live.empty()&&owned);hit("music:"+b(changed));}
template<bool changed> static void __fastcall killable(CGameScriptInterfaceBase* self,void*,const CScriptThing* target,bool value,bool second){mc(self==&game&&target==(emptyHero?nullptr:&mh)&&!second);hit("killable:"+b(value)+":"+b(changed));}
template<bool changed> static void __fastcall complete(CGameScriptInterfaceBase* self,void*,const CCharString* name,bool a,bool z,bool c){checkActive(self,name);mc(!a&&!z&&!c);hit("complete:"+std::to_string(aliasMode)+":"+b(changed));}
template<bool changed> static void __fastcall deactivate(CGameScriptInterfaceBase* self,void*,const CCharString* name,unsigned delay){checkActive(self,name);mc(!delay);hit("deactivate:"+std::to_string(aliasMode)+":"+b(changed));}
static void reset(){
    mc(live.empty()&&!owned);trace.clear();fault.clear();secondary=false;*reinterpret_cast<void***>(&game)=mt;
#define SLOT(slot,method) mt[slot/4]=reinterpret_cast<void*>(&method<false>);mo[slot/4]=reinterpret_cast<void*>(&method<true>);
    SLOT(0x178,turn);SLOT(0x120,lookup);SLOT(0x6bc,unlock);SLOT(0x6ac,doors);SLOT(0xa3c,active);SLOT(0x4d4,screen);SLOT(0xae0,music);SLOT(0x814,killable);SLOT(0x480,complete);SLOT(0x464,deactivate);
#undef SLOT
    mt[0x118/4]=mo[0x118/4]=reinterpret_cast<void*>(&hero);
}
static const char* call(const std::string& kind){
    if(kind=="child")return "scope:TurnOakvaleHeroIntoChild()";
    if(kind=="house")return "scope:PrepareOakvaleHouseAndStartScreen()";
    if(kind=="complete")return "scope:FinishOakvaleActiveQuest(false)";
    if(kind=="deactivate")return "scope:FinishOakvaleActiveQuest(true)";
    return kind=="killable.on"?"scope:SetOakvaleHeroKillable(true)":"scope:SetOakvaleHeroKillable(false)";
}
int main(){try{
    CCharString_Construct_Literal=reinterpret_cast<decltype(CCharString_Construct_Literal)>(&keyNew);CCharString_Destroy=reinterpret_cast<decltype(CCharString_Destroy)>(&keyClose);RetailThing_Destroy_API=reinterpret_cast<decltype(RetailThing_Destroy_API)>(&outputClose);
    sol::state lua;lua.open_libraries(sol::lib::base);auto type=lua.new_usertype<LuaRetailResources>("Scope",sol::no_constructor);
    type["TurnOakvaleHeroIntoChild"]=&LuaRetailResources::TurnOakvaleHeroIntoChild;type["PrepareOakvaleHouseAndStartScreen"]=&LuaRetailResources::PrepareOakvaleHouseAndStartScreen;
    type["FinishOakvaleActiveQuest"]=&LuaRetailResources::FinishOakvaleActiveQuest;type["SetOakvaleHeroKillable"]=&LuaRetailResources::SetOakvaleHeroKillable;
    unsigned count=0,failures=0;
    for(const auto& row:missionCases){reset();aliasMode=row.alias;emptyHero=row.empty;mutateTable=row.mutate;
        {LuaRetailResources scope(&game);lua["scope"]=&scope;auto result=lua.safe_script(call(row.kind),sol::script_pass_on_error);if(!result.valid()){sol::error e=result;throw std::runtime_error(e.what());}
         mc(trace==row.expected&&live.empty()&&!owned);scope.Close();auto before=trace;mc(!lua.safe_script(call(row.kind),sol::script_pass_on_error).valid());mc(trace==before);}
        ++count;
    }
    // Inject at every observed boundary, including cleanup, through real Lua.
    for(const auto& row:missionCases){if(row.alias!=1||row.empty||!row.mutate)continue;
        std::string expected=row.expected;std::set<std::string> points;size_t start=0;
        do{auto end=expected.find('|',start);auto item=expected.substr(start,end-start);points.insert(item.substr(0,item.find(':')));if(end==std::string::npos)break;start=end+1;}while(true);
        for(const auto& point:points)for(bool second:{false,true}){reset();aliasMode=1;emptyHero=false;mutateTable=true;fault=point;secondary=second;
            {LuaRetailResources scope(&game);lua["scope"]=&scope;auto result=lua.safe_script(call(row.kind),sol::script_pass_on_error);mc(!result.valid());sol::error e=result;
             // With secondary cleanup failures enabled, an earlier successful cleanup can be the primary failure.
             mc(std::string(e.what()).find("MISSION_"+point)!=std::string::npos||second&&std::string(e.what()).find("SECONDARY")!=std::string::npos);mc(live.empty()&&!owned);}
            ++failures;
        }
    }
    lua["scope"]=sol::nil;std::cout<<"PASS: "<<count<<" native mission scope traces and "<<failures<<" exception policies\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
