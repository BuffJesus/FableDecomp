#define main resource_smoke_main
#include "runtime_checks/retail_resources_smoke.cpp"
#undef main
#include "rock_copied_thing.h"
DWORD g_fableBase=0;
static CScriptThing original{};
static CCPPointerInfo retainedInfo{};
static int copyDestroys,consumers;
static bool hasInfo,cleanupError;
static void __fastcall destroyCopy(CScriptThing* thing,void*) {
    check(thing!=&original && thing->pVTable==original.pVTable && thing->pImp.Data==original.pImp.Data && thing->pImp.Info==original.pImp.Info);
    if(hasInfo){check(retainedInfo.RefCount==3);--retainedInfo.RefCount;}
    ++copyDestroys;if(cleanupError)throw std::runtime_error("CLEANUP");
}
static float __fastcall health(CGameScriptInterfaceBase*,void*,const CScriptThing* thing){
    check(thing!=&original && thing->pImp.Data==original.pImp.Data && (!hasInfo || retainedInfo.RefCount==3));
    ++consumers;return 0.25f;
}
int main(){try{
    RetailThing_Destroy_API=reinterpret_cast<tRetailThingDestroy>(&destroyCopy);
    GetHealth_API=reinterpret_cast<tGetHealth>(&health);
    int cases=0;
    for(bool empty:{false,true})for(bool info:{false,true})for(int mode=0;mode<3;++mode)for(bool cleanup:{false,true}){
        hasInfo=info;cleanupError=cleanup;retainedInfo.RefCount=2;copyDestroys=consumers=0;
        original.pVTable=reinterpret_cast<decltype(original.pVTable)>(0x1238c8c);
        original.pImp.Data=empty?nullptr:reinterpret_cast<decltype(original.pImp.Data)>(0x2345);
        original.pImp.Info=info?&retainedInfo:nullptr;
        sol::state lua;lua.open_libraries(sol::lib::base);RegisterRockCopiedThing(lua);
        lua["scope"]=[&](sol::protected_function callback){WithRockCopiedThing(reinterpret_cast<CGameScriptInterfaceBase*>(1),&original,callback);};
        lua["mode"]=mode;
        auto result=lua.safe_script(R"(
            scope(function(actor)
                escaped=actor
                if mode==1 then return end -- cancellation before consumer
                assert(actor:GetHealth()==0.25)
                if mode==2 then error('BODY') end
            end)
        )",sol::script_pass_on_error);
        check(result.valid()==(!cleanup && mode!=2));
        if(!result.valid()){sol::error e=result;check(std::string(e.what()).find(mode==2?"BODY":"CLEANUP")!=std::string::npos);}
        check(copyDestroys==1 && consumers==(mode==1?0:1) && retainedInfo.RefCount==2);
        check(lua.safe_script("assert(not pcall(function() escaped:GetHealth() end)); escaped=nil; collectgarbage()",sol::script_pass_on_error).valid());
        check(copyDestroys==1 && retainedInfo.RefCount==2);++cases;
    }
    std::cout<<"Rock copied Thing x86: "<<cases<<" ownership Lua policies passed\n";return 0;
}catch(const std::exception& error){std::cerr<<error.what()<<'\n';return 1;}}

