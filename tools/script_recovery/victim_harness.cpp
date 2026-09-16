#define main resource_smoke_main
#include "runtime_checks/retail_resources_smoke.cpp"
#undef main
static CScriptThing actor{},hero{};
static std::vector<std::string> trace;
static std::string fault;
static bool isInit=false,expectedFlag=false,populated=false;
static CScriptThing* retained=nullptr;
static int conversationExpected=0,lineCalls=0;
static void event(const std::string& value){trace.push_back(value);if(value==fault)throw std::runtime_error("VICTIM_BODY_ERROR");}
static void __fastcall damage(CGameScriptInterfaceBase*,void*,const CScriptThing* a,bool b){check(a==&actor&&!b);event("damage");}
static void __fastcall kill(CGameScriptInterfaceBase*,void*,const CScriptThing* a,bool b,bool c){check(a==&actor&&!b&&!c);event("kill");}
static void __fastcall combo(CGameScriptInterfaceBase*,void*,const CScriptThing* a,bool b){check(a==&actor&&!b);event("combo");}
static void __fastcall information(CGameScriptInterfaceBase*,void*,const CScriptThing* a,bool b,bool c,bool d){check(a==&actor&&!b&&!c&&!d);event("information");}
static void __fastcall pushable(CGameScriptInterfaceBase*,void*,CScriptThing a,bool b){check(a.pVTable==g_pCScriptThingVTable&&a.pImp.Data==actor.pImp.Data&&a.pImp.Info==actor.pImp.Info&&b==!isInit);if(a.pImp.Info){check(a.pImp.Info->RefCount==8);--a.pImp.Info->RefCount;}event("pushable");}
static void __fastcall movement(CGameScriptInterfaceBase*,void*,const CScriptThing* a,bool b){check(a==&actor&&b==!isInit);event("movement");}
static void __fastcall scared(CGameScriptInterfaceBase*,void*,const CScriptThing* a,bool b){check(a==&actor&&b==expectedFlag);event("scared");}
static void __fastcall clear(CGameScriptInterfaceBase*,void*,const CScriptThing* a){check(a==&actor);event("clear");}
static CScriptThing* __fastcall getHero(CGameScriptInterfaceBase*,void*){event("hero");return populated?&hero:nullptr;}
static CScriptThing* __fastcall getThing(CGameScriptInterfaceBase*,void*,CScriptThing* output,const CCharString* key){check(strings.at(const_cast<CCharString*>(key))=="NOVI_Bully");output->pVTable=g_pCScriptThingVTable;output->pImp.Data=populated?reinterpret_cast<decltype(output->pImp.Data)>(1):nullptr;output->pImp.Info=nullptr;retained=output;event("getBully");return output;}
static void __fastcall destroyThing(CScriptThing* value,void*){check(value==retained);retained=nullptr;trace.push_back("thing.destroy");}
static void __fastcall face(CGameScriptInterfaceBase*,void*,const CScriptThing* a,const CScriptThing* b,bool snap){check(a==&actor&&snap==expectedFlag);check(b==(retained?retained:(populated?&hero:nullptr)));event("face");}
static void __fastcall repeatLine(CGameScriptInterfaceBase*,void*,int conversation,const CCharString* key,bool flag,const CScriptThing* speaker,const CScriptThing* listener){
    ++lineCalls;check(conversation==conversationExpected&&!flag&&speaker==&actor&&listener==retained);
    check(strings.at(const_cast<CCharString*>(key))==(lineCalls==1?"TEXT_QST_048_VICTIM_EVIL_BROS":"TEXT_QST_048_BULLY_HERO_ATTACKS_VICTIM"));event("line"+std::to_string(lineCalls));
}
static void __fastcall tracedTextDestroy(CCharString* text,void*){trace.push_back("text.destroy:"+strings.at(text));stringDtor(text,nullptr);}
tEntitySetAsDamageable EntitySetAsDamageable_API=reinterpret_cast<tEntitySetAsDamageable>(&damage);
tEntitySetAsKillable EntitySetAsKillable_API=reinterpret_cast<tEntitySetAsKillable>(&kill);
tEntitySetAsToAddToComboMultiplierWhenHit EntitySetAsToAddToComboMultiplierWhenHit_API=reinterpret_cast<tEntitySetAsToAddToComboMultiplierWhenHit>(&combo);
tSetThingHasInformation SetThingHasInformation_API=reinterpret_cast<tSetThingHasInformation>(&information);
tSetIsPushableByHero SetIsPushableByHero_API=reinterpret_cast<tSetIsPushableByHero>(&pushable);
tEntitySetAsUseMovementInActions EntitySetAsUseMovementInActions_API=reinterpret_cast<tEntitySetAsUseMovementInActions>(&movement);
tEntitySetAsScared EntitySetAsScared_API=reinterpret_cast<tEntitySetAsScared>(&scared);
tClearThingHasInformation ClearThingHasInformation_API=reinterpret_cast<tClearThingHasInformation>(&clear);
tGetHero GetHero_API=reinterpret_cast<tGetHero>(&getHero);
tEntitySetFacingAngleTowardsThing EntitySetFacingAngleTowardsThing_API=reinterpret_cast<tEntitySetFacingAngleTowardsThing>(&face);
tAddLineToConversation AddLineToConversation_API=reinterpret_cast<tAddLineToConversation>(&repeatLine);
static bool run(const std::string& code){sol::state lua;lua.open_libraries(sol::lib::base);RegisterRetailResources(lua);lua["actor"]=&actor;lua["flag"]=expectedFlag;lua["conversation"]=conversationExpected;lua["scope"]=[&](sol::protected_function body){WithRetailResources(reinterpret_cast<CGameScriptInterfaceBase*>(1),body);};return lua.safe_script(code,sol::script_pass_on_error).valid();}
int main(){try{
    GetThingWithScriptName_ByName_API=reinterpret_cast<tGetThingWithScriptName1>(&getThing);RetailThing_Destroy_API=reinterpret_cast<tRetailThingDestroy>(&destroyThing);
    std::remove_pointer_t<decltype(actor.pImp.Info)> info{};info.RefCount=7;unsigned cases=0;
    for(bool init:{false,true})for(bool data:{false,true})for(bool counted:{false,true})for(const char* error:{"","pushable","movement"}){
        isInit=init;expectedFlag=true;actor.pImp.Data=data?reinterpret_cast<decltype(actor.pImp.Data)>(1):nullptr;actor.pImp.Info=counted?&info:nullptr;trace.clear();fault=error;
        check(run(init?"scope(function(r) r:InitializeVictimActor(actor) end)":"scope(function(r) r:SetVictimReleasedState(actor) end)")==fault.empty());check(info.RefCount==7);
        if(fault.empty())check(trace== (init?std::vector<std::string>{"damage","kill","combo","information","pushable","movement","scared"}:std::vector<std::string>{"pushable","movement","clear"}));++cases;
    }
    for(bool data:{false,true})for(bool snap:{false,true})for(bool error:{false,true}){
        populated=data;expectedFlag=snap;fault=error?"face":"";trace.clear();check(run("scope(function(r) local b=r:NewThingFromScriptName('NOVI_Bully'); r:FaceTowardsRetainedThing(actor,b,flag); r:DestroyThing(b) end)")==fault.empty());check(!retained&&strings.empty()&&trace==std::vector<std::string>{"getBully","face","thing.destroy"});++cases;
        trace.clear();check(run("scope(function(r) r:VictimFaceHero(actor,flag) end)")==fault.empty());check(trace==std::vector<std::string>{"hero","face"});++cases;
    }
    for(bool flag:{false,true})for(bool error:{false,true}){expectedFlag=flag;fault=error?"scared":"";trace.clear();check(run("scope(function(r) r:SetRawScared(actor,flag) end)")==fault.empty());check(trace==std::vector<std::string>{"scared"});++cases;}
    CCharString_Destroy=reinterpret_cast<decltype(CCharString_Destroy)>(&tracedTextDestroy);
    for(int id:{-1,7})for(bool data:{false,true})for(const char* error:{"","line1","line2"}){
        conversationExpected=id;populated=data;fault=error;lineCalls=0;trace.clear();
        check(run("scope(function(r) local b=r:NewThingFromScriptName('NOVI_Bully'); r:AddVictimRepeatConversationLines(conversation,actor,b); r:DestroyThing(b) end)")==fault.empty());
        check(!retained&&strings.empty()&&trace.back()=="thing.destroy");check(lineCalls==(fault=="line1"?1:2));
        check(trace[trace.size()-2]==(fault=="line1"?"text.destroy:TEXT_QST_048_VICTIM_EVIL_BROS":"text.destroy:TEXT_QST_048_BULLY_HERO_ATTACKS_VICTIM"));++cases;
    }
    check(cases==56);std::cout<<"Victim x86 actual-FSE/real-Lua: 56 Init/release/copy/facing/scared/repeat-line policies passed\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
