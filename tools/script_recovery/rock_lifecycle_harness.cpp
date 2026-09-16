#define main resource_smoke_main
#include "runtime_checks/retail_resources_smoke.cpp"
#undef main
DWORD g_fableBase=0;
static CScriptThing troll{};
static CCPPointerInfo trollInfo{};
static int failure;
static std::vector<std::string> events;
static void __fastcall persistent(CGameScriptInterfaceBase*,void*,const CScriptThing* actor,bool value) {
    check(actor==&troll && value && strings.empty());events.push_back("persistent");
    if(failure==1)throw std::runtime_error("PERSISTENT");
}
static void __fastcall marker(CGameScriptInterfaceBase*,void*,const CScriptThing* actor,const CCharString* name) {
    check(actor==&troll && strings.size()==1 && strings.at(const_cast<CCharString*>(name))=="HUD_ORB_RED_SMALL");
    events.push_back("marker");if(failure==2)throw std::runtime_error("MARKER");
}
static void __fastcall markerStringCtor(CCharString* self,void*,const char* text,int length) {
    check(events==std::vector<std::string>{"persistent"});events.push_back("string.new");
    stringCtor(self,nullptr,text,length);
}
static void __fastcall markerStringDtor(CCharString* self,void*) {
    events.push_back("string.destroy");stringDtor(self,nullptr);
}
tSetThingPersistent SetThingPersistent_API=reinterpret_cast<tSetThingPersistent>(&persistent);
tMiniMapAddMarker MiniMapAddMarker_API=reinterpret_cast<tMiniMapAddMarker>(&marker);
class LuaQuestState {public:
    CGameScriptInterfaceBase* m_pGameInterface=reinterpret_cast<CGameScriptInterfaceBase*>(1);
    void InitializeRockTrollMarker(CScriptThing*);
};
#include "rock_lifecycle_adapter.inc"
int main() {
    try {
        CCharString_Construct_Literal=reinterpret_cast<tCCharString_Constructor_Literal>(&markerStringCtor);
        CCharString_Destroy=reinterpret_cast<tCCharString_Destructor>(&markerStringDtor);
        troll.pImp.Info=&trollInfo;trollInfo.RefCount=17;
        int cases=0;
        for(bool empty:{false,true})for(int error=0;error<3;++error) {
            // Null vtable deliberately makes an added IsNull dereference fail.
            troll.pVTable=nullptr;troll.pImp.Data=empty?nullptr:reinterpret_cast<decltype(troll.pImp.Data)>(0x1234);
            failure=error;events.clear();
            sol::state lua;lua.open_libraries(sol::lib::base);RegisterRetailResources(lua);
            auto type=lua.new_usertype<LuaQuestState>("MarkerQuest",sol::no_constructor);
            type["InitializeRockTrollMarker"]=&LuaQuestState::InitializeRockTrollMarker;
            type["WithRetailResources"]=[](LuaQuestState& q,sol::protected_function cb){WithRetailResources(q.m_pGameInterface,cb);};
            LuaQuestState quest;lua["quest"]=&quest;lua["me"]=&troll;
            check(lua.safe_script_file("rock_lifecycle.lua",sol::script_pass_on_error).valid());
            auto result=lua.safe_script(R"(
                quest:WithRetailResources(function(resources)
                    local resource=resources:NewResource()
                    resources:PrepareResource(resource)
                    assert(resources:TryAcquire(resource,me,4))
                    Init(quest,me)
                    OnPredicateFail(quest,me)
                end)
            )",sol::script_pass_on_error);
            check(result.valid()==(error==0) && locals.empty() && strings.empty() && trollInfo.RefCount==17);
            std::vector<std::string> expected={"persistent"};
            if(error!=1)expected.insert(expected.end(),{"string.new","marker","string.destroy"});
            check(events==expected);
            if(error){sol::error e=result;check(std::string(e.what()).find(error==1?"PERSISTENT":"MARKER")!=std::string::npos);}
            ++cases;
        }
        std::cout<<"Rock lifecycle x86: "<<cases<<" bound-Thing Lua policies passed\n";return 0;
    }catch(const std::exception& error){std::cerr<<error.what()<<'\n';return 1;}
}
