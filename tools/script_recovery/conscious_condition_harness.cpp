// Compile the real helper/binding against runtime types and vendor sol/Lua.
// Execute the checked retail predicate bytes with controlled Thing vtable calls.
#define RETAIL_THING_CONDITION_NO_MAIN
#include "runtime_checks/retail_thing_condition.cpp"
#include "retail_conscious_condition.h"
#include "conscious-predicate.inc"

DWORD g_fableBase=0x400000;

static bool actorAlive=true, actorUnconscious=false;
static unsigned aliveQueries=0, unconsciousQueries=0;
static bool __fastcall queriedAlive(CScriptThing* thing,void*) {
    check(thing->pImp.Data==objectThing.pImp.Data); ++aliveQueries; return actorAlive;
}
static bool __fastcall queriedUnconscious(CScriptThing* thing,void*) {
    check(thing->pImp.Data==objectThing.pImp.Data); ++unconsciousQueries; return actorUnconscious;
}
static void __fastcall setConsciousCondition(void* entity,void*,const void* condition) {
    check(entity==expectedEntity);
    check(*static_cast<void***>(const_cast<void*>(condition))==ASLR<void**>(0x12c2fe8));
    if(throwRegistration) throw std::runtime_error("injected registration failure");
    if(conditionClone.pImp.Info) thingDestroy(&conditionClone,nullptr);
    thingCopy(&conditionClone,nullptr,reinterpret_cast<const CScriptThing*>(static_cast<const char*>(condition)+4));
    ++predicateSets;
}
struct ExecutablePredicate {
    void* memory=nullptr;
    ExecutablePredicate() {
        memory=VirtualAlloc(nullptr,sizeof(nativePredicateBytes),MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE);
        check(memory!=nullptr);
        std::memcpy(memory,nativePredicateBytes,sizeof(nativePredicateBytes));
        DWORD oldProtection=0;
        check(VirtualProtect(memory,sizeof(nativePredicateBytes),PAGE_EXECUTE_READ,&oldProtection)!=0);
        FlushInstructionCache(GetCurrentProcess(),memory,sizeof(nativePredicateBytes));
    }
    ~ExecutablePredicate(){if(memory) VirtualFree(memory,0,MEM_RELEASE);}
};
int main() {
    if(run_thing_condition()) return 1;
    try {
        static_assert(offsetof(CScriptThingVTable,IsAlive)==0x12c);
        static_assert(offsetof(CScriptThingVTable,IsUnconscious)==0xf4);
        CScriptThingVTable table{};
        table.IsAlive=reinterpret_cast<decltype(table.IsAlive)>(&queriedAlive);
        table.IsUnconscious=reinterpret_cast<decltype(table.IsUnconscious)>(&queriedUnconscious);
        objectThing.pVTable=reinterpret_cast<void**>(&table);
        objectThing.pImp.Data=reinterpret_cast<CScriptGameResourceObjectScriptedThingBase*>(0x4567);
        objectThing.pImp.Info=&objectInfo; objectInfo.RefCount=1;
        FakeHost host;host.m_Me=objectThing;auto* pEntityHost=&host;expectedEntity=&host;
        RetailEntity_SetCondition_API=reinterpret_cast<tRetailEntityCondition>(&setConsciousCondition);
        sol::state lua;lua.open_libraries(sol::lib::base);
        auto questState_type=lua.new_usertype<LuaQuestState>("Quest",sol::no_constructor);
        #include "conscious_condition_registration.inc"
        LuaQuestState quest;lua["quest"]=&quest;
        lua.script("assert(not pcall(function() quest:RegisterBoundConsciousCondition() end))");
        check(objectInfo.RefCount==1);
        void* task=reinterpret_cast<void*>(0x7890);
        std::memcpy(host.parent+0x2c,&task,sizeof(task));
        lua.script("quest:RegisterBoundConsciousCondition()");
        check(objectInfo.RefCount==2);
        ExecutablePredicate native;
        using Predicate=bool(__thiscall*)(void*);
        auto predicate=reinterpret_cast<Predicate>(native.memory);
        struct ConditionView {void** table;CScriptThing thing;} view{ASLR<void**>(0x12c2fe8),conditionClone};
        for(bool alive:{false,true}) for(bool unconscious:{false,true}) {
            actorAlive=alive;actorUnconscious=unconscious;aliveQueries=unconsciousQueries=0;
            check(predicate(&view)==(alive&&!unconscious));
            check(aliveQueries==1 && unconsciousQueries==(alive?1u:0u));
            check(objectInfo.RefCount==2);
        }
        throwRegistration=true;
        lua.script("assert(not pcall(function() quest:RegisterBoundConsciousCondition() end))");
        check(objectInfo.RefCount==2);throwRegistration=false;
        lua.script("quest:RegisterBoundConsciousCondition()");
        check(objectInfo.RefCount==2);
        thingDestroy(&conditionClone,nullptr);check(objectInfo.RefCount==1);
        thingDestroy(&objectThing,nullptr);check(objectInfo.RefCount==0);
        std::cout<<"PASS: native conscious predicate, short circuit, counted clone, failure cleanup, real Lua binding\n";
        return 0;
    } catch(const std::exception& error){std::cerr<<error.what()<<'\n';return 1;}
}
