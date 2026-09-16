#define main resource_smoke_main
#include "runtime_checks/retail_resources_smoke.cpp"
#undef main
#include "teddy_girl_distance_bytes.inc"
#include <limits>
DWORD g_fableBase=0x400000;
static CScriptThing girl{},hero{};
static C3DVector point{7,-3,11};
static const C3DVector* positionResult=&point;
static CScriptThing* heroResult=&hero;
static bool screenResult=false,targetPopulated=false;
static std::string fault;
static std::vector<std::string> trace;
static unsigned nullQueries=0,movementCalls=0,thingLives=0;
static bool girlAlive=true,otherAlive=true;
static bool talkedResult=false,hasTeddy=false;
static C3DVector origin{},otherPoint{};
static bool __fastcall alive(CScriptThing* self,void*){return self==&girl?girlAlive:otherAlive;}
static const C3DVector* __fastcall distancePosition(CScriptThing* self,void*){return self==&girl?&origin:&otherPoint;}
static void event(const std::string& text){trace.push_back(text);if(text==fault)throw std::runtime_error("TEDDY_BODY_FAULT");}
static void __fastcall textCtor(CCharString* self,void*,const char* text,int count){check(count==-1);stringCtor(self,nullptr,text,count);if(!*text)self->pStringData=nullptr;trace.push_back("new:"+std::string(text));}
static void __fastcall textDestroy(CCharString* self,void*){trace.push_back("destroy:"+strings.at(self));stringDtor(self,nullptr);}
static bool __fastcall nullQuery(CScriptThing*,void*){++nullQueries;return true;}
static void __fastcall damage(CGameScriptInterfaceBase*,void*,const CScriptThing* actor,bool value){check(actor==&girl&&!value);event("damage");}
static void __fastcall kill(CGameScriptInterfaceBase*,void*,const CScriptThing* actor,bool a,bool b){check(actor==&girl&&!a&&!b);event("kill");}
static void __fastcall combo(CGameScriptInterfaceBase*,void*,const CScriptThing* actor,bool value){check(actor==&girl&&!value);event("combo");}
static void __fastcall information(CGameScriptInterfaceBase*,void*,const CScriptThing* actor,bool a,bool b,bool c){check(actor==&girl&&!a&&b&&!c);event("information");}
static void __fastcall clear(CGameScriptInterfaceBase*,void*,const CScriptThing* actor){check(actor==&girl);event("clear");}
static void __fastcall removeActor(CGameScriptInterfaceBase*,void*,const CScriptThing* actor,bool a,bool b){check(actor==&girl&&!a&&b);event("remove");}
static bool __fastcall talked(CScriptThing* actor,void*,const CCharString* who){check(actor==&girl&&strings.at(const_cast<CCharString*>(who))=="SCRIPT_NAME_HERO");event("talked");return talkedResult;}
static bool __fastcall possession(CGameScriptInterfaceBase*,void*,const CCharString* key,const CScriptThing* actor){check(actor==heroResult&&strings.at(const_cast<CCharString*>(key))=="OBJECT_TEDDY_BEAR_UNGIVEABLE");event("possession");return hasTeddy;}
static void __fastcall take(CGameScriptInterfaceBase*,void*,const CCharString* text){check(strings.at(const_cast<CCharString*>(text))=="OBJECT_TEDDY_BEAR_UNGIVEABLE");event("take");}
static void __fastcall question(CGameScriptInterfaceBase*,void*,const CCharString* q,const CCharString* yes,const CCharString* no,const CCharString* empty,bool flag){
    check(strings.at(const_cast<CCharString*>(q))=="TEXT_QST_048_GIVE_TEDDY_TO_GIRL"&&strings.at(const_cast<CCharString*>(yes))=="TEXT_OBJECT_HERO_ANSWER_YES"&&strings.at(const_cast<CCharString*>(no))=="TEXT_OBJECT_HERO_ANSWER_NO"&&strings.at(const_cast<CCharString*>(empty)).empty()&&flag);event("question");
}
static CScriptThing* __fastcall getHero(CGameScriptInterfaceBase*,void*){event("hero");return heroResult;}
static void __fastcall line(CGameScriptInterfaceBase*,void*,int id,const CCharString* text,bool flag,const CScriptThing* actor,const CScriptThing* target){check(id==-7&&!flag&&actor==&girl&&target==heroResult&&strings.at(const_cast<CCharString*>(text))=="TEXT_QST_048_TEDDYGIRL_TEDDY_RUINED");event("line");}
static const C3DVector* __fastcall position(CScriptThing* actor,void*){check(actor==&girl);event("position");return positionResult;}
static bool __fastcall screen(CGameScriptInterfaceBase*,void*,const C3DVector* value){check(value==positionResult);event("screen");return screenResult;}
static CScriptThing* __fastcall lookup(CGameScriptInterfaceBase*,void*,CScriptThing* output,const CCharString* key){check(strings.at(const_cast<CCharString*>(key))=="NOVI_AffairWife");output->pImp.Data=targetPopulated?girl.pImp.Data:nullptr;++thingLives;return output;}
static void __fastcall thingDestroy(CScriptThing*,void*){check(thingLives>0);--thingLives;trace.push_back("target.destroy");}
static void __fastcall move(void* self,void*,const CScriptThing* target,float radius,EScriptEntityMoveType kind,CTCScriptedControl* wait,bool avoid,bool ignore,bool face){
    check(self==expectedExpert&&target->pImp.Data==(targetPopulated?girl.pImp.Data:nullptr)&&radius==3.0f&&static_cast<int>(kind)==1&&!wait&&!avoid&&!ignore&&face);++movementCalls;event("move");
}
static void* __fastcall movieCtor(void* self,void*){trace.push_back("movie.new");return self;}
static void __fastcall ownedMovieStart(CGameScriptInterfaceBase* game,void*,const CCharString* key,CScriptGameResourceObjectMovieBase* movie){check(strings.at(const_cast<CCharString*>(key)).empty());movieStart(game,nullptr,key,movie);event("movie.start");}
static void __fastcall ownedMovieDestroy(CScriptGameResourceObjectMovieBase* movie,void*){movieDestroy(movie,nullptr);trace.push_back("movie.destroy");}
tEntitySetAsDamageable EntitySetAsDamageable_API=reinterpret_cast<tEntitySetAsDamageable>(&damage);
tEntitySetAsKillable EntitySetAsKillable_API=reinterpret_cast<tEntitySetAsKillable>(&kill);
tEntitySetAsToAddToComboMultiplierWhenHit EntitySetAsToAddToComboMultiplierWhenHit_API=reinterpret_cast<tEntitySetAsToAddToComboMultiplierWhenHit>(&combo);
tSetThingHasInformation SetThingHasInformation_API=reinterpret_cast<tSetThingHasInformation>(&information);
tClearThingHasInformation ClearThingHasInformation_API=reinterpret_cast<tClearThingHasInformation>(&clear);
tRemoveThing RemoveThing_API=reinterpret_cast<tRemoveThing>(&removeActor);
tIsObjectInThingsPossession IsObjectInThingsPossession_API=reinterpret_cast<tIsObjectInThingsPossession>(&possession);
tTakeObjectFromHero TakeObjectFromHero_API=reinterpret_cast<tTakeObjectFromHero>(&take);
tGiveHeroYesNoQuestion GiveHeroYesNoQuestion_API=reinterpret_cast<tGiveHeroYesNoQuestion>(&question);
tGetHero GetHero_API=reinterpret_cast<tGetHero>(&getHero);
tAddLineToConversation AddLineToConversation_API=reinterpret_cast<tAddLineToConversation>(&line);
tIsCameraPosOnScreen IsCameraPosOnScreen_API=reinterpret_cast<tIsCameraPosOnScreen>(&screen);
static bool run(const std::string& source){sol::state lua;lua.open_libraries(sol::lib::base);RegisterRetailResources(lua);lua["girl"]=&girl;lua["expectedTalk"]=talkedResult;lua["expectedTeddy"]=talkedResult&&hasTeddy;lua["expectedScreen"]=screenResult;lua["scope"]=[](sol::protected_function body){WithRetailResources(reinterpret_cast<CGameScriptInterfaceBase*>(1),body);};auto result=lua.safe_script(source,sol::script_pass_on_error);if(!result.valid()){sol::error e=result;check(std::string(e.what()).find("TEDDY_BODY_FAULT")!=std::string::npos);}return result.valid();}
int main(){try{
    CScriptThingVTable table{};table.IsNull=reinterpret_cast<decltype(table.IsNull)>(&nullQuery);table.GetPos=reinterpret_cast<decltype(table.GetPos)>(&position);girl.pVTable=reinterpret_cast<void**>(&table);
    CCharString_Construct_Literal=reinterpret_cast<tCCharString_Constructor_Literal>(&textCtor);CCharString_Destroy=reinterpret_cast<tCCharString_Destructor>(&textDestroy);
    for(bool populated:{false,true})for(const char* failure:{"","damage","kill","combo","information"}){
        girl.pImp.Data=populated?reinterpret_cast<decltype(girl.pImp.Data)>(0x4242):nullptr;fault=failure;trace.clear();check(run("scope(function(r) r:InitializeTeddyGirlActor(girl) end)")==fault.empty());
        const std::vector<std::string> all={"damage","kill","combo","information"};auto count=fault.empty()?all.size():std::find(all.begin(),all.end(),fault)-all.begin()+1;check(trace==std::vector<std::string>(all.begin(),all.begin()+count)&&!nullQueries);
    }
    for(const char* failure:{"","question"}){fault=failure;trace.clear();check(run("scope(function(r) r:GiveTeddyGirlQuestion() end)")==fault.empty());check(strings.empty()&&trace==std::vector<std::string>{"new:","new:TEXT_OBJECT_HERO_ANSWER_NO","new:TEXT_OBJECT_HERO_ANSWER_YES","new:TEXT_QST_048_GIVE_TEDDY_TO_GIRL","question","destroy:TEXT_QST_048_GIVE_TEDDY_TO_GIRL","destroy:TEXT_OBJECT_HERO_ANSWER_YES","destroy:TEXT_OBJECT_HERO_ANSWER_NO","destroy:"});}
    for(const char* failure:{"","take"}){fault=failure;trace.clear();check(run("scope(function(r) r:TakeTeddyFromHero() end)")==fault.empty());check(strings.empty()&&trace==std::vector<std::string>{"new:OBJECT_TEDDY_BEAR_UNGIVEABLE","take","destroy:OBJECT_TEDDY_BEAR_UNGIVEABLE"});}
    for(bool present:{false,true})for(const char* failure:{"","hero","line"}){heroResult=present?&hero:nullptr;fault=failure;trace.clear();check(run("scope(function(r) r:TeddyGirlAddHeroLine(-7,girl,'TEXT_QST_048_TEDDYGIRL_TEDDY_RUINED') end)")==fault.empty());std::vector<std::string> expected={"new:TEXT_QST_048_TEDDYGIRL_TEDDY_RUINED","hero"};if(fault!="hero")expected.push_back("line");expected.push_back("destroy:TEXT_QST_048_TEDDYGIRL_TEDDY_RUINED");check(strings.empty()&&trace==expected);}
    for(bool present:{false,true})for(bool value:{false,true})for(const char* failure:{"","position","screen"}){positionResult=present?&point:nullptr;screenResult=value;fault=failure;trace.clear();check(run("scope(function(r) assert(r:IsActorPositionOnScreen(girl)==expectedScreen) end)")==fault.empty());check(trace==(fault=="position"?std::vector<std::string>{"position"}:std::vector<std::string>{"position","screen"}));}
    RetailThing_Destroy_API=reinterpret_cast<tRetailThingDestroy>(&thingDestroy);GetThingWithScriptName_ByName_API=reinterpret_cast<tGetThingWithScriptName1>(&lookup);
    void* expertTable[6]{};expertTable[5]=reinterpret_cast<void*>(&move);CScriptGameResourceObjectScriptedThingBase expert{};expert.pVTable=expertTable;expectedExpert=&expert;acquireExpert=&expert;
    for(bool acquired:{false,true})for(bool populated:{false,true})for(const char* failure:{"","move"}){acquireOK=acquired;targetPopulated=populated;fault=failure;trace.clear();auto before=movementCalls;check(run("scope(function(r) local c=r:NewResource(); r:TryAcquire(c,girl,4); local t=r:NewThingFromScriptName('NOVI_AffairWife'); r:TeddyGirlMoveToThing(c,t,3,1,nil,false,false,true) end)")==(!acquired||fault.empty()));check(movementCalls==before+static_cast<unsigned>(acquired)&&!thingLives&&strings.empty()&&locals.empty()&&trace.back()=="target.destroy");for(const auto& ref:refs)check(ref.second==0);}
    fault.clear();trace.clear();check(run("scope(function(r) r:ClearRawInformation(girl) end)")&&trace==std::vector<std::string>{"clear"}&&!nullQueries);
    CBaseObject_Construct_API=reinterpret_cast<tCBaseObject_Constructor>(&movieCtor);StartMovieSequence_API=reinterpret_cast<tStartMovieSequence>(&ownedMovieStart);MovieResource_Destroy_API=reinterpret_cast<decltype(MovieResource_Destroy_API)>(&ownedMovieDestroy);
    for(const char* failure:{"","movie.start"}){fault=failure;trace.clear();check(run("scope(function(r) local m=r:NewMovie(); r:StartOwnedMovie(m,''); r:DestroyMovie(m) end)")==fault.empty());check(!movies&&strings.empty()&&trace==std::vector<std::string>{"movie.new","new:","movie.start","destroy:","movie.destroy"});}
    table.IsAlive=reinterpret_cast<decltype(table.IsAlive)>(&alive);table.GetPos=reinterpret_cast<decltype(table.GetPos)>(&distancePosition);hero.pVTable=girl.pVTable;g_pCScriptThingVTable=girl.pVTable;
    auto* arena=static_cast<unsigned char*>(VirtualAlloc(nullptr,4096,MEM_COMMIT|MEM_RESERVE,PAGE_EXECUTE_READWRITE));check(arena!=nullptr);
    memcpy(arena+0x2ff,underBytes,sizeof underBytes);memcpy(arena+0x3ea,overBytes,sizeof overBytes);FlushInstructionCache(GetCurrentProcess(),arena,4096);g_fableBase=reinterpret_cast<DWORD>(arena)-(0xcbe000-0x400000);
    fault.clear();
    for(bool first:{false,true})for(bool second:{false,true})for(float x:{-5.0f,0.0f,10.0f,20.0f,21.0f,std::numeric_limits<float>::quiet_NaN()}){
        girlAlive=first;otherAlive=second;otherPoint={x,0,0};sol::state lua;lua.open_libraries(sol::lib::base);RegisterRetailResources(lua);lua["girl"]=&girl;lua["hero"]=&hero;lua["under"]=first&&second&&x*x<100;lua["over"]=first&&second&&x*x>400;lua["scope"]=[](sol::protected_function body){WithRetailResources(reinterpret_cast<CGameScriptInterfaceBase*>(1),body);};
        check(lua.safe_script("scope(function(r) local t=r:NewThingFromScriptName('NOVI_AffairWife'); assert(r:IsDistanceUnderThing(girl,t,10)==under); assert(r:IsDistanceOver(girl,hero,20)==over) end)",sol::script_pass_on_error).valid());check(!thingLives&&strings.empty());
    }
    VirtualFree(arena,0,MEM_RELEASE);
    table.MsgIsTalkedToBy=reinterpret_cast<decltype(table.MsgIsTalkedToBy)>(&talked);
    for(bool spoken:{false,true})for(bool teddy:{false,true})for(bool present:{false,true})for(const char* failure:{"","hero","possession"}){
        talkedResult=spoken;hasTeddy=teddy;heroResult=present?&hero:nullptr;fault=failure;trace.clear();check(run("scope(function(r) assert(r:TeddyGirlTalkedWithTeddy(girl)==expectedTeddy) end)")==(!spoken||fault.empty()));
        std::vector<std::string> expected={"new:SCRIPT_NAME_HERO","talked"};if(spoken){expected.push_back("new:OBJECT_TEDDY_BEAR_UNGIVEABLE");expected.push_back("hero");if(fault!="hero")expected.push_back("possession");expected.push_back("destroy:OBJECT_TEDDY_BEAR_UNGIVEABLE");}expected.push_back("destroy:SCRIPT_NAME_HERO");check(strings.empty()&&trace==expected);
    }
    for(bool spoken:{false,true})for(const char* failure:{"","talked"}){talkedResult=spoken;fault=failure;trace.clear();check(run("scope(function(r) assert(r:IsTalkedToByHero(girl)==expectedTalk) end)")==fault.empty());check(strings.empty()&&trace==std::vector<std::string>{"new:SCRIPT_NAME_HERO","talked","destroy:SCRIPT_NAME_HERO"});}
    fault.clear();trace.clear();check(run("scope(function(r) r:RemoveRawThing(girl,false,true) end)")&&trace==std::vector<std::string>{"remove"}&&!nullQueries);
    std::cout<<"TeddyGirl x86 actual-FSE/real-Lua: 96 policies passed (72 scoped adapter cases, 24 original-native distance cases)\n";return 0;
}catch(const std::exception& error){std::cerr<<error.what()<<'\n';return 1;}}
