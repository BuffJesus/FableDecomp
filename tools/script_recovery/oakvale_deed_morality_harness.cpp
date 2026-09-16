#define main TheresaReferenceMain
#include "theresa_scope_runtime_harness.cpp"
#undef main
#include <cstring>
static void __fastcall unexpectedMapClose(void*,void*){throw std::runtime_error("unexpected map");}
decltype(StdMap_String_Destroy_API) StdMap_String_Destroy_API=reinterpret_cast<decltype(StdMap_String_Destroy_API)>(&unexpectedMapClose);
static void* moralityTables[2][256]{};
static unsigned expectedMorality=0,moralityCalls=0;
static void __fastcall morality(CGameScriptInterfaceBase* self,void*,float amount){unsigned bits;std::memcpy(&bits,&amount,4);check(self==&game&&bits==expectedMorality);++moralityCalls;}
int main(){try{
    auto* arena=static_cast<unsigned char*>(VirtualAlloc(nullptr,0x2000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE));check(arena!=nullptr);
    g_fableBase=reinterpret_cast<DWORD>(arena)-(0x143e000-0x400000);
    unsigned char definitions[2][0xe00]{};sol::state lua;lua.open_libraries(sol::lib::base);
    auto type=lua.new_usertype<LuaRetailResources>("Scope",sol::no_constructor);type["ApplyOakvaleDeedMorality"]=&LuaRetailResources::ApplyOakvaleDeedMorality;
    unsigned cases=0;
    {LuaRetailResources scope(&game);lua["scope"]=&scope;
        for(unsigned bits:{0u,0x80000000u,0x3a83126fu,0x3e000000u,0xc0200000u,0x7f800000u,0xff800000u})for(bool good:{false,true})for(int index:{0,1}){
            *ASLR<unsigned char**>(0x143e90c)=definitions[index];std::memcpy(definitions[index]+0xd64,&bits,4);
            *reinterpret_cast<void***>(&game)=moralityTables[index];moralityTables[index][0x270/4]=reinterpret_cast<void*>(&morality);
            expectedMorality=good?bits:bits^0x80000000u;lua["good"]=good;lua.script("scope:ApplyOakvaleDeedMorality(good)");++cases;
        }
        check(moralityCalls==cases);*ASLR<unsigned char**>(0x143e90c)=nullptr;
        check(!lua.safe_script("scope:ApplyOakvaleDeedMorality(true)",sol::script_pass_on_error).valid());
        *ASLR<unsigned char**>(0x143e90c)=definitions[0];moralityTables[1][0x270/4]=nullptr;
        check(!lua.safe_script("scope:ApplyOakvaleDeedMorality(true)",sol::script_pass_on_error).valid());
        scope.Close();check(!lua.safe_script("scope:ApplyOakvaleDeedMorality(true)",sol::script_pass_on_error).valid());check(moralityCalls==cases);
    }
    lua["scope"]=sol::nil;VirtualFree(arena,0,MEM_RELEASE);
    std::cout<<"PASS: "<<cases<<" live morality value/sign/interface cases and three unavailable/closed guards\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
