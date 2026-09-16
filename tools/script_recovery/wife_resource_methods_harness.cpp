// Exercise the proposed resource header unchanged, through its real sol bindings.
#define main resource_smoke_main
#include "runtime_checks/retail_resources_smoke.cpp"
#undef main
#define main argument_key_cpp_main
#include "wife_argument_key_harness.cpp"
#undef main
#include <cstring>

DWORD g_fableBase=0;
tIsDistanceBetweenThingsUnder IsDistanceBetweenThingsUnder_API=nullptr;
tEntitySetFacingAngleTowardsThing EntitySetFacingAngleTowardsThing_API=nullptr;
tAddPersonToConversation AddPersonToConversation_API=nullptr;
tAddLineToConversation AddLineToConversation_API=reinterpret_cast<tAddLineToConversation>(&Line);
tGFIntToCharString GFIntToCharString_API=&Number;
tCCharString_AssignmentLiteral CCharString_AssignLiteral_API=reinterpret_cast<tCCharString_AssignmentLiteral>(&Assign);
tTextEntryExists TextEntryExists_API=reinterpret_cast<tTextEntryExists>(&Exists);
static unsigned actorDestructions=0;
static void __fastcall resourceLiteral(CCharString* result,void* unused,const char* text,int length) {
    Literal(result,unused,text,length);
    result->pStringData=reinterpret_cast<decltype(result->pStringData)>(1);
}
static CScriptThing* __fastcall lookupHusband(CGameScriptInterfaceBase* actual,void*,CScriptThing* output,const CCharString* name) {
    check(actual==game && live.at(name)=="NOVI_AffairMan");
    output->pImp={};husband=output;return output;
}
static void __fastcall destroyHusband(CScriptThing* actor,void*) {
    check(actor==husband && live.empty());++actorDestructions;
}

int main() {
    void* arena=nullptr;
    try {
        argument_key_cpp_main();
        // Relocate the exact proposed ASLR call into this private process only.
        arena=VirtualAlloc(nullptr,0x5A0000,MEM_RESERVE,PAGE_NOACCESS);check(arena!=nullptr);
        g_fableBase=reinterpret_cast<DWORD>(arena);
        auto* page=static_cast<unsigned char*>(VirtualAlloc(static_cast<char*>(arena)+0x59F000,4096,MEM_COMMIT,PAGE_READWRITE));
        check(page!=nullptr);auto* entry=page+0x570;entry[0]=0xE9;
        DWORD delta=reinterpret_cast<DWORD>(&Concat)-reinterpret_cast<DWORD>(entry)-5;
        std::memcpy(entry+1,&delta,4);DWORD previous;
        check(VirtualProtect(page,4096,PAGE_EXECUTE_READ,&previous)!=0);
        check(FlushInstructionCache(GetCurrentProcess(),page,4096)!=0);
        CCharString_Construct_Literal=reinterpret_cast<tCCharString_Constructor_Literal>(&resourceLiteral);
        CCharString_Destroy=reinterpret_cast<tCCharString_Destructor>(&Destroy);
        GetThingWithScriptName_ByName_API=reinterpret_cast<tGetThingWithScriptName1>(&lookupHusband);
        RetailThing_Destroy_API=reinterpret_cast<tRetailThingDestroy>(&destroyHusband);
        CScriptThing actor{};wife=&actor;
        for(bool exists:{false,true}) for(int failure:{0,1,2}) {
            check(live.empty());textExists=exists;events.clear();
            sol::state lua;lua.open_libraries(sol::lib::base,sol::lib::string);RegisterRetailResources(lua);
            auto scope=std::make_shared<LuaRetailResources>(game);
            lua["resources"]=scope;lua["me"]=wife;lua["failure"]=failure;
            lua["partner"]=scope->NewThingFromScriptName("NOVI_AffairMan");
            lua["wrongKind"]=scope->NewResource();
            lua.script(R"(
                local ok,err=pcall(function()
                    resources:WithArgumentKey(50,function(key)
                        escaped=key
                        if not key:Exists() then key:ResetToFirst() end
                        assert(not pcall(function() resources:AddArgumentKeyLine(key,-7,me,wrongKind) end))
                        assert(not pcall(function() resources:AddArgumentKeyLine(key,-7,me,nil) end))
                        assert(not pcall(function() resources:AddArgumentKeyLine(key,-7,me,partner+0.5) end))
                        if failure==1 then error('callback failure') end
                        resources:AddArgumentKeyLine(key,-7,me,partner)
                        if failure==2 then error('reply failure') end
                        return true
                    end)
                end)
                assert(ok==(failure==0))
                if failure~=0 then assert(string.find(err,'failure',1,true)) end
                assert(not pcall(function() resources:AddArgumentKeyLine(escaped,-7,me,partner) end))
            )");
            check(live.empty());
            auto before=actorDestructions;scope->Close();check(actorDestructions==before+1);
            lua.script(R"(
                assert(not pcall(function() resources:WithArgumentKey(50,function() return true end) end))
                assert(not pcall(function() resources:AddArgumentKeyLine(escaped,-7,me,partner) end))
            )");
        }
        check(VirtualFree(arena,0,MEM_RELEASE)!=0);arena=nullptr;
        std::cout<<"PASS: actual resource bindings, owned husband identity, dynamic text lookup, callback failures and escaped/closed guards\n";
        return 0;
    } catch(const std::exception& error) {
        if(arena)VirtualFree(arena,0,MEM_RELEASE);
        std::cerr<<error.what()<<'\n';return 1;
    }
}
