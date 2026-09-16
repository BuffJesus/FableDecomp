#define main resource_smoke_main
#include "runtime_checks/retail_resources_smoke.cpp"
#undef main
#include "maze_sword_slot.h"
static std::vector<std::string> events;
static CCPPointerInfo* nextInfo;
static CScriptGameResourceObjectScriptedThingBase* nextData;
static const CScriptThing* stableSlot;
static bool consumerFailure;
static void __fastcall deleteObject(void* data){events.push_back("delete:"+std::to_string(reinterpret_cast<uintptr_t>(data)));}
static void __cdecl freeInfo(void* info){events.push_back("free.info");std::free(info);}
static CCPPointerInfo* makeInfo(unsigned id){
    auto* info=static_cast<CCPPointerInfo*>(std::malloc(sizeof(CCPPointerInfo)));
    *info={0,&deleteObject,reinterpret_cast<void*>(id)};return info;
}
static CScriptThing* __fastcall lookupSword(CGameScriptInterfaceBase*,void*,CScriptThing* output,const CCharString* key){
    check(strings.size()==1&&strings.at(const_cast<CCharString*>(key))=="GoodSword");
    events.push_back("lookup");output->pImp={nextData,nextInfo};if(nextInfo)++nextInfo->RefCount;return output;
}
static void __fastcall destroySword(CScriptThing* thing,void*){
    events.push_back(strings.empty()?"slot.destroy":"output.destroy");
    auto* info=thing->pImp.Info;
    if(info&&--info->RefCount==0){info->DeleteFunc(info->Data);Game_free(info);}
    thing->pImp={};
}
static void __fastcall swordLimbo(CGameScriptInterfaceBase*,void*,const CScriptThing* thing,bool value,bool extra){
    check(strings.empty()&&extra);events.push_back(value?"limbo.true":"limbo.false");
    if(!stableSlot)stableSlot=thing;check(thing==stableSlot);
    if(thing->pImp.Info)check(thing->pImp.Info->RefCount==1);
    if(consumerFailure)throw std::runtime_error("consumer error");
}
static void __fastcall swordAlpha(CGameScriptInterfaceBase*,void*,const CScriptThing* thing,float value,bool extra){
    check(thing==stableSlot&&value==0.0f&&extra&&strings.empty());events.push_back("alpha");
}
tEntitySetInLimbo EntitySetInLimbo_API=reinterpret_cast<tEntitySetInLimbo>(&swordLimbo);
tEntitySetAlpha EntitySetAlpha_API=reinterpret_cast<tEntitySetAlpha>(&swordAlpha);
class LuaQuestState {
public:
    std::unique_ptr<MazeSwordSlot> m_mazeSwordSlot;
    void InitializeMazeSword();
    void UnlimboMazeSword();
};
#include "maze_sword_adapter.inc"
int main(){
    try{
        Game_free=&freeInfo;
        RetailThing_Destroy_API=reinterpret_cast<tRetailThingDestroy>(&destroySword);
        GetThingWithScriptName_ByName_API=reinterpret_cast<tGetThingWithScriptName1>(&lookupSword);
        LuaQuestState host;host.m_mazeSwordSlot=std::make_unique<MazeSwordSlot>(reinterpret_cast<CGameScriptInterfaceBase*>(1),&freeInfo);
        sol::state lua;lua.open_libraries(sol::lib::base);
        auto type=lua.new_usertype<LuaQuestState>("Quest",sol::no_constructor);
        type["InitializeMazeSword"]=&LuaQuestState::InitializeMazeSword;
        type["UnlimboMazeSword"]=&LuaQuestState::UnlimboMazeSword;
        lua["quest"]=&host;
        nextInfo=makeInfo(1);nextData=reinterpret_cast<decltype(nextData)>(1);
        lua.script("assert(quest:InitializeMazeSword()==nil)");check(nextInfo->RefCount==1);
        check(events==std::vector<std::string>{"lookup","output.destroy","limbo.true"});
        events.clear();nextData=reinterpret_cast<decltype(nextData)>(99);
        lua.script("quest:InitializeMazeSword()");
        check(stableSlot->pImp.Data==reinterpret_cast<decltype(nextData)>(1)); // same Info skips Data assignment
        check(events==std::vector<std::string>{"lookup","output.destroy","limbo.true"});
        events.clear();nextInfo=makeInfo(2);nextData=reinterpret_cast<decltype(nextData)>(2);
        lua.script("quest:InitializeMazeSword()");
        check(events==std::vector<std::string>{"lookup","delete:1","free.info","output.destroy","limbo.true"});
        events.clear();lua.script("assert(quest:UnlimboMazeSword()==nil)");
        check(events==std::vector<std::string>{"limbo.false","alpha"});
        events.clear();nextInfo=nullptr;nextData=nullptr;
        lua.script("quest:InitializeMazeSword(); quest:UnlimboMazeSword()");
        check(events==std::vector<std::string>{"lookup","delete:2","free.info","output.destroy","limbo.true","limbo.false","alpha"});
        events.clear();nextInfo=makeInfo(3);nextData=reinterpret_cast<decltype(nextData)>(3);consumerFailure=true;
        auto failure=lua.safe_script("quest:InitializeMazeSword()",sol::script_pass_on_error);
        check(!failure.valid()&&nextInfo->RefCount==1&&strings.empty());
        consumerFailure=false;events.clear();
        host.m_mazeSwordSlot->CloseAfterQuiescence();host.m_mazeSwordSlot->CloseAfterQuiescence();
        check(events==std::vector<std::string>{"slot.destroy","delete:3","free.info"});
        auto closed=lua.safe_script("quest:UnlimboMazeSword()",sol::script_pass_on_error);check(!closed.valid());
        host.m_mazeSwordSlot.reset();lua.collect_garbage();
        check(events.size()==3); // no Lua ownership and no second destruction
        std::cout<<"PASS: native Info-identity assignment, immediate replacement release, empty replacement, stable atomic consumers, no exported Lua owner, error retention, idempotent quiescent close\n";return 0;
    }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}
}
