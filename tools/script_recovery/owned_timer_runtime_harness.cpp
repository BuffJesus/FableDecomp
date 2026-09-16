#include "retail_owned_timer.h"
#include <iostream>
#include <vector>

static CGameScriptInterfaceBase games[4]{};
static CGameScriptInterfaceBase* current=&games[0];
DWORD g_fableBase=reinterpret_cast<DWORD>(&current)-(0x0143E8F8-0x400000);
static int timerId,remaining;
static bool failClose;
static std::vector<std::string> events;
static void check(bool value) {if(!value)throw std::runtime_error("timer check failed");}
static int __fastcall create(CGameScriptInterfaceBase* game,void*) {
    check(game==&games[0]);events.push_back("register");current=&games[1];return timerId;
}
static void __fastcall set(CGameScriptInterfaceBase* game,void*,int id,int value) {
    check(game==&games[1] && id==timerId && value==2);events.push_back("set");current=&games[2];
}
static int __fastcall get(CGameScriptInterfaceBase* game,void*,int id) {
    check(game==&games[2] && id==timerId);events.push_back("get");current=&games[3];return remaining;
}
static void __fastcall close(CGameScriptInterfaceBase* game,void*,int id) {
    check(game==current && id==timerId);events.push_back("deregister");
    if(failClose)throw std::runtime_error("CLOSE");
}
int main() {
    try {
        static_assert(sizeof(void*)==4);
        void* table[0x16C/4]{};
        table[0x15C/4]=reinterpret_cast<void*>(&create);table[0x160/4]=reinterpret_cast<void*>(&close);
        table[0x164/4]=reinterpret_cast<void*>(&set);table[0x168/4]=reinterpret_cast<void*>(&get);
        for(auto& game:games)*reinterpret_cast<void***>(&game)=table;
        sol::state lua;lua.open_libraries(sol::lib::base,sol::lib::string);
        RegisterRetailOwnedTimer(lua);lua.set_function("withTimer",&WithRetailOwnedTimer);
        int cases=0;
        for(int id:{0,1,73,-1}) for(int value:{-1,0,2}) for(bool continuation:{false,true}) {
            current=&games[0];events.clear();timerId=id;remaining=value;
            lua["expected"]=value;lua["continued"]=continuation;
            lua.script("assert(withTimer(function(timer) escaped=timer; timer:Set(2); assert(timer:Get()==expected); return continued end)==continued)");
            check(events==std::vector<std::string>{"register","set","get","deregister"});
            lua.script("local ok,err=pcall(function() escaped:Get() end); assert(not ok and string.find(err,'closed',1,true)); escaped=nil; collectgarbage() ");
            check(events.size()==4);++cases;
        }
        for(bool closeFailure:{false,true}) {
            current=&games[0];events.clear();failClose=closeFailure;
            lua.script("local ok,err=pcall(function() withTimer(function(timer) escaped=timer; error('BODY',0) end) end);assert(not ok and string.find(err,'BODY',1,true));escaped=nil;collectgarbage()");
            check(events==std::vector<std::string>{"register","deregister"});++cases;
        }
        failClose=false;current=&games[0];events.clear();
        lua.script("local ok,err=pcall(function() withTimer(function() return 1 end) end);assert(not ok and string.find(err,'continuation boolean',1,true))");
        check(events==std::vector<std::string>{"register","deregister"});++cases;
        std::cout<<"PASS: "<<cases<<" owned timer policies, fresh global receiver, IDs, signed results, escaped object and error cleanup\n";
        return 0;
    }catch(const std::exception& error){std::cerr<<error.what()<<'\n';return 1;}
}
