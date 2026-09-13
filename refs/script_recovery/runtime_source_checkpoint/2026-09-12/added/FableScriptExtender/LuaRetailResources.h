#pragma once

#include "FableAPI.h"
#include "GameInterface.h"
#include "EntityScriptingAPI.h"
#include "LuaRetailFlags.h"
#include "sol/sol.hpp"
#include <memory>
#include <stdexcept>
#include <vector>

// Explicit native locals for reviewed ports. No cached actor control, acquisition
// retry, movie setup or post-macro termination policy is implicit here.
// Close is deterministic at the callback boundary; retained Lua references become
// inert rather than releasing engine resources later during Lua garbage collection.
class LuaRetailResources {
public:
    explicit LuaRetailResources(CGameScriptInterfaceBase* game) : m_game(game) {
        if (!game || !Game_malloc || !Game_free || !CBaseObject_Construct_API ||
            !g_pCScriptGameResourceObjectScriptedThingBaseVTable || !CSGROSTB_Destroy_API ||
            !InitScriptObjectHelper1_API || !InitScriptObjectHelper2_API ||
            !StartScriptingEntity_API || !StdMap_Construct_API || !StdMap_Destroy_API ||
            !StdMap_OperatorBracket_API || !CBaseObject_Assign_API || !RunCutsceneMacro_Func ||
            !CCharString_Construct_Literal || !CCharString_Destroy || !g_pMovieObjectVTable ||
            !StartMovieSequence_API || !MovieResource_Destroy_API || !PauseAllNonScriptedEntities_API ||
            !NewScriptFrame_API)
            throw std::runtime_error("Retail resource APIs are unavailable");
    }
    ~LuaRetailResources() { Close(); }
    LuaRetailResources(const LuaRetailResources&) = delete;
    LuaRetailResources& operator=(const LuaRetailResources&) = delete;

    unsigned NewResource() {
        auto& e = Add(Kind::Resource);
        CBaseObject_Construct_API(&e.resource);
        e.resource.pVTable = g_pCScriptGameResourceObjectScriptedThingBaseVTable;
        e.resource.pImp.Data = nullptr;
        e.resource.pImp.Info = nullptr;
        e.live = true;
        return static_cast<unsigned>(m_entries.size());
    }
    void PrepareResource(unsigned id) {
        auto& e = Get(id, Kind::Resource);
        if (InitScriptObjectHelper1_API(&e.resource)) InitScriptObjectHelper2_API(&e.resource);
    }
    bool TryAcquire(unsigned id, CScriptThing* actor, int priority) {
        auto& e = Get(id, Kind::Resource);
        if (!actor) throw std::runtime_error("Retail TryAcquire requires an actor");
        return StartScriptingEntity_API(m_game, actor, &e.resource, static_cast<EScriptAIPriority>(priority));
    }
    void ReleaseResource(unsigned id) { Destroy(Get(id, Kind::Resource)); }
    void Frame() { CheckOpen(); NewScriptFrame_API(m_game); }

    // Native 7E7390 / 7E7450 dispatch through this local's pImp, never the
    // LuaEntityAPI cached control handle. An empty live resource is a no-op.
    void Speak(unsigned id, CScriptThing* target, const std::string& key,
               int selection, bool listen, bool sound2D, bool overFade) {
        auto* expert = Get(id, Kind::Resource).resource.pImp.Data;
        if (!expert) return;
        auto* table = reinterpret_cast<CScriptGameResourceObjectScriptedThingBaseVTable*>(expert->pVTable);
        if (!table || !table->Speak) throw std::runtime_error("Retail resource Speak is unavailable");
        table->Speak(expert, target, key.c_str(), static_cast<ETextGroupSelectionMethod>(selection),
                     listen, sound2D, overFade);
    }
    bool IsPerformingScriptTask(unsigned id) {
        auto* expert = Get(id, Kind::Resource).resource.pImp.Data;
        if (!expert) return false;
        auto* table = reinterpret_cast<CScriptGameResourceObjectScriptedThingBaseVTable*>(expert->pVTable);
        if (!table || !table->IsPerformingScriptTask)
            throw std::runtime_error("Retail resource task query is unavailable");
        return table->IsPerformingScriptTask(expert);
    }

    unsigned NewThingFromResource(unsigned id) {
        auto* expert = Get(id, Kind::Resource).resource.pImp.Data;
        if (!RetailThing_Destroy_API || !g_pCScriptThingVTable)
            throw std::runtime_error("Retail Thing APIs are unavailable");
        auto& e = Add(Kind::Thing);
        e.thing.pVTable = g_pCScriptThingVTable;
        e.live = true;
        if (expert) {
            auto* table = reinterpret_cast<CScriptGameResourceObjectScriptedThingBaseVTable*>(expert->pVTable);
            if (!table || !table->GetScriptThing) throw std::runtime_error("Retail resource Thing getter is unavailable");
            table->GetScriptThing(expert, &e.thing);
        }
        return static_cast<unsigned>(m_entries.size());
    }
    unsigned NewThingFromScriptName(const std::string& name) {
        CheckOpen();
        if (!RetailThing_Destroy_API || !GetThingWithScriptName_ByName_API || !g_pCScriptThingVTable)
            throw std::runtime_error("Retail Thing lookup APIs are unavailable");
        auto& e = Add(Kind::Thing);
        e.thing.pVTable = g_pCScriptThingVTable;
        e.live = true;
        FableString key(name.c_str());
        GetThingWithScriptName_ByName_API(m_game, &e.thing, key);
        return static_cast<unsigned>(m_entries.size());
    }
    float ThingHealth(unsigned id) {
        auto& e = Get(id, Kind::Thing);
        if (!GetHealth_API) throw std::runtime_error("Retail health API is unavailable");
        return GetHealth_API(m_game, &e.thing);
    }
    bool ThingAlive(unsigned id) {
        auto& e = Get(id, Kind::Thing);
        auto* table = reinterpret_cast<CScriptThingVTable*>(e.thing.pVTable);
        if (!table || !table->IsAlive) throw std::runtime_error("Retail Thing alive query is unavailable");
        return table->IsAlive(&e.thing);
    }
    void SetTeleporterActive(unsigned id, bool active) {
        auto& e = Get(id, Kind::Thing);
        if (!SetTeleporterAsActive_API) throw std::runtime_error("Retail teleporter API is unavailable");
        SetTeleporterAsActive_API(m_game, &e.thing, active);
    }
    void DestroyThing(unsigned id) { Destroy(Get(id, Kind::Thing)); }

    unsigned NewActorMap() {
        auto& e = Add(Kind::ActorMap);
        e.map = Game_malloc(0x24);
        if (!e.map) throw std::bad_alloc();
        StdMap_Construct_API(e.map);
        e.live = true;
        return static_cast<unsigned>(m_entries.size());
    }
    void SetActor(unsigned map, const std::string& name, unsigned resource) {
        auto& m = Get(map, Kind::ActorMap);
        auto& r = Get(resource, Kind::Resource);
        FableString key(name.c_str());
        auto* entry = StdMap_OperatorBracket_API(m.map, key);
        if (!entry) throw std::runtime_error("Retail actor map insertion failed");
        // Copy the resource even when acquisition failed and pImp is empty.
        CBaseObject_Assign_API(entry, &r.resource);
    }
    void DestroyActorMap(unsigned id) { Destroy(Get(id, Kind::ActorMap)); }
    void RunMacro(const std::string& name, unsigned map, bool setup, bool skippable) {
        auto& e = Get(map, Kind::ActorMap);
        FableString key(name.c_str());
        RunCutsceneMacro_Func(key, e.map, nullptr, nullptr, setup, skippable);
    }
    void RunMacroWithFlags(const std::string& name, unsigned map,
                           std::shared_ptr<LuaRetailFlags> flags, bool setup, bool skippable) {
        auto& e = Get(map, Kind::ActorMap);
        if (!flags) throw std::runtime_error("Retail macro requires a flag map");
        FableString key(name.c_str());
        // The local shared owner survives the blocking call and native yields.
        RunCutsceneMacro_Func(key, e.map, flags->NativeMap(), nullptr, setup, skippable);
    }

    unsigned StartMovie(const std::string& name) {
        CheckOpen();
        for (const auto& e : m_entries)
            if (e->live && e->kind == Kind::Movie)
                throw std::runtime_error("Retail resource scope already owns a movie");
        auto& e = Add(Kind::Movie);
        CBaseObject_Construct_API(&e.movie);
        e.movie.pVTable = g_pMovieObjectVTable;
        e.movie.pImp.Data = nullptr;
        e.movie.pImp.Info = nullptr;
        e.live = true;
        FableString key(name.c_str());
        StartMovieSequence_API(m_game, key, &e.movie);
        return static_cast<unsigned>(m_entries.size());
    }
    void DestroyMovie(unsigned id) { Destroy(Get(id, Kind::Movie)); }
    void Pause(bool paused) {
        CheckOpen();
        PauseAllNonScriptedEntities_API(m_game, paused);
        m_paused = paused;
    }

    void Close() noexcept {
        if (m_closed) return;
        // Error/early-return fallback only. Reviewed Lua explicitly releases each
        // local at its retail position; those entries are already inactive here.
        if (m_paused) PauseAllNonScriptedEntities_API(m_game, false);
        m_paused = false;
        for (auto it = m_entries.rbegin(); it != m_entries.rend(); ++it) Destroy(**it);
        m_closed = true;
    }

private:
    enum class Kind { Resource, ActorMap, Movie, Thing };
    struct Entry {
        explicit Entry(Kind k) : kind(k) {}
        Kind kind;
        bool live = false;
        CScriptGameResourceObjectScriptedThingBase resource{};
        CScriptGameResourceObjectMovieBase movie{};
        CScriptThing thing{};
        void* map = nullptr;
    };
    void CheckOpen() const {
        if (m_closed) throw std::runtime_error("Retail resource scope is closed");
    }
    Entry& Add(Kind kind) {
        CheckOpen();
        m_entries.emplace_back(std::make_unique<Entry>(kind));
        return *m_entries.back();
    }
    Entry& Get(unsigned id, Kind kind) {
        CheckOpen();
        if (!id || id > m_entries.size() || m_entries[id - 1]->kind != kind || !m_entries[id - 1]->live)
            throw std::runtime_error("Invalid or released retail resource");
        return *m_entries[id - 1];
    }
    static void Destroy(Entry& e) noexcept {
        if (!e.live) return;
        switch (e.kind) {
        case Kind::Resource: CSGROSTB_Destroy_API(&e.resource); break;
        case Kind::ActorMap: StdMap_Destroy_API(e.map); Game_free(e.map); e.map = nullptr; break;
        case Kind::Movie: MovieResource_Destroy_API(&e.movie); break;
        case Kind::Thing: RetailThing_Destroy_API(&e.thing); break;
        }
        e.live = false;
    }
    CGameScriptInterfaceBase* m_game;
    bool m_closed = false;
    bool m_paused = false;
    std::vector<std::unique_ptr<Entry>> m_entries;
};

inline void RegisterRetailResources(sol::state& lua) {
    RegisterRetailFlags(lua);
    auto type = lua.new_usertype<LuaRetailResources>("RetailResources", sol::no_constructor);
    type["NewResource"] = &LuaRetailResources::NewResource;
    type["PrepareResource"] = &LuaRetailResources::PrepareResource;
    type["TryAcquire"] = &LuaRetailResources::TryAcquire;
    type["ReleaseResource"] = &LuaRetailResources::ReleaseResource;
    type["Frame"] = &LuaRetailResources::Frame;
    type["Speak"] = &LuaRetailResources::Speak;
    type["IsPerformingScriptTask"] = &LuaRetailResources::IsPerformingScriptTask;
    type["NewThingFromResource"] = &LuaRetailResources::NewThingFromResource;
    type["NewThingFromScriptName"] = &LuaRetailResources::NewThingFromScriptName;
    type["ThingHealth"] = &LuaRetailResources::ThingHealth;
    type["ThingAlive"] = &LuaRetailResources::ThingAlive;
    type["SetTeleporterActive"] = &LuaRetailResources::SetTeleporterActive;
    type["DestroyThing"] = &LuaRetailResources::DestroyThing;
    type["NewActorMap"] = &LuaRetailResources::NewActorMap;
    type["SetActor"] = &LuaRetailResources::SetActor;
    type["DestroyActorMap"] = &LuaRetailResources::DestroyActorMap;
    type["RunMacro"] = &LuaRetailResources::RunMacro;
    type["RunMacroWithFlags"] = &LuaRetailResources::RunMacroWithFlags;
    type["StartMovie"] = &LuaRetailResources::StartMovie;
    type["DestroyMovie"] = &LuaRetailResources::DestroyMovie;
    type["Pause"] = &LuaRetailResources::Pause;
}

inline void WithRetailResources(CGameScriptInterfaceBase* game, sol::protected_function callback) {
    auto scope = std::make_shared<LuaRetailResources>(game);
    sol::protected_function_result result = callback(scope);
    scope->Close();
    if (!result.valid()) {
        sol::error error = result;
        throw error;
    }
}
