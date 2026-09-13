// Real quest storage methods, real sol/Lua, extracted current usable method.
// Engine APIs and the quest constructor are doubles; no native scheduler runs.
#define main resource_smoke_main
#include "retail_resources_smoke.cpp"
#undef main
#include "LuaQuestState.h"

LuaQuestState::LuaQuestState(LuaQuestHost* parent, CGameScriptInterfaceBase* game)
    : m_pParentHost(parent), m_pGameInterface(game) {}
static unsigned usableCalls = 0, logCalls = 0;
static CScriptThing* usableThing = nullptr;
void LuaQuestState::Log(const std::string&) { ++logCalls; }
static void __fastcall usable(CGameScriptInterfaceBase*, void*, const CScriptThing* thing, bool value) {
    check(value && thing == usableThing); ++usableCalls;
}
tSetThingAsUsable SetThingAsUsable_API = reinterpret_cast<tSetThingAsUsable>(&usable);
// Generated verbatim from the current runtime cpp; signature/body drift is tested.
#include "usable-method.inc"

int main() {
    try {
        auto game = reinterpret_cast<CGameScriptInterfaceBase*>(1);
        auto quest = std::make_unique<LuaQuestState>(nullptr, game);
        CScriptThing borrowed{}; usableThing = &borrowed;
        auto sword = std::make_shared<CScriptThing>();
        std::weak_ptr<CScriptThing> weak = sword;
        quest->RetainRetailThing("MazeResearch", sword); sword.reset();
        check(!weak.expired() && quest->GetRetainedRetailThing("missing") == nullptr);
        sol::state entity, thread;
        for (auto* lua : {&entity, &thread}) {
            lua->open_libraries(sol::lib::base);
            RegisterRetailFlags(*lua);
            lua->new_usertype<CScriptThing>("Thing", sol::no_constructor);
            auto type = lua->new_usertype<LuaQuestState>("Quest", sol::no_constructor);
            type["RetailFlags"] = &LuaQuestState::RetailFlags;
            type["GetRetainedRetailThing"] = &LuaQuestState::GetRetainedRetailThing;
            type["SetThingAsUsable"] = &LuaQuestState::SetThingAsUsable;
            (*lua)["quest"] = quest.get();
        }
        entity["me"] = &borrowed;
        entity.script("quest:SetThingAsUsable(me, true); quest:RetailFlags('MazeResearch'):Set('UNLIMBO', false)");
        check(usableCalls == 1 && logCalls == 0);
        thread.script("flags = quest:RetailFlags('MazeResearch'); assert(not flags:Get('UNLIMBO')); flags:Set('UNLIMBO', true); sword = quest:GetRetainedRetailThing('MazeResearch')");
        entity.script("assert(quest:RetailFlags('MazeResearch'):Get('UNLIMBO'))");
        check(quest->RetailFlags("MazeResearch")->NativeMap() == thread["flags"].get<std::shared_ptr<LuaRetailFlags>>()->NativeMap());
        auto owned = std::make_shared<CScriptThing>(); usableThing = owned.get(); entity["owned"] = owned;
        entity.script("quest:SetThingAsUsable(owned, true); quest:SetThingAsUsable(nil, true)");
        check(usableCalls == 2 && logCalls == 1);
        entity["quest"] = sol::nil; thread["quest"] = sol::nil;
        quest.reset(); entity.collect_garbage(); thread.collect_garbage();
        check(!weak.expired() && boolDestroys == 0);
        thread.script("assert(flags:Get('UNLIMBO')); sword=nil; flags=nil");
        thread.collect_garbage(); check(weak.expired() && boolDestroys == 1);
        std::cout << "PASS: shared quest flags/thing across isolated VMs, last-owner cleanup, borrowed/owned/nil usable arguments\n";
        return 0;
    } catch (const std::exception& error) { std::cerr << error.what() << "\n"; return 1; }
}
