#define main resource_smoke_main
#include "runtime_checks/retail_resources_smoke.cpp"
#undef main
static CScriptThing actor{},hero{};
static std::vector<std::string> trace;
static std::string fault,activeName;
static bool hasChocolate=false;
static int amountExpected=0,counterValue=0;
static void event(const std::string& value){trace.push_back(value);if(value==fault)throw std::runtime_error("LIVE_FATHER_BODY_ERROR");}
static void __fastcall literal(CCharString* self,void*,const char* text,int length){stringCtor(self,nullptr,text,length);trace.push_back("new:"+std::string(text));}
static void __fastcall destroyText(CCharString* self,void*){trace.push_back("destroy:"+strings.at(self));stringDtor(self,nullptr);}
static void __fastcall damage(CGameScriptInterfaceBase*,void*,const CScriptThing* a,bool b){check(a==&actor&&!b);event("damage");}
static void __fastcall kill(CGameScriptInterfaceBase*,void*,const CScriptThing* a,bool b,bool c){check(a==&actor&&!b&&!c);event("kill");}
static void __fastcall combo(CGameScriptInterfaceBase*,void*,const CScriptThing* a,bool b){check(a==&actor&&!b);event("combo");}
static void __fastcall information(CGameScriptInterfaceBase*,void*,const CScriptThing* a,bool b,bool c,bool d){check(a==&actor&&!b&&c&&!d);event("information");}
static void __fastcall pushable(CGameScriptInterfaceBase*,void*,CScriptThing a,bool b){check(a.pVTable==g_pCScriptThingVTable&&a.pImp.Data==actor.pImp.Data&&a.pImp.Info==actor.pImp.Info&&!b);if(a.pImp.Info){check(a.pImp.Info->RefCount==8);--a.pImp.Info->RefCount;}event("pushable");}
static void __fastcall deeds(CGameScriptInterfaceBase*,void*,const CScriptThing* a,bool b){check(a==&actor&&!b);event("deeds");}
static void __fastcall give(CGameScriptInterfaceBase*,void*,int amount){check(amount==amountExpected);event("gold");}
static CScriptThing* __fastcall getHero(CGameScriptInterfaceBase*,void*){event("hero");return &hero;}
static bool __fastcall possession(CGameScriptInterfaceBase*,void*,const CCharString* text,const CScriptThing* a){check(strings.at(const_cast<CCharString*>(text))=="OBJECT_CHOCOLATE_BOX_UNGIVEABLE"&&a==&hero);event("possession");return hasChocolate;}
static CCharString* __fastcall active(CGameScriptInterfaceBase*,void*,CCharString* output){stringCtor(output,nullptr,activeName.c_str(),-1);trace.push_back("active:"+activeName);return output;}
static void __fastcall objective(CGameScriptInterfaceBase*,void*,const CCharString* name,const CCharString* key,const CCharString* region1,const CCharString* region2){check(strings.at(const_cast<CCharString*>(name))==activeName&&strings.at(const_cast<CCharString*>(key))=="OBJECTIVE"&&strings.at(const_cast<CCharString*>(region1)).empty()&&strings.at(const_cast<CCharString*>(region2)).empty());event("objective");}
static void __fastcall display(CGameScriptInterfaceBase*,void*,const CCharString* key){check(strings.at(const_cast<CCharString*>(key))=="INFO");event("info");}
static int __fastcall counter(CGameScriptInterfaceBase*,void*,const CCharString* key,int count,float scale){check(strings.at(const_cast<CCharString*>(key))=="HUD_DEED_GOOD_ICON"&&count==0&&scale==1.0f);event("counter");return counterValue;}
tEntitySetAsDamageable EntitySetAsDamageable_API=reinterpret_cast<tEntitySetAsDamageable>(&damage);
tEntitySetAsKillable EntitySetAsKillable_API=reinterpret_cast<tEntitySetAsKillable>(&kill);
tEntitySetAsToAddToComboMultiplierWhenHit EntitySetAsToAddToComboMultiplierWhenHit_API=reinterpret_cast<tEntitySetAsToAddToComboMultiplierWhenHit>(&combo);
tSetThingHasInformation SetThingHasInformation_API=reinterpret_cast<tSetThingHasInformation>(&information);
tSetIsPushableByHero SetIsPushableByHero_API=reinterpret_cast<tSetIsPushableByHero>(&pushable);
tEntitySetDeedReactionsEnabled EntitySetDeedReactionsEnabled_API=reinterpret_cast<tEntitySetDeedReactionsEnabled>(&deeds);
tGiveHeroGold GiveHeroGold_API=reinterpret_cast<tGiveHeroGold>(&give);
tGetHero GetHero_API=reinterpret_cast<tGetHero>(&getHero);
tIsObjectInThingsPossession IsObjectInThingsPossession_API=reinterpret_cast<tIsObjectInThingsPossession>(&possession);
tGetActiveQuestName GetActiveQuestName_API=reinterpret_cast<tGetActiveQuestName>(&active);
tSetQuestCardObjective SetQuestCardObjective_API=reinterpret_cast<tSetQuestCardObjective>(&objective);
tDisplayGameInfo DisplayGameInfo_API=reinterpret_cast<tDisplayGameInfo>(&display);
tAddQuestInfoCounter1 AddQuestInfoCounter_ByValue_API=reinterpret_cast<tAddQuestInfoCounter1>(&counter);
static bool run(const std::string& code){sol::state lua;lua.open_libraries(sol::lib::base);RegisterRetailResources(lua);lua["actor"]=&actor;lua["amount"]=amountExpected;lua["expected"]=counterValue;lua["chocolate"]=hasChocolate;lua["scope"]=[&](sol::protected_function body){WithRetailResources(reinterpret_cast<CGameScriptInterfaceBase*>(1),body);};return lua.safe_script(code,sol::script_pass_on_error).valid();}
int main(){try{
    CCharString_Construct_Literal=reinterpret_cast<decltype(CCharString_Construct_Literal)>(&literal);CCharString_Destroy=reinterpret_cast<decltype(CCharString_Destroy)>(&destroyText);
    std::remove_pointer_t<decltype(actor.pImp.Info)> info{};info.RefCount=7;
    unsigned cases=0;
    for(bool data:{false,true})for(bool counted:{false,true})for(const char* error:{"","pushable","deeds"}){
        actor.pImp.Data=data?reinterpret_cast<decltype(actor.pImp.Data)>(1):nullptr;actor.pImp.Info=counted?&info:nullptr;trace.clear();fault=error;
        check(run("scope(function(r) r:InitializeLiveFatherActor(actor) end)")==fault.empty());check(info.RefCount==7);++cases;
    }
    for(int value:{INT_MIN,-1,0,1,INT_MAX}){fault.clear();amountExpected=value;trace.clear();check(run("scope(function(r) r:GiveRawHeroGold(amount) end)"));check(trace==std::vector<std::string>{"gold"});++cases;}
    for(bool value:{false,true})for(const char* error:{"","hero","possession"}){hasChocolate=value;fault=error;trace.clear();check(run("scope(function(r) assert(r:LiveFatherHeroHasChocolate()==chocolate) end)")==fault.empty());check(strings.empty());check(trace.back()=="destroy:OBJECT_CHOCOLATE_BOX_UNGIVEABLE");++cases;}
    for(const char* name:{"","OtherActiveQuest"})for(const char* error:{"","objective"}){activeName=name;fault=error;trace.clear();check(run("scope(function(r) r:SetActiveQuestObjective('OBJECTIVE') end)")==fault.empty());check(strings.empty());check(std::vector<std::string>(trace.end()-4,trace.end())==std::vector<std::string>{"destroy:"+activeName,"destroy:OBJECTIVE","destroy:","destroy:"});++cases;}
    for(const char* error:{"","info"}){fault=error;trace.clear();check(run("scope(function(r) r:DisplayRawGameInfo('INFO') end)")==fault.empty());check(strings.empty()&&trace.back()=="destroy:INFO");++cases;}
    for(int value:{INT_MIN,-1,0,INT_MAX})for(bool error:{false,true}){counterValue=value;fault=error?"counter":"";trace.clear();check(run("scope(function(r) local key=r:NewLiteralText('HUD_DEED_GOOD_ICON'); assert(r:AddLiveFatherGoodDeedCounter(key)==expected); r:DestroyText(key) end)")==fault.empty());check(strings.empty()&&trace.back()=="destroy:HUD_DEED_GOOD_ICON");++cases;}
    check(cases==37);std::cout<<"LiveFather x86 actual-FSE/real-Lua: 37 Init/amount/possession/objective/info/counter policies passed\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
