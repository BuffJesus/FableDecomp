// Reuse the existing actual-runtime smoke engine doubles, then exercise the
// candidate-only API and destruction paths through vendor sol and Lua.
#define main original_resource_smoke_main
#include "retail_resources_smoke.cpp"
#undef main

DWORD g_fableBase = 0;
static volatile unsigned char animationByte = 0;
static C3DVector currentPosition{1.25f, -9.5f, 37.0f};
static int positionQueries = 0;
static const C3DVector* __fastcall queryPosition(CGameScriptThing*, void*) {
    ++positionQueries; return &currentPosition;
}
static bool ordinaryResult, anyResult, excludedResult;
static std::vector<int> hitCalls;
static bool __fastcall ordinaryHitQuery(CScriptThing*, void*, const CCharString* key) {
    check(strings.size() == 1 && strings.at(const_cast<CCharString*>(key)) == "SCRIPT_NAME_HERO");
    hitCalls.push_back(1); return ordinaryResult;
}
static bool __fastcall anyHitQuery(CScriptThing*, void*, const CCharString* key) {
    check(strings.size() == 2 && strings.at(const_cast<CCharString*>(key)) == "SCRIPT_NAME_HERO");
    hitCalls.push_back(2); return anyResult;
}
static bool __fastcall excludedHitQuery(CScriptThing*, void*, EHeroAbility ability, const CCharString* key) {
    check(strings.size() == 3 && static_cast<int>(ability) == 14 && strings.at(const_cast<CCharString*>(key)) == "SCRIPT_NAME_HERO");
    hitCalls.push_back(3); return excludedResult;
}
static std::vector<std::string> cleanup;
static const CScriptThing* expectedFrom = nullptr;
static const CScriptThing* expectedTo = nullptr;
static CScriptThing* capturedLookup = nullptr;
static bool __fastcall thingDistance(const CScriptThing* first, const CScriptThing* second, float distance) {
    check(first == expectedFrom && second == expectedTo && distance == 5.0f); return true;
}
static void __fastcall faceThing(CGameScriptInterfaceBase*, void*, const CScriptThing* first, const CScriptThing* second, bool snap) {
    check(first == expectedFrom && second == expectedTo && snap);
}
static void __fastcall conversationPerson(CGameScriptInterfaceBase*, void*, int id, const CScriptThing* actor) {
    check(id == 42 && actor == expectedFrom);
}
static void __fastcall conversationLine(CGameScriptInterfaceBase*, void*, int id, const CCharString* line, bool flag,
                                       const CScriptThing* speaker, const CScriptThing* listener) {
    check(id == 42 && flag && speaker == expectedFrom && listener == expectedTo);
    check(strings.at(const_cast<CCharString*>(line)) == "TEST_LINE");
}
static CScriptThing* __fastcall lookupThing(CGameScriptInterfaceBase*, void*, CScriptThing* output, const CCharString* name) {
    check(strings.at(const_cast<CCharString*>(name)) == "NOVI_AffairWife");
    capturedLookup = output; return output;
}
tIsDistanceBetweenThingsUnder IsDistanceBetweenThingsUnder_API = &thingDistance;
tEntitySetFacingAngleTowardsThing EntitySetFacingAngleTowardsThing_API = reinterpret_cast<tEntitySetFacingAngleTowardsThing>(&faceThing);
tAddPersonToConversation AddPersonToConversation_API = reinterpret_cast<tAddPersonToConversation>(&conversationPerson);
tAddLineToConversation AddLineToConversation_API = reinterpret_cast<tAddLineToConversation>(&conversationLine);
static void __fastcall trackedThingDestroy(CScriptThing*, void*) { cleanup.push_back("thing"); }
static void __fastcall trackedMovieDestroy(CScriptGameResourceObjectMovieBase* self, void* unused) {
    cleanup.push_back("movie"); movieDestroy(self, unused);
}
static void __fastcall trackedResourceDestroy(void* self, void* unused) {
    cleanup.push_back("resource"); release(self, unused);
}
static void __fastcall trackedPause(CGameScriptInterfaceBase* self, void* unused, bool value) {
    cleanup.push_back(value ? "pause" : "unpause"); pause(self, unused, value);
}

int main() {
    try {
        check(original_resource_smoke_main() == 0);
        g_fableBase = reinterpret_cast<DWORD>(&animationByte) - (0x01375748 - 0x400000);
        RetailThing_Destroy_API = reinterpret_cast<tRetailThingDestroy>(&trackedThingDestroy);
        MovieResource_Destroy_API = reinterpret_cast<decltype(MovieResource_Destroy_API)>(&trackedMovieDestroy);
        CSGROSTB_Destroy_API = reinterpret_cast<decltype(CSGROSTB_Destroy_API)>(&trackedResourceDestroy);
        PauseAllNonScriptedEntities_API = reinterpret_cast<decltype(PauseAllNonScriptedEntities_API)>(&trackedPause);
        auto* game = reinterpret_cast<CGameScriptInterfaceBase*>(1);
        sol::state lua; lua.open_libraries(sol::lib::base);
        lua.new_usertype<CScriptThing>("CScriptThing", sol::no_constructor);
        RegisterRetailResources(lua);
        auto scope = std::make_shared<LuaRetailResources>(game);
        lua["resources"] = scope;
        static_assert(offsetof(CScriptThingVTable, MsgIsHitBy) == 0x54);
        static_assert(offsetof(CScriptThingVTable, MsgIsHitByAnySpecialAbilityFrom) == 0xA8);
        static_assert(offsetof(CScriptThingVTable, MsgIsHitBySpecialAbilityFrom) == 0xA4);
        CScriptThingVTable hitTable{};
        hitTable.MsgIsHitBy = reinterpret_cast<decltype(hitTable.MsgIsHitBy)>(&ordinaryHitQuery);
        hitTable.MsgIsHitByAnySpecialAbilityFrom = reinterpret_cast<decltype(hitTable.MsgIsHitByAnySpecialAbilityFrom)>(&anyHitQuery);
        hitTable.MsgIsHitBySpecialAbilityFrom = reinterpret_cast<decltype(hitTable.MsgIsHitBySpecialAbilityFrom)>(&excludedHitQuery);
        CScriptThing hitActor{}; hitActor.pVTable = reinterpret_cast<void**>(&hitTable);
        lua["actor"] = &hitActor;
        static_assert(offsetof(CGameScriptThingVTable, GetPos) == 0x18);
        CGameScriptThingVTable positionTable{};
        positionTable.GetPos = reinterpret_cast<decltype(positionTable.GetPos)>(&queryPosition);
        CGameScriptThing positionImplementation{};
        positionImplementation.pVTable = reinterpret_cast<void**>(&positionTable);
        hitActor.pImp.Data = reinterpret_cast<decltype(hitActor.pImp.Data)>(&positionImplementation);
        lua.script("savedPosition = RetailThingPosition(actor); assert(savedPosition.x == 1.25 and savedPosition.y == -9.5 and savedPosition.z == 37)");
        currentPosition.x = 99;
        lua.script("assert(savedPosition.x == 1.25 and RetailThingPosition(actor).x == 99)");
        check(positionQueries == 2);
        hitActor.pImp.Data = nullptr;
        const DWORD animationBase = g_fableBase;
        C3DVector fallback{4.5f, 6.25f, -8.0f};
        g_fableBase = reinterpret_cast<DWORD>(&fallback) - (0x0143E8E0 - 0x400000);
        lua.script("savedPosition = RetailThingPosition(actor); assert(savedPosition.x == 4.5 and savedPosition.y == 6.25 and savedPosition.z == -8)");
        fallback.x = -17;
        lua.script("assert(savedPosition.x == 4.5 and RetailThingPosition(actor).x == -17); assert(RetailThingPosition(nil).x == -17)");
        check(positionQueries == 2);
        g_fableBase = animationBase;
        for (unsigned bits = 0; bits < 8; ++bits) {
            ordinaryResult=bits&1; anyResult=bits&2; excludedResult=bits&4; hitCalls.clear();
            lua["expected"] = ordinaryResult || (anyResult && !excludedResult);
            lua.script("assert(resources:IsHitByHeroExceptAbility(actor,14) == expected)");
            const auto expected = ordinaryResult ? std::vector<int>{1} : anyResult ? std::vector<int>{1,2,3} : std::vector<int>{1,2};
            check(hitCalls == expected && strings.empty());
        }
        GetThingWithScriptName_ByName_API = reinterpret_cast<tGetThingWithScriptName1>(&lookupThing);
        auto wife = scope->NewThingFromScriptName("NOVI_AffairWife");
        lua["wife"] = wife;
        expectedFrom = &hitActor; expectedTo = capturedLookup;
        lua.script("assert(resources:ThingsAreWithinDistance(actor,wife,5)); resources:FaceThing(actor,wife,true)");
        lua.script("resources:AddConversationPerson(42,actor); resources:AddConversationLine(42,'TEST_LINE',actor,wife,true)");
        auto ownedActor = std::make_shared<CScriptThing>();
        lua["ownedActor"] = ownedActor; expectedFrom = ownedActor.get();
        lua.script("assert(resources:ThingsAreWithinDistance(ownedActor,wife,5))");
        lua.script(R"(
            for _, invalid in ipairs({0, -1, 0.5, 'bad'}) do
                assert(not pcall(function() resources:ThingsAreWithinDistance(actor,invalid,5) end))
            end
        )");
        scope->DestroyThing(wife);
        lua.script("assert(not pcall(function() resources:FaceThing(actor,wife,true) end))");
        check(strings.empty());
        for (unsigned value : {0u, 1u, 2u, 255u, 0u}) {
            animationByte = static_cast<unsigned char>(value);
            lua["expected"] = value != 0;
            lua.script("assert(type(resources:ReadAnimationArgument5()) == 'boolean'); assert(resources:ReadAnimationArgument5() == expected)");
        }
        auto resource = scope->NewResource();
        unsigned previous = 0;
        for (unsigned i = 0; i < 10000; ++i) {
            auto thing = scope->NewThingFromResource(resource);
            check(thing > previous);
            scope->DestroyThing(thing);
            bool rejected = false;
            try { scope->DestroyThing(previous ? previous : thing); } catch (const std::runtime_error&) { rejected = true; }
            check(rejected);
            auto movie = scope->StartMovie(""); scope->DestroyMovie(movie);
            previous = thing;
        }
        scope->ReleaseResource(resource); scope->Close();
        lua.script("assert(not pcall(function() resources:ReadAnimationArgument5() end))");
        cleanup.clear();
        sol::protected_function callback = lua.load(R"(
            return function(r)
                local actor = r:NewResource()
                r:StartMovie('')
                r:Pause(true)
                r:NewThingFromResource(actor)
                error('candidate injected error')
            end
        )")();
        bool propagated = false;
        try { WithRetailResources(game, callback); }
        catch (const sol::error& error) { propagated = std::string(error.what()).find("candidate injected error") != std::string::npos; }
        check(propagated);
        check(cleanup == std::vector<std::string>({"pause", "unpause", "thing", "movie", "resource"}));
        check(!paused && !movies);
        std::cout << "PASS: candidate hit truth table/string lifetimes, live animation boolean via sol, repeated Thing/movie lifetimes, stale handles, ordered error cleanup\n";
        return 0;
    } catch (const std::exception& error) { std::cerr << error.what() << "\n"; return 1; }
}
