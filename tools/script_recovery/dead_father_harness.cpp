#define main resource_smoke_main
#include "runtime_checks/retail_resources_smoke.cpp"
#undef main
#include <cmath>
#include <fstream>
#include <iterator>
DWORD g_fableBase=0x400000;
static CScriptThing actor{};
static CScriptThingVTable markerTable{};
static CScriptGameResourceObjectScriptedThingBase expert{};
static std::map<CScriptThing*,int> markerIds;
static std::vector<std::string> trace;
static std::string fault;
static bool markerPop[2]{};
static int lookups=0,loopCalls=0;
static unsigned char expectedRaw=0;
static float markerAngle=0;
static std::string candidateSource;
static int cancelAt=6;
static void event(const std::string& text){trace.push_back(text);if(text==fault)throw std::runtime_error("DEAD_FATHER_BODY_ERROR");}
static void __fastcall literal(CCharString* self,void*,const char* key,int n){stringCtor(self,nullptr,key,n);trace.push_back("text.new:"+std::string(key));if(std::string(key)=="CS_DEAD_DAD")*ASLR<volatile unsigned char*>(0x1375748)=expectedRaw;}
static void __fastcall textDestroy(CCharString* self,void*){trace.push_back("text.destroy:"+strings.at(self));stringDtor(self,nullptr);}
static void __fastcall addMarker(CGameScriptInterfaceBase*,void*,const CScriptThing* a,const CCharString* key){check(a==&actor&&strings.at(const_cast<CCharString*>(key))=="HUD_ORB_QUEST_CORE");event("marker.add");}
static void __fastcall removeMarker(CGameScriptInterfaceBase*,void*,const CScriptThing* a){check(a==&actor);event("marker.remove");}
static void __fastcall pushable(CGameScriptInterfaceBase*,void*,CScriptThing copy,bool flag){check(!flag&&copy.pVTable==g_pCScriptThingVTable&&copy.pImp.Data==actor.pImp.Data&&copy.pImp.Info==actor.pImp.Info);if(copy.pImp.Info){check(copy.pImp.Info->RefCount==8);--copy.pImp.Info->RefCount;}event("pushable");}
static CScriptThing* __fastcall lookup(CGameScriptInterfaceBase*,void*,CScriptThing* output,const CCharString* key){check(strings.at(const_cast<CCharString*>(key))=="MK_OVID_DAD"&&lookups<2);++lookups;output->pVTable=reinterpret_cast<void**>(&markerTable);output->pImp.Data=markerPop[lookups-1]?reinterpret_cast<decltype(output->pImp.Data)>(1):nullptr;output->pImp.Info=nullptr;markerIds[output]=lookups;trace.push_back("lookup"+std::to_string(lookups));return output;}
static void __fastcall thingDestroy(CScriptThing* output,void*){check(markerIds.count(output)==1);trace.push_back("thing.destroy"+std::to_string(markerIds.at(output)));markerIds.erase(output);}
static void __fastcall teleport(CGameScriptInterfaceBase*,void*,const CScriptThing* a,const CScriptThing* target,bool flag){check(a==&actor&&!flag&&markerIds.at(const_cast<CScriptThing*>(target))==1);event("teleport");}
static float __fastcall angle(CScriptThing* self,void*){check(markerIds.at(self)==2);event("angle");return markerAngle;}
static void __fastcall face(CGameScriptInterfaceBase*,void*,const CScriptThing* a,float value,bool flag){check(a==&actor&&flag&&(value==markerAngle||(std::isnan(value)&&std::isnan(markerAngle))));event("face");}
static void __fastcall loop(void* self,void*,const CCharString* key,int count,bool a,bool b,bool c,bool d,unsigned char raw,bool f,bool g){check(self==&expert&&strings.at(const_cast<CCharString*>(key))=="CS_DEAD_DAD"&&count==-1&&!a&&b&&!c&&d&&raw==expectedRaw&&!f&&!g);++loopCalls;event("loop");}
tMiniMapAddMarker MiniMapAddMarker_API=reinterpret_cast<tMiniMapAddMarker>(&addMarker);
tMiniMapRemoveMarker MiniMapRemoveMarker_API=reinterpret_cast<tMiniMapRemoveMarker>(&removeMarker);
tSetIsPushableByHero SetIsPushableByHero_API=reinterpret_cast<tSetIsPushableByHero>(&pushable);
tEntityTeleportToThing EntityTeleportToThing_API=reinterpret_cast<tEntityTeleportToThing>(&teleport);
tEntitySetFacingAngle EntitySetFacingAngle_API=reinterpret_cast<tEntitySetFacingAngle>(&face);
static bool run(const std::string& code){sol::state lua;lua.open_libraries(sol::lib::base);RegisterRetailResources(lua);lua["actor"]=&actor;lua["cancelAt"]=cancelAt;lua["scope"]=[&](sol::protected_function body){WithRetailResources(reinterpret_cast<CGameScriptInterfaceBase*>(1),body);};check(lua.safe_script(candidateSource,sol::script_pass_on_error).valid());return lua.safe_script(code,sol::script_pass_on_error).valid();}
int main(){try{
    std::ifstream candidate("../CANDIDATE.lua");check(candidate.good());candidateSource=std::string(std::istreambuf_iterator<char>(candidate),std::istreambuf_iterator<char>());
    CCharString_Construct_Literal=reinterpret_cast<decltype(CCharString_Construct_Literal)>(&literal);CCharString_Destroy=reinterpret_cast<decltype(CCharString_Destroy)>(&textDestroy);GetThingWithScriptName_ByName_API=reinterpret_cast<tGetThingWithScriptName1>(&lookup);RetailThing_Destroy_API=reinterpret_cast<tRetailThingDestroy>(&thingDestroy);
    markerTable.GetAngleXY=reinterpret_cast<decltype(markerTable.GetAngleXY)>(&angle);
    std::remove_pointer_t<decltype(actor.pImp.Info)> info{};info.RefCount=7;unsigned cases=0;
    for(bool data:{false,true})for(bool counted:{false,true})for(const char* error:{"","marker.add","pushable"}){
        actor.pImp.Data=data?reinterpret_cast<decltype(actor.pImp.Data)>(1):nullptr;actor.pImp.Info=counted?&info:nullptr;fault=error;trace.clear();
        check(run("scope(function(r) r:InitializeDeadFatherActor(actor) end)")==fault.empty());check(info.RefCount==7&&strings.empty());
        check(trace[2]=="text.destroy:HUD_ORB_QUEST_CORE");if(fault!="marker.add")check(trace.back()=="pushable");++cases;
    }
    for(bool first:{false,true})for(bool second:{false,true})for(float value:{-2.25f,std::numeric_limits<float>::quiet_NaN()})for(const char* error:{"","teleport","angle","face"}){
        markerPop[0]=first;markerPop[1]=second;markerAngle=value;fault=error;lookups=0;trace.clear();
        check(run("scope(function(r) r:PlaceDeadFatherAtMarker(actor) end)")==fault.empty());check(strings.empty()&&markerIds.empty());
        const int last=fault=="teleport"?1:2;check(lookups==last);check(trace[trace.size()-2]=="thing.destroy"+std::to_string(last));check(trace.back()=="text.destroy:MK_OVID_DAD");++cases;
    }
    void* arena=VirtualAlloc(nullptr,0x10000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);check(arena!=nullptr);g_fableBase=reinterpret_cast<DWORD>(arena)-(0x01370000-0x400000);
    CScriptGameResourceObjectScriptedThingBaseVTable expertTable{};static_assert(offsetof(CScriptGameResourceObjectScriptedThingBaseVTable,PlayLoopingAnimation)==0x50);
    expertTable.PlayLoopingAnimation=reinterpret_cast<decltype(expertTable.PlayLoopingAnimation)>(&loop);expert.pVTable=reinterpret_cast<void**>(&expertTable);acquireExpert=&expert;
    for(bool populated:{false,true})for(unsigned raw:{0u,1u,2u,255u})for(bool error:{false,true}){
        acquireOK=populated;expectedRaw=static_cast<unsigned char>(raw);*ASLR<volatile unsigned char*>(0x1375748)=0x55;fault=error?"loop":"";trace.clear();const int before=loopCalls;
        check(run("scope(function(r) local c=r:NewResource(); r:TryAcquire(c,actor,4); r:PlayDeadFatherPose(c) end)")==(!error||!populated));
        check(loopCalls==before+static_cast<int>(populated)&&strings.empty()&&locals.empty());check(trace.front()=="text.new:CS_DEAD_DAD"&&trace.back()=="text.destroy:CS_DEAD_DAD");++cases;
    }
    fault.clear();trace.clear();check(run("scope(function(r) r:RemoveDeadFatherMarker(actor) end)"));check(trace==std::vector<std::string>{"marker.remove"});++cases;
    for(int stop:{1,2,3,6,9})for(bool populated:{false,true})for(const char* error:{"","teleport","loop"}){
        cancelAt=stop;acquireOK=populated;fault=error;lookups=0;expectedRaw=255;markerPop[0]=false;markerPop[1]=true;markerAngle=0.75f;trace.clear();
        const bool result=run(R"lua(
            local checks, conditions = 0, 0
            local q = {}
            function q:RegisterBoundAliveCondition(a) assert(a==actor); conditions=conditions+1 end
            function q:NewScriptFrame() end
            function q:IsActiveThreadTerminating() checks=checks+1; return checks>=cancelAt end
            function q:GetStateBool(key) assert(key=='DadFound'); return checks>=4 end
            function q:WithRetailResources(body) scope(body) end
            DeadFatherMain(q,actor)
            assert(conditions==1)
        )lua");
        check(result==!(populated&&stop>2&&!fault.empty()));check(strings.empty()&&markerIds.empty()&&locals.empty());for(const auto& ref:refs)check(ref.second==0);++cases;
    }
    acquireExpert=nullptr;VirtualFree(arena,0,MEM_RELEASE);
    check(cases==91);std::cout<<"DeadFather x86 actual-FSE/real-Lua: 91 policies passed (61 capabilities + 30 emitted Main scenarios)\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
