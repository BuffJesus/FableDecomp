// Actual conversion helper, AddLine body and overloads; native queue is doubled.
#define main resource_smoke_main
#include "retail_resources_smoke.cpp"
#undef main
#include "LuaQuestState.h"
#include <sstream>

LuaQuestState::LuaQuestState(LuaQuestHost* parent,CGameScriptInterfaceBase* game)
    : m_pParentHost(parent),m_pGameInterface(game) {}
void LuaQuestState::Log(const std::string&) {}
static CScriptThing* expectedSpeaker = nullptr;
static CScriptThing* expectedListener = nullptr;
static std::string expectedText;
static bool expectedBoolean = false;
static unsigned calls = 0, before = 0;
static void __fastcall addLine(CGameScriptInterfaceBase*,void*,int id,const CCharString* key,
                              bool flag,const CScriptThing* speaker,const CScriptThing* listener) {
    check(id==42 && flag==expectedBoolean && speaker==expectedSpeaker);
    check(strings.at(const_cast<CCharString*>(key))==expectedText);
    if (expectedListener) check(listener==expectedListener);
    else check(listener && listener->pVTable==g_pCScriptThingVTable && !listener->pImp.Data && !listener->pImp.Info);
    ++calls;
}
tAddLineToConversation AddLineToConversation_API = reinterpret_cast<tAddLineToConversation>(&addLine);
#include "conversation-helper.inc"
#include "conversation-method.inc"

#ifndef CONVERSATION_LINES_NO_MAIN
int main(int argc,char** argv) {
    try {
        if(argc!=2) throw std::runtime_error("Expected conversation-line fixture");
        sol::state lua;lua.open_libraries(sol::lib::base,sol::lib::table,sol::lib::string);
        lua.new_usertype<CScriptThing>("Thing",sol::no_constructor);
        auto questState_type=lua.new_usertype<LuaQuestState>("Quest",sol::no_constructor);
#include "conversation-registration.inc"
        LuaQuestState quest(nullptr,reinterpret_cast<CGameScriptInterfaceBase*>(1));
        CScriptThing me{};auto hero=std::make_shared<CScriptThing>();auto partner=std::make_shared<CScriptThing>();
        lua["quest"]=&quest;lua["me"]=&me;lua["hero"]=hero;
        for(const auto* name:{"woman","man","victim","bully"})lua[name]=partner;
        lua["get_hero"]=[hero](){return hero;};
        questState_type["GetHero"]=[hero](LuaQuestState&){return hero;};
        lua["expect"] = [](const std::string& text,CScriptThing* speaker,CScriptThing* listener,bool flag) {
            expectedText=text;expectedSpeaker=speaker;expectedListener=listener;expectedBoolean=flag;before=calls;
        };
        lua["verify"] = [](){check(calls==before+1);};
        lua.script(R"(
            conv=42; n=10; suffix=""; IntimidateSpeechLoop=10
            key="OPERAND_PROBE"; line="OPERAND_PROBE"
            index=0; lines={"OPERAND_PROBE"}
            TEXT=setmetatable({}, {__index=function() return "OPERAND_PROBE" end})
            setmetatable(_G,{__index=function() return "OPERAND_PROBE" end})
        )");
        lua.script_file(argv[1]);
        check(calls==27 && strings.empty());
        // Explicit true and nil exercise overload/default handling separately.
        lua.script("expect('EXPLICIT',me,nil,true); quest:AddLineToConversation(42,'EXPLICIT',me,nil,true); verify()");
        lua.script("expect('NIL',me,nil,false); quest:AddLineToConversation(42,'NIL',me,nil,nil); verify()");
        auto count=calls;
        lua.script("quest:AddLineToConversation(42,'NO_SPEAKER',nil,hero)");
        check(calls==count && strings.empty());
        std::cout<<"PASS: 27 actual line expressions, participant ownership, empty nil listener, boolean defaults/explicit values\n";
        return 0;
    }catch(const std::exception& error){std::cerr<<error.what()<<"\n";return 1;}
}
#endif
