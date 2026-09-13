// Actual ambient-conversation method and binding, with native factory doubles.
#define CONVERSATION_LINES_NO_MAIN
#include "conversation_lines.cpp"
static unsigned creations=0,participants=0;
static bool want2D=false,wantCutscene=false;
static int factoryResult=42;
static int __fastcall addNew(CGameScriptInterfaceBase*,void*,const CScriptThing* speaker,bool sound2D,bool cutscene){
    check(speaker==expectedSpeaker && sound2D==want2D && cutscene==wantCutscene);++creations;return factoryResult;
}
static void __fastcall addPerson(CGameScriptInterfaceBase*,void*,int id,const CScriptThing* listener){
    check(id==42 && listener==expectedListener);++participants;
}
tAddNewConversation AddNewConversation_API=reinterpret_cast<tAddNewConversation>(&addNew);
tAddPersonToConversation AddPersonToConversation_API=reinterpret_cast<tAddPersonToConversation>(&addPerson);
#include "ambient-method.inc"
int main(){
    try {
        sol::state lua;lua.open_libraries(sol::lib::base);
        lua.new_usertype<CScriptThing>("Thing",sol::no_constructor);
        auto questState_type=lua.new_usertype<LuaQuestState>("Quest",sol::no_constructor);
#include "ambient-registration.inc"
        LuaQuestState quest(nullptr,reinterpret_cast<CGameScriptInterfaceBase*>(1));lua["quest"]=&quest;
        CScriptThing me{};auto hero=std::make_shared<CScriptThing>();auto owned=std::make_shared<CScriptThing>();
        lua["me"]=&me;lua["hero"]=hero;lua["owned"]=owned;
        expectedSpeaker=owned.get();expectedListener=hero.get();
        lua.script("assert(quest:StartAmbientConversation(owned,hero)==42)");
        check(creations==1 && participants==1);
        std::cout<<"PASS owned participants and default booleans"<<std::endl;
        expectedSpeaker=&me;
        lua.script("assert(quest:StartAmbientConversation(me,hero,false,false)==42)");
        check(creations==2 && participants==2);
        std::cout<<"PASS borrowed bound speaker with owned hero"<<std::endl;
        expectedSpeaker=hero.get();expectedListener=&me;want2D=wantCutscene=true;
        lua.script("assert(quest:StartAmbientConversation(hero,me,true,true)==42)");
        check(creations==3 && participants==3);
        want2D=wantCutscene=false;
        lua.script("assert(quest:StartAmbientConversation(hero,me)==42)");
        check(creations==4 && participants==4);
        auto heroOwners=hero.use_count(),speakerOwners=owned.use_count();
        lua["expect_flags"]=[](bool first,bool second){want2D=first;wantCutscene=second;};
        lua.script("for _,flags in ipairs({{false,false},{false,true},{true,false},{true,true}}) do expect_flags(flags[1],flags[2]);assert(quest:StartAmbientConversation(hero,me,flags[1],flags[2])==42) end");
        want2D=true;wantCutscene=false;lua.script("assert(quest:StartAmbientConversation(hero,me,true)==42)");
        want2D=false;lua.script("assert(quest:StartAmbientConversation(hero,me,nil,nil)==42)");
        check(creations==10 && participants==10 && hero.use_count()==heroOwners && owned.use_count()==speakerOwners);
        lua.script("assert(quest:StartAmbientConversation(nil,hero)==-1);assert(quest:StartAmbientConversation(me,nil)==-1);assert(quest:StartAmbientConversation({},hero)==-1)");
        check(creations==10 && participants==10);
        factoryResult=-1;
        lua.script("assert(quest:StartAmbientConversation(hero,me,false,false)==-1)");
        check(creations==11 && participants==10);
        AddNewConversation_API=nullptr;
        lua.script("assert(quest:StartAmbientConversation(hero,me)==-1)");
        check(creations==11 && participants==10);
        std::cout<<"PASS reversed ownership, explicit booleans, nil participants, factory/API failure"<<std::endl;
        return 0;
    }catch(const std::exception& e){std::cerr<<e.what()<<std::endl;return 1;}
}
