#define main resource_smoke_main
#include "runtime_checks/retail_resources_smoke.cpp"
#undef main
#include <cmath>
struct NativeGlobals { C3DVector fallback; char gap[32]; const unsigned char* definition; };
static NativeGlobals globals{};
DWORD g_fableBase=0;
static unsigned char definitionA[0x730]{},definitionB[0x730]{};
static C3DVector markerPosition{12.0f,-3.0f,8.5f};
static CScriptGameResourceObjectScriptedThingBase markerData{};
static CCPPointerInfo markerInfo{},spawnedInfo{};
static bool emptyMarker,emptySpawn,throwCreate;
static CScriptThing* spawnedOutput;
static std::vector<std::string> events;
class LuaQuestState { public: float GetRockTrollTriggerProximity(); };
#include "rock_trigger_proximity.inc"
static CScriptThing* __fastcall markerLookup(CGameScriptInterfaceBase*,void*,CScriptThing* out,const CCharString* key){
    check(strings.at(const_cast<CCharString*>(key))=="M_RTFERockTrollSpawnPos");
    events.push_back("lookup");
    if(!emptyMarker){out->pImp.Data=&markerData;out->pImp.Info=&markerInfo;++markerInfo.RefCount;}
    return out;
}
static const C3DVector* __fastcall markerGetPos(CGameScriptThing* data,void*){
    check(data==reinterpret_cast<CGameScriptThing*>(&markerData)&&strings.size()==2&&markerInfo.RefCount==1);
    events.push_back("position");return &markerPosition;
}
static CScriptThing* __fastcall spawnCreature(CGameScriptInterfaceBase*,void*,CScriptThing* out,
    const CCharString* definition,const C3DVector* position,const CCharString* script,bool flag){
    check(strings.at(const_cast<CCharString*>(definition))=="CREATURE_ROCK_TROLL_START_STANDING");
    check(strings.at(const_cast<CCharString*>(script))=="RTFE_RockTroll"&&!flag);
    check(position==(emptyMarker?&globals.fallback:&markerPosition));
    check(markerInfo.RefCount==(emptyMarker?0:1));
    spawnedOutput=out;events.push_back("create");
    if(!emptySpawn){out->pImp.Data=&markerData;out->pImp.Info=&spawnedInfo;++spawnedInfo.RefCount;}
    if(throwCreate)throw std::runtime_error("create error");return out;
}
static void __fastcall trackedDestroy(CScriptThing* thing,void*){
    if(thing==spawnedOutput){check(strings.size()==2);events.push_back("output.destroy");}
    else {check(strings.empty());events.push_back("marker.destroy");}
    if(thing->pImp.Info){check(thing->pImp.Info->RefCount==1);--thing->pImp.Info->RefCount;}
    thing->pImp={};
}
static void __fastcall trackedStringDestroy(CCharString* key,void*){
    events.push_back("string.destroy:"+strings.at(key));stringDtor(key,nullptr);
}
tCreateCreature CreateCreature_API=reinterpret_cast<tCreateCreature>(&spawnCreature);
int main(){
    try{
        static_assert(offsetof(NativeGlobals,definition)==0x2c,"Native fallback/definition global spacing");
        g_fableBase=reinterpret_cast<DWORD>(&globals.fallback)-(0x143e8e0-0x400000);
        globals.fallback={91.0f,92.0f,93.0f};globals.definition=definitionA;
        LuaQuestState quest;float value=5.0f;std::memcpy(definitionA+0x72c,&value,4);check(quest.GetRockTrollTriggerProximity()==5.0f);
        value=8.25f;std::memcpy(definitionA+0x72c,&value,4);check(quest.GetRockTrollTriggerProximity()==8.25f);
        value=-2.0f;std::memcpy(definitionB+0x72c,&value,4);globals.definition=definitionB;check(quest.GetRockTrollTriggerProximity()==-2.0f);
        globals.definition=nullptr;bool rejected=false;try{quest.GetRockTrollTriggerProximity();}catch(const std::runtime_error&){rejected=true;}check(rejected);
        void* table[7]{};table[6]=reinterpret_cast<void*>(&markerGetPos);markerData.pVTable=table;
        GetThingWithScriptName_ByName_API=reinterpret_cast<tGetThingWithScriptName1>(&markerLookup);
        RetailThing_Destroy_API=reinterpret_cast<tRetailThingDestroy>(&trackedDestroy);
        CCharString_Destroy=reinterpret_cast<tCCharString_Destructor>(&trackedStringDestroy);
        for(bool markerEmpty:{false,true})for(bool spawnEmpty:{false,true})for(bool failure:{false,true}){
            emptyMarker=markerEmpty;emptySpawn=spawnEmpty;throwCreate=failure;spawnedOutput=nullptr;events.clear();
            LuaRetailResources scope(reinterpret_cast<CGameScriptInterfaceBase*>(1));
            unsigned id=scope.NewThingFromScriptName("M_RTFERockTrollSpawnPos");events.clear();
            sol::state lua;lua.open_libraries(sol::lib::base);RegisterRetailResources(lua);
            lua["resources"]=&scope;lua["marker"]=id;
            auto result=lua.safe_script("assert(resources:CreateCreatureAtThingPosition(marker,'CREATURE_ROCK_TROLL_START_STANDING','RTFE_RockTroll',false)==nil)",sol::script_pass_on_error);
            const bool threw=!result.valid();
            check(threw==failure&&spawnedInfo.RefCount==0&&markerInfo.RefCount==(markerEmpty?0:1)&&strings.empty());
            std::vector<std::string> expected;if(!markerEmpty)expected.push_back("position");
            expected.insert(expected.end(),{"create","output.destroy","string.destroy:CREATURE_ROCK_TROLL_START_STANDING","string.destroy:RTFE_RockTroll"});
            check(events==expected);scope.DestroyThing(id);check(markerInfo.RefCount==0&&events.back()=="marker.destroy");
            auto size=events.size();scope.Close();check(events.size()==size);
        }
        std::cout<<"PASS: live proximity pointer/value rereads; Data-vtable position; exact nonzero native fallback address; spawn flags/output identity; output-definition-script cleanup; empty outputs and exception cleanup; retained marker lifetime\n";return 0;
    }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}
}
