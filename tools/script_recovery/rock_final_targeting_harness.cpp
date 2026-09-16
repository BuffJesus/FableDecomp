#define main resource_smoke_main
#include "runtime_checks/retail_resources_smoke.cpp"
#undef main
DWORD g_fableBase=0;
static CScriptThing troll{},heroA{},heroB{};
static CCPPointerInfo infoA{},infoB{},infoTroll{};
static bool nullA,nullB;
static int queries,failure;
static std::vector<std::string> events;
static CScriptThing* __fastcall getHero(CGameScriptInterfaceBase*,void*) {
    ++queries;events.push_back("hero"+std::to_string(queries));
    check(queries<=2);return queries==1?(nullA?nullptr:&heroA):(nullB?nullptr:&heroB);
}
static void __fastcall look(CGameScriptInterfaceBase*,void*,const CScriptThing* source,const CScriptThing* target) {
    check(source==&troll && queries==1 && target==(nullA?nullptr:&heroA));events.push_back("look");
    if(failure==1)throw std::runtime_error("LOOK");
}
static void __fastcall enemy(CGameScriptInterfaceBase*,void*,const CScriptThing* source,const CScriptThing* target) {
    check(source==&troll && queries==2 && target==(nullB?nullptr:&heroB));events.push_back("enemy");
    if(failure==2)throw std::runtime_error("ENEMY");
}
tGetHero GetHero_API=reinterpret_cast<tGetHero>(&getHero);
tEntityForceToLookAtThing EntityForceToLookAtThing_API=reinterpret_cast<tEntityForceToLookAtThing>(&look);
tGiveThingBestEnemyTarget GiveThingBestEnemyTarget_API=reinterpret_cast<tGiveThingBestEnemyTarget>(&enemy);
class LuaQuestState {public:
    CGameScriptInterfaceBase* m_pGameInterface=reinterpret_cast<CGameScriptInterfaceBase*>(1);
    void TargetRockTrollAtHero(CScriptThing*);
};
#include "rock_final_targeting_adapter.inc"
int main() {
    try {
        heroA.pImp.Info=&infoA;heroB.pImp.Info=&infoB;troll.pImp.Info=&infoTroll;
        infoA.RefCount=7;infoB.RefCount=9;infoTroll.RefCount=11;
        int cases=0;
        for(bool a:{false,true})for(bool b:{false,true})for(int error=0;error<3;++error) {
            nullA=a;nullB=b;failure=error;queries=0;events.clear();
            sol::state lua;lua.open_libraries(sol::lib::base);RegisterRetailResources(lua);
            auto type=lua.new_usertype<LuaQuestState>("TargetQuest",sol::no_constructor);
            type["TargetRockTrollAtHero"]=&LuaQuestState::TargetRockTrollAtHero;
            type["WithRetailResources"]=[](LuaQuestState& quest,sol::protected_function cb){WithRetailResources(quest.m_pGameInterface,cb);};
            LuaQuestState quest;lua["quest"]=&quest;lua["me"]=&troll;
            auto result=lua.safe_script(R"(
                quest:WithRetailResources(function(resources)
                    local self=resources:NewResource()
                    resources:PrepareResource(self)
                    assert(resources:TryAcquire(self,me,4))
                    quest:TargetRockTrollAtHero(me)
                end)
            )",sol::script_pass_on_error);
            check(result.valid()==(error==0) && locals.empty());
            std::vector<std::string> expected={"hero1","look"};
            if(error!=1)expected.insert(expected.end(),{"hero2","enemy"});
            check(events==expected && infoA.RefCount==7 && infoB.RefCount==9 && infoTroll.RefCount==11);
            if(error){sol::error e=result;check(std::string(e.what()).find(error==1?"LOOK":"ENEMY")!=std::string::npos);}
            ++cases;
        }
        std::cout<<"Rock final targeting x86: "<<cases<<" raw borrowed-target Lua policies passed\n";return 0;
    }catch(const std::exception& error){std::cerr<<error.what()<<'\n';return 1;}
}
