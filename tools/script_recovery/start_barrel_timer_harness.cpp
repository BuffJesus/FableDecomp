#define main resource_smoke_main
#include "runtime_checks/retail_resources_smoke.cpp"
#undef main
DWORD g_fableBase=0x400000;
static CGameScriptInterfaceBase questGame{},timerGame[2]{};
static void* questTable[340]{},*timerTable[100]{};
static CScriptThing hero{};
static std::set<CScriptThing*> guards;
static bool populated=false,fired=false,published=false,inside=false;
static std::string fault;
static int timerCalls=0,frames=0,queries=0,loops=0,removes=0,bar=0;
static std::vector<std::string> keys;
static void hit(const char* point){if(fault==point){fired=true;throw std::runtime_error("BARREL_FAULT");}}
static void __fastcall keyCtor(CCharString* p,void*,const char* key,int n){check(n==-1);stringCtor(p,nullptr,key,n);keys.push_back("new:"+std::string(key));}
static void __fastcall keyDtor(CCharString* p,void*){keys.push_back("destroy:"+strings.at(p));stringDtor(p,nullptr);}
static CScriptThing* __fastcall lookup(CGameScriptInterfaceBase* self,void*,CScriptThing* out,const CCharString* key){check(self==&questGame&&published&&strings.at(const_cast<CCharString*>(key))=="M_WHouse_GuardPoint");out->pImp.Data=populated?reinterpret_cast<decltype(out->pImp.Data)>(1):nullptr;out->pImp.Info=nullptr;guards.insert(out);return out;}
static void __fastcall closeThing(CScriptThing* out,void*){check(strings.empty()&&guards.erase(out)==1);}
static void __fastcall wrongUpdate(CGameScriptInterfaceBase*,void*,int,float,float,float){check(false);}
static int __fastcall timer(CGameScriptInterfaceBase* self,void*,int id){check(self==&timerGame[timerCalls%2]&&id==-7);++timerCalls;if(timerCalls>2)questTable[0x530/4]=reinterpret_cast<void*>(&wrongUpdate);*ASLR<CGameScriptInterfaceBase**>(0x143e8f8)=&timerGame[timerCalls%2];hit(timerCalls>2?"timer.update":"timer.wait");return timerCalls==1?0:timerCalls==2?1:2147483647;}
static int __fastcall addBar(CGameScriptInterfaceBase* self,void*,float current,float max,const CRGBColour* a,const CRGBColour* b,const CCharString* icon,const CCharString* text,float scale){check(self==&questGame&&current==45.f&&max==0.f&&scale==1.f&&a!=b&&a->B==0&&a->G==255&&a->R==0&&a->A==255&&memcmp(a,b,4)==0);check(strings.size()==2&&strings.at(const_cast<CCharString*>(text)).empty()&&strings.at(const_cast<CCharString*>(icon))=="HUD_CLOCK_ICON");check(keys==std::vector<std::string>{"new:","new:HUD_CLOCK_ICON"});hit("add");return bar;}
static CScriptThing* __fastcall getHero(CGameScriptInterfaceBase* self,void*){check(self==&questGame&&guards.size()==1&&strings.empty());hit("hero");return &hero;}
static bool __fastcall distance(const CScriptThing* a,const CScriptThing* b,float value){check(a==&hero&&guards.count(const_cast<CScriptThing*>(b))==1&&value==2.f);hit("distance");return inside;}
static void __fastcall colour(CGameScriptInterfaceBase* self,void*,int id,const CRGBColour* a,const CRGBColour* b){check(self==&questGame&&id==bar&&a!=b&&memcmp(a,b,4)==0&&a->B==0&&a->A==255&&a->G==(inside?255:0)&&a->R==(inside?0:255));hit("colour");}
static void __fastcall update(CGameScriptInterfaceBase* self,void*,int id,float current,float max,float scale){check(self==&questGame&&id==bar&&current==2147483648.f&&max==-1.f&&scale==-1.f);hit("update");}
static void __fastcall removeBar(CGameScriptInterfaceBase* self,void*,int id){check(self==&questGame&&id==bar);++removes;hit("remove");}
tIsDistanceBetweenThingsUnder IsDistanceBetweenThingsUnder_API=&distance;
int main(){try{
    void* arena=VirtualAlloc(nullptr,0x10000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);check(arena!=nullptr);g_fableBase=reinterpret_cast<DWORD>(arena)-(0x143e000-0x400000);
    *reinterpret_cast<void***>(&questGame)=questTable;for(auto& game:timerGame)*reinterpret_cast<void***>(&game)=timerTable;
    timerTable[0x168/4]=reinterpret_cast<void*>(&timer);questTable[0x510/4]=reinterpret_cast<void*>(&addBar);questTable[0x118/4]=reinterpret_cast<void*>(&getHero);questTable[0x534/4]=reinterpret_cast<void*>(&colour);questTable[0x530/4]=reinterpret_cast<void*>(&update);questTable[0x548/4]=reinterpret_cast<void*>(&removeBar);
    CCharString_Construct_Literal=reinterpret_cast<decltype(CCharString_Construct_Literal)>(&keyCtor);CCharString_Destroy=reinterpret_cast<decltype(CCharString_Destroy)>(&keyDtor);GetThingWithScriptName_ByName_API=reinterpret_cast<tGetThingWithScriptName1>(&lookup);RetailThing_Destroy_API=reinterpret_cast<tRetailThingDestroy>(&closeThing);
    unsigned cases=0;
    for(bool output:{false,true})for(bool within:{false,true})for(int cancel:{1,3,99})for(const char* fail:{"","timer.wait","add","publish","frame.loop","hero","distance","colour","timer.update","update","remove"}){
        questTable[0x530/4]=reinterpret_cast<void*>(&update);populated=output;inside=within;fault=fail;fired=published=false;timerCalls=frames=queries=loops=removes=0;bar=output?0:-1;keys.clear();*ASLR<CGameScriptInterfaceBase**>(0x143e8f8)=&timerGame[0];
        sol::state lua;lua.open_libraries(sol::lib::base);RegisterRetailResources(lua);auto q=lua.create_table();
        q["WithRetailResources"]=[&](sol::object,sol::protected_function body){WithRetailResources(&questGame,body);};
        q["NewScriptFrame"]=[&](sol::object){if(++frames>1)hit("frame.loop");};q["IsActiveThreadTerminating"]=[&](sol::object){return ++queries>=cancel;};
        q["GetStateInt"]=[&](sol::object,const std::string& key){check(key=="WatchTimer"||key=="GUIBarrelCounter");return key=="WatchTimer"?-7:bar;};
        q["SetStateInt"]=[&](sol::object,const std::string& key,int value){check(key=="GUIBarrelCounter"&&value==bar&&strings.size()==2);published=true;hit("publish");};
        q["GetStateBool"]=[&](sol::object,const std::string& key){check(key=="BarrelManSpokenToHeroOnReturn");return loops++>=1;};lua["quest"]=q;
        check(lua.safe_script_file("../CANDIDATE.lua",sol::script_pass_on_error).valid());auto result=lua.safe_script("StartBarrelTimer(quest)",sol::script_pass_on_error);check(result.valid()==!fired);
        if(!result.valid()){sol::error e=result;check(std::string(e.what()).find("BARREL_FAULT")!=std::string::npos);}
        check(guards.empty()&&strings.empty());if(cancel!=99)check(removes==0);
        if(published)check(keys[0]=="new:"&&keys[1]=="new:HUD_CLOCK_ICON"&&keys[2]=="destroy:HUD_CLOCK_ICON"&&keys[3]=="destroy:");++cases;
    }
    VirtualFree(arena,0,MEM_RELEASE);printf("StartBarrelTimer x86 actual-FSE/real-Lua: %u full-helper policies passed\n",cases);return 0;
}catch(const std::exception& e){fprintf(stderr,"%s\n",e.what());return 1;}}
