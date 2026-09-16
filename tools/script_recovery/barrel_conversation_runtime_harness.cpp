#include "LuaRetailResources.h"
#include <cstring>
#include <iostream>
#include <vector>
DWORD g_fableBase=0;
static CGameScriptInterfaceBase game{};
static CScriptThing actor{},alternate{};
static CCharString* liveKey;
static CScriptThing* liveListener;
static bool emptyKey,aliasTarget,failLine,failCleanup;
static int conversationId;
static std::string expectedText;
static std::vector<std::string> events;
static void check(bool b){if(!b)throw std::runtime_error("conversation check failed");}
static int __fastcall create(CGameScriptInterfaceBase* receiver,void*,const CScriptThing* speaker,bool a,bool b){
    check(receiver==&game && speaker==&actor && !a && !b && !liveKey && !liveListener);
    events.emplace_back("conversation");return conversationId;
}
static void __fastcall key(CCharString* value,void*,const char* text,int length){
    check(!liveKey && !liveListener && text==expectedText && length==-1);liveKey=value;
    value->pStringData=reinterpret_cast<decltype(value->pStringData)>(emptyKey?0:1);events.emplace_back("key");
}
static CScriptThing* __fastcall listener(CScriptThing* value,void*){
    check(liveKey && !liveListener);liveListener=value;events.emplace_back("listener");
    return aliasTarget?&alternate:value;
}
static void __fastcall line(CGameScriptInterfaceBase* receiver,void*,int id,const CCharString* text,bool flag,const CScriptThing* speaker,const CScriptThing* target){
    check(receiver==&game && id==conversationId && text==liveKey && !flag && speaker==&actor && target==(aliasTarget?&alternate:liveListener));
    events.emplace_back("line");if(failLine)throw std::runtime_error("LINE");
}
static void __fastcall destroyListener(CScriptThing* value,void*){
    check(value==liveListener && liveKey);liveListener=nullptr;events.emplace_back("listener.destroy");
    if(failCleanup)throw std::runtime_error("CLEANUP");
}
static void __fastcall destroyKey(CCharString* value,void*){
    check(value==liveKey && !liveListener);liveKey=nullptr;events.emplace_back("key.destroy");
}
tAddNewConversation AddNewConversation_API=reinterpret_cast<tAddNewConversation>(&create);
tAddLineToConversation AddLineToConversation_API=reinterpret_cast<tAddLineToConversation>(&line);
tCCharString_Constructor_Literal CCharString_Construct_Literal=reinterpret_cast<tCCharString_Constructor_Literal>(&key);
tCCharString_Destructor CCharString_Destroy=reinterpret_cast<tCCharString_Destructor>(&destroyKey);
tRetailThingDestroy RetailThing_Destroy_API=reinterpret_cast<tRetailThingDestroy>(&destroyListener);
struct Scope {
    CGameScriptInterfaceBase* m_game=&game;bool closed=false;
    void CheckOpen(){if(closed)throw std::runtime_error("CLOSED");}
#include "retail_barrel_conversation.inc"
};
int main(){try{
    static_assert(sizeof(void*)==4);
    auto* memory=VirtualAlloc(nullptr,0x400000,MEM_RESERVE|MEM_COMMIT,PAGE_EXECUTE_READWRITE);
    check(memory!=nullptr);g_fableBase=reinterpret_cast<DWORD>(memory);
    auto* code=ASLR<unsigned char*>(0x6E7B40);code[0]=0xE9;
    DWORD delta=reinterpret_cast<DWORD>(&listener)-(reinterpret_cast<DWORD>(code)+5);
    std::memcpy(code+1,&delta,4);FlushInstructionCache(GetCurrentProcess(),code,5);
    Scope scope;sol::state lua;lua.open_libraries(sol::lib::base,sol::lib::string);
    lua.set_function("say",[&](const std::string& text){scope.AddBarrelConversation(&actor,text);});int cases=0;
    for(const char* text:{"TEXT_QST_048_SCRMSG_BARRELMAN_WHERE_GONE","TEXT_QST_048_BARRELMAN_OVERHEAR"})
    for(int id:{-1,0,73})for(bool empty:{false,true})for(bool alias:{false,true}){
        expectedText=text;conversationId=id;emptyKey=empty;aliasTarget=alias;events.clear();lua["text"]=text;
        lua.script("say(text)");check(!liveKey && !liveListener);
        check(events==std::vector<std::string>{"conversation","key","listener","line","listener.destroy","key.destroy"});++cases;
    }
    for(bool bodyError:{false,true})for(bool cleanupError:{false,true}){
        if(!bodyError && !cleanupError)continue;
        failLine=bodyError;failCleanup=cleanupError;events.clear();lua["expected"]=bodyError?"LINE":"CLEANUP";
        lua.script("local ok,e=pcall(say,text);assert(not ok and string.find(e,expected,1,true))");
        check(!liveKey && !liveListener && events.size()==6);++cases;
    }
    scope.closed=true;events.clear();
    lua.script("local ok,e=pcall(say,text);assert(not ok and string.find(e,'CLOSED',1,true))");check(events.empty());++cases;
    std::cout<<"PASS: "<<cases<<" conversation policies; exact arguments, temporary order and original error preservation\n";
    VirtualFree(memory,0,MEM_RELEASE);return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
