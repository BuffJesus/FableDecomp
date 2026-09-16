#define main resource_smoke_main
#include "runtime_checks/retail_resources_smoke.cpp"
#undef main
static CCPPointerInfo outputInfo{};
static CScriptThing* outputIdentity;
static bool emptyOutput, consumerError;
static std::vector<std::string> events;
class LuaQuestState {
public:
    CGameScriptInterfaceBase* m_pGameInterface=reinterpret_cast<CGameScriptInterfaceBase*>(1);
    void AddMiniMapMarkerByScriptName(const std::string&,const std::string&);
};
static CScriptThing* __fastcall namedLookup(CGameScriptInterfaceBase*,void*,CScriptThing* out,const CCharString* key){
    check(strings.size()==2&&strings.at(const_cast<CCharString*>(key))=="EmptyGrave");
    events.push_back("lookup");outputIdentity=out;
    if(!emptyOutput){out->pImp.Data=reinterpret_cast<decltype(out->pImp.Data)>(&outputInfo);out->pImp.Info=&outputInfo;++outputInfo.RefCount;}
    return out;
}
static void __fastcall markerAdd(CGameScriptInterfaceBase*,void*,const CScriptThing* target,const CCharString* marker){
    check(target==outputIdentity&&strings.size()==2&&strings.at(const_cast<CCharString*>(marker))=="HUD_ORB_QUEST_VIGNETTE");
    check(outputInfo.RefCount==(emptyOutput?0:1));events.push_back("marker");
    if(consumerError)throw std::runtime_error("marker error");
}
static void __fastcall outputDestroy(CScriptThing* target,void*){
    check(target==outputIdentity&&strings.size()==2);events.push_back("thing.destroy");
    if(target->pImp.Info){check(outputInfo.RefCount==1);--outputInfo.RefCount;}target->pImp={};
}
static void __fastcall trackedStringDestroy(CCharString* value,void*){
    events.push_back("string.destroy:"+strings.at(value));stringDtor(value,nullptr);
}
tMiniMapAddMarker MiniMapAddMarker_API=reinterpret_cast<tMiniMapAddMarker>(&markerAdd);
#include "maze_marker_adapter.inc"
int main(){
    try{
        GetThingWithScriptName_ByName_API=reinterpret_cast<tGetThingWithScriptName1>(&namedLookup);
        RetailThing_Destroy_API=reinterpret_cast<tRetailThingDestroy>(&outputDestroy);
        CCharString_Destroy=reinterpret_cast<tCCharString_Destructor>(&trackedStringDestroy);
        LuaQuestState host;
        for(bool empty:{false,true})for(bool error:{false,true}){
            emptyOutput=empty;consumerError=error;events.clear();bool threw=false;
            try{host.AddMiniMapMarkerByScriptName("EmptyGrave","HUD_ORB_QUEST_VIGNETTE");}
            catch(const std::runtime_error&){threw=true;}
            check(threw==error&&outputInfo.RefCount==0&&strings.empty());
            check(events==std::vector<std::string>{"lookup","marker","thing.destroy","string.destroy:EmptyGrave","string.destroy:HUD_ORB_QUEST_VIGNETTE"});
        }
        std::cout<<"PASS: named marker exact output, populated/empty result, Thing/key/marker destruction order, consumer-error cleanup\n";return 0;
    }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}
}
