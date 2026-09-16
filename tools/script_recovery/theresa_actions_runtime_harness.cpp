#include "LuaRetailResources.h"
#include <iostream>
#include "retail_theresa_guard_vector.h"
#include <vector>
#include <string>
DWORD g_fableBase=0;
static CGameScriptInterfaceBase game{};
static CScriptThing actor{},hero{};
static CScriptGameResourceObjectScriptedThingBase expert{};
static std::vector<CCharString*> live;
static std::vector<std::string> events;
static bool emptyString=false,nullHero=false,nearResult=false,failAction=false,failCleanup=false;
static int failConstruct=0,constructs=0;
static int giftFailure=0;
static bool aliasName=false;
static CCharString otherName{};
static void check(bool ok){if(!ok)throw std::runtime_error("Theresa action check failed");}
static void __fastcall construct(CCharString* out,void*,const char* text,int length){
    check(length==-1);++constructs;
    if(constructs==failConstruct)throw std::runtime_error("CONSTRUCT");
    live.push_back(out);out->pStringData=reinterpret_cast<decltype(out->pStringData)>(emptyString?0:1);
    events.push_back(std::string("new:")+text);
}
static void __fastcall destroy(CCharString* out,void*){
    check(!live.empty() && live.back()==out);live.pop_back();events.emplace_back("destroy");
    if(failCleanup)throw std::runtime_error("CLEANUP");
}
static CScriptThing* __fastcall getHero(CGameScriptInterfaceBase* self,void*){
    check(self==&game);events.emplace_back("hero");return nullHero?nullptr:&hero;
}
static bool __fastcall proximity(const CScriptThing* first,const CScriptThing* second,float distance){
    check(first==&actor && second==(nullHero?nullptr:&hero) && distance==5.0f);
    events.emplace_back("near");return nearResult;
}
static void __fastcall animation(CScriptGameResourceObjectScriptedThingBase* self,void*,const CCharString* key,
                                bool b1,bool b2,bool b3,bool b4,bool b5,bool b6){
    check(self==&expert && live.size()==1 && key==live.back() && b1 && !b2 && !b3 && b4 && !b5 && !b6);
    events.emplace_back("skip");if(failAction)throw std::runtime_error("ACTION");
}
static void __fastcall question(CGameScriptInterfaceBase* self,void*,const CCharString* q,const CCharString* yes,
                               const CCharString* no,const CCharString* empty,bool flag){
    check(self==&game && live.size()==4 && q==live[3] && yes==live[2] && no==live[1] && empty==live[0] && flag);
    events.emplace_back("question");if(failAction)throw std::runtime_error("ACTION");
}
static void __fastcall speak(CScriptGameResourceObjectScriptedThingBase* self,void*,const CScriptThing* target,const char* key,ETextGroupSelectionMethod selection,bool listen,bool sound,bool fade){
    check(self==&expert && target==(nullHero?nullptr:&hero) && std::string(key)=="TEXT_QST_048_THERESA_DONT_HIT" && static_cast<int>(selection)==0 && !listen && sound && !fade);
    events.emplace_back("speak");
}
tCCharString_Constructor_Literal CCharString_Construct_Literal=reinterpret_cast<tCCharString_Constructor_Literal>(&construct);
tCCharString_Destructor CCharString_Destroy=reinterpret_cast<tCCharString_Destructor>(&destroy);
tGetHero GetHero_API=reinterpret_cast<tGetHero>(&getHero);
tIsDistanceBetweenThingsUnder IsDistanceBetweenThingsUnder_API=reinterpret_cast<tIsDistanceBetweenThingsUnder>(&proximity);
static void __fastcall take(CGameScriptInterfaceBase* self,void*,const CCharString* key){
    check(self==&game && live.size()==1 && key==live.back());events.emplace_back("take");
    if(giftFailure==1)throw std::runtime_error("GIFT");
}
static CCharString* __fastcall questName(CGameScriptInterfaceBase* self,void*,CCharString* out){
    check(self==&game && live.size()==3);
    if(giftFailure==2)throw std::runtime_error("GIFT");
    live.push_back(out);out->pStringData=reinterpret_cast<decltype(out->pStringData)>(emptyString?0:1);
    events.emplace_back("quest-name");return aliasName?&otherName:out;
}
static void __fastcall objective(CGameScriptInterfaceBase* self,void*,const CCharString* name,const CCharString* text,const CCharString* first,const CCharString* second){
    check(self==&game && live.size()==4 && name==(aliasName?&otherName:live[3]) && text==live[2] && first==live[1] && second==live[0]);
    events.emplace_back("objective");if(giftFailure==3)throw std::runtime_error("GIFT");
}
static void __fastcall clearInfo(CGameScriptInterfaceBase* self,void*,const CScriptThing* who){
    check(self==&game && who==&actor && live.empty());events.emplace_back("clear-info");
}
tTakeObjectFromHero TakeObjectFromHero_API=reinterpret_cast<tTakeObjectFromHero>(&take);
tGetActiveQuestName GetActiveQuestName_API=reinterpret_cast<tGetActiveQuestName>(&questName);
tSetQuestCardObjective SetQuestCardObjective_API=reinterpret_cast<tSetQuestCardObjective>(&objective);
tClearThingHasInformation ClearThingHasInformation_API=reinterpret_cast<tClearThingHasInformation>(&clearInfo);
static CScriptThing guards[3]{};
static int vectorFailure=0,vectorCount=0,vectorDestroys=0;
static int __fastcall getGuards(CGameScriptInterfaceBase* self,void*,const CCharString* key,RetailTheresaGuardVector::Vector* out){
    check(self==&game && live.size()==1 && key==live.back());
    check(out->begin==nullptr && out->end==nullptr && out->capacity==nullptr);
    out->begin=guards;out->end=guards+vectorCount;out->capacity=guards+3;
    events.emplace_back("get-guards");if(vectorFailure==1)throw std::runtime_error("VECTOR");return vectorCount;
}
static void __fastcall removeGuards(CGameScriptInterfaceBase* self,RetailTheresaGuardVector::Vector* vector,bool flag){
    check(self==&game && !flag && live.empty() && vector->begin==guards && vector->end==guards+vectorCount);
    events.emplace_back("remove-guards");if(vectorFailure==2)throw std::runtime_error("VECTOR");
}
static void __fastcall destroyGuards(RetailTheresaGuardVector::Vector* vector,void*){
    check(vector->begin==guards && vector->end==guards+vectorCount);++vectorDestroys;events.emplace_back("destroy-vector");
}
static bool __fastcall acquireHero(CGameScriptInterfaceBase* self,void*,const CScriptThing* target,CScriptGameResourceObjectScriptedThingBase* resource,EScriptAIPriority priority){
    check(self==&game && target==(nullHero?nullptr:&hero) && static_cast<int>(priority)==4);
    resource->pImp.Data=&expert;events.emplace_back("acquire");return nearResult;
}
static bool __fastcall hasChocolate(CGameScriptInterfaceBase* self,void*,const CCharString* key,const CScriptThing* target){
    check(self==&game && target==(nullHero?nullptr:&hero) && live.size()==1 && key==live.back());
    events.emplace_back("has-chocolate");if(failAction)throw std::runtime_error("ACTION");return nearResult;
}
tStartScriptingEntity StartScriptingEntity_API=reinterpret_cast<tStartScriptingEntity>(&acquireHero);
tIsObjectInThingsPossession IsObjectInThingsPossession_API=reinterpret_cast<tIsObjectInThingsPossession>(&hasChocolate);
struct Scope {
    enum class Kind{Resource};
    struct Entry{CScriptGameResourceObjectScriptedThingBase resource{};} entry;
    CGameScriptInterfaceBase* m_game=&game;bool closed=false;
    void CheckOpen(){if(closed)throw std::runtime_error("CLOSED");}
    Entry& Get(unsigned id,Kind){check(id==1);return entry;}
#include "retail_theresa_actions.inc"
};
int main(){try{
    void* gameTable[0x1cc/4]{};gameTable[0x1c8/4]=reinterpret_cast<void*>(&question);
    *reinterpret_cast<void***>(&game)=gameTable;
    CScriptGameResourceObjectScriptedThingBaseVTable actionTable{};
    actionTable.PlayCombatAnimation=reinterpret_cast<decltype(actionTable.PlayCombatAnimation)>(&animation);
    actionTable.Speak=reinterpret_cast<decltype(actionTable.Speak)>(&speak);
    expert.pVTable=reinterpret_cast<decltype(expert.pVTable)>(&actionTable);
    Scope scope;sol::state lua;lua.open_libraries(sol::lib::base,sol::lib::string);
    auto type=lua.new_usertype<Scope>("Scope",sol::no_constructor);
    type["PlayTheresaSkip"]=&Scope::PlayTheresaSkip;
    type["SpeakTheresa"]=&Scope::SpeakTheresa;
    type["TryAcquireTheresaHero"]=&Scope::TryAcquireTheresaHero;
    type["DoesTheresaHeroHaveChocolates"]=&Scope::DoesTheresaHeroHaveChocolates;
    type["IsTheresaNearHero"]=&Scope::IsTheresaNearHero;
    type["ShowTheresaChocolateQuestion"]=&Scope::ShowTheresaChocolateQuestion;
    type["TakeTheresaChocolatesAndUpdateObjective"]=&Scope::TakeTheresaChocolatesAndUpdateObjective;
    type["ClearTheresaInformation"]=&Scope::ClearTheresaInformation;
    lua["resources"]=&scope;lua["actor"]=&actor;int cases=0;
    auto reset=[](){events.clear();constructs=0;check(live.empty());};
    for(bool nil:{false,true})for(bool result:{false,true}){
        reset();nullHero=nil;nearResult=result;lua["expected"]=result;
        lua.script("assert(resources:IsTheresaNearHero(actor,5)==expected)");
        check(events==std::vector<std::string>{"hero","near"});++cases;
    }
    for(bool storage:{false,true})for(bool acquired:{false,true}){
        reset();emptyString=storage;scope.entry.resource.pImp.Data=acquired?&expert:nullptr;
        lua.script("resources:PlayTheresaSkip(1)");
        check(live.empty() && events==(acquired?std::vector<std::string>{"new:SKIP","skip","destroy"}:std::vector<std::string>{"new:SKIP","destroy"}));++cases;
        reset();lua.script("resources:ShowTheresaChocolateQuestion()");
        check(live.empty() && events==std::vector<std::string>{"new:","new:TEXT_OBJECT_HERO_ANSWER_NO","new:TEXT_OBJECT_HERO_ANSWER_YES","new:TEXT_QST_048_GIVE_CHOCOLATE_BOX","question","destroy","destroy","destroy","destroy"});++cases;
    }
    scope.entry.resource.pImp.Data=&expert;
    for(bool cleanup:{false,true})for(bool skip:{false,true}){
        reset();failAction=true;failCleanup=cleanup;lua["skip"]=skip;
        lua.script("local ok,e=pcall(function() if skip then resources:PlayTheresaSkip(1) else resources:ShowTheresaChocolateQuestion() end end);assert(not ok and string.find(e,'ACTION',1,true))");
        check(live.empty());++cases;
    }
    failAction=false;
    for(int failed=1;failed<=4;++failed){
        reset();failConstruct=failed;failCleanup=true;
        lua.script("local ok,e=pcall(function() resources:ShowTheresaChocolateQuestion() end);assert(not ok and string.find(e,'CONSTRUCT',1,true))");
        check(live.empty());++cases;
    }
    failConstruct=0;failCleanup=true;reset();
    lua.script("local ok,e=pcall(function() resources:ShowTheresaChocolateQuestion() end);assert(not ok and string.find(e,'CLEANUP',1,true))");check(live.empty());++cases;
    failCleanup=false;
    for(bool empty:{false,true})for(bool alias:{false,true}){
        reset();emptyString=empty;aliasName=alias;
        lua.script("resources:TakeTheresaChocolatesAndUpdateObjective();resources:ClearTheresaInformation(actor)");
        check(live.empty() && events==std::vector<std::string>{"new:OBJECT_CHOCOLATE_BOX_UNGIVEABLE","take","destroy","new:","new:","new:TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_05","quest-name","objective","destroy","destroy","destroy","destroy","clear-info"});++cases;
    }
    for(int failure:{1,2,3}){
        reset();giftFailure=failure;
        lua.script("local ok,e=pcall(function() resources:TakeTheresaChocolatesAndUpdateObjective() end);assert(not ok and string.find(e,'GIFT',1,true))");
        check(live.empty());++cases;
    }
    giftFailure=0;
    for(int failure=1;failure<=4;++failure){
        reset();failConstruct=failure;
        lua.script("local ok,e=pcall(function() resources:TakeTheresaChocolatesAndUpdateObjective() end);assert(not ok and string.find(e,'CONSTRUCT',1,true))");
        check(live.empty());++cases;
    }
    failConstruct=0;scope.closed=true;reset();
    lua.script("assert(not pcall(function() resources:PlayTheresaSkip(1) end));assert(not pcall(function() resources:ShowTheresaChocolateQuestion() end));assert(not pcall(function() resources:IsTheresaNearHero(actor,5) end))");
    check(events.empty());++cases;
    lua.script("assert(not pcall(function() resources:TakeTheresaChocolatesAndUpdateObjective() end));assert(not pcall(function() resources:ClearTheresaInformation(actor) end))");
    check(events.empty());++cases;
    scope.closed=false;
    for(bool nil:{false,true})for(bool populated:{false,true}){
        reset();nullHero=nil;scope.entry.resource.pImp.Data=populated?&expert:nullptr;
        lua.script("resources:SpeakTheresa(1,'TEXT_QST_048_THERESA_DONT_HIT')");
        check(events==(populated?std::vector<std::string>{"hero","speak"}:std::vector<std::string>{"hero"}));++cases;
    }
    for(bool nil:{false,true})for(bool value:{false,true})for(bool empty:{false,true}){
        reset();nullHero=nil;nearResult=value;emptyString=empty;lua["expected"]=value;
        scope.entry.resource.pImp.Data=nullptr;
        lua.script("assert(resources:TryAcquireTheresaHero(1,4)==expected)");
        check(scope.entry.resource.pImp.Data==&expert && events==std::vector<std::string>{"hero","acquire"});++cases;
        reset();lua.script("assert(resources:DoesTheresaHeroHaveChocolates()==expected)");
        check(live.empty() && events==std::vector<std::string>{"new:OBJECT_CHOCOLATE_BOX_UNGIVEABLE","hero","has-chocolate","destroy"});++cases;
    }
    for(bool cleanup:{false,true}){
        reset();failAction=true;failCleanup=cleanup;
        lua.script("local ok,e=pcall(function() resources:DoesTheresaHeroHaveChocolates() end);assert(not ok and string.find(e,'ACTION',1,true))");
        check(live.empty());++cases;
    }
    failAction=false;failCleanup=false;
    auto vectors=lua.new_usertype<RetailTheresaGuardVector>("GuardVector",sol::no_constructor);
    vectors["RemoveLivingGuards"]=&RetailTheresaGuardVector::RemoveLivingGuards;
    vectors["Close"]=&RetailTheresaGuardVector::Close;
    RetailTheresaGuardVector::APIs vectorApis{reinterpret_cast<RetailTheresaGuardVector::Getter>(&getGuards),
        &removeGuards,reinterpret_cast<RetailTheresaGuardVector::Destroy>(&destroyGuards)};
    for(int count=0;count<=3;++count)for(bool empty:{false,true}){
        reset();vectorCount=count;vectorDestroys=0;emptyString=empty;
        {RetailTheresaGuardVector owner(&game,vectorApis);lua["guards"]=&owner;
            lua.script("guards:RemoveLivingGuards();guards:Close();guards:Close();assert(not pcall(function() guards:RemoveLivingGuards() end))");}
        check(vectorDestroys==1 && live.empty() && events==std::vector<std::string>{"new:NOVI_Guard","get-guards","destroy","remove-guards","destroy-vector"});++cases;
    }
    for(int failure:{1,2}){
        reset();vectorFailure=failure;vectorDestroys=0;
        try {RetailTheresaGuardVector owner(&game,vectorApis);owner.RemoveLivingGuards();check(false);}
        catch(const std::runtime_error& error){check(std::string(error.what())=="VECTOR");}
        check(live.empty() && vectorDestroys==1);++cases;
    }
    vectorFailure=0;reset();vectorDestroys=0;failCleanup=true;
    try {RetailTheresaGuardVector owner(&game,vectorApis);check(false);}
    catch(const std::runtime_error& error){check(std::string(error.what())=="CLEANUP");}
    check(live.empty() && vectorDestroys==1);failCleanup=false;++cases;
    std::cout<<"PASS: "<<cases<<" Theresa runtime action policies\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
