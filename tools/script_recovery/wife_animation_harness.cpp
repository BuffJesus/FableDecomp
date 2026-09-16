#define main resource_smoke_main
#include "runtime_checks/retail_resources_smoke.cpp"
#undef main
DWORD g_fableBase=0x400000;
static CScriptThing actor{};
static CScriptGameResourceObjectScriptedThingBase expert{};
static unsigned char expectedRaw=0;
static std::string expectedKey;
static bool failAction=false;
static unsigned animationCalls=0;
static std::vector<std::string> trace;
static void __fastcall keyCtor(CCharString* key,void*,const char* text,int n){stringCtor(key,nullptr,text,n);check(text==expectedKey);trace.push_back("key.new");*ASLR<volatile unsigned char*>(0x1375748)=expectedRaw;}
static void __fastcall keyDtor(CCharString* key,void*){check(strings.at(key)==expectedKey);stringDtor(key,nullptr);trace.push_back("key.destroy");}
static void __fastcall animation(void* self,void*,const CCharString* key,bool a,bool b,bool c,bool d,unsigned char raw,bool f,bool g){check(self==&expert&&strings.at(const_cast<CCharString*>(key))==expectedKey&&!a&&!b&&!c&&d&&raw==expectedRaw&&!f&&!g);++animationCalls;trace.push_back("animation");if(failAction)throw std::runtime_error("WIFE_ANIMATION_FAULT");}
int main(){try{
    void* arena=VirtualAlloc(nullptr,0x10000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);check(arena!=nullptr);g_fableBase=reinterpret_cast<DWORD>(arena)-(0x1370000-0x400000);
    CCharString_Construct_Literal=reinterpret_cast<decltype(CCharString_Construct_Literal)>(&keyCtor);CCharString_Destroy=reinterpret_cast<decltype(CCharString_Destroy)>(&keyDtor);
    CScriptGameResourceObjectScriptedThingBaseVTable table{};static_assert(offsetof(CScriptGameResourceObjectScriptedThingBaseVTable,PlayAnimation)==0x48);
    table.PlayAnimation=reinterpret_cast<decltype(table.PlayAnimation)>(&animation);expert.pVTable=reinterpret_cast<void**>(&table);acquireExpert=&expert;unsigned cases=0;
    for(const char* key:{"ST_ARGUING_POINT_AWAY","ST_ARGUING_POINT_AT"})for(unsigned raw:{0u,1u,2u,255u})for(bool populated:{false,true})for(bool error:{false,true}){
        expectedKey=key;expectedRaw=static_cast<unsigned char>(raw);*ASLR<volatile unsigned char*>(0x1375748)=0x55;acquireOK=populated;failAction=error;trace.clear();const auto before=animationCalls;
        sol::state lua;lua.open_libraries(sol::lib::base);RegisterRetailResources(lua);lua["actor"]=&actor;lua["key"]=expectedKey;
        lua["scope"]=[&](sol::protected_function body){WithRetailResources(reinterpret_cast<CGameScriptInterfaceBase*>(1),body);};
        auto result=lua.safe_script("scope(function(r) local c=r:NewResource();r:TryAcquire(c,actor,4);r:PlayWifeArgumentAnimation(c,key) end)",sol::script_pass_on_error);
        check(result.valid()==(!error||!populated));check(strings.empty()&&locals.empty()&&animationCalls==before+static_cast<unsigned>(populated));
        check(trace==(populated?std::vector<std::string>{"key.new","animation","key.destroy"}:std::vector<std::string>{"key.new","key.destroy"}));++cases;
    }
    VirtualFree(arena,0,MEM_RELEASE);printf("Wife animation x86 actual-FSE/real-Lua: %u policies passed\n",cases);return 0;
}catch(const std::exception& e){fprintf(stderr,"%s\n",e.what());return 1;}}
