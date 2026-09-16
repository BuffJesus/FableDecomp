#define main resource_smoke_main
#include "runtime_checks/retail_resources_smoke.cpp"
#undef main
#include "rock_region_message.h"
DWORD g_fableBase=0;
static CCharString* buffer;
static int constructors, destructors, polls, mode;
static bool cleanupFault;
static CCharString* __fastcall construct(CCharString* self,void*) {
    check(!buffer); buffer=self; self->pStringData=nullptr; ++constructors; return self;
}
static void __fastcall destroy(CCharString* self,void*) {
    check(self==buffer); ++destructors; buffer=nullptr;
    if(cleanupFault) throw std::runtime_error("CLEANUP");
}
static bool __fastcall poll(CGameScriptInterfaceBase*,void*,CCharString* out) {
    check(out==buffer); ++polls;
    if(polls==1) { check(!out->pStringData); out->pStringData=reinterpret_cast<decltype(out->pStringData)>(1); return false; }
    check(out->pStringData); // Same native output survives the failed/populated poll.
    if(mode==2) throw std::runtime_error("POLL");
    out->pStringData=nullptr; return true; // Empty output still succeeds.
}
tMsgOnRegionLoaded MsgOnRegionLoaded_API=reinterpret_cast<tMsgOnRegionLoaded>(&poll);
class LuaQuestState { public:
    CGameScriptInterfaceBase* m_pGameInterface=reinterpret_cast<CGameScriptInterfaceBase*>(1);
    void WithRegionLoadedMessage(sol::protected_function);
};
#include "rock_region_message_adapter.inc"
int main() {
    try {
        g_fableBase=reinterpret_cast<DWORD>(&construct)-(0x99e4b0-0x400000);
        CCharString_Destroy=reinterpret_cast<tCCharString_Destructor>(&destroy);
        for(mode=0;mode<4;++mode) for(bool fault:{false,true}) {
            cleanupFault=fault; constructors=destructors=polls=0;
            sol::state lua; lua.open_libraries(sol::lib::base);
            RegisterRockRegionMessage(lua);
            auto type=lua.new_usertype<LuaQuestState>("Quest",sol::no_constructor);
            type["WithRegionLoadedMessage"]=&LuaQuestState::WithRegionLoadedMessage;
            LuaQuestState quest; lua["quest"]=&quest; lua["mode"]=mode;
            lua["observe_live"]=[](){check(buffer!=nullptr && destructors==0);};
            auto result=lua.safe_script(R"(
                quest:WithRegionLoadedMessage(function(message)
                    retained=message
                    assert(message:Poll()==false)
                    if mode==1 then return end -- cancellation at the intervening frame
                    assert(message:Poll()==true)
                    observe_live()
                    if mode==3 then error('BODY') end
                    completed=true -- success effects run before CString destruction
                end)
            )",sol::script_pass_on_error);
            check(constructors==1 && destructors==1 && buffer==nullptr);
            check(polls==(mode==1?1:2));
            if(mode>=2) { check(!result.valid()); sol::error error=result;
                check(std::string(error.what()).find(mode==2?"POLL":"BODY")!=std::string::npos); }
            else check(result.valid()==!fault);
            auto stale=lua.safe_script("retained:Poll()",sol::script_pass_on_error);
            check(!stale.valid() && destructors==1);
            lua.collect_garbage(); check(destructors==1);
        }
        std::cout<<"Rock region message x86: 8 lifetime/error policies passed\n";
        return 0;
    } catch(const std::exception& error) { std::cerr<<error.what()<<'\n'; return 1; }
}
