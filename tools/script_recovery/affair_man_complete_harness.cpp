#define main resource_smoke_main
#include "runtime_checks/retail_resources_smoke.cpp"
#undef main
#include <fstream>
#include <iterator>
static CScriptThing actor{};
static std::vector<std::string> trace;
static std::string fault,source;
static void event(const std::string& text){trace.push_back(text);if(text==fault)throw std::runtime_error("AFFAIR_MAN_FAULT");}
static void __fastcall damage(CGameScriptInterfaceBase*,void*,const CScriptThing* a,bool b){check(a==&actor&&!b);event("damage");}
static void __fastcall kill(CGameScriptInterfaceBase*,void*,const CScriptThing* a,bool b,bool c){check(a==&actor&&!b&&!c);event("kill");}
static void __fastcall combo(CGameScriptInterfaceBase*,void*,const CScriptThing* a,bool b){check(a==&actor&&!b);event("combo");}
static void __fastcall information(CGameScriptInterfaceBase*,void*,const CScriptThing* a,bool b,bool c,bool d){check(a==&actor&&!b&&!c&&!d);event("information");}
static void __fastcall movement(CGameScriptInterfaceBase*,void*,const CScriptThing* a,bool b){check(a==&actor&&!b);event("movement");}
static void __fastcall deeds(CGameScriptInterfaceBase*,void*,const CScriptThing* a,bool b){check(a==&actor&&!b);event("deeds");}
static void __fastcall pushable(CGameScriptInterfaceBase*,void*,CScriptThing copy,bool b){check(!b&&copy.pVTable==g_pCScriptThingVTable&&copy.pImp.Data==actor.pImp.Data&&copy.pImp.Info==actor.pImp.Info);if(copy.pImp.Info){check(copy.pImp.Info->RefCount==8);--copy.pImp.Info->RefCount;}event("pushable");}
tEntitySetAsDamageable EntitySetAsDamageable_API=reinterpret_cast<tEntitySetAsDamageable>(&damage);
tEntitySetAsKillable EntitySetAsKillable_API=reinterpret_cast<tEntitySetAsKillable>(&kill);
tEntitySetAsToAddToComboMultiplierWhenHit EntitySetAsToAddToComboMultiplierWhenHit_API=reinterpret_cast<tEntitySetAsToAddToComboMultiplierWhenHit>(&combo);
tSetThingHasInformation SetThingHasInformation_API=reinterpret_cast<tSetThingHasInformation>(&information);
tEntitySetAsUseMovementInActions EntitySetAsUseMovementInActions_API=reinterpret_cast<tEntitySetAsUseMovementInActions>(&movement);
tSetIsPushableByHero SetIsPushableByHero_API=reinterpret_cast<tSetIsPushableByHero>(&pushable);
DWORD g_fableBase=0x400000;
static CScriptGameResourceObjectScriptedThingBase expert{};
static unsigned char expectedRaw=0;
static bool expectedSecond=false;
static int animationCalls=0;
static std::string expectedKey;
static void __fastcall keyCtor(CCharString* key,void*,const char* text,int n){stringCtor(key,nullptr,text,n);check(text==expectedKey);trace.push_back("key.new");*ASLR<volatile unsigned char*>(0x1375748)=expectedRaw;}
static void __fastcall keyDtor(CCharString* key,void*){stringDtor(key,nullptr);trace.push_back("key.destroy");}
static void __fastcall animation(void* self,void*,const CCharString* key,bool a,bool b,bool c,bool d,unsigned char raw,bool f,bool g){check(self==&expert&&strings.at(const_cast<CCharString*>(key))==expectedKey&&!a&&b==expectedSecond&&!c&&d&&raw==expectedRaw&&!f&&!g);++animationCalls;event("animation");}
int main(){try{
    std::ifstream input("../CANDIDATE.lua");check(input.good());source=std::string(std::istreambuf_iterator<char>(input),std::istreambuf_iterator<char>());
    std::remove_pointer_t<decltype(actor.pImp.Info)> info{};info.RefCount=7;unsigned cases=0;
    for(bool populated:{false,true})for(bool counted:{false,true})for(const char* error:{"","damage","kill","combo","pushable","movement","information"}){
        actor.pImp.Data=populated?reinterpret_cast<decltype(actor.pImp.Data)>(1):nullptr;actor.pImp.Info=counted?&info:nullptr;fault=error;trace.clear();
        sol::state lua;lua.open_libraries(sol::lib::base);RegisterRetailResources(lua);lua["actor"]=&actor;
        lua["scope"]=[&](sol::protected_function body){WithRetailResources(reinterpret_cast<CGameScriptInterfaceBase*>(1),body);};
        check(lua.safe_script(source,sol::script_pass_on_error).valid());
        auto result=lua.safe_script("Init({WithRetailResources=function(_,body) scope(body) end},actor)",sol::script_pass_on_error);
        check(result.valid()==fault.empty());check(info.RefCount==7&&locals.empty());
        std::vector<std::string> expected={"damage","kill","combo","pushable","movement","information"};
        if(!fault.empty()){auto last=std::find(expected.begin(),expected.end(),fault);check(last!=expected.end());expected.erase(last+1,expected.end());}check(trace==expected);++cases;
    }
    void* arena=VirtualAlloc(nullptr,0x10000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);check(arena!=nullptr);g_fableBase=reinterpret_cast<DWORD>(arena)-(0x1370000-0x400000);
    CCharString_Construct_Literal=reinterpret_cast<decltype(CCharString_Construct_Literal)>(&keyCtor);CCharString_Destroy=reinterpret_cast<decltype(CCharString_Destroy)>(&keyDtor);
    CScriptGameResourceObjectScriptedThingBaseVTable table{};static_assert(offsetof(CScriptGameResourceObjectScriptedThingBaseVTable,PlayAnimation)==0x48);
    table.PlayAnimation=reinterpret_cast<decltype(table.PlayAnimation)>(&animation);expert.pVTable=reinterpret_cast<void**>(&table);acquireExpert=&expert;
    for(const char* key:{"ST_OPINION_FEAR_IDLE_COWERING","GIVE_KISS","GIVE_HUG"})for(unsigned raw:{0u,1u,2u,255u})for(bool populated:{false,true})for(bool error:{false,true}){
        expectedKey=key;expectedSecond=expectedKey!="ST_OPINION_FEAR_IDLE_COWERING";expectedRaw=static_cast<unsigned char>(raw);*ASLR<volatile unsigned char*>(0x1375748)=0x55;acquireOK=populated;fault=error?"animation":"";trace.clear();const int before=animationCalls;
        sol::state lua;lua.open_libraries(sol::lib::base);RegisterRetailResources(lua);lua["actor"]=&actor;lua["key"]=expectedKey;lua["second"]=expectedSecond;
        lua["scope"]=[&](sol::protected_function body){WithRetailResources(reinterpret_cast<CGameScriptInterfaceBase*>(1),body);};
        auto result=lua.safe_script("scope(function(r) local c=r:NewResource();r:TryAcquire(c,actor,4);r:PlayAffairManAnimation(c,key,second) end)",sol::script_pass_on_error);
        check(result.valid()==(!error||!populated));check(strings.empty()&&locals.empty()&&animationCalls==before+static_cast<int>(populated));
        check(trace==(populated?std::vector<std::string>{"key.new","animation","key.destroy"}:std::vector<std::string>{"key.new","key.destroy"}));++cases;
    }
    VirtualFree(arena,0,MEM_RELEASE);printf("AffairMan x86 actual-FSE/real-Lua: %u policies passed (28 Init + 48 animation)\n",cases);return 0;
}catch(const std::exception& e){fprintf(stderr,"%s\n",e.what());return 1;}}
