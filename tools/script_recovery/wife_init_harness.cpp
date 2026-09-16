#define main resource_smoke_main
#include "runtime_checks/retail_resources_smoke.cpp"
#undef main
#include <fstream>
#include <iterator>
static CScriptThing actor{};
static std::vector<std::string> trace;
static std::string fault,source;
static void event(const std::string& text){trace.push_back(text);if(text==fault)throw std::runtime_error("WIFE_INIT_FAULT");}
static void __fastcall damage(CGameScriptInterfaceBase*,void*,const CScriptThing* a,bool b){check(a==&actor&&!b);event("damage");}
static void __fastcall kill(CGameScriptInterfaceBase*,void*,const CScriptThing* a,bool b,bool c){check(a==&actor&&!b&&!c);event("kill");}
static void __fastcall combo(CGameScriptInterfaceBase*,void*,const CScriptThing* a,bool b){check(a==&actor&&!b);event("combo");}
static void __fastcall information(CGameScriptInterfaceBase*,void*,const CScriptThing* a,bool b,bool c,bool d){check(a==&actor&&!b&&c&&!d);event("information");}
static void __fastcall movement(CGameScriptInterfaceBase*,void*,const CScriptThing* a,bool b){check(a==&actor&&!b);event("movement");}
static void __fastcall deeds(CGameScriptInterfaceBase*,void*,const CScriptThing* a,bool b){check(a==&actor&&!b);event("deeds");}
static void __fastcall pushable(CGameScriptInterfaceBase*,void*,CScriptThing copy,bool b){check(!b&&copy.pVTable==g_pCScriptThingVTable&&copy.pImp.Data==actor.pImp.Data&&copy.pImp.Info==actor.pImp.Info);if(copy.pImp.Info){check(copy.pImp.Info->RefCount==8);--copy.pImp.Info->RefCount;}event("pushable");}
tEntitySetAsDamageable EntitySetAsDamageable_API=reinterpret_cast<tEntitySetAsDamageable>(&damage);
tEntitySetAsKillable EntitySetAsKillable_API=reinterpret_cast<tEntitySetAsKillable>(&kill);
tEntitySetAsToAddToComboMultiplierWhenHit EntitySetAsToAddToComboMultiplierWhenHit_API=reinterpret_cast<tEntitySetAsToAddToComboMultiplierWhenHit>(&combo);
tSetThingHasInformation SetThingHasInformation_API=reinterpret_cast<tSetThingHasInformation>(&information);
tEntitySetAsUseMovementInActions EntitySetAsUseMovementInActions_API=reinterpret_cast<tEntitySetAsUseMovementInActions>(&movement);
tSetIsPushableByHero SetIsPushableByHero_API=reinterpret_cast<tSetIsPushableByHero>(&pushable);
tEntitySetDeedReactionsEnabled EntitySetDeedReactionsEnabled_API=reinterpret_cast<tEntitySetDeedReactionsEnabled>(&deeds);
static int conversationId=0,heroes=0;static bool heroPop[2]{};static CScriptThing heroActors[2]{};
static int __fastcall conversation(CGameScriptInterfaceBase*,void*,const CScriptThing* a,bool b,bool c){check(a==&actor&&!b&&!c);event("conversation");return conversationId;}
static CScriptThing* __fastcall getHero(CGameScriptInterfaceBase*,void*){check(heroes<2);check(strings.empty()==(heroes==0));const int index=heroes++;event("hero"+std::to_string(index));return heroPop[index]?&heroActors[index]:nullptr;}
static void __fastcall person(CGameScriptInterfaceBase*,void*,int id,const CScriptThing* a){check(id==conversationId&&a==(heroPop[0]?&heroActors[0]:nullptr));event("person");}
static void __fastcall line(CGameScriptInterfaceBase*,void*,int id,const CCharString* key,bool flag,const CScriptThing* a,const CScriptThing* target){check(id==conversationId&&!flag&&a==&actor&&target==(heroPop[1]?&heroActors[1]:nullptr)&&strings.at(const_cast<CCharString*>(key))=="TEXT_QST_048_AFFAIR_WIFE_WHERES_HUSBAND");event("line");}
static void __fastcall keyCtor(CCharString* key,void*,const char* text,int size){stringCtor(key,nullptr,text,size);event("key.new");}
static void __fastcall keyDtor(CCharString* key,void*){stringDtor(key,nullptr);event("key.destroy");}
tAddNewConversation AddNewConversation_API=reinterpret_cast<tAddNewConversation>(&conversation);
tGetHero GetHero_API=reinterpret_cast<tGetHero>(&getHero);
tAddPersonToConversation AddPersonToConversation_API=reinterpret_cast<tAddPersonToConversation>(&person);
tAddLineToConversation AddLineToConversation_API=reinterpret_cast<tAddLineToConversation>(&line);
int main(){try{
    std::ifstream input("../INIT.lua");check(input.good());source=std::string(std::istreambuf_iterator<char>(input),std::istreambuf_iterator<char>());
    std::remove_pointer_t<decltype(actor.pImp.Info)> info{};info.RefCount=7;unsigned cases=0;
    for(bool populated:{false,true})for(bool counted:{false,true})for(const char* error:{"","damage","kill","combo","information","movement","pushable","deeds"}){
        actor.pImp.Data=populated?reinterpret_cast<decltype(actor.pImp.Data)>(1):nullptr;actor.pImp.Info=counted?&info:nullptr;fault=error;trace.clear();
        sol::state lua;lua.open_libraries(sol::lib::base);RegisterRetailResources(lua);lua["actor"]=&actor;
        lua["scope"]=[&](sol::protected_function body){WithRetailResources(reinterpret_cast<CGameScriptInterfaceBase*>(1),body);};
        lua["recordState"]=[&](const std::string& name,bool value){check(value==(name=="ForceFirstTimeSpeak"));event(name);};
        check(lua.safe_script(source,sol::script_pass_on_error).valid());
        auto result=lua.safe_script("local q={WithRetailResources=function(_,body) scope(body) end};local state={SetStateBool=function(_,key,value) recordState(key,value) end};WifeRecoveredInit(q,actor,state)",sol::script_pass_on_error);
        check(result.valid()==fault.empty());check(info.RefCount==7&&locals.empty());
        std::vector<std::string> expected={"GoingForHusband","ForceFirstTimeSpeak","damage","kill","combo","information","movement","pushable","SaidRunningLine","deeds"};
        if(!fault.empty()){auto last=std::find(expected.begin(),expected.end(),fault);check(last!=expected.end());expected.erase(last+1,expected.end());}
        check(trace==expected);++cases;
    }
    check(cases==32);
    CCharString_Construct_Literal=reinterpret_cast<decltype(CCharString_Construct_Literal)>(&keyCtor);CCharString_Destroy=reinterpret_cast<decltype(CCharString_Destroy)>(&keyDtor);
    for(int id:{-2147483647-1,-1,0,1,2147483647})for(bool first:{false,true})for(bool second:{false,true})for(const char* error:{"","hero1","line"}){
        conversationId=id;heroPop[0]=first;heroPop[1]=second;heroes=0;fault=error;trace.clear();
        sol::state lua;lua.open_libraries(sol::lib::base);RegisterRetailResources(lua);lua["actor"]=&actor;
        lua["scope"]=[&](sol::protected_function body){WithRetailResources(reinterpret_cast<CGameScriptInterfaceBase*>(1),body);};
        auto result=lua.safe_script("scope(function(r) r:AddWifeWhereHusbandConversation(actor) end)",sol::script_pass_on_error);
        check(result.valid()==fault.empty());check(strings.empty()&&heroes==2);
        std::vector<std::string> expected={"conversation","hero0","person","key.new","hero1"};if(fault!="hero1")expected.push_back("line");expected.push_back("key.destroy");check(trace==expected);++cases;
    }
    printf("Wife x86 actual-FSE/real-Lua: %u policies passed (32 Init + 60 conversation)\n",cases);return 0;
}catch(const std::exception& e){fprintf(stderr,"%s\n",e.what());return 1;}}
