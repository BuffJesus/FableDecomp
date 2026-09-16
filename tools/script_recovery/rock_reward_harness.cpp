#define main resource_smoke_main
#include "runtime_checks/retail_resources_smoke.cpp"
#undef main
#include <cstring>
#include <fstream>
DWORD g_fableBase=0;
static unsigned char definitionA[0x738]{},definitionB[0x738]{};
static const unsigned char** definitionSlot;
static CScriptThing target{};
static int calls, conversions, errorItem;
static bool replaceDefinition, added, continued, cancel;
static std::vector<std::string> events;
class LuaQuestState {
public:
    CGameScriptInterfaceBase* m_pGameInterface=reinterpret_cast<CGameScriptInterfaceBase*>(1);
    void AddRockTrollReward(CScriptThing*,int);
};
#include "rock_reward_adapter.inc"
static CCharString* __fastcall convertReward(const void* field,void*,CCharString* output) {
    check(strings.empty() && locals.size()==1);
    int token;std::memcpy(&token,field,4);++conversions;
    if(token==99)throw std::runtime_error("CONVERSION");
    std::string name=token==-1?"":token==1?"REWARD_A":token==2?"REWARD_B":"REWARD_CHANGED";
    strings[output]=name;
    output->pStringData=token==-1?nullptr:reinterpret_cast<decltype(output->pStringData)>(1);
    events.push_back("new:"+name);return output;
}
static void __fastcall destroyReward(CCharString* item,void*) {
    check(locals.size()==1 && strings.size()==1);
    events.push_back("destroy:"+strings.at(item));strings.erase(item);
}
static void __fastcall addReward(CGameScriptInterfaceBase* game,void*,const CScriptThing* actor,const CCharString* item) {
    check(game==reinterpret_cast<CGameScriptInterfaceBase*>(1) && actor==&target && locals.size()==1 && strings.size()==1);
    ++calls;events.push_back("add:"+strings.at(const_cast<CCharString*>(item)));
    if(calls==1 && replaceDefinition)*definitionSlot=definitionB;
    if(calls==errorItem)throw std::runtime_error("CONSUMER");
}
tAddItemToContainer AddItemToContainer_API=reinterpret_cast<tAddItemToContainer>(&addReward);
int main() {
    void* arena=nullptr;
    try {
        // Private test-process address arena. The adapter's real ASLR accesses are
        // exercised unchanged; the conversion address jumps to an engine double.
        arena=VirtualAlloc(nullptr,0x1040000,MEM_RESERVE,PAGE_NOACCESS);check(arena!=nullptr);
        g_fableBase=reinterpret_cast<DWORD>(arena);
        auto* code=static_cast<unsigned char*>(VirtualAlloc(static_cast<char*>(arena)+0x15000,4096,MEM_COMMIT,PAGE_READWRITE));check(code!=nullptr);
        auto* entry=code+0xd70;entry[0]=0xe9;
        DWORD delta=reinterpret_cast<DWORD>(&convertReward)-reinterpret_cast<DWORD>(entry)-5;std::memcpy(entry+1,&delta,4);
        DWORD oldProtect;check(VirtualProtect(code,4096,PAGE_EXECUTE_READ,&oldProtect)!=0);
        check(FlushInstructionCache(GetCurrentProcess(),code,4096)!=0);
        check(VirtualAlloc(static_cast<char*>(arena)+0x103e000,4096,MEM_COMMIT,PAGE_READWRITE)!=nullptr);
        definitionSlot=ASLR<const unsigned char**>(0x143e90c);
        int changed=3;std::memcpy(definitionB+0x734,&changed,4);
        CCharString_Destroy=reinterpret_cast<tCCharString_Destructor>(&destroyReward);
        unsigned cases=0;
        for(int first:{1,-1,99})for(int second:{2,-1})for(bool replace:{false,true})for(int failure:{0,1,2}) {
            check(strings.empty()&&locals.empty());events.clear();calls=conversions=0;added=continued=false;cancel=false;
            errorItem=failure;replaceDefinition=replace;*definitionSlot=definitionA;
            std::memcpy(definitionA+0x730,&first,4);std::memcpy(definitionA+0x734,&second,4);
            sol::state lua;lua.open_libraries(sol::lib::base);RegisterRetailResources(lua);
            auto type=lua.new_usertype<LuaQuestState>("RewardQuest",sol::no_constructor);
            type["AddRockTrollReward"]=&LuaQuestState::AddRockTrollReward;
            type["WithRetailResources"]=[](LuaQuestState& quest,sol::protected_function cb){WithRetailResources(quest.m_pGameInterface,cb);};
            type["GetStateBool"]=[](LuaQuestState&,const std::string& name){check(name=="AddedItemsToRockTroll");return added;};
            type["SetStateBool"]=[](LuaQuestState&,const std::string& name,bool value){check(name=="AddedItemsToRockTroll"&&strings.empty()&&locals.size()==1);added=value;};
            type["IsActiveThreadTerminating"]=[](LuaQuestState&){return cancel;};
            LuaQuestState quest;lua["quest"]=&quest;lua["me"]=&target;
            lua["continue_live"]=[](){check(added&&locals.size()==1&&strings.empty());continued=true;};
            auto load=lua.safe_script_file("rock_rewards.lua",sol::script_pass_on_error);check(load.valid());
            auto result=lua.safe_script(R"(
                quest:WithRetailResources(function(resources)
                    local self=resources:NewResource()
                    resources:PrepareResource(self)
                    assert(resources:TryAcquire(self,me,4))
                    WithRockTrollRewardsPhase(quest,me,continue_live)
                end)
            )",sol::script_pass_on_error);
            bool success=first!=99 && failure==0;
            check(result.valid()==success && added==success && continued==success);
            check(strings.empty()&&locals.empty());
            if(first==99){check(calls==0&&conversions==1&&events.empty());}
            else {
                std::string a=first==-1?"":"REWARD_A",b=replace?"REWARD_CHANGED":second==-1?"":"REWARD_B";
                std::vector<std::string> expected={"new:"+a,"add:"+a,"destroy:"+a};
                if(failure!=1)expected.insert(expected.end(),{"new:"+b,"add:"+b,"destroy:"+b});
                check(events==expected);
            }
            ++cases;
        }
        LuaQuestState quest;bool rejected=false;
        try{quest.AddRockTrollReward(&target,3);}catch(const std::runtime_error&){rejected=true;}check(rejected);
        *definitionSlot=nullptr;rejected=false;
        try{quest.AddRockTrollReward(&target,1);}catch(const std::runtime_error&){rejected=true;}check(rejected);
        check(VirtualFree(arena,0,MEM_RELEASE)!=0);arena=nullptr;
        std::cout<<"Rock reward x86: "<<cases<<" real-Lua lifetime policies and 2 input guards passed\n";
        return 0;
    } catch(const std::exception& error) {
        if(arena)VirtualFree(arena,0,MEM_RELEASE);
        std::cerr<<error.what()<<'\n';return 1;
    }
}
