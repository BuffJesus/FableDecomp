# Quest binding RE — how Fable TLC binds a quest name to its logic

*Created 2026-07-20. Question: when the engine activates a registered quest name, does it
run compiled C++ code, or interpret data? Answer determines whether custom quests can ever
be pure data or whether an injector (FSE) is structurally required.*

**Status: COMPLETE (2026-07-20). All decompile evidence from read-only headless passes on
the FableTLC Ghidra project; logs/artifacts under `ghidra_out/quest_*`.**

## VERDICT

**HYBRID — quest LOGIC is NATIVE-CODE-BOUND (CONFIRMED); everything around it
(selection, activation, rewards, presentation, persistence framing, and the
cutscene/region-script layer) is name-keyed DATA (CONFIRMED).**

1. **Quest LOGIC = compiled C++ (CONFIRMED).** Each quest is a compiled
   `NScript::CQ_<Name>Script` class (derived from `CScriptBase`) linked into `Fable.exe`.
   The registry maps quest **name (CCharString) → native allocator function pointer**
   (`CScriptInfo::pAllocFunc`), built by a compiled-in registration routine
   (`0x00CD52D0`, 161 entries — E3). Activation instantiates the class through that raw
   code pointer (E6). There is **no quest bytecode, no quest interpreter, and no fallback
   path for unregistered names** (E5): a quest name that is not in the compiled table is
   silently dropped at the `IsQuestRegistered` gate.
2. **Selection/activation = data.** Quest *names* arrive from data (world initial-quest
   string at `CMainGameComponent::PostInit`, script commands, save files, other quests)
   and are honored — but a name can only ever select one of the 161 compiled classes.
3. **Cutscene/region scripts = interpreted data** (`CCutsceneDef`/`CRegionScriptDef`
   payloads are text command lists run by `RunCutsceneMacro_Func @ 0x00CBFB7D` —
   docs/SCRIPT_VM_MAP.md). Compiled quests *invoke* this layer; it does not host
   quest state machines. It is fully data-editable.
4. **Consequence: brand-new quest logic REQUIRES native code injection** (FSE/DLL or a
   patched exe). This is structural, not a tooling gap. FSE is architecturally the
   *minimal* correct solution: it appends `CScriptInfo` records whose allocators return
   Lua-hosting `CScriptBase` clones (E1).

## Evidence log (incremental)

### E1. FSE source = a working map of the retail registration seam (CONFIRMED, source read)

`D:\Code\FQT\SourceFilesToReference\FSE\FableScriptExtender-master\FableScriptExtender\dllmain.cpp`:

- FSE installs a JMP at retail **`0x00CDB355`** — the tail of the engine's own script
  registration routine (hook epilogue re-executes `pop edi/esi/ebp/ebx; add esp,18h; retn`,
  i.e. the original function's epilogue). The script-manager object is in `ESI` at that point.
- To add a quest, FSE builds the engine's own record type and calls engine functions:

```cpp
CScriptInfo newScriptInfo;                       // engine struct
newScriptInfo.pAllocFunc     = QuestAllocator<N>;   // returns a CScriptBase-derived object
newScriptInfo.pAllocDataFunc = pSunnyvaleDataAlloc_func; // engine fn @ 0x00CDBD20
newScriptInfo.ID             = q.id;
newScriptInfo.MasterScript   = false;
CCharString_Construct_Copy(&newScriptInfo.Name, name);
CheckSection(pMan, "S_"+name);                   // engine fn @ 0x00CB5AC0
AddScript(pMan, &newScriptInfo, "S_"+name);      // engine fn @ 0x00CB5C90
SetScriptActiveStatus_Func(&Name, ESAS_UNSTARTED); // engine fn @ 0x00CBFAB8
```

- A quest object is a `CScriptBase` (ctor @ **`0x00CB8110`**) whose **vtable defines the
  quest contract**: `[0]=Destructor, [1]=RegisterMain, [2]=Main, [3]=Init, [4]=OnPersist`
  (FSE's `g_LuaQuestHostVTable` replicates exactly this shape and the engine calls through it).
- Quest logic runs as cooperative "spawned functions" (threads): `CSpawnedFunc` ctor @
  **`0x00CDD450`**, added per-section via `AddSpawnedFunction @ 0x00CB7E50`; termination poll
  `IsActiveThreadTerminating(Quest) @ 0x00CB7940`. `CSpawnedFunc` carries a **raw native code
  pointer** `pThunkToMain` — the "thread body" is machine code, not data.
- Persistence: engine calls vtable `OnPersist(CPersistContext*)`; primitives via
  `CPersistContext::Transfer` overloads (`bool 0x004045C0`, `int 0x00410BE0`,
  `string 0x004109A0`, `float 0x00410620`, `uint 0x004106F0`).

Interpretation: the ONLY way FSE can add a quest is to hand the engine a **native function
pointer**. If quests were data-interpreted, FSE would inject data instead. This alone makes
NATIVE-CODE-BOUND highly likely; Ghidra decompile of the registration function is the
confirming second source (E3).

### E2. Retail exe strings corroborate name-keyed registry (CONFIRMED, bytes read)

Direct scan of `C:\Programs\Steam\...\Fable.exe`: `QuestManager: Activate Quest`,
`QuestNotInScripts`, `QuestActivated/QuestStarted/QuestComplete/QuestFailed/...` (state-machine
strings), and ~100 section names `S_QBC, S_QHSA, S_QGTW, S_VB, ...` matching FSE's
`"S_"+questName` convention — the shipped quest set is enumerated in `.rdata` as strings
next to a registration table/routine.

### E3. The retail registration function — CONFIRMED by decompile

**`RegisterAllScripts` = `FUN_00cd52d0` @ `0x00CD52D0`** (undefined-named in DB; ends at the
FSE hook `0x00CDB355` — the hook patches its final bytes). Found via xrefs to
`AddScript @ 0x00CB5C90`: **60+ calls, one every ~0x99 bytes, all from this one function**
(`ghidra_out/quest_refs.log`). Full decomp: `ghidra_out/quest_register_all.c` (5,149 lines).

It is a straight-line, **compiled-in** registration list. Verbatim block shape (one per
script; names below have known BSim mislabels — FSE-verified identities in brackets):

```c
CCharString::CCharString(local_18,"Q_HeroSoulsArena",-1);   // quest NAME
CCharString::CCharString(local_c,local_18);                 //   -> CScriptInfo.Name
local_8 = 1;                                                //   ID field
local_4 = 0;                                                //   MasterScript flag (1 only for Q_SunnyvaleMaster)
local_14 = (code *)&LAB_00d80ba0;                           //   pAllocFunc  = PER-QUEST NATIVE CODE PTR
local_10 = pSunnyvaleDataAlloc_func;                        //   pAllocDataFunc @ 0x00CDBD20
CCharString::CCharString(&stack..., "S_QHSA", -1);          //   section name
AddScript(&local_14);                                       // 0x00CB5C90
...
CCharString::CCharString(local_18,"Q_HeroSoulsArena",-1);
CRegion::SetMiniMapRegionExitTextOffsetX(...);              // [misname; = SetScriptActiveStatus @ 0x00CBFAB8 per FSE]
```

**161 script registrations extracted** → `ghidra_out/quest_registry_table.tsv`
(quest_name / id / master / allocFunc / dataAlloc / section). Full plain-text quest names
recovered: `Q_SunnyvaleMaster` (master script), `Gameflow`, `HeroBoasts`,
`PersonalScriptMain`, `Q_Arena`, `Q_BountyHunt`, `Q_DragonBossFight`, `Q_EndGameBossBattle`,
all `Q_*` story quests + `V_*`-section village scripts. Every entry's `pAllocFunc` is a
**distinct code address** in `0x00CF9xxx–0x00D8xxxx` — megabytes of per-quest compiled logic.

There is **no data table** behind this: the name→allocator pairs are immediate operands in
`.text`. Registration is compiled into the exe, executed once at startup (progress strings
"Registering Master Script" / "Registering Important Scripts" shown on the loading screen).

The real class scheme is visible through ported PDB names (fablewin editor build + ego_r
BSim): `NScript::CQ_<QuestName>Script` classes — `CQ_HeroSoulsArenaScript`,
`CQ_BountyHuntScript`, `CQ_ArenaScript`, `CQ_GameflowScript`, `CQ_SunnyvaleMasterData`, ...
(thousands of member-function name hits across `ghidra_out/egor_pdb_names.tsv`,
`fablewin_pdb_names.tsv`). One quest = one compiled C++ class.

### E4. What an allocator does — CONFIRMED by decompile (Q_HeroSoulsArena)

`FUN_00d80ba0` (the `pAllocFunc` registered for `Q_HeroSoulsArena`):

```c
this = operator_new(0x68);                       // heap-alloc quest object (0x68 bytes)
CScriptBase::CScriptBase(this);                  // base ctor @ 0x00CB8110 [DB misname CHeroMorphDef]
*(this+0x40) = param_2;  *(this+0x44) = param_1; // pInterface / pMasterData
*(void***)this = &PTR_..._012d3994;              // PER-QUEST COMPILED VTABLE @ 0x012D3994
uVar1 = (**(code **)(*DAT_0143e8f8 + 0x15c))();  // grab from CGameScriptInterface
```

i.e. `new NScript::CQ_HeroSoulsArenaScript(data, iface)` — a C++ object whose vtable
(`0x012D3994`, in `.rdata`) carries the quest contract FSE mirrors:
`[0]=deleting-dtor, [1]=RegisterMain, [2]=Main, [3]=Init, [4]=OnPersist`. The quest's Main
is a compiled function; quest "threads" are `CSpawnedFunc` records whose `pThunkToMain` is a
raw machine-code pointer (E1).

### E5. Activation = registry lookup by name; unregistered names are DROPPED (CONFIRMED)

`CGameScriptInterface::ActivateQuest @ 0x00892e80` → thin wrapper →
`CQuestManager::ActivateMultipleQuests @ 0x004B4260` (singleton `DAT_013b89fc`; decomp in
`ghidra_out/quest_activate_decomp.c`):

```c
for each name in array {
    NProgressDisplay::DisplayProgress("QuestManager: Activate Quest", ...);
    if (IsQuestRegistered(this, name)) {         // registry gate
        iVar5 = FUN_00cb5ad0(name);              // section/script-info lookup
        ...build CActivateQuestInfo{name, flags}...
        FUN_004bb720(...);                       // push onto pending-activation list
        bVar7 = true;
    }                                            // NOT registered -> silently skipped
}
FUN_004b3ce0(&local_24);                         // process activations
```

A quest name arriving from DATA (WLD initial quests, .qst, script commands) can only ever
**select** one of the 161 compiled entries. There is no fallback interpreter path for an
unknown name — it is skipped at the `IsQuestRegistered` gate.

### E6. The binding moment — activation queue processor (CONFIRMED by decompile)

`CQuestManager::ProcessQuestActivations` = `FUN_004b3ce0 @ 0x004B3CE0` (decomp in
`ghidra_out/quest_decomp3.log`). Pass 1 preloads each pending quest's code section
(`CCodeSectionManager::LoadSection`) and calls
`CQuestManager::LoadResourcesForQuests`. Pass 2 binds — the load-bearing lines:

```c
pCVar5 = *(CScriptInfo **)(pCVar1 + 4);              // the registered CScriptInfo
if (pCVar5[0x10] == 0)  CQuestManager::GetMasterScriptDataForScript(this, pCVar5);
else {
  pCVar25 = (CScriptDataBase *)(**(code **)(pCVar5 + 4))();  // call pAllocDataFunc
  (**(code **)(*(int *)pCVar25 + 8))();                      // init the data object
}
pCVar18 = (CScriptBase *)(*(code *)**(undefined4 **)(pCVar1 + 4))(); // CALL pAllocFunc
this_01 = operator_new(0x34);
CQuestManager::CActiveQuest::CActiveQuest(this_01, ..., pCVar1,
                                          *(CScriptInfo **)(pCVar1+4), pCVar18, ...);
CScriptBase::Activate(pCStack_24);                   // start the quest object
if (saved) CScriptBase::LoadGameState(scriptBase, savedState); // restore persisted state
```

Binding a name to logic is literally an **indirect call through the compiled allocator
pointer** stored at registration. The result is wrapped in a
`CQuestManager::CActiveQuest` (class confirmed by ego_r BSim names
`CQuestManager::CActivateQuestInfo`, `CQuestManager::CActiveQuest`,
`CQuestManager::CQuestCard` — `ghidra_out/quest_manager_decomp.c`).
Saved-state lookup on the way in: `FUN_00cb5ad0 @ 0x00CB5AD0` = LowerBound over
`CVectorMap<CCharString, NQuestManager::CRuntimeQuestPersistence::CPersistInfo>` —
per-quest persisted blobs are keyed by quest name.

### E7. A quest object's vtable = per-quest compiled functions (CONFIRMED)

Vtable registered by the `Q_HeroSoulsArena` allocator, dumped at `0x012D3994` (.rdata):

```
[0] 0x00D80C00   deleting dtor
[1] 0x00D7C740   RegisterMain
[2] 0x00D7C890   Main
[3] 0x00F26B90   (shared default)
[4] 0x00D7C800
[5] 0x00D7C7E0   Init / OnPersist family (FSE replaces exactly slots 0-4)
```

All slots point into the per-quest code region. Quest "threads" are `CSpawnedFunc`
records (`ctor 0x00CDD450`) whose `pThunkToMain` is a raw machine-code pointer, pumped by
the script scheduler (`AddSpawnedFunction @ 0x00CB7E50`,
`IsActiveThreadTerminating @ 0x00CB7940`). Per-frame quest execution = calling compiled
functions, full stop.

### E8a. Reconciliation with .qst (two-level registry) — see docs/QST_FORMAT.md

The concurrent .qst RE established that `FinalAlbion.qst`/`GlobalQuests.qst` are ASCII
`AddQuest("Name", TRUE|FALSE);` scripts read during "Load Quests". This is the **data-side
quest list** (CQuestManager: which quests exist / start active). It is a *different level*
from the **code-side script table** built by `0x00CD52D0` (script manager:
name → `pAllocFunc`). Both are keyed by the same name string. Appending an `AddQuest` line
registers a *name* (data) — but when activation reaches the binding step (E6), the name
must also resolve to a compiled `CScriptInfo`, or there is no logic to run (the engine
ships explicit vocabulary for that case: `"QuestNotInScripts"`,
`"DUMMY_QUEST_HAS_NO_SCRIPT"`). FSE's shipped test .qst demonstrates the full contract:
it appends `AddQuest("MyFirstQuest", FALSE);` (data) AND injects the matching
`CScriptInfo` + allocator at the 0xCDB355 hook (code). Data registers the name; only code
supplies the logic.

### E8. Where activation names come from + persistence (CONFIRMED)

- **New game:** `CMainGameComponent::PostInit @ 0x00416xxx` (string xref `0x00416bd1`):
  after `CWorld::ActivateWorld`, shows progress "Activate Initial Quests" and calls the
  ActivateQuest wrapper (`FUN_004b4a10`) with a **CCharString member loaded from world/
  config data** (`this+0x161d8`) — data supplies the *name* (the WLD initial-quests list
  decoded by forge::wld is this feed).
- **During play:** shipped quests activate other quests by name (e.g. the compiled quest
  `Main` at `0x00EEExxx` references the quest name `"QuestNotInScripts"`); script-side
  activation surface is `CGameScriptInterface::ActivateQuest/ActivateMultipleQuests/
  DeactivateQuest/SetQuestAsCompleted/...` @ `0x00892E80–0x00893030`.
- **Saves:** `CWorld::SaveGameState`/`LoadGameState` write/read a `START_SAVED_QUESTS` …
  `END_SAVED_QUESTS` section (string xrefs `0x004b6177` / `0x004b5892`), containing quest
  names + per-quest `OnPersist` payloads (`CPersistContext::Transfer` primitives:
  bool `0x004045C0`, int `0x00410BE0`, string `0x004109A0`, float `0x00410620`,
  uint `0x004106F0`). Saves persist quest **state variables**, never quest code — on load
  the name re-selects the compiled class and `LoadGameState` refills its members (E6).
- Related data-side markers in the exe: `START_ACTIVE_QUESTS`, `START_NEW_QUEST`,
  `START_NEW_MASTER_QUEST_DATA`, `START_QUEST_CARDS`, `CURRENT_QUESTS_ENTRY` — the same
  text-sectioned serialization family (see docs/SAVE_ENTITY_GRAPH.md and the concurrent
  .qst RE for the file-side counterpart).

### E9. Entity scripts (TNG ScriptName) — same architecture (CONFIRMED via FSE + symbols)

TNG-placed things carry `ScriptName`/`ScriptData`. The engine binds a thing's ScriptName
through `AddEntityScriptBinding @ 0x00CB8230` + `PostAddScriptedEntities @ 0x00CB8930`
(both confirmed named in the DB) — again a **name → native allocator** binding (FSE's
`EntityAllocator`/`LuaEntityHost` mirrors the record; vtable contract
`[dtor, Main, Init, GetParentScript, OnPersist, OnPredicateFail, OnInterrupted]`).
`"ScriptQuestName"` (string @ `0x01264cd0`) is the field tying a scripted thing to its
owning quest. So per-entity behaviors are ALSO compiled classes selected by data names —
the same verdict applies one level down. (Loader-side decompile of the TNG field
consumption not done here; marked INFERRED for that last hop, everything else confirmed.)

## Quest lifecycle (as established)

```
STARTUP     FUN_00cd52d0 @ 0x00CD52D0 (compiled-in) registers 161 CScriptInfo records:
            {Name, ID, MasterScript, pAllocFunc(native), pAllocDataFunc(native), "S_xxx" section}
            via AddScript @ 0x00CB5C90 into the script manager; status = UNSTARTED
            (SetScriptActiveStatus @ 0x00CBFAB8).

SELECTION   A name arrives from DATA: world initial-quest string (PostInit), a compiled
            quest calling ActivateQuest("..."), or a save's START_SAVED_QUESTS section.

ACTIVATION  CGameScriptInterface::ActivateQuest @ 0x00892E80
            -> CQuestManager::ActivateMultipleQuests @ 0x004B4260 (singleton DAT_013b89fc)
               gate: IsQuestRegistered(name) — unknown names dropped
            -> queue CActivateQuestInfo -> FUN_004b3ce0:
               preload code section + resources, then
               obj = pAllocFunc()  (indirect call, per-quest compiled allocator)
               new CActiveQuest(name, scriptInfo, obj)
               CScriptBase::Activate(obj); optional CScriptBase::LoadGameState(obj, saved)

RUN         obj->vtbl: RegisterMain/Main (compiled). Main spawns CSpawnedFunc threads
            (raw code ptrs) via AddSpawnedFunction @ 0x00CB7E50; scheduler pumps them
            per frame; termination polled via IsActiveThreadTerminating @ 0x00CB7940.
            Quest code drives the world through CGameScriptInterface (891 methods,
            *DAT_0143e8f8) and can run data-side cutscene/region text scripts
            (RunCutsceneMacro_Func @ 0x00CBFB7D).

PERSIST     Save: quest name + OnPersist(CPersistContext) payload into
            START_SAVED_QUESTS..END_SAVED_QUESTS (SaveGameState @ 0x004b61xx).
            Load: name re-selects compiled class; LoadGameState refills members.
```

## What a custom quest minimally requires

**The boundary, precisely:**

| Aspect | Data-drivable today (no injector) | Needs native code |
|---|---|---|
| Quest logic / state machine / objectives flow | — | **YES** (compiled CQ_*Script class; FSE Lua host or DLL) |
| Which quests activate at world start | YES (WLD initial-quest names) | — |
| Activating/completing/failing an EXISTING quest from script data | YES (cutscene/region text verbs + existing quest cross-activation) | — |
| Quest cards (name, gold/renown reward, objectives text, icon) | YES (`CQuestCardDef`/`CActivateQuestDef` in game.bin; `SetQuestCardGoldReward` etc. are data-reachable) | — |
| Cutscenes / region scripts (staging, dialogue, spawns, camera) | YES (script.bin text commands — SCRIPT_VM_MAP) | — |
| Placed-thing behavior (TNG ScriptName) | only selects among compiled entity scripts | new behavior = native |
| Quest state persistence in saves | automatic once a quest exists (name-keyed OnPersist) | — |
| Re-plotting an EXISTING quest (its markers, creatures, regions, rewards, cutscenes) | LARGELY (its data inputs) | its control flow is frozen in code |

So: **"pure-data custom quest" is impossible in retail TLC** beyond recombining shipped
quest logic. The minimum viable custom-quest stack is: FSE (or equivalent DLL) providing
one `CScriptInfo` + a `CScriptBase`-shaped host (5-slot vtable) — everything else (cards,
rewards, text, cutscenes, regions, activation, saves) can then be authored as data.
FableForge's authoring surface + ForgeFSE runtime is exactly the right split.

## Evidence index

- `ghidra_out/quest_register_all.c` — full decomp of RegisterAllScripts `0x00CD52D0`.
- `ghidra_out/quest_registry_table.tsv` — 161 extracted registrations (name/id/master/alloc/section).
- `ghidra_out/quest_activate_decomp.c` — `CQuestManager::ActivateMultipleQuests 0x004B4260` + wrappers.
- `ghidra_out/quest_decomp3.log` — `FUN_004b3ce0` binding moment, `FUN_00cb5ad0` persistence map,
  vtable dump `0x012D3994`, save/initial-quest string xrefs.
- `ghidra_out/quest_manager_decomp.c` — CQuestManager inner classes.
- `ghidra_out/quest_refs.log`, `quest_nameat.log`, `quest_postinit.log`, `quest_initial.log`.
- FSE source (read-only): `D:\Code\FQT\SourceFilesToReference\FSE\FableScriptExtender-master`.
- Known DB misnames encountered (BSim noise; FSE addresses are authoritative):
  `0x00CB8110` "CHeroMorphDef" = CScriptBase ctor; `0x00CBFAB8`
  "SetMiniMapRegionExitTextOffsetX" = SetScriptActiveStatus; several `pAllocFunc`s carry
  bogus `NCreatureAction::*::Alloc` names.
