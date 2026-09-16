#include "LuaRetailResources.h"
#include <cstring>
#include <iostream>
#include <vector>
DWORD g_fableBase=0;
static CGameScriptInterfaceBase game{};
static CScriptThing actor{},hero{};
static CCharString suffix{},prefixAlias{},resultAlias{};
static CCharString *prefixLocal=nullptr,*resultLocal=nullptr;
static bool male,hit,alias,empty,nullHero,failLine,failCleanup,ambient;
static int conversation;
static int heroQueries;
static std::vector<std::string> events;
static void check(bool ok){if(!ok)throw std::runtime_error("Villager text check failed");}
static CCharString* __fastcall assign(CCharString* out,void*,const char* text){
    check(out==&suffix && std::string(text)==(male?"_MALE":"_FEMALE"));events.emplace_back("assign");return out;
}
static CScriptThing* __fastcall getHero(CGameScriptInterfaceBase* receiver,void*){
    check(receiver==&game && !prefixLocal && !resultLocal);events.emplace_back("hero");++heroQueries;return nullHero?nullptr:(heroQueries%2?&hero:&actor);
}
static void __fastcall face(CGameScriptInterfaceBase* receiver,void*,const CScriptThing* first,const CScriptThing* second,bool flag){
    check(receiver==&game && first==&actor && second==(nullHero?nullptr:&hero) && !flag);events.emplace_back("face");
}
static int __fastcall create(CGameScriptInterfaceBase* receiver,void*,const CScriptThing* speaker,bool first,bool second){
    check(receiver==&game && speaker==&actor && !first && !second);events.emplace_back("conversation");return conversation;
}
static void __fastcall person(CGameScriptInterfaceBase* receiver,void*,int id,const CScriptThing* participant){
    check(receiver==&game && id==conversation && participant==(nullHero?nullptr:&actor));events.emplace_back("person");
}
static CCharString* __fastcall literal(CCharString* out,void*,const char* text,int length){
    check(events.back()=="hero" && length==-1 && std::string(text)==(hit?
        "TEXT_QST_048_VILLAGER_DONE_BAD_DEEDS":"TEXT_QST_048_VILLAGER_SPOKEN_TO"));
    prefixLocal=out;out->pStringData=reinterpret_cast<decltype(out->pStringData)>(empty?0:1);
    events.emplace_back("prefix");return alias?&prefixAlias:out;
}
static CCharString* __fastcall concat(CCharString* out,const CCharString* left,const CCharString* right){
    check(left==(alias?&prefixAlias:prefixLocal) && right==&suffix && !resultLocal);
    resultLocal=out;out->pStringData=reinterpret_cast<decltype(out->pStringData)>(empty?0:2);
    events.emplace_back("concat");return alias?&resultAlias:out;
}
static void __fastcall line(CGameScriptInterfaceBase* receiver,void*,int id,const CCharString* text,bool flag,const CScriptThing* speaker,const CScriptThing* listener){
    check(receiver==&game && id==conversation && text==(ambient?&suffix:alias?&resultAlias:resultLocal) && !flag && speaker==&actor && listener==(nullHero?nullptr:&hero));
    events.emplace_back("line");if(failLine)throw std::runtime_error("LINE");
}
static void __fastcall destroy(CCharString* value,void*){
    if(value==resultLocal){check(prefixLocal!=nullptr);resultLocal=nullptr;events.emplace_back("result.destroy");if(failCleanup)throw std::runtime_error("CLEANUP");}
    else {check(value==prefixLocal && !resultLocal);prefixLocal=nullptr;events.emplace_back("prefix.destroy");}
}
tGetHero GetHero_API=reinterpret_cast<tGetHero>(&getHero);
tEntitySetFacingAngleTowardsThing EntitySetFacingAngleTowardsThing_API=reinterpret_cast<tEntitySetFacingAngleTowardsThing>(&face);
tAddNewConversation AddNewConversation_API=reinterpret_cast<tAddNewConversation>(&create);
tAddPersonToConversation AddPersonToConversation_API=reinterpret_cast<tAddPersonToConversation>(&person);
tAddLineToConversation AddLineToConversation_API=reinterpret_cast<tAddLineToConversation>(&line);
tCCharString_AssignmentLiteral CCharString_AssignLiteral_API=reinterpret_cast<tCCharString_AssignmentLiteral>(&assign);
tCCharString_Destructor CCharString_Destroy=reinterpret_cast<tCCharString_Destructor>(&destroy);
struct Scope {
    CGameScriptInterfaceBase* m_game=&game;
    enum class Kind {Text};
    struct Entry {CCharString& text;} entry{suffix};
    bool closed=false;
    void CheckOpen(){if(closed)throw std::runtime_error("HANDLE");}
    void Speak(unsigned id,CScriptThing* target,const std::string& key,int selection,bool listen,bool sound2D,bool overFade){
        check(id==5 && target==(nullHero?nullptr:&hero) && key==(male?"TEXT_QST_048_VILLAGER_ATTACKED_MALE":"TEXT_QST_048_VILLAGER_ATTACKED_FEMALE") && selection==1 && !listen && sound2D && !overFade);
        events.emplace_back("speak");
    }
    Entry& Get(unsigned id,Kind){if(closed || id!=7)throw std::runtime_error("HANDLE");return entry;}
#include "retail_villager_text_actions.inc"
#include "retail_villager_speech.inc"
};
static void jump(DWORD address,void* target){auto* code=ASLR<unsigned char*>(address);code[0]=0xE9;
    DWORD delta=reinterpret_cast<DWORD>(target)-(reinterpret_cast<DWORD>(code)+5);std::memcpy(code+1,&delta,4);FlushInstructionCache(GetCurrentProcess(),code,5);}
int main(){try{
    static_assert(sizeof(void*)==4);
    auto* memory=VirtualAlloc(nullptr,0x600000,MEM_RESERVE|MEM_COMMIT,PAGE_EXECUTE_READWRITE);check(memory!=nullptr);
    g_fableBase=reinterpret_cast<DWORD>(memory);jump(0x99EBF0,reinterpret_cast<void*>(&literal));jump(0x99F570,reinterpret_cast<void*>(&concat));
    Scope scope;sol::state lua;lua.open_libraries(sol::lib::base,sol::lib::string);
    auto type=lua.new_usertype<Scope>("Scope",sol::no_constructor);
    type["AssignVillagerSuffix"]=&Scope::AssignVillagerSuffix;type["AddVillagerTalkLine"]=&Scope::AddVillagerTalkLine;
    type["StartVillagerTalkConversation"]=&Scope::StartVillagerTalkConversation;
    type["SpeakVillagerAttacked"]=&Scope::SpeakVillagerAttacked;
    type["AddVillagerAmbientText"]=&Scope::AddVillagerAmbientText;
    lua["scope"]=&scope;lua["actor"]=&actor;int cases=0;
    for(bool m:{false,true})for(bool h:{false,true})for(bool a:{false,true})for(bool e:{false,true})for(bool n:{false,true})for(int id:{-1,0,73}){
        male=m;hit=h;alias=a;empty=e;nullHero=n;conversation=id;heroQueries=0;events.clear();lua["male"]=m;lua["hit"]=h;lua["id"]=id;
        lua.script("scope:AssignVillagerSuffix(7,male);assert(scope:StartVillagerTalkConversation(actor)==id);scope:AddVillagerTalkLine(id,7,actor,hit)");
        check(!prefixLocal && !resultLocal && &scope.Get(7,Scope::Kind::Text).text==&suffix);
        check(events==std::vector<std::string>{"assign","hero","face","conversation","hero","person","hero","prefix","concat","line","result.destroy","prefix.destroy"});
        heroQueries=0;events.clear();lua.script("scope:SpeakVillagerAttacked(5,male)");
        check(events==std::vector<std::string>{"hero","speak"});
        ambient=true;heroQueries=0;events.clear();lua.script("scope:AddVillagerAmbientText(id,7,actor)");
        check(events==std::vector<std::string>{"hero","line"} && !prefixLocal && !resultLocal);
        ambient=false;++cases;
    }
    for(bool bodyError:{false,true})for(bool cleanupError:{false,true}){
        if(!bodyError && !cleanupError)continue;failLine=bodyError;failCleanup=cleanupError;heroQueries=0;events.clear();lua["expected"]=bodyError?"LINE":"CLEANUP";
        lua.script("local ok,e=pcall(function() scope:AddVillagerTalkLine(id,7,actor,hit) end);assert(not ok and string.find(e,expected,1,true))");
        check(!prefixLocal && !resultLocal && events==std::vector<std::string>{"hero","prefix","concat","line","result.destroy","prefix.destroy"});++cases;
    }
    scope.closed=true;events.clear();lua.script("local ok,e=pcall(function() scope:AddVillagerTalkLine(id,7,actor,hit) end);assert(not ok and string.find(e,'HANDLE',1,true))");check(events.empty());++cases;
    std::cout<<"PASS: "<<cases<<" Villager text policies; suffix ownership, native operands and reverse cleanup\n";
    VirtualFree(memory,0,MEM_RELEASE);return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
