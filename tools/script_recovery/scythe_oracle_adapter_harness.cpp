// Real x86 API types and sol/Lua; only engine calls and the host shell are doubles.
#define main resource_smoke_main
#include "runtime_checks/retail_resources_smoke.cpp"
#undef main

static CScriptThing scytheThing{}, heroThing{};
static CScriptGameResourceObjectScriptedThingBase scytheControl{};
static std::set<void*> inputMaps;
static std::vector<std::string> events;
static bool throwMacro = false;
static bool populatedFailure = false;
namespace LuaEntityAPI {
static CScriptGameResourceObjectScriptedThingBase* FindControlHandle(CScriptThing* thing) {
    check(thing == &scytheThing); // A cached Hero handle must never be requested.
    return &scytheControl;
}
}
class LuaQuestState {
public:
    CGameScriptInterfaceBase* m_pGameInterface = reinterpret_cast<CGameScriptInterfaceBase*>(1);
    std::map<lua_State*, void*> m_movieHandlesByLuaState;
    void RunScytheOracleCutscene(CScriptThing*, sol::this_state);
};
static CScriptThing* __fastcall heroGet(CGameScriptInterfaceBase*, void*) { return &heroThing; }
static void __fastcall inputCtor(void* p, void*) { check(inputMaps.insert(p).second); }
static void __fastcall inputDestroy(void* p, void*) { check(inputMaps.erase(p) == 1); events.push_back("inputs.destroy"); }
static void __fastcall camera(CGameScriptInterfaceBase*, void*, bool value) { events.push_back(value ? "camera.on" : "camera.off"); }
static bool __fastcall acquireScythe(CGameScriptInterfaceBase* g, void*, const CScriptThing* thing,
        CScriptGameResourceObjectScriptedThingBase* r, EScriptAIPriority priority) {
    check(thing == &heroThing && static_cast<int>(priority) == 4);
    events.push_back("hero.attempt");
    const bool result = acquireOK;
    acquireOK = result || populatedFailure;
    acquire(g, nullptr, thing, r, priority);
    acquireOK = result;
    return result;
}
static void __fastcall oracleMacro(const CCharString* key, void* actors, void* flags, void* inputs, bool setup, bool skip) {
    check(strings.at(const_cast<CCharString*>(key)) == "CS_ORACLE_AWAKENS");
    check(!flags && !setup && skip && inputs && inputMaps.count(inputs));
    check(movies == 1 && paused && maps.at(actors).size() == 2);
    check(maps.at(actors).at("SCYTHE").pImp.Data == scytheControl.pImp.Data);
    check(bool(maps.at(actors).at("HERO").pImp.Data) == (acquireOK || populatedFailure));
    events.push_back("macro"); ++macros;
    if (throwMacro) throw std::runtime_error("engine double macro failure");
}
tGetHero GetHero_API = reinterpret_cast<tGetHero>(&heroGet);
tFixMovieSequenceCamera FixMovieSequenceCamera_API = reinterpret_cast<tFixMovieSequenceCamera>(&camera);
tStdMap_String_Construct StdMap_String_Construct_API = reinterpret_cast<tStdMap_String_Construct>(&inputCtor);
tStdMap_String_Destructor StdMap_String_Destroy_API = reinterpret_cast<tStdMap_String_Destructor>(&inputDestroy);
#include "scythe_oracle_adapter.inc"

int main() {
    try {
        static_assert(sizeof(void*) == 4, "Retail ABI requires x86");
        StartScriptingEntity_API = reinterpret_cast<tStartScriptingEntity>(&acquireScythe);
        RunCutsceneMacro_Func = &oracleMacro;
        sol::state lua;
        LuaQuestState host;
        lua.new_usertype<LuaQuestState>("Quest", "RunScytheOracleCutscene", &LuaQuestState::RunScytheOracleCutscene);
        lua["host"] = &host;
        lua["me"] = &scytheThing;
        host.m_movieHandlesByLuaState[lua.lua_state()] = reinterpret_cast<void*>(3);
        scytheControl.pImp.Data = &scytheControl;
        refs[&scytheControl] = 1;
        movies = 1; paused = true;
        for (bool success : {false, true}) for (bool populated : {false, true}) for (bool exception : {false, true}) {
            populatedFailure = populated;
            acquireOK = success; throwMacro = exception; events.clear();
            auto before = attempts, beforeMacros = macros;
            bool threw = false;
            try { lua.script("host:RunScytheOracleCutscene(me)"); }
            catch (const std::runtime_error&) { threw = true; }
            check(threw == exception && attempts == before + 1 && macros == beforeMacros + 1);
            check(events == std::vector<std::string>({"hero.attempt", "camera.on", "macro", "camera.off", "inputs.destroy"}));
            check(movies == 1 && paused && refs[&scytheControl] == 1);
            check(maps.empty() && inputMaps.empty() && locals.empty() && strings.empty());
            for (const auto& ref : refs) check(ref.second == (ref.first == &scytheControl ? 1u : 0u));
        }
        host.m_movieHandlesByLuaState.clear();
        auto before = attempts;
        bool rejected = false;
        try { host.RunScytheOracleCutscene(&scytheThing, sol::this_state(lua.lua_state())); }
        catch (const std::runtime_error&) { rejected = true; }
        check(rejected && attempts == before);
        std::cout << "PASS: one attempt including failure, explicit empty inputs, borrowed movie/control, flags/camera, exception cleanup, missing caller movie rejection\n";
        return 0;
    } catch (const std::exception& error) { std::cerr << error.what() << '\n'; return 1; }
}
