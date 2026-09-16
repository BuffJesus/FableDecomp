#define main resource_smoke_main
#include "runtime_checks/retail_resources_smoke.cpp"
#undef main
static void guardCheck(bool value,int line){if(!value)throw std::runtime_error("Guard harness assertion line "+std::to_string(line));}
#define check(...) guardCheck((__VA_ARGS__),__LINE__)
DWORD g_fableBase=0x400000;
static CScriptThing actor{},heroActor{};
static CScriptThing* heroResult=nullptr;
static std::vector<std::string> trace;
static std::string fault;
static unsigned nullQueries=0,followCalls=0;
static void event(const std::string& text){trace.push_back(text);if(text==fault)throw std::runtime_error("GUARD_BODY_FAULT");}
static void __fastcall literalCtor(CCharString* self,void*,const char* text,int n){stringCtor(self,nullptr,text,n);if(!*text)self->pStringData=nullptr;event("text:"+std::string(text));}
static void __fastcall literalDestroy(CCharString* self,void*){trace.push_back("text.destroy");stringDtor(self,nullptr);}
static bool __fastcall isNull(CScriptThing*,void*){++nullQueries;return true;}
static C3DVector* __fastcall home(CScriptThing* self,void*,C3DVector* output){check(self==&actor);*output={7.5f,-3.25f,11.0f};event("home");return nullptr;}
static void __fastcall damage(CGameScriptInterfaceBase*,void*,const CScriptThing* self,bool flag){check(self==&actor && !flag);event("damage");}
static void __fastcall killable(CGameScriptInterfaceBase*,void*,const CScriptThing* self,bool a,bool b){check(self==&actor && !a && !b);event("kill");}
static void __fastcall combo(CGameScriptInterfaceBase*,void*,const CScriptThing* self,bool flag){check(self==&actor && !flag);event("combo");}
static void __fastcall sheathe(CGameScriptInterfaceBase*,void*,const CScriptThing* self,bool flag){check(self==&actor && !flag);event("sheathe");}
static void consume(CScriptThing value,const char* name){
    check(value.pVTable==g_pCScriptThingVTable && value.pImp.Data==actor.pImp.Data && value.pImp.Info==actor.pImp.Info);
    if(value.pImp.Info){check(value.pImp.Info->RefCount==8);--value.pImp.Info->RefCount;}
    event(name); // Faults model an error after the native callee consumed its argument.
}
static void __fastcall centre(CGameScriptInterfaceBase*,void*,CScriptThing value,C3DVector point){check(point.x==7.5f && point.y==-3.25f && point.z==11.0f);consume(value,"centre");}
static void __fastcall minimum(CGameScriptInterfaceBase*,void*,CScriptThing value,float range){check(range==0.0f);consume(value,"minimum");}
static void __fastcall maximum(CGameScriptInterfaceBase*,void*,CScriptThing value,float range){check(range==6.0f);consume(value,"maximum");}
static void __fastcall stateGroup(CGameScriptInterfaceBase*,void*,CScriptThing value,EScriptingStateGroups group){check(static_cast<int>(group)==4);consume(value,"group");}
static CScriptThing* __fastcall getHero(CGameScriptInterfaceBase*,void*){event("hero");return heroResult;}
static const CScriptThing* faceFirst=nullptr,*faceSecond=nullptr;
static bool faceSnap=false;
static void __fastcall face(CGameScriptInterfaceBase*,void*,const CScriptThing* first,const CScriptThing* second,bool snap){check(first==faceFirst && second==faceSecond && snap==faceSnap);event("face");}
static int expectedConversation=0;
static const CScriptThing* expectedPerson=nullptr;
static void __fastcall person(CGameScriptInterfaceBase*,void*,int conversation,const CScriptThing* value){check(conversation==expectedConversation && value==expectedPerson);event("person");}
static std::string expectedKey;
static void __fastcall line(CGameScriptInterfaceBase*,void*,int conversation,const CCharString* key,bool flag,const CScriptThing* speaker,const CScriptThing* listener){
    check(conversation==expectedConversation && strings.at(const_cast<CCharString*>(key))==expectedKey && !flag && speaker==&actor && listener==heroResult);event("line");
}
static int expectedBehavior=0;
static void __fastcall behavior(CGameScriptInterfaceBase*,void*,const CScriptThing* value,ECutsceneBehaviour setting){check(value==expectedPerson && static_cast<int>(setting)==expectedBehavior);event("behavior");}
static const CScriptThing* followTarget=nullptr;
static void __fastcall follow(CScriptGameResourceObjectScriptedThingBase* self,void*,const CScriptThing* target,float range,bool flag){check(self==expectedExpert && target==followTarget && range==1.0f && flag);++followCalls;event("follow");}
tEntitySetAsDamageable EntitySetAsDamageable_API=reinterpret_cast<tEntitySetAsDamageable>(&damage);
tEntitySetAsKillable EntitySetAsKillable_API=reinterpret_cast<tEntitySetAsKillable>(&killable);
tEntitySetAsToAddToComboMultiplierWhenHit EntitySetAsToAddToComboMultiplierWhenHit_API=reinterpret_cast<tEntitySetAsToAddToComboMultiplierWhenHit>(&combo);
tEntitySheatheWeapons EntitySheatheWeapons_API=reinterpret_cast<tEntitySheatheWeapons>(&sheathe);
tSetWanderCentrePoint SetWanderCentrePoint_API=reinterpret_cast<tSetWanderCentrePoint>(&centre);
tSetWanderMinDistance SetWanderMinDistance_API=reinterpret_cast<tSetWanderMinDistance>(&minimum);
tSetWanderMaxDistance SetWanderMaxDistance_API=reinterpret_cast<tSetWanderMaxDistance>(&maximum);
tSetScriptingStateGroup SetScriptingStateGroup_API=reinterpret_cast<tSetScriptingStateGroup>(&stateGroup);
tGetHero GetHero_API=reinterpret_cast<tGetHero>(&getHero);
tEntitySetFacingAngleTowardsThing EntitySetFacingAngleTowardsThing_API=reinterpret_cast<tEntitySetFacingAngleTowardsThing>(&face);
tAddPersonToConversation AddPersonToConversation_API=reinterpret_cast<tAddPersonToConversation>(&person);
tAddLineToConversation AddLineToConversation_API=reinterpret_cast<tAddLineToConversation>(&line);
tEntitySetCutsceneBehaviour EntitySetCutsceneBehaviour_API=reinterpret_cast<tEntitySetCutsceneBehaviour>(&behavior);
static void bind(sol::state& lua){lua.open_libraries(sol::lib::base);RegisterRetailResources(lua);lua["actor"]=&actor;lua["scope"]=[&](sol::protected_function body){WithRetailResources(reinterpret_cast<CGameScriptInterfaceBase*>(1),body);};}
int main(){try{
    CScriptThingVTable table{};table.GetHomePos=reinterpret_cast<decltype(table.GetHomePos)>(&home);table.IsNull=reinterpret_cast<decltype(table.IsNull)>(&isNull);
    actor.pVTable=reinterpret_cast<void**>(&table);
    std::remove_pointer_t<decltype(actor.pImp.Info)> info{};
    for(bool data:{false,true})for(bool counted:{false,true})for(const char* failure:{"","centre","minimum","maximum","group"}){
        info.RefCount=7;actor.pImp.Info=counted?&info:nullptr;actor.pImp.Data=data?reinterpret_cast<decltype(actor.pImp.Data)>(0x4242):nullptr;fault=failure;trace.clear();
        sol::state lua;bind(lua);auto result=lua.safe_script("scope(function(r) r:InitializeGuardActor(actor) end)",sol::script_pass_on_error);
        check(result.valid()==fault.empty() && info.RefCount==7 && !nullQueries && actor.pVTable==reinterpret_cast<void**>(&table));
        const std::vector<std::string> expected={"damage","kill","combo","home","centre","minimum","maximum","group","sheathe"};
        const auto count=fault.empty()?expected.size():static_cast<size_t>(std::find(expected.begin(),expected.end(),fault)-expected.begin()+1);
        check(trace==std::vector<std::string>(expected.begin(),expected.begin()+count));
        if(!result.valid()){sol::error e=result;check(std::string(e.what()).find("GUARD_BODY_FAULT")!=std::string::npos);}
    }
    CCharString_Construct_Literal=reinterpret_cast<tCCharString_Constructor_Literal>(&literalCtor);CCharString_Destroy=reinterpret_cast<tCCharString_Destructor>(&literalDestroy);
    for(bool hasHero:{false,true})for(const char* key:{"","TEXT_QST_048_GUARD_ON_TALK_GOOD"})for(const char* failure:{"","hero","line"}){
        heroResult=hasHero?&heroActor:nullptr;expectedConversation=-7;expectedKey=key;fault=failure;trace.clear();
        sol::state lua;bind(lua);lua["key"]=expectedKey;auto result=lua.safe_script("scope(function(r) r:GuardAddHeroConversationLine(-7,key,actor) end)",sol::script_pass_on_error);
        check(result.valid()==fault.empty() && strings.empty());
        std::vector<std::string> expected={"text:"+expectedKey,"hero"};if(fault!="hero")expected.push_back("line");expected.push_back("text.destroy");check(trace==expected);
    }
    fault.clear();
    for(bool hasHero:{false,true})for(bool snap:{false,true})for(bool reverse:{false,true}){
        heroResult=hasHero?&heroActor:nullptr;faceFirst=reverse?heroResult:&actor;faceSecond=reverse?&actor:heroResult;faceSnap=snap;trace.clear();
        sol::state lua;bind(lua);lua["snap"]=snap;lua["reverse"]=reverse;auto result=lua.safe_script("scope(function(r) r:GuardFaceHero(actor,snap,reverse) end)",sol::script_pass_on_error);check(result.valid() && trace==std::vector<std::string>{"hero","face"});
    }
    for(bool populated:{false,true})for(int id:{-7,0,29}){
        expectedConversation=id;expectedPerson=populated?&actor:nullptr;sol::state lua;bind(lua);lua["person"]=expectedPerson;lua["id"]=id;
        check(lua.safe_script("scope(function(r) r:AddRawConversationPerson(id,person) end)",sol::script_pass_on_error).valid());
    }
    for(bool populated:{false,true})for(int setting:{1,2}){
        expectedPerson=populated?&actor:nullptr;expectedBehavior=setting;sol::state lua;bind(lua);lua["person"]=expectedPerson;lua["setting"]=setting;
        check(lua.safe_script("scope(function(r) r:SetRawCutsceneBehaviour(person,setting) end)",sol::script_pass_on_error).valid());
    }
    CScriptGameResourceObjectScriptedThingBaseVTable expertTable{};expertTable.FollowThing=reinterpret_cast<decltype(expertTable.FollowThing)>(&follow);
    CScriptGameResourceObjectScriptedThingBase expert{};expert.pVTable=reinterpret_cast<void**>(&expertTable);expectedExpert=&expert;acquireExpert=&expert;
    for(bool populated:{false,true})for(bool target:{false,true}){
        acquireOK=populated;followTarget=target?&heroActor:nullptr;const auto before=followCalls;
        sol::state lua;bind(lua);lua["target"]=followTarget;
        check(lua.safe_script("scope(function(r) local c=r:NewResource(); r:TryAcquire(c,actor,4); r:FollowThing(c,target,1.0,true) end)",sol::script_pass_on_error).valid());
        check(followCalls==before+static_cast<unsigned>(populated) && locals.empty());
        for(const auto& reference:refs)check(reference.second==0);
    }
    acquireExpert=nullptr;
    void* arena=VirtualAlloc(nullptr,0x10000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);check(arena!=nullptr);
    g_fableBase=reinterpret_cast<DWORD>(arena)-(0x013A0000-0x400000);
    auto* range=ASLR<volatile float*>(0x013AC840);auto* alert=ASLR<volatile float*>(0x013AC844);
    for(float value:{2.5f,13.75f}){
        *range=value;alert[0]=value+1;alert[3]=value+9;sol::state lua;bind(lua);lua["value"]=value;
        check(lua.safe_script("scope(function(r) assert(r:ReadGuardLectureRange()==value); assert(r:ReadGuardAlertRange(-1)==value); assert(r:ReadGuardAlertRange(0)==value+1); assert(r:ReadGuardAlertRange(3)==value+9); assert(r:ReadGuardAlertRange(-2147483648)==value+1) end)",sol::script_pass_on_error).valid());
    }
    VirtualFree(arena,0,MEM_RELEASE);
    std::cout<<"Guard x86 actual-FSE/real-Lua: 20 Init copy, 12 line lifetime/error, 8 facing, 6 person, 4 cutscene, 4 retained follow and 2 live tuning policies passed\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
