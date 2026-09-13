// Compile against the actual runtime header and its sol/Lua dependencies.
// Engine entry points are mocked; this checks wrapper ownership and Lua errors.
#include "LuaRetailResources.h"
#include <iostream>
#include <set>

static void check(bool ok) { if (!ok) throw std::runtime_error("resource smoke assertion"); }
static std::map<void*, std::string> strings;
static std::map<void*, std::map<std::string, CScriptGameResourceObjectScriptedThingBase>> maps;
static std::map<void*, unsigned> refs;
static std::set<void*> locals;
static bool acquireOK = true, paused = false;
static unsigned attempts = 0, movies = 0, macros = 0, destroys = 0;
static void* lastResource = nullptr;
static void* lastMap = nullptr;
static void* expectedFlags = nullptr;
static CScriptGameResourceObjectScriptedThingBase* acquireExpert = nullptr;
static CScriptGameResourceObjectScriptedThingBase* expectedExpert = nullptr;
static const CScriptThing* expectedTarget = nullptr;
static int expectedSelection = 0;
static bool expectedListen = false, expected2D = true, expectedFade = false, taskBusy = false;
static unsigned speechCalls = 0, taskCalls = 0;
static void __fastcall resourceSpeak(CScriptGameResourceObjectScriptedThingBase* self, void*,
    const CScriptThing* target, const char* key, ETextGroupSelectionMethod selection, bool listen, bool sound2D, bool fade) {
    check(self == expectedExpert && target == expectedTarget && std::string(key) == "TEXT_QST_077_MAZE_REPEAT");
    check(static_cast<int>(selection) == expectedSelection && listen == expectedListen && sound2D == expected2D && fade == expectedFade);
    ++speechCalls;
}
static bool __fastcall resourceBusy(CScriptGameResourceObjectScriptedThingBase* self, void*) {
    check(self == expectedExpert); ++taskCalls; return taskBusy;
}
static unsigned boolDestroys = 0;
static std::map<void*, std::map<std::string, bool>> boolMaps;
static bool* __fastcall boolBracket(void* self, void*, const CCharString* key) {
    return &boolMaps[self][strings.at(const_cast<CCharString*>(key))];
}
static void __fastcall boolDestroy(void* self, void*) {
    auto head = *static_cast<void**>(self);
    check(head && static_cast<void**>(head)[1] == nullptr);
    check(static_cast<void**>(head)[2] == head && static_cast<void**>(head)[3] == head);
    check(static_cast<unsigned*>(self)[1] == 0);
    free(head); boolMaps.erase(self); ++boolDestroys;
}
static void* __fastcall baseCtor(void* self, void*) { return self; }
static void __fastcall stringCtor(CCharString* self, void*, const char* value, int) {
    strings[self] = value; self->pStringData = reinterpret_cast<decltype(self->pStringData)>(1);
}
static void __fastcall stringDtor(CCharString* self, void*) { strings.erase(self); }
static bool __fastcall prepare(CScriptGameResourceObjectScriptedThingBase*, void*) { return false; }
static void __fastcall prepare2(CScriptGameResourceObjectScriptedThingBase*, void*) { check(false); }
static bool __fastcall acquire(CGameScriptInterfaceBase*, void*, const CScriptThing*, CScriptGameResourceObjectScriptedThingBase* resource, EScriptAIPriority priority) {
    check(priority == SCRIPT_AI_PRIORITY_VERY_HIGH);
    ++attempts; lastResource = resource;
    if (acquireOK && !resource->pImp.Data) {
        resource->pImp.Data = acquireExpert ? acquireExpert : resource;
        refs[resource->pImp.Data]++;
        locals.insert(resource);
    }
    return acquireOK;
}
static void __fastcall release(void* self, void*) {
    auto* resource = static_cast<CScriptGameResourceObjectScriptedThingBase*>(self);
    if (resource->pImp.Data) {
        check(refs[resource->pImp.Data] > 0);
        --refs[resource->pImp.Data];
        check(locals.erase(self) == 1);
    }
    ++destroys;
}
static void __fastcall mapCtor(void* self, void*) { check(maps.emplace(self, decltype(maps)::mapped_type{}).second); }
static void __fastcall mapDtor(void* self, void*) {
    for (const auto& p : maps.at(self)) if (p.second.pImp.Data) {
        check(refs[p.second.pImp.Data] > 0); --refs[p.second.pImp.Data];
    }
    check(maps.erase(self) == 1);
}
static CScriptGameResourceObjectScriptedThingBase* __fastcall bracket(void* self, void*, const CCharString* key) {
    return &maps.at(self)[strings.at(const_cast<CCharString*>(key))];
}
static void* __fastcall assign(void* self, void*, const void* source) {
    auto& target = *static_cast<CScriptGameResourceObjectScriptedThingBase*>(self);
    if (target.pImp.Data) { check(refs[target.pImp.Data] > 0); --refs[target.pImp.Data]; }
    target = *static_cast<const CScriptGameResourceObjectScriptedThingBase*>(source);
    if (target.pImp.Data) ++refs[target.pImp.Data];
    return self;
}
static void __fastcall movieStart(CGameScriptInterfaceBase*, void*, const CCharString*, CScriptGameResourceObjectMovieBase*) { check(movies == 0); ++movies; }
static void __fastcall movieDestroy(CScriptGameResourceObjectMovieBase*, void*) { check(movies == 1); --movies; }
static void __fastcall pause(CGameScriptInterfaceBase*, void*, bool value) { paused = value; }
static void __fastcall frame(CGameScriptInterfaceBase*, void*) {}
static void __fastcall macro(const CCharString*, void* map, void* flags, void* input, bool setup, bool skippable) {
    check(flags == expectedFlags && !input && !setup && skippable && movies == 1 && paused);
    if (flags) boolMaps.at(flags)["UNLIMBO"] = true;
    check(maps.at(map).size() == 2); lastMap = map; ++macros;
}

t_malloc Game_malloc = &malloc;
t_free Game_free = &free;
tCBaseObject_Constructor CBaseObject_Construct_API = reinterpret_cast<tCBaseObject_Constructor>(&baseCtor);
tCCharString_Constructor_Literal CCharString_Construct_Literal = reinterpret_cast<tCCharString_Constructor_Literal>(&stringCtor);
tCCharString_Destructor CCharString_Destroy = reinterpret_cast<tCCharString_Destructor>(&stringDtor);
void** g_pCScriptGameResourceObjectScriptedThingBaseVTable = reinterpret_cast<void**>(1);
void** g_pMovieObjectVTable = reinterpret_cast<void**>(2);
tCSGROSTB_Destructor CSGROSTB_Destroy_API = reinterpret_cast<tCSGROSTB_Destructor>(&release);
tInitScriptObjectHelper1 InitScriptObjectHelper1_API = reinterpret_cast<tInitScriptObjectHelper1>(&prepare);
tInitScriptObjectHelper2 InitScriptObjectHelper2_API = reinterpret_cast<tInitScriptObjectHelper2>(&prepare2);
tStartScriptingEntity StartScriptingEntity_API = reinterpret_cast<tStartScriptingEntity>(&acquire);
tStdMap_Constructor StdMap_Construct_API = reinterpret_cast<tStdMap_Constructor>(&mapCtor);
tStdMap_Destructor StdMap_Destroy_API = reinterpret_cast<tStdMap_Destructor>(&mapDtor);
tStdMap_OperatorBracket StdMap_OperatorBracket_API = reinterpret_cast<tStdMap_OperatorBracket>(&bracket);
tStdMap_Bool_OperatorBracket StdMap_Bool_OperatorBracket_API = reinterpret_cast<tStdMap_Bool_OperatorBracket>(&boolBracket);
tStdMap_Bool_Destructor StdMap_Bool_Destroy_API = reinterpret_cast<tStdMap_Bool_Destructor>(&boolDestroy);
tCBaseObject_AssignmentOperator CBaseObject_Assign_API = reinterpret_cast<tCBaseObject_AssignmentOperator>(&assign);
tRunCutsceneMacro RunCutsceneMacro_Func = &macro;
tStartMovieSequence StartMovieSequence_API = reinterpret_cast<tStartMovieSequence>(&movieStart);
tMovieResource_Destructor MovieResource_Destroy_API = reinterpret_cast<tMovieResource_Destructor>(&movieDestroy);
tPauseAllNonScriptedEntities PauseAllNonScriptedEntities_API = reinterpret_cast<tPauseAllNonScriptedEntities>(&pause);
tNewScriptFrame NewScriptFrame_API = reinterpret_cast<tNewScriptFrame>(&frame);
tRetailThingCopy RetailThing_Copy_API = nullptr;
tRetailThingDestroy RetailThing_Destroy_API = nullptr;
tRetailEntityCondition RetailEntity_SetCondition_API = nullptr;
void** g_pRetailAliveConditionVTable = nullptr;
void** g_pCScriptThingVTable = reinterpret_cast<void**>(0x1238C8C);
tGetThingWithScriptName1 GetThingWithScriptName_ByName_API = nullptr;
tGetHealth GetHealth_API = nullptr;
tSetTeleporterAsActive SetTeleporterAsActive_API = nullptr;

static int run_resource_smoke() {
    CScriptThing actor{};
    auto* game = reinterpret_cast<CGameScriptInterfaceBase*>(1);
    {
        LuaRetailResources scope(game);
        auto map = scope.NewActorMap();
        auto bound = scope.NewResource(), hero = scope.NewResource();
        scope.PrepareResource(bound);
        check(scope.TryAcquire(bound, &actor, 4));
        auto* boundAddress = lastResource;
        acquireOK = false;
        auto before = attempts;
        check(!scope.TryAcquire(hero, &actor, 4) && attempts == before + 1);
        scope.SetActor(map, "Hero", hero);
        scope.SetActor(map, "Messenger", bound);
        auto movie = scope.StartMovie(""); scope.Pause(true);
        scope.RunMacro("CS_ARENA_LEAVE_THERESA_01", map, false, true);
        check(maps.count(lastMap) == 1 && refs[boundAddress] == 2);
        check(maps.at(lastMap).at("Hero").pImp.Data == nullptr);
        scope.Pause(false); scope.DestroyMovie(movie);
        scope.ReleaseResource(hero); scope.ReleaseResource(bound);
        check(refs[boundAddress] == 1); // Map retains its independent native copy.
        scope.DestroyActorMap(map); check(refs[boundAddress] == 0);
        scope.Close(); scope.Close();
        bool rejected = false; try { scope.NewResource(); } catch (const std::runtime_error&) { rejected = true; }
        check(rejected);
    }
    acquireOK = true;
    {
        LuaRetailResources scope(game);
        auto first = scope.NewResource(); check(scope.TryAcquire(first, &actor, 4));
        auto* original = lastResource;
        for (unsigned i = 0; i < 1000; ++i) scope.NewResource();
        check(scope.TryAcquire(first, &actor, 4) && lastResource == original);
        bool rejected = false; try { scope.DestroyActorMap(first); } catch (const std::runtime_error&) { rejected = true; }
        check(rejected);
    }
    sol::state lua; lua.open_libraries(sol::lib::base);
    lua.new_usertype<CScriptThing>("CScriptThing", sol::no_constructor);
    RegisterRetailResources(lua);
    lua["actor"] = &actor;
    lua["With"] = [game](sol::protected_function callback) { WithRetailResources(game, callback); };
    lua.script(R"(
      local ok, err = pcall(function()
        With(function(r)
          retained = r
          local own = r:NewResource()
          assert(r:TryAcquire(own, actor, 4))
          local m = r:NewActorMap()
          r:SetActor(m, "Actor", own)
          r:StartMovie("")
          r:Pause(true)
          error("deliberate callback failure")
        end)
      end)
      assert(not ok)
      assert(not pcall(function() retained:NewResource() end))
      With(function(r)
        local own = r:NewResource()
        r:ReleaseResource(own)
        assert(not pcall(function() r:ReleaseResource(own) end))
      end)
    )");
    check(!paused && !movies && maps.empty() && locals.empty() && strings.empty());
    for (const auto& entry : refs) check(entry.second == 0);
    check(macros == 1);
    {
        auto flags = std::make_shared<LuaRetailFlags>();
        flags->Set("UNLIMBO", false);
        sol::state other; other.open_libraries(sol::lib::base);
        RegisterRetailFlags(other);
        lua["flags"] = flags; other["flags"] = flags;
        other.script("assert(not flags:Get('UNLIMBO')); flags:Set('UNLIMBO', true)");
        lua.script("assert(flags:Get('UNLIMBO')); flags:Set('UNLIMBO', false)");
        LuaRetailResources scope(game);
        auto resource = scope.NewResource(), map = scope.NewActorMap();
        scope.SetActor(map, "Hero", resource); scope.SetActor(map, "Other", resource);
        auto movie = scope.StartMovie(""); scope.Pause(true);
        expectedFlags = flags->NativeMap();
        scope.RunMacroWithFlags("TEST", map, flags, false, true);
        lua.script("assert(flags:Get('UNLIMBO'))");
        bool rejected = false;
        try { scope.RunMacroWithFlags("TEST", map, nullptr, false, true); }
        catch (const std::runtime_error&) { rejected = true; }
        check(rejected);
        scope.Pause(false); scope.DestroyMovie(movie); scope.DestroyActorMap(map); scope.ReleaseResource(resource);
        flags.reset(); lua["flags"] = sol::nil; lua.collect_garbage();
        check(boolDestroys == 0); // Other VM still owns the exact same map.
        other.script("assert(flags:Get('UNLIMBO'))");
        other["flags"] = sol::nil; other.collect_garbage();
        check(boolDestroys == 1 && boolMaps.empty()); expectedFlags = nullptr;
    }
    {
        CScriptGameResourceObjectScriptedThingBaseVTable table{};
        table.Speak = reinterpret_cast<decltype(table.Speak)>(&resourceSpeak);
        table.IsPerformingScriptTask = reinterpret_cast<decltype(table.IsPerformingScriptTask)>(&resourceBusy);
        CScriptGameResourceObjectScriptedThingBase experts[2]{};
        for (auto& expert : experts) expert.pVTable = reinterpret_cast<void**>(&table);
        auto scope = std::make_shared<LuaRetailResources>(game);
        unsigned ids[2];
        for (int i = 0; i < 2; ++i) {
            ids[i] = scope->NewResource(); acquireExpert = &experts[i];
            check(scope->TryAcquire(ids[i], &actor, 4));
        }
        acquireExpert = nullptr;
        lua["dispatch"] = scope; lua["first"] = ids[0]; lua["second"] = ids[1];
        expectedExpert = &experts[0]; expectedTarget = &actor;
        lua.script("dispatch:Speak(first, actor, 'TEXT_QST_077_MAZE_REPEAT', 0, false, true, false)");
        expectedExpert = &experts[1]; expectedTarget = nullptr;
        expectedSelection = 1; expectedListen = true; expected2D = false; expectedFade = true;
        lua.script("dispatch:Speak(second, nil, 'TEXT_QST_077_MAZE_REPEAT', 1, true, false, true)");
        expectedExpert = &experts[0]; expectedTarget = &actor;
        expectedSelection = 2; expectedListen = false; expectedFade = false;
        lua.script("dispatch:Speak(first, actor, 'TEXT_QST_077_MAZE_REPEAT', 2, false, false, false)");
        taskBusy = true;
        lua.script("assert(dispatch:IsPerformingScriptTask(first))");
        expectedExpert = &experts[1]; taskBusy = false;
        lua.script("assert(not dispatch:IsPerformingScriptTask(second))");
        lua.script(R"(
            local empty = dispatch:NewResource()
            dispatch:Speak(empty, nil, 'TEXT_QST_077_MAZE_REPEAT', 0, false, true, false)
            assert(not dispatch:IsPerformingScriptTask(empty))
            dispatch:ReleaseResource(empty)
            assert(not pcall(function() dispatch:Speak(empty, nil, 'x', 0, false, true, false) end))
            assert(not pcall(function() dispatch:IsPerformingScriptTask(empty) end))
            local map = dispatch:NewActorMap()
            assert(not pcall(function() dispatch:IsPerformingScriptTask(map) end))
            dispatch:DestroyActorMap(map)
        )");
        check(speechCalls == 3 && taskCalls == 2);
        scope->Close();
        lua.script("assert(not pcall(function() dispatch:IsPerformingScriptTask(first) end))");
        lua["dispatch"] = sol::nil; lua.collect_garbage();
    }
    check(macros == 2 && !paused && !movies && maps.empty() && strings.empty() && locals.empty());
    for (const auto& entry : refs) check(entry.second == 0);
    std::cout << "PASS: explicit resource speech/task dispatch, two expert identities, nil target, selections/booleans, empty/released/wrong-kind/closed resources\n";
    std::cout << "PASS: failed acquisition, empty map copy, map retention, stable resource addresses, wrong-kind/released handles, callback-error cleanup and retained-scope rejection\n";
    return 0;
}

int main() {
    try { return run_resource_smoke(); }
    catch (const std::exception& error) { std::cerr << error.what() << "\n"; return 1; }
}
