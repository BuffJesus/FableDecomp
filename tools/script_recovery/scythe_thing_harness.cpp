#define main resource_smoke_main
#include "runtime_checks/retail_resources_smoke.cpp"
#undef main
static CScriptThing bound{};
static CCPPointerInfo returnedInfo{};
static C3DVector position{12.0f,-4.0f,7.5f};
static std::vector<std::string> events;
static bool emptyOutput, throwConsumer;
static bool snapValue;
static std::string lookupName;
static CScriptThing* outputIdentity;
class LuaQuestState {
public:
    CGameScriptInterfaceBase* m_pGameInterface=reinterpret_cast<CGameScriptInterfaceBase*>(1);
    void SpawnScytheAndRemoveMarker(CScriptThing*);
    void FaceThingByScriptName(CScriptThing*, const std::string&, bool);
};
static const C3DVector* __fastcall pos(CScriptThing* me,void*){check(me==&bound);events.push_back("position");return &position;}
static void fill(CScriptThing* out){
    outputIdentity=out;
    if(!emptyOutput){out->pImp.Data=reinterpret_cast<decltype(out->pImp.Data)>(&returnedInfo);out->pImp.Info=&returnedInfo;++returnedInfo.RefCount;}
}
static CScriptThing* __fastcall create(CGameScriptInterfaceBase*,void*,CScriptThing* out,const CCharString* def,
    const C3DVector* p,const CCharString* script,bool flag){
    check(strings.at(const_cast<CCharString*>(def))=="CREATURE_RIVAL_HERO_SCYTHE");
    check(strings.at(const_cast<CCharString*>(script))=="ScytheNearOracle" && p==&position && !flag);
    events.push_back("create");fill(out);return out;
}
static CScriptThing* __fastcall lookup(CGameScriptInterfaceBase*,void*,CScriptThing* out,const CCharString* key){
    check(strings.at(const_cast<CCharString*>(key))==lookupName);events.push_back("lookup");fill(out);return out;
}
static void __fastcall removeBound(CGameScriptInterfaceBase*,void*,const CScriptThing* me,bool a,bool b){
    check(me==&bound&&!a&&b&&returnedInfo.RefCount==(emptyOutput?0:1)&&strings.empty());events.push_back("remove");
    if(throwConsumer)throw std::runtime_error("consumer error");
}
static void __fastcall facing(CGameScriptInterfaceBase*,void*,const CScriptThing* me,const CScriptThing* target,bool snap){
    check(me==&bound&&target==outputIdentity&&snap==snapValue&&returnedInfo.RefCount==(emptyOutput?0:1)&&strings.size()==1);events.push_back("face");
    if(throwConsumer)throw std::runtime_error("consumer error");
}
static void __fastcall destroyReturned(CScriptThing* t,void*){
    check(strings.size()==(events.back()=="face"?1u:0u));
    check(t==outputIdentity);if(t->pImp.Info){check(returnedInfo.RefCount==1);--returnedInfo.RefCount;}
    events.push_back("destroy");t->pImp={};
}
tCreateCreature CreateCreature_API=reinterpret_cast<tCreateCreature>(&create);
tRemoveThing RemoveThing_API=reinterpret_cast<tRemoveThing>(&removeBound);
tEntitySetFacingAngleTowardsThing EntitySetFacingAngleTowardsThing_API=reinterpret_cast<tEntitySetFacingAngleTowardsThing>(&facing);
#include "scythe_thing_adapters.inc"
int main(){
    try{
        CScriptThingVTable table{};table.GetPos=reinterpret_cast<tCScriptThing_GetPos>(&pos);
        bound.pVTable=reinterpret_cast<void**>(&table);
        RetailThing_Destroy_API=reinterpret_cast<tRetailThingDestroy>(&destroyReturned);
        GetThingWithScriptName_ByName_API=reinterpret_cast<tGetThingWithScriptName1>(&lookup);
        LuaQuestState host;
        for(bool empty:{false,true})for(bool failure:{false,true})for(bool spawn:{false,true})for(bool snap:{false,true}){
            emptyOutput=empty;throwConsumer=failure;events.clear();
            snapValue=snap;lookupName=snap?"NOVI_Theresa":"MK_OW_SCYTHE3";
            bool threw=false;
            try{if(spawn)host.SpawnScytheAndRemoveMarker(&bound);else host.FaceThingByScriptName(&bound,lookupName,snapValue);}
            catch(const std::runtime_error&){threw=true;}
            check(threw==failure&&returnedInfo.RefCount==0&&strings.empty());
            check(events==(spawn?std::vector<std::string>{"position","create","remove","destroy"}:
                                 std::vector<std::string>{"lookup","face","destroy"}));
        }
        std::cout<<"PASS: native target/position/flags, output identity, release after consumer, empty output, consumer exception cleanup\n";
        return 0;
    }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}
}
