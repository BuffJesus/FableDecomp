# Scripting redesign — a first-class script runtime for the modernized engine

Design doc, 2026-09-26. Status: **proposal**, nothing built. Scope: the post-recreation
modernization fork (see [ARCHITECTURE](../ARCHITECTURE.md) and the "x64 / broad modernization" row in
[ROADMAP](../ROADMAP.md#parked--shelved)). The byte-exact rebuild is untouched: it keeps the retail
compiled quest classes, because that is what the retail bytes are.

Companion: [IN_ENGINE_MODDING_ENVIRONMENT](IN_ENGINE_MODDING_ENVIRONMENT.md) (the in-engine editor /
"Garry's Mod" question; this doc covers only the scripting runtime it would sit on).

Contents

- [Where we are](#where-we-are)
- [What goes wrong today](#what-goes-wrong-today)
- [Goals and non-goals](#goals-and-non-goals)
- [Design](#design)
- [Compatibility](#compatibility)
- [Sequencing](#sequencing)
- [Open questions](#open-questions)

---

## Where we are

**Retail** ([QUEST_SCRIPTS](QUEST_SCRIPTS.md)):

- Quest logic is compiled C++. `RegisterAllScripts` @ `0x00CD52D0` registers 161 `CScriptInfo`
  records `{Name, ID, MasterScript, pAllocFunc, pAllocDataFunc, "S_xxx"}`; activation calls the
  allocator through a raw code pointer. No VM, no bytecode.
- Everything around the logic is name-keyed data: `.qst` `AddQuest("Name", TRUE)` lists, quest cards,
  rewards, persistence framing.
- Cutscenes and region scripts are ASCII `[Actor.]Verb args` lines in `script.bin`, run by a
  `strncmp` chain of ~185 verbs in `RunCutsceneMacro_Func` @ `0x00CBFB7D` against
  `CGameScriptInterface`.
- Quest threads are cooperative: they run under the engine's scheduler and wait on engine state.

**Modding layer** (ForgeFSE, `D:\Code\ForgeFSE-retail-shadow`):

- A DLL detours the registration hook `0x00CDB355` and hosts Lua. Each binding wraps a native call.
- Our script recovery lane converts the 161 retail quests to that Lua
  ([SCRIPT_RECOVERY_PIPELINE](../scripts/SCRIPT_RECOVERY_PIPELINE.md), Aeon's ports as the oracle).

The Lua layer works — Guild training, Orchard, Bandit Camp, White Balverine and more play through on
converted scripts — but it is a guest in an engine that was never designed to host it, and most of
our playtest time goes into rediscovering what the engine does underneath a binding.

---

## What goes wrong today

Every row below cost at least one playtest round.

| Class | Evidence | Root cause |
|---|---|---|
| Hidden yields | `StartScriptingEntity` / `StartMovieSequence` park the calling thread *inside* the binding ([GUILD_TRAINING_RECOVERY](../scripts/GUILD_TRAINING_RECOVERY.md)) | Waiting is invisible at the call site |
| Ambiguous blocking | Retail `Speak` blocks; FSE `SpeakAndWait` is not retail `Speak`; `AcquireControl` is a retry loop in retail | Binding semantics live only in the DLL body |
| Resource deadlock | Nesting a second scheduler resource for the script's own actor hung the Wife scene (New Oakvale v10); per-frame re-acquires had to saturate, not count (v11) | Acquire/release is manual and unchecked |
| Type traps | Master flags are Lua booleans, not ints; entity `OnPersist(quest, me, context)` argument order | No declared types across the boundary |
| Persistence | Hand-written `OnPersist` per script; saves cache region entities so tests need a fresh New Game | Save format is implicit and unversioned |
| Activation | A quest only exists after `AddQuest(name, TRUE)` is edited into `FinalAlbion.qst`; Steam verify-files wipes it | Registration is a patched retail data file |
| Feedback loop | No hot reload, no debugger; every change is a launch + staged save + autopilot run ([INGAME_RUNNER](../scripts/INGAME_RUNNER.md)) | The only test host is the full game |

None of these are Lua problems. They are missing contracts between the script and the engine.

---

## Goals and non-goals

Goals:

1. Waiting, ownership and persistence are visible in the script source, not discovered in playtests.
2. One source of truth for the script API: bindings, editor types and docs are generated, never
   hand-kept.
3. A broken script reports an error and stops its quest; it never hangs or crashes the game.
4. Quests, cutscenes and region scripts are data that mods add without patching retail files.
5. Most quest logic is testable without launching the game.
6. Every retail quest, every converted script, and existing FSE mods keep running.

Non-goals:

- Changing the byte-exact rebuild. It keeps `RegisterAllScripts` and the compiled quest classes.
- Replacing Lua. It is what the FSE ecosystem, Aeon's ports and our converter already target.
- A new visual language. FableForge's Blueprint-style graphs compile to this runtime.

---

## Design

### 1. Engine-native Lua host

Lua becomes an engine subsystem, not a detour. `CQuestManager` activation looks up a quest by name in
one registry holding both kinds of entry: the retail compiled classes (via their `CScriptInfo`) and
script-defined quests. There is no hook address and no load-order dependence on a DLL.

Each quest runs in its own Lua environment. An uncaught error is logged with a traceback, shown in the
dev overlay, and fails that quest (configurable: fail, pause, or retry on hot reload); the rest of the
game continues.

### 2. Generated bindings

The script API is declared once, in C++, next to the engine function it exposes:

```cpp
SCRIPT_API(Entity, Speak,
    Yields::UntilDone,               // the thread waits for the line to finish
    Doc("Play a line of dialogue; returns when it ends or is interrupted"))
EResult CScriptThing::Speak(CTextRef line, SpeakFlags flags = {});
```

A build step generates from these declarations:

- the Lua bindings (argument checking, enum names instead of ints, bool/int coercion rules);
- a LuaLS annotations file (`---@param`, `---@async`) so editors give autocomplete and type errors;
- the API reference page, including the yield behaviour of every function.

We can start this *now* against ForgeFSE: the PDB gives signatures and enum names, and the DLL
bodies give yield behaviour (see [Sequencing](#sequencing)).

### 3. Explicit async model

Every call that can wait is marked `Yields::*` and is `---@async` in the annotations. A script waits
with `await`, so waiting is always visible:

```lua
local q = Quest "Q_OakvaleReborn"

q:thread("bully", function()
  local ctl <close> = control(bully)             -- scoped, see §4
  await(bully:speak("TEXT_BULLY_01"))
  local why = await(any(timer(10), entered(hero, "Oakvale_Square")))
  if why == "timer" then q:fail("hero_left") end
end)
```

- Combinators: `any`, `all`, `timer`, `entered`, `killed`, `flag_changed`, `input`.
- **Structured concurrency:** threads belong to their quest. Completing, failing or unloading the
  quest cancels its threads and runs their cleanup. No orphaned per-frame loops.
- A synchronous call on an `@async` function from outside a thread is a load-time error, not a park.

### 4. Scoped control and resources

`AcquireControl` and scheduler resources become handles with `<close>` semantics: released on scope
exit, error or cancellation. The runtime keeps an ownership table and enforces rules at acquire time:

- re-acquiring a resource the thread already holds is a no-op (the v11 "saturate" rule, made
  structural);
- acquiring a second scheduler resource for an actor the script already drives is an error that names
  both holders (the v10 Wife hang, made a message);
- a wait cycle between threads is detected and reported with the chain.

The retail retry-loop behaviour of `AcquireControl` is the default acquire policy, so converted scripts
behave as before.

### 5. Declarative persistence

A quest declares what it saves:

```lua
q:state {
  version  = 2,
  met_wife = false,
  bribes   = 0,
  migrate  = { [1] = function(s) s.bribes = s.gold_paid or 0 end },
}
```

The engine serializes declared state inside the existing quest persistence framing, stamps the
version, and runs migrations on load. Hand-written `OnPersist` stays supported for converted scripts
but is no longer needed for new ones. Entity script state gets the same treatment.

Separately, the dev build gets a switch to refresh cached region entities on load, so a changed
level can be tested on an existing save instead of a fresh New Game.

### 6. Data-driven registration

A mod folder declares its quests, cutscenes and region scripts in a manifest; the engine merges all
manifests in load order (the same load order FableForge's Mods tab already computes). Retail
`FinalAlbion.qst` / `GlobalQuests.qst` are read, never written. Nothing a Steam verify can wipe.

Cutscene and region scripts keep the retail `[Actor.]Verb args` text format for compatibility, but
the verb table becomes a registry that Lua can extend, and a cutscene can call into a Lua function.

### 7. Developer tooling

- **Hot reload:** reload one quest's Lua in place; declared state (§5) survives, threads restart
  from their entry points.
- **Console / REPL:** run Lua against the live world, extending the retail console command registry
  ([CONSOLE_COMMAND_SYSTEM](CONSOLE_COMMAND_SYSTEM.md)).
- **Debugger:** a Debug Adapter Protocol server, so VS Code can set breakpoints and inspect threads.
- **Overlay:** live threads per quest, what each is awaiting, resource ownership, recent errors.
- **Trace log:** every binding call with arguments and result, replaces most of today's `print`
  instrumentation.

### 8. Headless test host

The biggest win for iteration speed. The same Lua runtime runs against a **fake world**: entity
registry, flags, timers, dialogue completion, quest-card state, all driven by the test. Tests read
like the quest's own logic:

```lua
test("Orchard good route completes", function(w)
  w:start_quest("Q_OrchardFarm")
  w:choose_dialogue("TEXT_FARMER_ACCEPT")
  w:kill_all("CREATURE_BANDIT", { near = "Orchard_Barn" })
  w:advance_until(quest_state("Q_OrchardFarm"), "completed", { max_seconds = 600 })
end)
```

This runs in seconds, catches ordering, persistence and deadlock bugs, and leaves the in-game runner
for what only the real engine can show (navigation, combat, presentation). The fake world's behaviour
per binding comes from the same `SCRIPT_API` declarations, so it cannot drift silently.

### 9. One compile target

Hand-written Lua, converted retail scripts, and FableForge node graphs all target this runtime. The
graph compiler emits annotated Lua, so graphs get the same type checking, tests and debugger.

---

## Compatibility

- **Retail compiled quests** stay native and share the registry (§1). Converting one to Lua is
  optional, quest by quest.
- **FSE API shim:** a Lua module exposing the ForgeFSE / upstream FSE function names with their
  retail semantics (blocking `Speak`, retry-loop `AcquireControl`, name-or-nil
  `MsgExpressionPerformedTo`). Converted scripts and existing FSE mods load unchanged; new scripts use
  the §3 API. The shim is implemented on top of the new API, so both paths exercise the same code.
- **`OnPersist`** keeps its retail argument order through the shim.
- **Saves** from retail load in the modernized build; the declared-state blocks are additive.

---

## Sequencing

**Now, on ForgeFSE (benefits the conversion lane immediately):**

1. `SCRIPT_API` table in data form: every ForgeFSE binding with PDB signature, enum names, and yield
   class taken from the DLL body. Output: LuaLS annotations for our converted scripts.
2. A lint over converted scripts using that table: calls to yielding bindings outside threads, int
   flags where booleans are expected, nested resource acquisition for the same actor.
3. A first headless test host: a Lua fake of the bindings the converted quests use, starting with
   Guild training and Orchard, where we already have in-game ground truth to compare against.

**After the recreation reaches the game loop (modernization fork):**

4. Engine-native host and unified registry (§1, §6).
5. Generated bindings from in-source declarations (§2), replacing the data table.
6. Async model, scoped resources, declarative state (§3–§5), with the FSE shim (Compatibility).
7. Tooling (§7), then the node-graph compile target (§9).

---

## Open questions

- Lua version: FSE is on Lua 5.1-era APIs; `<close>` needs 5.4. Options are a 5.4 host with a 5.1
  compatibility shim, or scoped handles via `pcall` wrappers on 5.1. Needs a survey of what FSE mods
  actually rely on.
- How much of the scheduler's resource model can the fake world reproduce faithfully without
  reconstructing it first.
- Whether hot reload should restart threads or attempt to resume them at the same await point.
- Mod sandboxing: which APIs (file I/O, `os`, native calls) a downloaded mod script may use.
