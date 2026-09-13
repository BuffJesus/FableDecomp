// Actual Guardian reference/adapter and RetailResources; engine/Quest are doubles.
#define RETAIL_THING_CONDITION_NO_MAIN
#include "retail_thing_condition.cpp"
#include <sstream>

static std::vector<std::string> events;
static std::map<void*,std::string> resourceNames;
static std::map<void*,std::string> thingKinds;
static std::vector<std::unique_ptr<CScriptGameResourceObjectScriptedThingBase>> experts;
static CScriptGameResourceObjectScriptedThingBaseVTable expertTable{};
static CScriptThing heroThing{};
static sol::table acquisitionValues,healthValues,busyValues;
static unsigned acquisitionIndex=0,healthIndex=0,busyIndex=0,macroIndex=0,cancelMacro=0;
static bool cancelled=false;
static bool watcher=false;
static sol::table aliveValues;
static unsigned aliveIndex=0;
static sol::function frameCallback;
static std::string currentMapKey;
static std::string boolean(bool v){return v?"true":"false";}
static std::string number(double v){std::ostringstream s;s<<v;return s.str();}
static void event(std::initializer_list<std::string> values){
    std::string s;for(auto& v:values){if(!s.empty())s+='|';s+=v;}events.push_back(s);
}
static void record(sol::variadic_args args){
    std::string s;for(auto arg:args){
        if(!s.empty())s+='|';
        if(arg.get_type()==sol::type::boolean)s+=boolean(arg.as<bool>());
        else if(arg.get_type()==sol::type::number)s+=number(arg.as<double>());
        else s+=arg.as<std::string>();
    }events.push_back(s);
}
static bool __fastcall prepareEvent(CScriptGameResourceObjectScriptedThingBase* r,void*){
    if(!resourceNames.count(r))resourceNames[r]=resourceNames.empty()?"maze":"hero";
    event({"prepare",resourceNames.at(r),boolean(r->pImp.Data!=nullptr)});return r->pImp.Data!=nullptr;
}
static void __fastcall resetEvent(CScriptGameResourceObjectScriptedThingBase* r,void*){
    event({"reset",resourceNames.at(r)});release(r,nullptr);r->pImp={};
}
static bool __fastcall acquireEvent(CGameScriptInterfaceBase* game,void*,const CScriptThing* actor,
    CScriptGameResourceObjectScriptedThingBase* r,EScriptAIPriority priority){
    auto name=resourceNames.at(r);check(actor==(name=="maze"?&objectThing:&heroThing));
    ++acquisitionIndex;check(acquisitionValues[acquisitionIndex].valid());
    acquireOK=acquisitionValues[acquisitionIndex];
    if(acquireOK){
        experts.emplace_back(std::make_unique<CScriptGameResourceObjectScriptedThingBase>());
        experts.back()->pVTable=reinterpret_cast<void**>(&expertTable);acquireExpert=experts.back().get();
    }
    auto result=acquire(game,nullptr,actor,r,priority);acquireExpert=nullptr;
    event({"acquire",name,boolean(result),std::to_string(static_cast<int>(priority))});return result;
}
static void __fastcall releaseEvent(void* r,void*){event({"release",resourceNames.at(r)});release(r,nullptr);}
static void __fastcall frameEvent(CGameScriptInterfaceBase*,void*){event({"frame"});if(watcher)frameCallback();}
static bool __fastcall aliveEvent(CScriptThing* t,void*){
    // Native CScriptThing::IsAlive returns false without expert dispatch on null.
    if(!t->pImp.Data)return false;
    check(watcher && aliveValues[++aliveIndex].valid());bool value=aliveValues[aliveIndex];
    event({value?"alive_1":"alive_0"});return value;
}
static void __fastcall mapStartEvent(void* m,void*){event({"map_start"});mapCtor(m,nullptr);}
static void __fastcall mapEndEvent(void* m,void*){event({"map_end"});mapDtor(m,nullptr);}
static CScriptGameResourceObjectScriptedThingBase* __fastcall bracketEvent(void* m,void*,const CCharString* key){
    currentMapKey=strings.at(const_cast<CCharString*>(key));return bracket(m,nullptr,key);
}
static void* __fastcall assignEvent(void* to,void*,const void* from){
    event({"map_actor",currentMapKey,resourceNames.at(const_cast<void*>(from))});return assign(to,nullptr,from);
}
static void __fastcall movieStartEvent(CGameScriptInterfaceBase* game,void*,const CCharString* key,CScriptGameResourceObjectMovieBase* movie){
    event({"movie_start",strings.at(const_cast<CCharString*>(key))});movieStart(game,nullptr,key,movie);
}
static void __fastcall movieEndEvent(CScriptGameResourceObjectMovieBase* movie,void*){event({"movie_end"});movieDestroy(movie,nullptr);}
static void __fastcall pauseEvent(CGameScriptInterfaceBase* game,void*,bool value){event({"pause",boolean(value)});pause(game,nullptr,value);}
static void __fastcall macroEvent(const CCharString* key,void* m,void* flags,void* input,bool setup,bool skip){
    check(!flags&&!input&&!setup&&maps.at(m).size()==2&&movies==1&&paused);
    event({"macro",strings.at(const_cast<CCharString*>(key)),boolean(skip)});
    if(++macroIndex==cancelMacro)cancelled=true;
}
static CScriptThing* __fastcall getThingEvent(CScriptGameResourceObjectScriptedThingBase* expert,void*,CScriptThing* out){
    check(expert!=nullptr);event({"resource_thing"});thingKinds[out]="release_thing";return thingCopy(out,nullptr,&objectThing);
}
static CScriptThing* __fastcall lookupEvent(CGameScriptInterfaceBase*,void*,CScriptThing* out,const CCharString* key){
    check(strings.at(const_cast<CCharString*>(key))==(watcher?"GTDI_Maze":"WitchwoodTeleporter"));
    if(watcher)event({"lookup"});else event({"lookup","WitchwoodTeleporter"});
    thingKinds[out]="release_lookup";
    if(watcher&&lookupMissing){out->pVTable=g_pCScriptThingVTable;out->pImp={};return out;}
    return thingCopy(out,nullptr,&objectThing);
}
static void __fastcall destroyThingEvent(CScriptThing* t,void*){
    if(thingKinds.count(t)){event({thingKinds.at(t)});thingKinds.erase(t);}thingDestroy(t,nullptr);
}
static float __fastcall healthEvent(CGameScriptInterfaceBase*,void*,const CScriptThing* t){
    check(t->pImp.Data==objectThing.pImp.Data);
    auto index=std::min(++healthIndex,static_cast<unsigned>(healthValues.size()));float value=healthValues[index];
    event({"health",number(value)});return value;
}
static bool __fastcall busyEvent(CScriptGameResourceObjectScriptedThingBase* expert,void*){
    check(expert!=nullptr);auto index=std::min(++busyIndex,static_cast<unsigned>(busyValues.size()));bool value=busyValues[index];
    event({"busy",boolean(value)});return value;
}
static void __fastcall speakEvent(CScriptGameResourceObjectScriptedThingBase* expert,void*,const CScriptThing* target,
    const char* key,ETextGroupSelectionMethod selection,bool listen,bool sound2D,bool fade){
    check(expert&&target==&heroThing);event({"speak",key,std::to_string(static_cast<int>(selection)),boolean(listen),boolean(sound2D),boolean(fade)});
}
static void __fastcall teleporterEvent(CGameScriptInterfaceBase*,void*,const CScriptThing* t,bool active){
    check(t->pImp.Data==objectThing.pImp.Data);event({"teleporter",boolean(active)});
}
static void __fastcall conditionEvent(void* entity,void*,const void* condition){setCondition(entity,nullptr,condition);event({"register_alive_condition"});}

int main(int argc,char** argv){
    try{
        check(argc==2);
        CScriptThingVTable thingTable{};thingTable.IsAlive=reinterpret_cast<decltype(thingTable.IsAlive)>(&aliveEvent);
        g_pCScriptThingVTable=reinterpret_cast<void**>(&thingTable);g_pRetailAliveConditionVTable=reinterpret_cast<void**>(0x1234);
        objectThing.pVTable=g_pCScriptThingVTable;objectThing.pImp.Data=reinterpret_cast<CScriptGameResourceObjectScriptedThingBase*>(0x4567);
        objectThing.pImp.Info=&objectInfo;objectInfo.RefCount=1;
        heroThing.pVTable=g_pCScriptThingVTable;heroThing.pImp.Data=reinterpret_cast<CScriptGameResourceObjectScriptedThingBase*>(0x5678);
        RetailThing_Copy_API=reinterpret_cast<tRetailThingCopy>(&thingCopy);RetailThing_Destroy_API=reinterpret_cast<tRetailThingDestroy>(&destroyThingEvent);
        RetailEntity_SetCondition_API=reinterpret_cast<tRetailEntityCondition>(&conditionEvent);
        InitScriptObjectHelper1_API=reinterpret_cast<tInitScriptObjectHelper1>(&prepareEvent);
        InitScriptObjectHelper2_API=reinterpret_cast<tInitScriptObjectHelper2>(&resetEvent);
        StartScriptingEntity_API=reinterpret_cast<tStartScriptingEntity>(&acquireEvent);
        CSGROSTB_Destroy_API=reinterpret_cast<tCSGROSTB_Destructor>(&releaseEvent);
        NewScriptFrame_API=reinterpret_cast<tNewScriptFrame>(&frameEvent);
        StdMap_Construct_API=reinterpret_cast<tStdMap_Constructor>(&mapStartEvent);StdMap_Destroy_API=reinterpret_cast<tStdMap_Destructor>(&mapEndEvent);
        StdMap_OperatorBracket_API=reinterpret_cast<tStdMap_OperatorBracket>(&bracketEvent);CBaseObject_Assign_API=reinterpret_cast<tCBaseObject_AssignmentOperator>(&assignEvent);
        StartMovieSequence_API=reinterpret_cast<tStartMovieSequence>(&movieStartEvent);MovieResource_Destroy_API=reinterpret_cast<tMovieResource_Destructor>(&movieEndEvent);
        PauseAllNonScriptedEntities_API=reinterpret_cast<tPauseAllNonScriptedEntities>(&pauseEvent);RunCutsceneMacro_Func=&macroEvent;
        GetHealth_API=reinterpret_cast<tGetHealth>(&healthEvent);GetThingWithScriptName_ByName_API=reinterpret_cast<tGetThingWithScriptName1>(&lookupEvent);
        SetTeleporterAsActive_API=reinterpret_cast<tSetTeleporterAsActive>(&teleporterEvent);
        expertTable.GetScriptThing=reinterpret_cast<decltype(expertTable.GetScriptThing)>(&getThingEvent);
        expertTable.Speak=reinterpret_cast<decltype(expertTable.Speak)>(&speakEvent);
        expertTable.IsPerformingScriptTask=reinterpret_cast<decltype(expertTable.IsPerformingScriptTask)>(&busyEvent);
        sol::state lua;
        // Harness callbacks/tables must release their Lua references before VM teardown.
        struct ClearLuaReferences { ~ClearLuaReferences(){
            acquisitionValues=sol::table{};healthValues=sol::table{};busyValues=sol::table{};
            aliveValues=sol::table{};frameCallback=sol::function{};
        }} clearLuaReferences;
        lua.open_libraries(sol::lib::base,sol::lib::package,sol::lib::math,sol::lib::table,sol::lib::string);
        auto meType=lua.new_usertype<CScriptThing>("CScriptThing",sol::no_constructor);
        RegisterRetailResources(lua);lua["me"]=&objectThing;lua["hero"]=&heroThing;lua["record"]=&record;
        lua["with_resources"] = [](sol::protected_function callback){WithRetailResources(reinterpret_cast<CGameScriptInterfaceBase*>(1),callback);};
        lua["native_cancelled"]=[](){return cancelled;};
        lua["set_frame_callback"]=[](sol::function f){frameCallback=f;};
        FakeHost host;host.m_Me=objectThing;auto* pEntityHost=&host;expectedEntity=&host;
        void* task=reinterpret_cast<void*>(0x7890);std::memcpy(host.parent+0x2c,&task,sizeof(task));
        auto questState_type=lua.new_usertype<LuaQuestState>("NativeQuest",sol::no_constructor);
        #include "alive-registration.inc"
        LuaQuestState nativeQuest;lua["nativeQuest"]=&nativeQuest;
        lua["bind_actor_methods"]=[&meType](sol::table methods){for(auto pair:methods)meType[pair.first.as<std::string>()]=pair.second;};
        lua["reset_native"]=[&](sol::table c){
            events.clear();resourceNames.clear();experts.clear();refs.clear();
            acquisitionValues=c["acquisitions"];healthValues=c["health"];busyValues=c["busy"];
            acquisitionIndex=healthIndex=busyIndex=macroIndex=0;cancelMacro=c["cancel_macro"];cancelled=false;
            watcher=c.get_or("watcher",false);lookupMissing=c.get_or("missing",false);aliveIndex=0;
            if(watcher)aliveValues=c["alive"];
            check(locals.empty()&&maps.empty()&&thingKinds.empty()&&objectInfo.RefCount==1);
        };
        lua["verify_native"]=[&](const std::string& name,sol::table expected){
            check(events.size()==expected.size());
            for(unsigned i=0;i<events.size();++i){std::string wanted=expected[i+1];
                if(events[i]!=wanted)throw std::runtime_error(name+" event "+std::to_string(i)+": "+events[i]+" != "+wanted);}
            check(locals.empty()&&maps.empty()&&!movies&&!paused&&thingKinds.empty());
            for(auto& p:refs)check(p.second==0);
            if(conditionClone.pImp.Info)thingDestroy(&conditionClone,nullptr);
            check(objectInfo.RefCount==1);
            std::cout<<"PASS "<<name<<" "<<events.size()<<" events\n";
        };
        lua.script_file(argv[1]);return 0;
    }catch(const std::exception& error){std::cerr<<error.what()<<"\n";return 1;}
}
