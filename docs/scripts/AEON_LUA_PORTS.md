# Aeon FSE upstream review — 2026-09-02

Upstream: `https://github.com/eeeeeAeoN/FableScriptExtender` @ `master`
(commits dated 2026-08-31 … 2026-09-02). Our fork is `D:\Code\ForgeFSE`
(no git remote configured; 8 local commits ahead, working tree dirty).

## Verdict

**Yes, directly useful, and it unblocks a lane that was blocked.** Upstream added
exactly the API surface our reconstructed-script corpus was missing, and Aeon's
own Lua ports of retail quest scripts are a ground-truth oracle for the script
recovery lane.

**The 14 bindings are ported into ForgeFSE** (see "Port into ForgeFSE" below); the
corpus is now `111/111 callable, 0 missing, 0 wrong-scope` against our own fork.
One gap remains, the only failing check in
`refs/script_recovery/foundation_audit.json` (17/18):

- **`V_StatueMaster` and `Q_DragonBossFight` need a Ghidra anchor-discovery run.**
  Their clusters are already decompiled (allocator/constructor/vtable/lifecycle);
  only `evidenceAnchors` is empty, so the "anchored native decompilation" gate fails.

## What upstream added

14 new Lua bindings, **zero removed** (diff of the `*_type["Name"]` registration
tables, upstream 943 rows vs our stale reference checkout 929):

| Scope | Binding | Notes |
|---|---|---|
| Entity | `GetAngleXY` | facing angle, XY plane |
| Entity | `FadeOutAndKillEntity` | defaults `true / 1.0s / true` |
| Entity | `SetIsPushableByHero` | the "fixed SetPushableByHero" commit — an entity-scope overload of the existing quest-scope setter |
| Quest | `StartCutscene` | opens a manual cutscene session (registers actors, starts movie sequence) |
| Quest | `RunCutscene` | runs one macro inside an open session; blocking |
| Quest | `EndCutscene` | releases actors/camera/buffers |
| Quest | `RunCutsceneWithSetup` | one-shot setup+run+teardown |
| Quest | `GetCutsceneFlag` | reads the active cutscene flag map |
| Quest | `SetCutsceneActionMode` | |
| Quest | `FadeOutAndKillEntity` | quest-scope form |
| Quest | `AddLogbookStoryEntry` | `sol::overload(int, string)` |
| Quest | `AddLogbookStoryEntryString` | |
| Quest | `GetDistanceBetweenThings` | |
| Quest | `SetMasterGameState` | writes into the master (Gameflow) script state |

Plus a **threading refactor** in `LuaQuestHost.cpp`: each host now allocates a
dedicated `lua_newthread` coroutine stack for `Main()` and one per spawned
thread, holds them via `luaL_ref(LUA_REGISTRYINDEX)`, and releases them in a new
`CleanupThreads()` called from both the destructor and `Destructor(bool)`. Thread
runners execute in `sol::environment(threadL, m_env)` rather than the shared
state. `MAX_QUEST_THREADS` is 20. Aeon's release note for it: *"a totally
refactored threading system to mirror the original game's script thread execution
more closely."*

## Divergence: ForgeFSE vs upstream

Clean split — **no binding-table conflicts**:

- **Upstream-only (we lack):** the 14 above.
- **ForgeFSE-only (41 bindings we added):** region streaming (`GoToMap`,
  `GoToMapSlot*`, `PreloadNewScene`, `IsRegionDefLoaded`,
  `IsRegionLoadedAndPreloaded`, `ProbeRegionVectorSize`, `ProbeRegionContainsSlot`,
  `AliasMapResources`, `ClearMapResourceAlias`), doors/houses (`CloseDoor`,
  `JamDoor`, `OpenHouseDoors`, `CloseHouseDoors`, `SetHouseOwnedByPlayer`),
  conversations (`AddNewConversation`, `AddPersonToConversation`), sound
  (`PlaySoundId`, `PlaySoundIdOnThing`, `GetSoundIdFromSymbol`), expert verbs
  (`HeroGoFishing`, `HeroGoDigging`, `GetHeroFishingLevel`), follow-blocking
  entity verbs, and `GetFSEInfo` / `GetGameLanguage` / `GetGroundHeightAt`.
- ForgeFSE's `LuaQuestHost` ctor also opens `sol::lib::package` and prepends
  `g_fseBasePath` to `package.path` so scripts can `require`. Upstream does not.
  **A merge must keep both** the `package.path` setup and the new thread stacks.

Merging is therefore a port, not a fast-forward: `ForgeFSE` has no remote, so a
manual port was done rather than a merge. See below.

## Port into ForgeFSE — done 2026-09-02

All 14 bindings are in. `ForgeFSE/LuaManager.cpp` is now a strict superset of
upstream's binding table: **984 rows = upstream's 943 + our 41**, with zero
upstream rows missing and none of ours lost. Release x86 `/t:Rebuild` is clean
(`D:\Code\ForgeFSE\Release\FableScriptExtender.dll`), and the corpus audit
against the ported fork reports `111/111 callable, 0 missing, 0 wrong-scope`
(892 quest + 92 entity bindings registered).

What the port touched:

- **`FableAPI.h` / `FableAPI.cpp`** — five new API pointers. Three are Aeon's
  addresses: `FadeOutAndKillEntity_API` `0x00CBE716`,
  `AddLogbookStoryEntry_API` `0x00CBE87F`,
  `AddLogbookStoryEntryString_API` `0x00CBE960`. Two are the
  `std::map<CCharString,bool>` cutscene-flag map accessors:
  `StdMap_Bool_OperatorBracket_API` `0x008ADF10`,
  `StdMap_Bool_Destroy_API` `0x007ADD50`.
  *Address caveat, recorded in the source:* our own BSim label for `0x00CBE87F`
  is `CSubtitleRenderer::SetText` at similarity 0.51. All three addresses sit in
  the `0x00CBxxxx` script-interface thunk band (same band as the already-trusted
  `0x00CBFB7D` `RunCutsceneMacro` and `0x00CBFAB8` `SetScriptActiveStatus`), so
  per CLAUDE.md the FSE ASLR address wins over the low-confidence BSim name.
  This has **not** been confirmed at runtime.
- **`LuaQuestState.h`** — 11 method declarations, the `ActiveCutsceneActor`
  struct and the five cutscene-session members, plus a `~LuaQuestState()`
  declaration (ForgeFSE had no destructor at all).
- **`LuaQuestState.cpp`** — upstream's bodies verbatim (560 lines across the 11
  methods), the file-local `GetScriptThingFromSolObject` coercion helper that
  those bodies need (ForgeFSE's own bindings take `shared_ptr<CScriptThing>&`
  directly), and the destructor that closes a leaked cutscene session.
- **`LuaEntityAPI.h` / `.cpp`** — `LuaEntityAPI::GetAngleXY`. The
  `CScriptThingVTable::GetAngleXY` slot (`0x28`, index 10) was already declared
  in ForgeFSE's `EntityScriptingAPI.h`; only the accessor was missing.
- **`LuaManager.cpp`** — the 14 registration rows (3 entity lambdas, 11 quest).
- **`GameInterface.h` + `LuaQuestState.cpp`** — **a real ABI bug in our fork,
  found by the port.** ForgeFSE's `tSetIsPushableByHero` typedef took
  `CScriptThing*`; retail takes `CScriptThing` **by value** as a refcounted
  handle copy. This is what Aeon's "fixed SetPushableByHero" commit changed. The
  compile failed on it, which is how it surfaced. ForgeFSE's own quest-scope
  `SetIsPushableByHero` had been passing a pointer where retail expects the
  struct, so it was broken before this port; it now passes a refcount-bumped
  copy by value like upstream.

**Not committed.** The ForgeFSE working tree already carried substantial
uncommitted work in the same files (`+3404/-128` across 16 files, including
`FableAPI.cpp` `+1868` and `LuaQuestState.cpp` `+1057`), so the port cannot be
isolated into its own commit by file. It is staged in the working tree for review.

## Threading refactor port — done 2026-09-02

Aeon's release note for it: *"a totally refactored threading system to mirror the
original game's script thread execution more closely."*

Ported into `LuaQuestHost.{h,cpp}`. After the port, a newline-normalised diff
against upstream shows **only three remaining differences, all deliberately ours**:
the `sol::lib::package` + `package.path` block, our `Destructor(bool)`, and our
doc comments. Every line of thread logic is upstream's. Release x86 `/t:Rebuild`
is clean.

What changed:

- **A dedicated `lua_State` coroutine stack per execution context.** The
  constructor now allocates one for `Main()` via `lua_newthread` and anchors it
  with `luaL_ref(LUA_REGISTRYINDEX)`; `CreateThread` does the same per thread
  slot. Previously every thread and `Main()` shared the host's single `lua_State`,
  so a blocking API call that yielded inside one thread ran on the same Lua stack
  as every other. `ThreadRunner<N>` and `Main()` now build a
  `sol::environment(threadL, m_env)` over their own stack. This is what makes it
  resemble the engine's own per-script-thread execution.
- **Thread bookkeeping replaced.** The parallel `m_threadFunctions` /
  `m_threadArgs` maps became one `std::map<int, QuestThreadInfo>` carrying the
  function name, bound args, `lua_State*` and registry ref together. Those two
  maps had no other users, so the swap is local to this file pair.
- **`CleanupThreads()`** releases every registry reference (thread stacks plus
  `Main()`'s) and is idempotent.
- **Termination guards.** `Main()` and `ThreadRunner<N>` now bail if
  `IsActiveThreadTerminating_Quest_API(&base)` or
  `IsActiveThreadTerminating_API(pInterface)` is true, so no Lua is entered while
  the engine is tearing the script thread down. `Main()` previously had no guard
  at all and `ThreadRunner` had only the first. Needed `#include "GameInterface.h"`.
- **Thread diagnostics.** `ThreadRunner<N>` was silently swallowing every C++
  exception (`catch (const std::exception&) {}`) and said nothing when a thread's
  Lua function was missing. Both now log, as upstream does.

### The one place the merge had to decide

`Destructor(bool bDelete)` conflicts. ForgeFSE's version is a deliberate fix
(commit `43ecd61`, defect B2): it honours the scalar-deleting-destructor contract
with `this->~LuaQuestHost(); operator delete(this);`, because `QuestAllocator<N>`
heap-allocates the host with a plain `new` and sets up no deleter block — without
it, a `LuaQuestHost` plus its whole isolated Lua VM leaked on every load.
**Upstream's `Destructor` never deletes**; it only calls `CleanupThreads()`, so
upstream still leaks the host.

Resolution: keep our delete. `~LuaQuestHost()` calls `CleanupThreads()`, so the
deleting path is covered; the `else` branch calls `CleanupThreads()` directly to
keep upstream's behaviour for the non-deleting call. Both fixes survive.

### Deliberately not ported

`LuaEntityHost::Main`'s guard. Upstream changed it to check the parent quest and
the interface; ForgeFSE checks `IsActiveThreadTerminating_Entity_API(this)`, which
is the entity-specific API and strictly more precise for an entity host. Swapping
it would be an unrelated behavioural change, so it stays.

### Not runtime-tested

The DLL builds and the diff is clean, but nothing here has been exercised in-game.
A script that spawns threads via `CreateThread` and blocks inside them (the
`HerosOldHouse` port is the obvious candidate — it uses cutscenes and multiple
entities) is the test that matters, along with a region unload to prove
`CleanupThreads()` runs without a double-unref.

## Aeon's ported retail Lua scripts

Aeon is reverse-engineering retail game scripts to Lua and publishing them as
reference. Eight packages received so far; the six previously ingested are
byte-identical to what is in the corpus, so only two are new:

- `StatueMaster.zip` → retail `V_StatueMaster` (`S_VSM`, allocator `0x00ED4A40`)
- `DragonBossFight.zip` → retail `Q_DragonBossFight` (`S_QDBF`, allocator `0x00D27240`)

Deployment notes from Aeon that apply to every package: the script must be added
to `data\Levels\FinalAlbion.qst`, the TNG quest-section names must be repointed
(`V_StatueMaster` → `StatueMaster` in `Greatwood_1.tng` and `LookoutPoint.tng`),
and shipping one means disabling the retail original so both do not run. He also
notes retail `V_StatueMaster` contains **unused code detecting the statue pointing
at the Guild**, and assumes a reward was intended there and cut.

Note on our own StatueMaster reward, to keep the provenance straight: **The Statue
Master's Hammer is authored by us, not a recovered cut reward.** It is a new
`OBJECT_STATUEMASTER_HAMMER` built on a CC BY 4.0 model by `angeldavidvillanueva`
over the `OBJECT_LEGENDARY_WARHAMMER` donor chain (`mods/StatueMasterRestoration/RewardPlan.lua`,
staged in `work/statuemaster-hammer-package`, still gated at `RewardPlan.Enabled = false`).
Aeon's unused Guild-pointing branch is a separate, unrelated observation about
retail; we have never located a cut retail reward for this quest. If the two are
ever connected, that connection still has to be proven from the retail code.

`Q_WaspBoss` is in progress upstream.

## Cross-checks run against the two new packages

Independent corroboration from our own native IR — not taken on trust:

- **Entity bindings.** Every binding declared in both new packages' `quests.lua`
  appears in the retail lifecycle IR. Shared string literals: StatueMaster 8,
  DragonBossFight 4 (`refs/script_recovery/seed_native_comparison.json`).
- **Real port gap found in DragonBossFight.** Retail `Q_DragonBossFight` binds
  **three** entities — `DBMinion`, `DBSummoner`, `Dragon`. Aeon's `quests.lua`
  registers only `Dragon`, even though `DBMinion.lua` and `DBSummoner.lua` ship
  in the zip. Worth reporting upstream; the fix is two registry rows.
- **StatueMaster is not a gap.** Retail binds exactly the three registered
  entities; `StatuePointing.lua` is an extra Lua-side helper with no native
  counterpart (consistent with Aeon's statue-pointing note).

## Pipeline state after ingest

| Artifact | Before | After |
|---|---|---|
| `refs/fse_api_manifest.json` functions | 933 | 947 |
| Corpus packages / scripts / IR | 6 / 16 / 16 | 8 / 25 / 25 |
| Compat report: API call sites / unique pairs | 542 / 84 | 825 / 111 |
| Compat report: **missing APIs** | **6** | **0** |
| Corpus validation errors | 1 (`wrong-api-scope`) | 0 |
| Catalog matched seeds | 6 | 8 |
| Priority-0 conversion-queue rows | 6 | 8 |
| Shadow manifest entries (registered scripts only) | 16 | 22 |
| Foundation audit | 18/18 | 16/18 (two gaps above) |

Three shipped entity scripts are deliberately **not** shadow entries because
their package `quests.lua` never registers them: `StatuePointing.lua`,
`DBMinion.lua`, `DBSummoner.lua`.

## Reproduce

    python tools/script_recovery/update_fse_manifest_2026_09.py

    python tools/script_recovery/recover_scripts.py \
      D:/Downloads/GuardianSisterInfo.zip D:/Downloads/GuardianSisterInfo2.zip \
      D:/Downloads/GuardianTrophyDealerInfo.zip D:/Downloads/HerosOldHouse.zip \
      D:/Downloads/MazeResearch.zip D:/Downloads/MeetSister.zip \
      D:/Downloads/StatueMaster.zip D:/Downloads/DragonBossFight.zip \
      --output refs/script_recovery/seed_corpus

    python tools/script_recovery/analyze_compatibility.py \
      --corpus refs/script_recovery/seed_corpus \
      --json refs/script_recovery/compatibility.json \
      --markdown docs/modding/README.md#forgefse_script_compatibility

    python tools/script_recovery/build_native_catalog.py \
      --output refs/script_recovery/native_catalog.json \
      --corpus refs/script_recovery/seed_corpus/corpus_index.json

    python tools/script_recovery/build_conversion_queue.py \
      --catalog refs/script_recovery/native_catalog.json \
      --json refs/script_recovery/conversion_queue.json \
      --tsv refs/script_recovery/conversion_queue.tsv \
      --clusters refs/script_recovery/native_clusters \
      --native-ir refs/script_recovery/native_operation_ir

    python tools/script_recovery/audit_forgefse_runtime.py \
      --compatibility refs/script_recovery/compatibility.json \
      --lua-manager D:/Code/ForgeFSE/FableScriptExtender/LuaManager.cpp \
      --json refs/script_recovery/forgefse_runtime_audit.json \
      --markdown docs/modding/README.md#forgefse_runtime_script_audit

    python tools/script_recovery/build_override_manifest.py \
      --catalog refs/script_recovery/native_catalog.json \
      --corpus refs/script_recovery/seed_corpus/corpus_index.json \
      --output refs/script_recovery/shadow_manifest.json \
      --lua-output refs/script_recovery/retail_shadow.lua

    python tools/script_recovery/compare_seed_native_ir.py \
      --sources refs/script_recovery/seed_corpus/sources \
      --native-ir refs/script_recovery/native_operation_ir \
      --output refs/script_recovery/seed_native_comparison.json

    python tools/script_recovery/verify_foundation.py --root refs/script_recovery \
      --output refs/script_recovery/foundation_audit.json

    python -m pytest tools/script_recovery -q

Extracted upstream packages for inspection: `work/aeon_lua_ports/`.
