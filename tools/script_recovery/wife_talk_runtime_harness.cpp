#include "LuaEntityAPI.h"
#include "FableAPI.h"
#include <iostream>
#include <vector>
#include <stdexcept>
#include "wife_talk_runtime_body.inc"

// Only the query method is under test; no host is attached to this API instance.
LuaEntityAPI::~LuaEntityAPI() {}
static std::vector<std::string> events;
static CCharString* live=nullptr;
static bool answer,failQuery;
static CScriptThing actor{};
static void check(bool value) {if(!value)throw std::runtime_error("talk check failed");}
static void __fastcall construct(CCharString* value,void*,const char* text,int length) {
    check(!live && std::string(text)=="SCRIPT_NAME_HERO" && length==-1);
    live=value;value->pStringData=reinterpret_cast<decltype(value->pStringData)>(1);
    events.push_back("construct");
}
static void __fastcall destroy(CCharString* value,void*) {
    check(value==live);live=nullptr;events.push_back("destroy");
}
static bool __fastcall query(CScriptThing* value,void*,const CCharString* text) {
    check(value==&actor && text==live);events.push_back("query");
    if(failQuery)throw std::runtime_error("query failure");
    return answer;
}
tCCharString_Constructor_Literal CCharString_Construct_Literal=reinterpret_cast<tCCharString_Constructor_Literal>(&construct);
tCCharString_Destructor CCharString_Destroy=reinterpret_cast<tCCharString_Destructor>(&destroy);
int main() {
    try {
        static_assert(sizeof(void*)==4);
        static_assert(offsetof(CScriptThingVTable,MsgIsTalkedToBy)==0x6c);
        LuaEntityAPI api;CScriptThingVTable table{};
        table.MsgIsTalkedToBy=reinterpret_cast<tCScriptThing_MsgIsTalkedToBy>(&query);
        actor.pVTable=reinterpret_cast<void**>(&table);
        sol::state lua;lua.open_libraries(sol::lib::base,sol::lib::string);
        lua.set_function("talkedTo",[&](){return api.IsTalkedToByHero(&actor);});
        for(bool value:{false,true}) {
            events.clear();answer=value;lua["expected"]=value;
            lua.script("assert(talkedTo()==expected)");
            check(!live && events==std::vector<std::string>{"construct","query","destroy"});
        }
        events.clear();failQuery=true;
        lua.script("local ok,err=pcall(talkedTo); assert(not ok and string.find(err,'query failure',1,true))");
        check(!live && events==std::vector<std::string>{"construct","query","destroy"});
        events.clear();check(!api.IsTalkedToByHero(nullptr));
        actor.pVTable=nullptr;check(!api.IsTalkedToByHero(&actor));
        actor.pVTable=reinterpret_cast<void**>(&table);table.MsgIsTalkedToBy=nullptr;
        check(!api.IsTalkedToByHero(&actor));check(events.empty());
        std::cout<<"PASS: unchanged FSE talk method, native slot and string identity, true/false and exception cleanup, invalid-actor guards\n";
        return 0;
    } catch(const std::exception& error) {std::cerr<<error.what()<<'\n';return 1;}
}
