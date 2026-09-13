// Actual staged Lua + adapter + sol + LuaRetailResources; engine/Quest are doubles.
#define main resource_smoke_main
#include "retail_resources_smoke.cpp"
#undef main
#include <sstream>

static std::vector<std::string> trace;
static std::map<void*, std::string> names;
static CScriptThing boundActor{}, heroActor{};
static int created, frames, regionCalls, acquireCalls, regionWait, acquireWait, stopFrame;
static bool stopped, heroOK, stopInMacro;
static std::string mapKey;
static std::string boolean(bool v) { return v ? "true" : "false"; }
static void emit(std::initializer_list<std::string> fields) {
    std::string line;
    for (const auto& f : fields) { if (!line.empty()) line += "|"; line += f; }
    trace.push_back(line);
}
static void* __fastcall ctorTrace(void* self, void*) {
    if (created < 2) {
        names[self] = created ? "hero" : "bound";
        emit({"resource.new", names[self]});
    }
    ++created; return self;
}
static bool __fastcall prepareTrace(CScriptGameResourceObjectScriptedThingBase* r, void*) {
    emit({"prepare", names.at(r)}); return false;
}
static bool __fastcall acquireTrace(CGameScriptInterfaceBase* game, void*, const CScriptThing* actor,
                                    CScriptGameResourceObjectScriptedThingBase* r, EScriptAIPriority priority) {
    check(actor == &boundActor || actor == &heroActor);
    emit({"acquire", actor == &heroActor ? "actual-hero" : "bound-instance-not-a-name-lookup",
          names.at(r), std::to_string(priority)});
    acquireOK = actor == &heroActor ? heroOK : ++acquireCalls > acquireWait;
    return acquire(game, nullptr, actor, r, priority);
}
static void __fastcall releaseTrace(void* self, void*) {
    emit({"release", names.at(self)}); release(self, nullptr);
}
static void __fastcall mapCtorTrace(void* self, void*) { emit({"map.new"}); mapCtor(self, nullptr); }
static void __fastcall mapDtorTrace(void* self, void*) { emit({"map.destroy"}); mapDtor(self, nullptr); }
static CScriptGameResourceObjectScriptedThingBase* __fastcall bracketTrace(void* self, void*, const CCharString* key) {
    mapKey = strings.at(const_cast<CCharString*>(key)); return bracket(self, nullptr, key);
}
static void* __fastcall assignTrace(void* self, void*, const void* source) {
    emit({"map.actor", mapKey, names.at(const_cast<void*>(source))}); return assign(self, nullptr, source);
}
static void __fastcall startTrace(CGameScriptInterfaceBase* g, void*, const CCharString* key, CScriptGameResourceObjectMovieBase* m) {
    emit({"movie.start", strings.at(const_cast<CCharString*>(key))}); movieStart(g, nullptr, key, m);
}
static void __fastcall destroyTrace(CScriptGameResourceObjectMovieBase* m, void*) {
    check(!paused); emit({"movie.destroy"}); movieDestroy(m, nullptr);
}
static void __fastcall pauseTrace(CGameScriptInterfaceBase* g, void*, bool v) {
    emit({"pause", boolean(v)}); pause(g, nullptr, v);
}
static void __fastcall frameTrace(CGameScriptInterfaceBase*, void*) {
    emit({"frame"}); if (stopFrame > 0 && ++frames >= stopFrame) stopped = true;
}
static void __fastcall macroTrace(const CCharString* key, void* m, void* flags, void* input, bool setup, bool skip) {
    emit({"macro", strings.at(const_cast<CCharString*>(key)), flags ? "non-null" : "nil",
          input ? "non-null" : "nil", boolean(setup), boolean(skip)});
    macro(key, m, flags, input, setup, skip);
    if (stopInMacro) stopped = true;
}

static int run_integration(int argc, char** argv) {
    check(argc == 2);
    CBaseObject_Construct_API = reinterpret_cast<tCBaseObject_Constructor>(&ctorTrace);
    InitScriptObjectHelper1_API = reinterpret_cast<tInitScriptObjectHelper1>(&prepareTrace);
    StartScriptingEntity_API = reinterpret_cast<tStartScriptingEntity>(&acquireTrace);
    CSGROSTB_Destroy_API = reinterpret_cast<tCSGROSTB_Destructor>(&releaseTrace);
    StdMap_Construct_API = reinterpret_cast<tStdMap_Constructor>(&mapCtorTrace);
    StdMap_Destroy_API = reinterpret_cast<tStdMap_Destructor>(&mapDtorTrace);
    StdMap_OperatorBracket_API = reinterpret_cast<tStdMap_OperatorBracket>(&bracketTrace);
    CBaseObject_Assign_API = reinterpret_cast<tCBaseObject_AssignmentOperator>(&assignTrace);
    StartMovieSequence_API = reinterpret_cast<tStartMovieSequence>(&startTrace);
    MovieResource_Destroy_API = reinterpret_cast<tMovieResource_Destructor>(&destroyTrace);
    PauseAllNonScriptedEntities_API = reinterpret_cast<tPauseAllNonScriptedEntities>(&pauseTrace);
    NewScriptFrame_API = reinterpret_cast<tNewScriptFrame>(&frameTrace);
    RunCutsceneMacro_Func = &macroTrace;
    sol::state lua; lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::table, sol::lib::string);
    lua.new_usertype<CScriptThing>("CScriptThing", sol::no_constructor);
    RegisterRetailResources(lua);
    lua["me"] = &boundActor;
    lua["with_resources"] = [](sol::protected_function f) { WithRetailResources(reinterpret_cast<CGameScriptInterfaceBase*>(1), f); };
    lua["reset_case"] = [](sol::table options) {
        check(maps.empty() && locals.empty() && strings.empty() && !movies && !paused);
        for (const auto& p : refs) check(p.second == 0);
        refs.clear(); names.clear(); trace.clear(); created = frames = regionCalls = acquireCalls = 0;
        regionWait = options.get_or("region_wait", 0); acquireWait = options.get_or("acquire_wait", 0);
        stopFrame = options.get_or("stop_frame", 0); stopped = options.get_or("initial_stop", false);
        heroOK = options.get_or("hero_ok", true); stopInMacro = options.get_or("stop_in_macro", false);
    };
    lua["region"] = [](std::string name) { emit({"region", name}); return ++regionCalls > regionWait; };
    lua["terminating"] = []() { emit({"terminating", boolean(stopped)}); return stopped; };
    lua["hero"] = []() { emit({"hero"}); return &heroActor; };
    lua["record"] = [](std::string line) { trace.push_back(line); };
    lua["verify"] = [](std::string name, sol::table expected) {
        check(maps.empty() && locals.empty() && strings.empty() && !movies && !paused);
        for (const auto& p : refs) check(p.second == 0);
        if (expected.size() != trace.size()) throw std::runtime_error(name + ": trace length mismatch");
        for (size_t i = 0; i < trace.size(); ++i) {
            std::string wanted = expected[i + 1];
            if (wanted != trace[i]) throw std::runtime_error(name + ": event " + std::to_string(i) + ": " + trace[i] + " != " + wanted);
        }
        std::cout << "PASS " << name << " " << trace.size() << " events\n";
    };
    auto result = lua.safe_script_file(argv[1]);
    if (!result.valid()) { sol::error err = result; std::cerr << err.what() << "\n"; return 1; }
    return 0;
}

int main(int argc, char** argv) {
    try { return run_integration(argc, argv); }
    catch (const std::exception& error) { std::cerr << error.what() << "\n"; return 1; }
}
