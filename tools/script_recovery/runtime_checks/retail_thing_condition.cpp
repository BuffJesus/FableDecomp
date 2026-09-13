// Actual scoped Thing methods and alive-condition helper/binding; engine doubles.
#define main resource_smoke_main
#include "retail_resources_smoke.cpp"
#undef main
#include "LuaRetailCondition.h"

static CScriptThing objectThing{};
static CCPPointerInfo objectInfo{};
static unsigned thingCopies=0, thingDestroys=0, objectDeletes=0, predicateSets=0;
static CScriptThing conditionClone{};
static bool lookupMissing=false, throwRegistration=false;
static void* expectedEntity=nullptr;
static CScriptThing* __fastcall thingCopy(CScriptThing* out, void*, const CScriptThing* source) {
    *out=*source; if(out->pImp.Info) ++out->pImp.Info->RefCount;
    ++thingCopies;return out;
}
static void __fastcall thingDestroy(CScriptThing* thing,void*) {
    if(thing->pImp.Info) {
        check(thing->pImp.Info->RefCount>0);
        if(--thing->pImp.Info->RefCount==0) ++objectDeletes;
    }
    thing->pImp={};++thingDestroys;
}
static CScriptThing* __fastcall resourceThing(CScriptGameResourceObjectScriptedThingBase* self,void*,CScriptThing* out) {
    check(self==expectedExpert);return thingCopy(out,nullptr,&objectThing);
}
static float __fastcall thingHealth(CGameScriptInterfaceBase*,void*,const CScriptThing* thing) {
    check(thing->pVTable==g_pCScriptThingVTable);
    return thing->pImp.Data ? 12.5f : 0.0f;
}
static bool __fastcall thingAlive(CScriptThing* thing,void*) { return thing->pImp.Data!=nullptr; }
static CScriptThing* __fastcall thingLookup(CGameScriptInterfaceBase*,void*,CScriptThing* out,const CCharString* key) {
    check(strings.at(const_cast<CCharString*>(key))=="WitchwoodTeleporter");
    if(lookupMissing){out->pVTable=g_pCScriptThingVTable;out->pImp={};return out;}
    return thingCopy(out,nullptr,&objectThing);
}
static void __fastcall teleporterActive(CGameScriptInterfaceBase*,void*,const CScriptThing* thing,bool active) {
    check(active && thing->pImp.Data==objectThing.pImp.Data);
}
static void __fastcall setCondition(void* entity,void*,const void* condition) {
    check(entity==expectedEntity && *static_cast<void***>(const_cast<void*>(condition))==g_pRetailAliveConditionVTable);
    if(throwRegistration) throw std::runtime_error("injected registration failure");
    if(conditionClone.pImp.Info) thingDestroy(&conditionClone,nullptr);
    thingCopy(&conditionClone,nullptr,reinterpret_cast<const CScriptThing*>(static_cast<const char*>(condition)+4));
    ++predicateSets;
}
struct FakeHost {
    CScriptThing m_Me;
    char parent[64]{};
    void* GetParentScript_Stub(){return parent;}
};
struct LuaQuestState {};

static int run_thing_condition() {
    try {
        CScriptThingVTable thingTable{};
        thingTable.IsAlive=reinterpret_cast<decltype(thingTable.IsAlive)>(&thingAlive);
        g_pCScriptThingVTable=reinterpret_cast<void**>(&thingTable);
        g_pRetailAliveConditionVTable=reinterpret_cast<void**>(0x1234);
        RetailThing_Copy_API=reinterpret_cast<tRetailThingCopy>(&thingCopy);
        RetailThing_Destroy_API=reinterpret_cast<tRetailThingDestroy>(&thingDestroy);
        RetailEntity_SetCondition_API=reinterpret_cast<tRetailEntityCondition>(&setCondition);
        GetHealth_API=reinterpret_cast<tGetHealth>(&thingHealth);
        GetThingWithScriptName_ByName_API=reinterpret_cast<tGetThingWithScriptName1>(&thingLookup);
        SetTeleporterAsActive_API=reinterpret_cast<tSetTeleporterAsActive>(&teleporterActive);
        objectThing.pVTable=g_pCScriptThingVTable;
        objectThing.pImp.Data=reinterpret_cast<CScriptGameResourceObjectScriptedThingBase*>(0x4567);
        objectThing.pImp.Info=&objectInfo;objectInfo.RefCount=1;
        CScriptGameResourceObjectScriptedThingBaseVTable expertTable{};
        expertTable.GetScriptThing=reinterpret_cast<decltype(expertTable.GetScriptThing)>(&resourceThing);
        CScriptGameResourceObjectScriptedThingBase expert{};expert.pVTable=reinterpret_cast<void**>(&expertTable);
        expectedExpert=acquireExpert=&expert;
        sol::state lua;lua.open_libraries(sol::lib::base);
        lua.new_usertype<CScriptThing>("CScriptThing",sol::no_constructor);
        RegisterRetailResources(lua);
        auto scope=std::make_shared<LuaRetailResources>(reinterpret_cast<CGameScriptInterfaceBase*>(1));
        auto resource=scope->NewResource();check(scope->TryAcquire(resource,&objectThing,4));acquireExpert=nullptr;
        lua["r"]=scope;lua["resource"]=resource;
        lua.script(R"(
            a=r:NewThingFromResource(resource)
            b=r:NewThingFromResource(resource)
            r:ReleaseResource(resource)
            assert(r:ThingHealth(a)==12.5 and r:ThingAlive(b))
            r:DestroyThing(a)
            assert(not pcall(function() r:ThingHealth(a) end))
            r:DestroyThing(b)
            local emptyResource=r:NewResource()
            local empty=r:NewThingFromResource(emptyResource)
            assert(r:ThingHealth(empty)==0 and not r:ThingAlive(empty))
            r:DestroyThing(empty);r:ReleaseResource(emptyResource)
            local t=r:NewThingFromScriptName('WitchwoodTeleporter')
            r:SetTeleporterActive(t,true);r:DestroyThing(t)
            assert(not pcall(function() r:DestroyThing(resource) end))
        )");
        check(objectInfo.RefCount==1 && objectDeletes==0);
        lookupMissing=true;
        lua.script("local t=r:NewThingFromScriptName('WitchwoodTeleporter');assert(not r:ThingAlive(t));r:DestroyThing(t)");
        lookupMissing=false;
        lua.script("retainedThing=r:NewThingFromScriptName('WitchwoodTeleporter')");
        check(objectInfo.RefCount==2);scope->Close();check(objectInfo.RefCount==1);
        lua.script("assert(not pcall(function() r:ThingAlive(retainedThing) end))");
        FakeHost host;host.m_Me=objectThing;auto* pEntityHost=&host;expectedEntity=&host;
        auto questState_type=lua.new_usertype<LuaQuestState>("Quest",sol::no_constructor);
        #include "alive-registration.inc"
        LuaQuestState quest;lua["quest"]=&quest;
        lua.script("assert(not pcall(function() quest:RegisterBoundAliveCondition() end))");
        check(predicateSets==0 && objectInfo.RefCount==1);
        void* task=reinterpret_cast<void*>(0x7890);std::memcpy(host.parent+0x2c,&task,sizeof(task));
        lua.script("quest:RegisterBoundAliveCondition();quest:RegisterBoundAliveCondition()");
        check(predicateSets==2 && objectInfo.RefCount==2);
        throwRegistration=true;
        lua.script("assert(not pcall(function() quest:RegisterBoundAliveCondition() end))");
        check(objectInfo.RefCount==2);throwRegistration=false;
        thingDestroy(&conditionClone,nullptr);check(objectInfo.RefCount==1);
        thingDestroy(&objectThing,nullptr);check(objectDeletes==1);
        std::cout<<"PASS: scoped Thing ownership/copy/release/lookup, resource-independent copies, empty and stale handles; actual condition binding, clone ownership, replacement, missing-thread and error cleanup\n";
        return 0;
    } catch(const std::exception& error){std::cerr<<error.what()<<"\n";return 1;}
}

#ifndef RETAIL_THING_CONDITION_NO_MAIN
int main() { return run_thing_condition(); }
#endif
