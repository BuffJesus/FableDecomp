#include "LuaRetailResources.h"
static void IgnoreResourceRegistration(sol::state&) {}
#define RegisterRetailResources IgnoreResourceRegistration
#define main resource_smoke_main
#include "runtime_checks/retail_resources_smoke.cpp"
#undef main
#undef RegisterRetailResources
#include <cmath>
#include <cstring>
static void __fastcall unexpectedStringMapClose(void*,void*){throw std::runtime_error("unexpected string map");}
decltype(StdMap_String_Destroy_API) StdMap_String_Destroy_API=reinterpret_cast<decltype(StdMap_String_Destroy_API)>(&unexpectedStringMapClose);

static CGameScriptInterfaceBase questGame{};
static CScriptThing questActor{},questHero{},questAlias{};
static CScriptThing* questOutput=nullptr;
static std::vector<std::string> questEvents;
static std::string questFault;
static int questAliasMode=0;
static bool questSnap=false;
class LuaQuestState {
public:
    CGameScriptInterfaceBase* m_pGameInterface=&questGame;
    std::map<std::string,int> ints;
    void SetStateInt(const std::string& key,int value){if(key=="WatchTimer")throw std::runtime_error("owned timer");ints[key]=value;}
    int GetStateInt(const std::string& key){return ints[key];}
    void SetStateFloat(const std::string&,float);
    float GetStateFloat(const std::string&);
    int StartConversationWithHero(CScriptThing*,bool,bool);
    void AddConversationLineToHero(int,const std::string&,CScriptThing*,bool);
    void FaceThingByScriptName(CScriptThing*,const std::string&,bool);
};
#include "book_trader_conversation_adapter.inc"
#include "oakvale_quest_adapter.inc"
static void questHit(const char* point){if(questFault==point)throw std::runtime_error(std::string("QUEST_")+point);}
static void __fastcall questKeyNew(CCharString* key,void*,const char* value,int length){questEvents.emplace_back("key.new");questHit("key.new");stringCtor(key,nullptr,value,length);}
static void __fastcall questKeyClose(CCharString* key,void*){check(!questOutput);questEvents.emplace_back("key.close");stringDtor(key,nullptr);questHit("key.close");}
static CScriptThing* __fastcall questLookup(CGameScriptInterfaceBase* game,void*,CScriptThing* out,const CCharString* key){
    check(game==&questGame&&strings.at(const_cast<CCharString*>(key))=="NOVI_Theresa");questEvents.emplace_back("lookup");questHit("lookup");questOutput=out;
    return questAliasMode==0?out:questAliasMode==1?&questAlias:nullptr;
}
static void __fastcall questFace(CGameScriptInterfaceBase* game,void*,const CScriptThing* actor,const CScriptThing* target,bool snap){
    check(game==&questGame&&actor==&questActor&&target==(questAliasMode==0?questOutput:questAliasMode==1?&questAlias:nullptr)&&snap==questSnap&&strings.size()==1);
    questEvents.emplace_back("face");questHit("face");
}
static void __fastcall questOutputClose(CScriptThing* value,void*){check(value==questOutput&&strings.size()==1);questOutput=nullptr;questEvents.emplace_back("output.close");questHit("output.close");}
decltype(EntitySetFacingAngleTowardsThing_API) EntitySetFacingAngleTowardsThing_API=reinterpret_cast<decltype(EntitySetFacingAngleTowardsThing_API)>(&questFace);
static CScriptThing* __fastcall questGetHero(CGameScriptInterfaceBase* game,void*){check(game==&questGame);questEvents.emplace_back("hero");questHit("hero");return &questHero;}
static int __fastcall questConversation(CGameScriptInterfaceBase* game,void*,const CScriptThing* actor,bool a,bool b){check(game==&questGame&&actor==&questActor&&!a&&b);questEvents.emplace_back("conversation");return -7;}
static void __fastcall questPerson(CGameScriptInterfaceBase* game,void*,int id,const CScriptThing* actor){check(game==&questGame&&id==-7&&actor==&questHero);questEvents.emplace_back("person");}
static void __fastcall questLine(CGameScriptInterfaceBase* game,void*,int id,const CCharString* key,bool flag,const CScriptThing* actor,const CScriptThing* listener){
    check(game==&questGame&&id==-7&&!flag&&actor==&questActor&&listener==&questHero&&strings.at(const_cast<CCharString*>(key))=="line");questEvents.emplace_back("line");questHit("line");
}
decltype(GetHero_API) GetHero_API=reinterpret_cast<decltype(GetHero_API)>(&questGetHero);
decltype(AddNewConversation_API) AddNewConversation_API=reinterpret_cast<decltype(AddNewConversation_API)>(&questConversation);
decltype(AddPersonToConversation_API) AddPersonToConversation_API=reinterpret_cast<decltype(AddPersonToConversation_API)>(&questPerson);
decltype(AddLineToConversation_API) AddLineToConversation_API=reinterpret_cast<decltype(AddLineToConversation_API)>(&questLine);
DWORD g_fableBase=0;
int main(){try{
    GetThingWithScriptName_ByName_API=reinterpret_cast<decltype(GetThingWithScriptName_ByName_API)>(&questLookup);
    CCharString_Construct_Literal=reinterpret_cast<decltype(CCharString_Construct_Literal)>(&questKeyNew);CCharString_Destroy=reinterpret_cast<decltype(CCharString_Destroy)>(&questKeyClose);
    RetailThing_Destroy_API=reinterpret_cast<decltype(RetailThing_Destroy_API)>(&questOutputClose);
    LuaQuestState state;sol::state lua;lua.open_libraries(sol::lib::base,sol::lib::math,sol::lib::string);
    lua.new_usertype<CScriptThing>("Thing",sol::no_constructor);
    auto type=lua.new_usertype<LuaQuestState>("Quest",sol::no_constructor);
    type["SetStateFloat"]=&LuaQuestState::SetStateFloat;type["GetStateFloat"]=&LuaQuestState::GetStateFloat;
    type["FaceThingByScriptName"]=&LuaQuestState::FaceThingByScriptName;
    type["StartConversationWithHero"]=&LuaQuestState::StartConversationWithHero;type["AddConversationLineToHero"]=&LuaQuestState::AddConversationLineToHero;
    lua["quest"]=&state;lua["actor"]=&questActor;unsigned cases=0;
    for(int alias=0;alias<3;++alias)for(bool snap:{false,true})for(const std::string fault:{"","key.new","lookup","face","output.close","key.close"}){
        questEvents.clear();questAliasMode=alias;questSnap=snap;questFault=fault;lua["snap"]=snap;
        auto outcome=lua.safe_script("quest:FaceThingByScriptName(actor,'NOVI_Theresa',snap)",sol::script_pass_on_error);check(outcome.valid()==fault.empty());
        if(!outcome.valid()){sol::error e=outcome;check(std::string(e.what()).find("QUEST_"+fault)!=std::string::npos);}
        check(!questOutput&&strings.empty());std::vector<std::string> expected{"key.new"};
        if(fault!="key.new"){expected.push_back("lookup");if(fault!="lookup"){expected.push_back("face");expected.push_back("output.close");}expected.push_back("key.close");}
        check(questEvents==expected);++cases;
    }
    questFault="";questEvents.clear();lua.script("assert(quest:StartConversationWithHero(actor,false,true)==-7)");
    check(questEvents==std::vector<std::string>({"conversation","hero","person"}));++cases;
    for(const std::string fault:{"","hero","line"}){
        questEvents.clear();questFault=fault;auto outcome=lua.safe_script("quest:AddConversationLineToHero(-7,'line',actor,false)",sol::script_pass_on_error);check(outcome.valid()==fault.empty());
        check(strings.empty());check(questEvents==(fault=="hero"?std::vector<std::string>{"key.new","hero","key.close"}:std::vector<std::string>{"key.new","hero","line","key.close"}));++cases;
    }
    lua.script(R"(
        for _,value in ipairs({0.0,-0.0,1.25,-83.125,math.huge,-math.huge}) do
            quest:SetStateFloat('position',value)
            local result=quest:GetStateFloat('position')
            assert(result==value)
            if value==0 then assert(1/result==1/value) end
        end
        quest:SetStateFloat('nan',0/0);assert(quest:GetStateFloat('nan')~=quest:GetStateFloat('nan'))
        assert(quest:GetStateFloat('missing')==0)
        assert(not pcall(function()quest:SetStateFloat('WatchTimer',2.5)end))
    )");
    for(unsigned bits:{0u,0x80000000u,0x3fa00000u,0xc2a64000u,0x7f800000u,0xff800000u}){
        float value;std::memcpy(&value,&bits,4);state.SetStateFloat("bits",value);check(static_cast<unsigned>(state.ints.at("bits"))==bits);++cases;
    }
    std::cout<<"PASS: "<<cases<<" quest scope/float policies plus Lua float special-value and owned-timer rejection checks\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
