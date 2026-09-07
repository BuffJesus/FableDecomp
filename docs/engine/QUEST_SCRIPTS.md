# Quest and script system — compiled quest classes, text-command cutscene interpreter

Merged reference (2026-09-07) for how Fable TLC binds quest names to logic and how its shipped
"scripts" execute. Consolidates the former `QUEST_VM_RE.md` (2026-07-20), `SCRIPT_VM_MAP.md`
(2026-07-19) and `SCRIPT_VM_RE_PLAN.md` (2026-07-19).

**Headline:** quest logic is **compiled C++** — a 161-entry name→allocator table built by the
compiled-in registration routine @ **`0x00CD52D0`**, no quest VM, no bytecode. `CScriptBase` ctor is
**`0x00CB8110`**; `SetScriptActiveStatus` is **`0x00CBFAB8`** (trust these FSE-verified identities over
the BSim misnames "CHeroMorphDef" / "SetMiniMapRegionExitTextOffsetX"). Cutscenes/region scripts are
**ASCII text command lines** run by a `strncmp` verb chain in `RunCutsceneMacro_Func` @ `0x00CBFB7D`.
Recovering the *content* of the compiled quests as readable Lua is the **script recovery lane** in
[docs/scripts/](../scripts/SCRIPT_RECOVERY_PIPELINE.md) (`SCRIPT_RECOVERY_PIPELINE.md`,
`AEON_LUA_PORTS.md`, `CGSI_BIND_CANDIDATES.md`, `SCRIPT_BINDINGS_CANDIDATES.md`, per-quest native
comparisons).

Contents

- [Dated findings](#dated-findings)
- [Quest binding: compiled C++ classes](#quest-binding-compiled-c-classes)
- [Cutscene and region script interpreter](#cutscene-and-region-script-interpreter)
- [RE plan and authoring roadmap](#re-plan-and-authoring-roadmap)
- [Evidence index](#evidence-index)
- [Superseded](#superseded)

---

## Dated findings

- **2026-07-19** — TLC has no numeric-opcode bytecode VM: `script.bin` cutscene records are lists of
  ASCII `[Actor.]Verb arg1,arg2,...` lines; `RunCutsceneMacro_Func` 0x00cbfb7d dispatches by `strncmp`
  over ~185 verbs and calls `CGameScriptInterface` (instance ptr `DAT_0143e8f8`) via fixed vtable
  offsets. Validated on all 69 commands of `CS_ARENA_ROUND_1`.
- **2026-07-19** — `script.bin` = 611 entries: 595 `CCutsceneDef`, 14 `CRegionScriptDef`, 2
  `CScriptDef` (incl. `NULLDEF_CScriptDef`). Decompiler coverage 49,081/49,082; the one failure is
  `Main@0x00DECAD0` (`Pcode: Decoding error: Attribute size is not present`).
- **2026-07-20** — Quest binding RE COMPLETE: `RegisterAllScripts` = `FUN_00cd52d0` registers 161
  `CScriptInfo` records `{Name, ID, MasterScript, pAllocFunc, pAllocDataFunc, "S_xxx"}` via `AddScript`
  0x00CB5C90; activation (`CQuestManager::ActivateMultipleQuests` 0x004B4260 → `FUN_004b3ce0`
  0x004B3CE0) calls the allocator through a raw code pointer; unregistered names are silently dropped
  at `IsQuestRegistered`. Brand-new quest logic REQUIRES native code injection (FSE hook 0x00CDB355).
- **2026-07-20** — `.qst` reconciliation: `FinalAlbion.qst`/`GlobalQuests.qst` are ASCII
  `AddQuest("Name", TRUE|FALSE);` (data-side quest list); a name must ALSO resolve to a compiled
  `CScriptInfo` or there is no logic (`"QuestNotInScripts"`, `"DUMMY_QUEST_HAS_NO_SCRIPT"`).

---

## Quest binding: compiled C++ classes

*Originally `QUEST_VM_RE.md`. Question: when the engine activates a registered quest name, does it
run compiled C++ code, or interpret data? Answer determines whether custom quests can ever be pure
data or whether an injector (FSE) is structurally required. Status: COMPLETE (2026-07-20). All
decompile evidence from read-only headless passes on the FableTLC Ghidra project; logs/artifacts under
`ghidra_out/quest_*`.*

### VERDICT

**HYBRID — quest LOGIC is NATIVE-CODE-BOUND (CONFIRMED); everything around it (selection,
activation, rewards, presentation, persistence framing, and the cutscene/region-script layer) is
name-keyed DATA (CONFIRMED).**

1. **Quest LOGIC = compiled C++ (CONFIRMED).** Each quest is a compiled `NScript::CQ_<Name>Script`
   class (derived from `CScriptBase`) linked into `Fable.exe`. The registry maps quest **name
   (CCharString) → native allocator function pointer** (`CScriptInfo::pAllocFunc`), built by a
   compiled-in registration routine (`0x00CD52D0`, 161 entries — E3). Activation instantiates the
   class through that raw code pointer (E6). There is **no quest bytecode, no quest interpreter, and
   no fallback path for unregistered names** (E5): a quest name that is not in the compiled table is
   silently dropped at the `IsQuestRegistered` gate.
2. **Selection/activation = data.** Quest *names* arrive from data (world initial-quest string at
   `CMainGameComponent::PostInit`, script commands, save files, other quests) and are honored — but a
   name can only ever select one of the 161 compiled classes.
3. **Cutscene/region scripts = interpreted data** (`CCutsceneDef`/`CRegionScriptDef` payloads are text
   command lists run by `RunCutsceneMacro_Func @ 0x00CBFB7D` — see the
   [interpreter section](#cutscene-and-region-script-interpreter)). Compiled quests *invoke* this
   layer; it does not host quest state machines. It is fully data-editable.
4. **Consequence: brand-new quest logic REQUIRES native code injection** (FSE/DLL or a patched exe).
   This is structural, not a tooling gap. FSE is architecturally the *minimal* correct solution: it
   appends `CScriptInfo` records whose allocators return Lua-hosting `CScriptBase` clones (E1).

### Evidence log

#### E1. FSE source = a working map of the retail registration seam (CONFIRMED, source read)

`D:\Code\FQT\SourceFilesToReference\FSE\FableScriptExtender-master\FableScriptExtender\dllmain.cpp`:

- FSE installs a JMP at retail **`0x00CDB355`** — the tail of the engine's own script registration
  routine (hook epilogue re-executes `pop edi/esi/ebp/ebx; add esp,18h; retn`, i.e. the original
  function's epilogue). The script-manager object is in `ESI` at that point.
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

- A quest object is a `CScriptBase` (ctor @ **`0x00CB8110`**) whose **vtable defines the quest
  contract**: `[0]=Destructor, [1]=RegisterMain, [2]=Main, [3]=Init, [4]=OnPersist` (FSE's
  `g_LuaQuestHostVTable` replicates exactly this shape and the engine calls through it).
- Quest logic runs as cooperative "spawned functions" (threads): `CSpawnedFunc` ctor @
  **`0x00CDD450`**, added per-section via `AddSpawnedFunction @ 0x00CB7E50`; termination poll
  `IsActiveThreadTerminating(Quest) @ 0x00CB7940`. `CSpawnedFunc` carries a **raw native code
  pointer** `pThunkToMain` — the "thread body" is machine code, not data.
- Persistence: engine calls vtable `OnPersist(CPersistContext*)`; primitives via
  `CPersistContext::Transfer` overloads (`bool 0x004045C0`, `int 0x00410BE0`, `string 0x004109A0`,
  `float 0x00410620`, `uint 0x004106F0`).

Interpretation: the ONLY way FSE can add a quest is to hand the engine a **native function pointer**.
If quests were data-interpreted, FSE would inject data instead. Ghidra decompile of the registration
function is the confirming second source (E3).

#### E2. Retail exe strings corroborate name-keyed registry (CONFIRMED, bytes read)

Direct scan of `C:\Programs\Steam\...\Fable.exe`: `QuestManager: Activate Quest`,
`QuestNotInScripts`, `QuestActivated/QuestStarted/QuestComplete/QuestFailed/...` (state-machine
strings), and ~100 section names `S_QBC, S_QHSA, S_QGTW, S_VB, ...` matching FSE's `"S_"+questName`
convention — the shipped quest set is enumerated in `.rdata` as strings next to a registration
table/routine.

#### E3. The retail registration function — CONFIRMED by decompile

**`RegisterAllScripts` = `FUN_00cd52d0` @ `0x00CD52D0`** (undefined-named in DB; ends at the FSE hook
`0x00CDB355` — the hook patches its final bytes). Found via xrefs to `AddScript @ 0x00CB5C90`: **60+
calls, one every ~0x99 bytes, all from this one function** (`ghidra_out/quest_refs.log`). Full
decomp: `ghidra_out/quest_register_all.c` (5,149 lines).

It is a straight-line, **compiled-in** registration list. Verbatim block shape (one per script; names
below have known BSim mislabels — FSE-verified identities in brackets):

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

**161 script registrations extracted** → `ghidra_out/quest_registry_table.tsv` (quest_name / id /
master / allocFunc / dataAlloc / section). Full plain-text quest names recovered: `Q_SunnyvaleMaster`
(master script), `Gameflow`, `HeroBoasts`, `PersonalScriptMain`, `Q_Arena`, `Q_BountyHunt`,
`Q_DragonBossFight`, `Q_EndGameBossBattle`, all `Q_*` story quests + `V_*`-section village scripts.
Every entry's `pAllocFunc` is a **distinct code address** in `0x00CF9xxx–0x00D8xxxx` — megabytes of
per-quest compiled logic.

There is **no data table** behind this: the name→allocator pairs are immediate operands in `.text`.
Registration is compiled into the exe, executed once at startup (progress strings "Registering Master
Script" / "Registering Important Scripts" shown on the loading screen).

The real class scheme is visible through ported PDB names (fablewin editor build + ego_r BSim):
`NScript::CQ_<QuestName>Script` classes — `CQ_HeroSoulsArenaScript`, `CQ_BountyHuntScript`,
`CQ_ArenaScript`, `CQ_GameflowScript`, `CQ_SunnyvaleMasterData`, ... (thousands of member-function
name hits across `ghidra_out/egor_pdb_names.tsv`, `fablewin_pdb_names.tsv`). One quest = one compiled
C++ class.

#### E4. What an allocator does — CONFIRMED by decompile (Q_HeroSoulsArena)

`FUN_00d80ba0` (the `pAllocFunc` registered for `Q_HeroSoulsArena`):

```c
this = operator_new(0x68);                       // heap-alloc quest object (0x68 bytes)
CScriptBase::CScriptBase(this);                  // base ctor @ 0x00CB8110 [DB misname CHeroMorphDef]
*(this+0x40) = param_2;  *(this+0x44) = param_1; // pInterface / pMasterData
*(void***)this = &PTR_..._012d3994;              // PER-QUEST COMPILED VTABLE @ 0x012D3994
uVar1 = (**(code **)(*DAT_0143e8f8 + 0x15c))();  // grab from CGameScriptInterface
```

i.e. `new NScript::CQ_HeroSoulsArenaScript(data, iface)` — a C++ object whose vtable (`0x012D3994`,
in `.rdata`) carries the quest contract FSE mirrors: `[0]=deleting-dtor, [1]=RegisterMain, [2]=Main,
[3]=Init, [4]=OnPersist`. The quest's Main is a compiled function; quest "threads" are `CSpawnedFunc`
records whose `pThunkToMain` is a raw machine-code pointer (E1).

#### E5. Activation = registry lookup by name; unregistered names are DROPPED (CONFIRMED)

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

A quest name arriving from DATA (WLD initial quests, .qst, script commands) can only ever **select**
one of the 161 compiled entries. There is no fallback interpreter path for an unknown name — it is
skipped at the `IsQuestRegistered` gate.

#### E6. The binding moment — activation queue processor (CONFIRMED by decompile)

`CQuestManager::ProcessQuestActivations` = `FUN_004b3ce0 @ 0x004B3CE0` (decomp in
`ghidra_out/quest_decomp3.log`). Pass 1 preloads each pending quest's code section
(`CCodeSectionManager::LoadSection`) and calls `CQuestManager::LoadResourcesForQuests`. Pass 2 binds
— the load-bearing lines:

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

Binding a name to logic is literally an **indirect call through the compiled allocator pointer**
stored at registration. The result is wrapped in a `CQuestManager::CActiveQuest` (class confirmed by
ego_r BSim names `CQuestManager::CActivateQuestInfo`, `CQuestManager::CActiveQuest`,
`CQuestManager::CQuestCard` — `ghidra_out/quest_manager_decomp.c`). Saved-state lookup on the way in:
`FUN_00cb5ad0 @ 0x00CB5AD0` = LowerBound over `CVectorMap<CCharString,
NQuestManager::CRuntimeQuestPersistence::CPersistInfo>` — per-quest persisted blobs are keyed by quest
name.

#### E7. A quest object's vtable = per-quest compiled functions (CONFIRMED)

Vtable registered by the `Q_HeroSoulsArena` allocator, dumped at `0x012D3994` (.rdata):

```
[0] 0x00D80C00   deleting dtor
[1] 0x00D7C740   RegisterMain
[2] 0x00D7C890   Main
[3] 0x00F26B90   (shared default)
[4] 0x00D7C800
[5] 0x00D7C7E0   Init / OnPersist family (FSE replaces exactly slots 0-4)
```

All slots point into the per-quest code region. Quest "threads" are `CSpawnedFunc` records (`ctor
0x00CDD450`) whose `pThunkToMain` is a raw machine-code pointer, pumped by the script scheduler
(`AddSpawnedFunction @ 0x00CB7E50`, `IsActiveThreadTerminating @ 0x00CB7940`). Per-frame quest
execution = calling compiled functions, full stop.

#### E8a. Reconciliation with .qst (two-level registry) — see docs/formats/QST_FORMAT.md

The concurrent .qst RE established that `FinalAlbion.qst`/`GlobalQuests.qst` are ASCII
`AddQuest("Name", TRUE|FALSE);` scripts read during "Load Quests". This is the **data-side quest
list** (CQuestManager: which quests exist / start active). It is a *different level* from the
**code-side script table** built by `0x00CD52D0` (script manager: name → `pAllocFunc`). Both are keyed
by the same name string. Appending an `AddQuest` line registers a *name* (data) — but when activation
reaches the binding step (E6), the name must also resolve to a compiled `CScriptInfo`, or there is no
logic to run (the engine ships explicit vocabulary for that case: `"QuestNotInScripts"`,
`"DUMMY_QUEST_HAS_NO_SCRIPT"`). FSE's shipped test .qst demonstrates the full contract: it appends
`AddQuest("MyFirstQuest", FALSE);` (data) AND injects the matching `CScriptInfo` + allocator at the
0xCDB355 hook (code). Data registers the name; only code supplies the logic. (FSE custom quests need
`AddQuest(name,TRUE)` in `data/Levels/FinalAlbion.qst` to activate; Steam verify-files wipes it.)

#### E8. Where activation names come from + persistence (CONFIRMED)

- **New game:** `CMainGameComponent::PostInit @ 0x00416xxx` (string xref `0x00416bd1`): after
  `CWorld::ActivateWorld`, shows progress "Activate Initial Quests" and calls the ActivateQuest
  wrapper (`FUN_004b4a10`) with a **CCharString member loaded from world/config data**
  (`this+0x161d8`) — data supplies the *name* (the WLD initial-quests list decoded by forge::wld is
  this feed).
- **During play:** shipped quests activate other quests by name (e.g. the compiled quest `Main` at
  `0x00EEExxx` references the quest name `"QuestNotInScripts"`); script-side activation surface is
  `CGameScriptInterface::ActivateQuest/ActivateMultipleQuests/DeactivateQuest/SetQuestAsCompleted/...`
  @ `0x00892E80–0x00893030`.
- **Saves:** `CWorld::SaveGameState`/`LoadGameState` write/read a `START_SAVED_QUESTS` …
  `END_SAVED_QUESTS` section (string xrefs `0x004b6177` / `0x004b5892`), containing quest names +
  per-quest `OnPersist` payloads (`CPersistContext::Transfer` primitives: bool `0x004045C0`, int
  `0x00410BE0`, string `0x004109A0`, float `0x00410620`, uint `0x004106F0`). Saves persist quest
  **state variables**, never quest code — on load the name re-selects the compiled class and
  `LoadGameState` refills its members (E6).
- Related data-side markers in the exe: `START_ACTIVE_QUESTS`, `START_NEW_QUEST`,
  `START_NEW_MASTER_QUEST_DATA`, `START_QUEST_CARDS`, `CURRENT_QUESTS_ENTRY` — the same text-sectioned
  serialization family (see docs/formats/SAVE_ENTITY_GRAPH.md and the .qst RE for the file-side
  counterpart).

#### E9. Entity scripts (TNG ScriptName) — same architecture (CONFIRMED via FSE + symbols)

TNG-placed things carry `ScriptName`/`ScriptData`. The engine binds a thing's ScriptName through
`AddEntityScriptBinding @ 0x00CB8230` + `PostAddScriptedEntities @ 0x00CB8930` (both confirmed named
in the DB) — again a **name → native allocator** binding (FSE's `EntityAllocator`/`LuaEntityHost`
mirrors the record; vtable contract `[dtor, Main, Init, GetParentScript, OnPersist, OnPredicateFail,
OnInterrupted]`). `"ScriptQuestName"` (string @ `0x01264cd0`) is the field tying a scripted thing to
its owning quest. So per-entity behaviors are ALSO compiled classes selected by data names — the same
verdict applies one level down. (Loader-side decompile of the TNG field consumption not done here;
marked INFERRED for that last hop, everything else confirmed.) The already-decoded chest path is a
concrete example: `NScript::CGlobal_OpenChestScript::Main` is native C++ behavior, while the placed
TNG object supplies data such as its definition and contents.

### Quest lifecycle (as established)

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

### What a custom quest minimally requires

**The boundary, precisely:**

| Aspect | Data-drivable today (no injector) | Needs native code |
|---|---|---|
| Quest logic / state machine / objectives flow | — | **YES** (compiled CQ_*Script class; FSE Lua host or DLL) |
| Which quests activate at world start | YES (WLD initial-quest names) | — |
| Activating/completing/failing an EXISTING quest from script data | YES (cutscene/region text verbs + existing quest cross-activation) | — |
| Quest cards (name, gold/renown reward, objectives text, icon) | YES (`CQuestCardDef`/`CActivateQuestDef` in game.bin; `SetQuestCardGoldReward` etc. are data-reachable) | — |
| Cutscenes / region scripts (staging, dialogue, spawns, camera) | YES (script.bin text commands) | — |
| Placed-thing behavior (TNG ScriptName) | only selects among compiled entity scripts | new behavior = native |
| Quest state persistence in saves | automatic once a quest exists (name-keyed OnPersist) | — |
| Re-plotting an EXISTING quest (its markers, creatures, regions, rewards, cutscenes) | LARGELY (its data inputs) | its control flow is frozen in code |

So: **"pure-data custom quest" is impossible in retail TLC** beyond recombining shipped quest logic.
The minimum viable custom-quest stack is: FSE (or equivalent DLL) providing one `CScriptInfo` + a
`CScriptBase`-shaped host (5-slot vtable) — everything else (cards, rewards, text, cutscenes,
regions, activation, saves) can then be authored as data. FableForge's authoring surface + ForgeFSE
runtime is exactly the right split.

---

## Cutscene and region script interpreter

*Originally `SCRIPT_VM_MAP.md` (2026-07-19). RE of the compiled-script execution path: how
`script.bin` / cutscene records are executed and how they bridge to `CGameScriptInterface`. Retail
Fable.exe @ ImageBase 0x400000.*

### TL;DR — the headline architectural finding

**TLC has no numeric-opcode bytecode VM for its shipped scripts.** There is no opcode byte, no jump
table indexed by an opcode, no operand-immediate encoding, no push/pop/load/store instruction set.
What "compiled scripts" actually are is a **list of ASCII text command lines**, each of the form

```
[Actor.]Verb arg1,arg2,arg3,...
```

The executor is a **text-keyword command interpreter**: a very long `if (strncmp(cmd,"VerbName")==0)
{ parse string args; call CGameScriptInterface method } else if (...)` chain. Dispatch is by **string
compare on the verb name**, not by an integer opcode. Arguments are parsed at runtime from their text
form (`GFStringToFloat`, `GFStringToInt`, `IsFalse`) and the handler calls a native method on the
singleton `CGameScriptInterface` through a fixed vtable offset.

This is the answer to "recover the opcode set / dispatch loop / instruction format": the *opcode set*
is the ~184-entry **verb table**, the *dispatch loop* is the `strncmp` chain in
`RunCutsceneMacro_Func`, and the *instruction format* is `Actor.Verb comma-separated-string-args`
(line-oriented text tokens).

### The interpreter / dispatch

| Item | Address | Evidence |
|------|---------|----------|
| `RunCutsceneMacro_Func` (the executor) | **retail `0x00cbfb7d`** | FSE source anchor `FableAPI.cpp:183` type `tRunCutsceneMacro` (2nd source) + Ghidra decomp `ghidra_out/runcutscenemacro_full.c` |
| `CGameScriptInterface` instance ptr | **`DAT_0143e8f8`** (`*DAT_0143e8f8` = its vtable) | 45 dispatch callsites in `RunCutsceneMacro_Func` |

Signature (Ghidra): `RunCutsceneMacro_Func(map<CCharString,C2DVector> *markerPositions, CRegion
*region, uint flags, ... )`. The `map<CCharString,C2DVector>` is the marker/actor position table the
verbs resolve names against; `CRegion*` is the region the script runs in.

Dispatch shape (verbatim pattern, repeated ~184×):

```c
CCharString::CCharString(local, "UseCamera", -1);
sVar13 = CCharString::GetLength(local);
iVar33 = strncmp(cmdText, "UseCamera", sVar13);
if (iVar33 == 0) {
    /* parse string args from local_44, local_40, ... via GFStringToFloat/Int/IsFalse */
    /* then dispatch to a CGameScriptInterface method: */
    (**(code **)(*DAT_0143e8f8 + 0x670))(...);   // native call bridge
    goto LAB_00cd17fd;                            // = "next command"
}
/* else fall through to next strncmp against the next verb name */
```

There is **no jump table** — Ghidra shows a linear `strncmp` cascade. `LAB_00cd17fd` is the common
"command handled, advance to next command" join point (the loop back-edge). Control-flow verbs
(`GamePause`, `WaitActiveDialog`, `DoScriptFrame`) are handled *inside* their own branch with real
`do/while` loops that pump `PlayCutsceneSound` and poll `IsCutsceneSkipped()` each frame — pacing is
done by the native handler, not by a VM program counter over a bytecode stream.

Per-verb decompiled slices (arg parsing + exact vtable offset called) are in
`ghidra_out/cutscene_verb_slices.md` for: UseCamera, NoLoadUseCamera, DoCameraPreloading, GamePause,
DoScriptFrame, Create, WaitActiveDialog, RegisterActor, RemoveAllThings, SetDoorOpen, and more.

### The "opcode" table — the verb set (CONFIRMED)

The full recovered verb list (185 entries) is in `ghidra_out/cutscene_native_verbs.txt`; per-verb
argument signatures in `ghidra_out/cutscene_verb_args.tsv`; native-present / actually-used-in-shipped-
data status in `ghidra_out/cutscene_verb_census.tsv`.

Two syntactic classes of verb (CONFIRMED from real data, below):

1. **Global verbs** (no actor prefix): `UseCamera`, `GamePause`, `Create`, `FadeIn`, `FadeOut`,
   `DoScriptFrame`, `DoCameraPreloading`, `NoLoadUseCamera`, `SetDoorOpen`, `RegisterActor`,
   `WaitActiveDialog`, `PutUpYourSwords`, ...
2. **Actor-targeted verbs** (`.`-prefixed in the native table, written `Actor.Verb` in data):
   `.WalkTo`, `.RunTo`, `.SneakTo`, `.Teleport`, `.Speak`, `.DialogSpeak`, `.PlayAnimation`,
   `.PlayLoopingAnim`, `.Drawable`, `.ClearCommands`, `.LookToThing`, `.EntitySetMaxWalkingSpeed`, ...
   The actor name before the `.` is resolved to a `CScriptThing` by `GetCutsceneActorScriptThing`
   (`0x00cbf9de`): empty/"HERO" → hero (`*DAT_0143e8f8 + 0x118`); a registered name → the
   `RegisterActor` map (`FindRegisteredCutsceneActor` `0x00cd2b86`); else find-by-name
   (`*DAT_0143e8f8 + 0x120`).

Argument encoding is **not** fixed-width binary. Each verb reads a fixed *count* of comma-separated
**text** tokens and coerces per-token:
- float:  `GFStringToFloat` / `FUN_0099e690` (`GFCharStringToFloat`, `0x0099e690`)
- int:    `GFStringToInt`
- bool:   `IsFalse` (token == "FALSE"/"0"/empty ⇒ false)
- string / name: used raw (object def name, marker name, actor name, TEXT_ tag)

`cutscene_verb_args.tsv` records the recovered per-verb arg count + signature, e.g. `.SneakTo` =
`arg1:str arg2:str arg3:bool arg4:bool arg5:bool arg6:bool`; `.PlayLoopingAnim` = `arg1:str arg2:int
arg3:bool arg4:bool arg5:bool arg6:bool`; `.SetPushable` = `arg1:bool`. These are CONFIRMED by
matching the decompiled `GFStringTo*`/`IsFalse` parse order in each branch.

#### Native-call bridge (CONFIRMED)

Every verb that does engine work calls `(**(code **)(*DAT_0143e8f8 + OFF))(...)`. `DAT_0143e8f8` =
pointer to the singleton `CGameScriptInterface`; `*DAT_0143e8f8` = its vtable (the same 891-method
interface catalogued in `ghidra_out/gamescriptinterface_catalog.tsv`, whose method bodies live at
`0x0088xxxx–0x008axxxx`). Distinct offsets seen in `RunCutsceneMacro_Func`:

```
0x1c  0x118(GetHero) 0x120(FindThingByName) 0x150 0x194 0x208 0x2f4 0x314 0x318
0x3bc 0x1f4(500) 0x5c0 0x5d0 0x5d4 0x5d8 0x5dc 0x5e0 0x618 0x61c 0x620 0x670
0x680 0x684 0x800 0x870
```

`+0x118` and `+0x120` are confirmed as GetHero / find-thing-by-name by `GetCutsceneActorScriptThing`.
The remaining offsets are the per-verb engine actions (camera, fade, create, door, etc.); mapping
each offset → catalog method name is straightforward follow-up (the catalog gives every method's own
body address; the vtable at instance-ptr `*DAT_0143e8f8` orders them by these offsets).

> Note vs. HANDOFF chest path: HANDOFF cites the `CGameScriptInterface` **vtable** at base
> `0x1260F0C` (e.g. `+0x6D4` GetNumberOfKeysNeededToUnlockChest). Here `DAT_0143e8f8` is the
> **instance pointer**; `*DAT_0143e8f8` dereferences to that same vtable region. Same interface, two
> ways of naming the anchor.

### Compiled-script container layout (`script.bin` / CompiledDefs)

`script.bin` (611 def entries) holds three script-bearing def types: `CCutsceneDef` (595),
`CRegionScriptDef` (14), `CScriptDef` (2, incl. `NULLDEF_CScriptDef`). A `CCutsceneDef` payload is a
**named list of command strings**. Validated live with `forge script cutscene <root>
CS_ARENA_ROUND_1`:

```
entry 15: CS_ARENA_ROUND_1 (2310 bytes, 69 commands)
0000  UseCamera        UseCamera P_ARENACUOC,-1,NULL,0,0
0002  Create           Create CREATURE_BS_VILLAGER_MALE, P_CROWD1, Crowd1
0006  Create           Create CREATURE_HORNET_01, P_ENEMYPOS1, Wasp1, false, ArenaEnemy
0007  DoScriptFrame    DoScriptFrame 1
0018  Wasp1.Drawable   Wasp1.Drawable FALSE
0029  Hero.Teleport    Hero.Teleport P_ARENAHEROSTART
0033  Hero.SneakTo     Hero.SneakTo P_ARENAHEROEND, 0.0, false, false, false
0036  Hero.DialogSpeak Hero.DialogSpeak HERO,$HEROTITLE,true
0037  GamePause        GamePause 3.5
0038  SetDoorOpen      SetDoorOpen ArenaHeroGate, true
0046  WaitActiveDialog WaitActiveDialog
```

So a compiled cutscene record = header (name + count/size) + a sequence of newline/record-delimited
**command strings**. Each command string is exactly the text the `strncmp` dispatch matches against,
args and all. There is no separate constant pool and no compiled bytecode stream — the "constants"
are inline in each command's text (object def names, marker names, `TEXT_*` dialogue tags, `$VAR`
substitutions like `$HEROTITLE`). Cross-scene verb histogram (proof of a stable command grammar over
all 595 scenes) is in `ghidra_out/cutscene_verb_census.tsv` and via `forge script command-stats`:
`GamePause`=2982, `UseCamera`=1713, `DoScriptFrame`=762, `FadeOut`=684, ...

`CScriptDef` (only 2 entries, mostly the NULL template) and `CRegionScriptDef` (14) are the same
family; region scripts run through the same `RunCutsceneMacro_Func` (its signature takes a
`CRegion*`), i.e. cutscenes and region scripts share one text-command executor.

Loose TNG placements attach script identity with top-level fields such as `ScriptName` and
`ScriptData`. BarrowFields examples include `MK_*` marker scripts, `CAM_*` camera points, and `CS_*`
cutscene-related names.

### VALIDATION — real instructions disassembled with the recovered map

Taking real bytes from shipped `CS_ARENA_ROUND_1` and "disassembling" them with the recovered verb
map, then confirming each maps to a decompiled dispatch branch and forms sane control flow ending sanely:

| # | Raw command (from script.bin) | Verb (matched branch) | Decoded semantics |
|---|-------------------------------|-----------------------|-------------------|
| 0000 | `UseCamera P_ARENACUOC,-1,NULL,0,0` | `UseCamera` @ dispatch xref 00cc9f39 | switch to camera marker `P_ARENACUOC`, no blend |
| 0002 | `Create CREATURE_BS_VILLAGER_MALE, P_CROWD1, Crowd1` | `Create` @ 00ccc246 | spawn creature def at marker `P_CROWD1`, register as actor `Crowd1` |
| 0007 | `DoScriptFrame 1` | `DoScriptFrame` @ 00cc7085 | advance/pump 1 script frame (the frame-pacing op) |
| 0033 | `Hero.SneakTo P_ARENAHEROEND, 0.0, false×3` | actor `.SneakTo` @ 00cc0cba | move Hero (actor-resolved via +0x118) to marker, speed 0.0, flags false |
| 0037 | `GamePause 3.5` | `GamePause` @ 00cc88d1 | busy-wait 3.5s pumping sound, breakable by `IsCutsceneSkipped()` |
| 0046 | `WaitActiveDialog` | `WaitActiveDialog` @ 00cc656b | block until active dialog finishes (loop on `+0x5c0`) |

All 69 commands of the scene decode cleanly under the map, in order, ending with fade/return commands
— sane, terminating control flow. The verb strings in the shipped data are byte-identical to the
string literals inside the decompiled dispatch branches (`ghidra_out/runcutscene_string_literals.txt`).
That two-way match — data string ⇄ decompiled `strncmp` literal ⇄ live forge decode — is the
confirmation the map is correct.

### Script-modding feasibility verdict

**Highly feasible, and easier than a bytecode VM would be.** Because compiled scripts are
line-oriented **text commands** (not packed bytecode with offsets/jump targets), editing is
edit-in-place of ASCII with no relocation math:

- **Read:** already done — `forge script cutscene/cutscenes/command-stats` decode all 595 cutscenes to
  readable command lists.
- **Author/edit:** a new command is just a new text line using a known verb + its arg signature
  (`cutscene_verb_args.tsv`). Grammar is fully enumerated (185 verbs). Risk is low: unknown verbs
  simply fall through the `strncmp` chain and are ignored (no crash), so additive edits are safe.
- **Write-back:** needs `forge` `CCutsceneDef` payload re-serialization (header count/size + the
  command-string blob) with the existing byte-safe def writer; this is a string-blob rewrite, the
  easiest def class to round-trip.
- **Semantics are known:** each verb's engine effect = the `CGameScriptInterface` method it calls;
  that interface is the 891-method catalog already reversed.

Bottom line: shipped quests/cutscenes/region scripts can be decompiled, edited, and re-emitted as
text; the FSE-Lua path remains for brand-new logic, but direct editing of shipped `script.bin`
content is unblocked by this map.

### What stays open (follow-ups, not blockers)

1. **Full offset→method table.** Map each of the ~24 distinct `*DAT_0143e8f8 + OFF` vtable offsets in
   `RunCutsceneMacro_Func` to its named catalog method (mechanical: index the vtable at the instance
   ptr against `gamescriptinterface_catalog.tsv`). Only `+0x118`/`+0x120` are name-confirmed here.
2. **Exact `CCutsceneDef` byte header.** `forge` already parses it; record the precise header field
   layout (name offset, command count, per-command length prefix vs delimiter) in the def schema for
   a clean write-back path.
3. **`$VAR` / `TEXT_*` substitution.** Confirm where `$HEROTITLE`-style tokens and `TEXT_*`/`'…'`
   dialogue tags are resolved (likely a pre-pass or inside `DialogSpeak`/`Speak` handlers) — needed
   for editing dialogue lines.
4. **`CRegionScriptDef` (14) / `CScriptDef` (2) payload shape.** Confirmed to share the executor; dump
   their payloads to verify identical text-command layout vs. a thin wrapper.

*No new headless Ghidra pass was run for the 2026-07-19 map: the Ghidra project was locked by another
single-writer agent (java.exe holding `ghidra_proj/FableTLC`), and the prior decomp artifacts already
contained the interpreter, verb table, and dispatch slices. Validation was done with read-only
`forge.exe` against the retail install at `C:\Programs\Steam\steamapps\common\Fable The Lost Chapters`.*

---

## RE plan and authoring roadmap

*Originally `SCRIPT_VM_RE_PLAN.md` (2026-07-19). Answers whether entity-attached scripts, quests,
dialogue, and cutscenes can be decompiled/reversed, and how that relates to the native executor.
Written before the interpreter map and the quest-binding verdict above; kept as the staged plan.*

### Answer

Yes, the entity-attached scripts can be reversed and understood. The native executor is the right
long-term layer for this, but it is not the only layer. There are three linked layers:

1. **Placement binding:** TNG `ScriptName` / `ScriptData` attach script-ish names and parameters to
   concrete world entities.
2. **Compiled script data:** `script.bin` stores `CCutsceneDef`, `CRegionScriptDef`, and `CScriptDef`
   payloads. These are the byte/data records FableForge should decode into readable script/cutscene
   records.
3. **Native executor:** `Fable.exe` contains the native C++ script classes (`NScript::*`),
   interfaces, and the interpreter/executor logic that reads those records and performs engine actions.

So: decomping the executor gives us the semantics. Decoding `script.bin` gives us the actual shipped
script/cutscene content. Indexing TNG gives us where those scripts are attached in the world.
FableForge needs all three. (`RunCutsceneMacro_Func` decompiles once Ghidra's decompiler payload cap
is raised.)

### Practical plan

#### Stage 1 - Script binding index

- Add `forge script refs <game-root>`: scan all TNGs; collect `ScriptName`, `ScriptData`, position,
  definition, level, UID; group names by prefix (`CS_`, `CAM_`, `MK_`, quest-specific names);
  cross-link matching `script.bin` entries where names match exactly or by known prefix rules.
- GUI use: select an object in the 3D world and see "attached scripts" directly in the inspector.

#### Stage 2 - `script.bin` payload mapping

- For each of the three definitions, dump payloads with name, length, and hex.
- Start with `CCutsceneDef`, because there are 595 examples and their names are descriptive.
  Differential comparison across similar cutscenes should reveal repeated fields and command blocks
  quickly.
- Use `RunCutsceneMacro_Func` as the executor oracle: find where it reads each payload field, name
  the field, then re-run the decoder.
- Output: JSON/CSV with best-known fields and unknown byte ranges.

#### Stage 3 - Native executor decomp

- Identify and label the executor entry points: `RunCutsceneMacro_Func`; `CScriptDef::Transfer` /
  `CCutsceneDef::Transfer` / `CRegionScriptDef::Transfer`; script lookup by name; dispatch functions
  that call `CGameScriptInterface` methods.
- Build an opcode/command table if the payload contains bytecode-like command IDs. If it is instead
  a serialized object graph, build a field schema and record graph. *(Resolved: it is neither — a
  text verb table; see the interpreter section.)*
- Verify every decoded command against at least one shipped cutscene/script and one runtime API effect.

#### Stage 4 - Authoring

- First authoring target: readable/decompiled cutscene timeline and entity script reference browser.
- Then write-back: modify `script.bin` payloads through `forge::bin::save`, with semantic round-trip
  checks like the game.bin writer.
- FSE Lua remains the near-term "make new quests/scripts" path. Native script decomp is the path to
  importing and editing shipped quests/cutscenes directly — now pursued as the Lua recovery lane in
  [docs/scripts/](../scripts/SCRIPT_RECOVERY_PIPELINE.md).

### Relationship to 3D editing

The 3D world view should make script attachments visible: markers and camera points are selectable
scene objects; clicking a `CAM_*` object shows camera spline/point data; clicking a `CS_*` or quest
marker shows linked `script.bin`/FSE/native script records; moving a placement writes back to TNG
using the existing byte-safe TNG mutation API; later, editing a timeline updates `CCutsceneDef` or
emits FSE Lua, depending on mode. This is more intuitive than a raw table because scripts are spatial
in TLC: triggers, markers, camera points, NPCs, doors, chests, and quest regions all live in the level.

### Decompiler coverage blocker

Decompiler coverage is 49,081 / 49,082 functions clean. The one remaining failure is
`Main@0x00DECAD0` with `Pcode: Decoding error: Attribute size is not present`.

Already checked: every instruction in the function decodes to Pcode; function metadata reset did not
fix it; 10 direct callee prototypes look sane; direct local/parameter type walk found no bad type;
whole-program data type scan found only one zero-length typedef, `/mmsystem.h/TIMECALLBACK`, which is
probably unrelated Windows header noise.

Next attempt, if true 100.000% coverage matters: clone the Ghidra project, delete/recreate the
function and any custom data types reachable from it, then retry decompile in the clean clone. Do not
risk the main DB until the clone proves the fix.

---

## Evidence index

Quest binding (all under `ghidra_out/`):
- `quest_register_all.c` — full decomp of RegisterAllScripts `0x00CD52D0`.
- `quest_registry_table.tsv` — 161 extracted registrations (name/id/master/alloc/section).
- `quest_activate_decomp.c` — `CQuestManager::ActivateMultipleQuests 0x004B4260` + wrappers.
- `quest_decomp3.log` — `FUN_004b3ce0` binding moment, `FUN_00cb5ad0` persistence map, vtable dump
  `0x012D3994`, save/initial-quest string xrefs.
- `quest_manager_decomp.c` — CQuestManager inner classes.
- `quest_refs.log`, `quest_nameat.log`, `quest_postinit.log`, `quest_initial.log`.
- FSE source (read-only): `D:\Code\FQT\SourceFilesToReference\FSE\FableScriptExtender-master`.
- Known DB misnames encountered (BSim noise; FSE addresses are authoritative): `0x00CB8110`
  "CHeroMorphDef" = CScriptBase ctor; `0x00CBFAB8` "SetMiniMapRegionExitTextOffsetX" =
  SetScriptActiveStatus; several `pAllocFunc`s carry bogus `NCreatureAction::*::Alloc` names.

Cutscene interpreter (all under `ghidra_out/`):
- `runcutscenemacro_full.c` — full decomp of `RunCutsceneMacro_Func` @ 0x00cbfb7d (FSE-anchored).
- `cutscene_verb_slices.md` — per-verb decompiled dispatch slices + arg parsing + vtable offset.
- `cutscene_native_verbs.txt` (185 verbs), `cutscene_verb_args.tsv` (arg signatures),
  `cutscene_verb_census.tsv` (native/used status).
- `runcutscene_string_literals.txt` — verb string literals inside the dispatch (data⇄code match).
- `labels_cutscene_helpers.tsv` — `GetCutsceneActorScriptThing` 0x00cbf9de,
  `FindRegisteredCutsceneActor` 0x00cd2b86, `GFCharStringToFloat` 0x0099e690.
- `gamescriptinterface_catalog.tsv` — all 891 `CGameScriptInterface` methods + addresses.
- `scriptvm_decomp.c` — 1010 script-related functions decompiled (prior pass).
- Live: `forge script cutscene "<root>" CS_ARENA_ROUND_1` and `forge script command-stats "<root>"`.

Lua recovery lane: `docs/scripts/SCRIPT_RECOVERY_PIPELINE.md`, `docs/scripts/AEON_LUA_PORTS.md`,
`docs/scripts/CGSI_BIND_CANDIDATES.md`, `docs/scripts/SCRIPT_BINDINGS_CANDIDATES.md`,
`docs/scripts/FORGEFSE_SCRIPT_COMPATIBILITY.md`, `docs/scripts/FORGEFSE_RUNTIME_SCRIPT_AUDIT.md`,
`docs/scripts/FSE_TUTORIAL_API_AUDIT.md`, and the per-quest native comparisons (`QS_MEET_SISTER_*`,
`SEED_NATIVE_OPERATION_*`, `STATUEMASTER_*`, `V_MAZE_RESEARCH_*`).

## Superseded

- **"Build an opcode/command table if the payload contains bytecode-like command IDs" (RE plan
  Stage 3, 2026-07-19)** — resolved the same day: there is no bytecode; the "opcode set" is a
  ~185-verb text table dispatched by `strncmp`.
- **"The VM is the right long-term layer" framing (RE plan)** — there is no VM; the native executor is
  a text-command interpreter for cutscenes/region scripts, and quest logic is compiled C++ classes.
- **BSim names `CHeroMorphDef` @0x00CB8110 and `SetMiniMapRegionExitTextOffsetX` @0x00CBFAB8** —
  wrong; FSE-verified identities are `CScriptBase::CScriptBase` and `SetScriptActiveStatus`.
- **`RunCutsceneMacro_Func` "likely a major interpreter/executor" (RE plan, hedged)** — confirmed as
  THE executor for cutscenes and region scripts.

## Verified facts (from FINDINGS log)

- **2026-07-20 — Quest binding: logic is compiled C++; everything around it is data (2026-07-20).**
  No quest VM exists (evidence `docs/engine/QUEST_VM_RE.md`, artifacts `ghidra_out/quest_*`).
  - Registration routine `FUN_00cd52d0 @ 0x00CD52D0` (ends at FSE's hook 0x00CDB355): 161
    straight-line `AddScript(0x00CB5C90)` calls, each registering `CScriptInfo{Name="Q_...",
    pAllocFunc=<native code ptr>, pAllocDataFunc=0x00CDBD20, "S_x" section}`. Table:
    `ghidra_out/quest_registry_table.tsv`. Real classes = `NScript::CQ_<Name>Script`.
  - Binding = `CQuestManager (DAT_013b89fc)::ActivateMultipleQuests @ 0x004B4260` ->
    `FUN_004b3ce0 @ 0x004B3CE0`: `obj = (*pAllocFunc)()`, wrap in `CActiveQuest`,
    `CScriptBase::Activate`, optional per-quest `LoadGameState`. Unregistered names silently dropped
    (`IsQuestRegistered` gate).
  - Quest object contract = 5-slot `CScriptBase` vtable (dtor/RegisterMain/Main/Init/OnPersist;
    e.g. 0x012D3994 for Q_HeroSoulsArena); threads = `CSpawnedFunc` raw code pointers.
  - Two-level registry: `.qst AddQuest` registers the name (data), the compiled table supplies the
    logic; name-without-script is anticipated ("QuestNotInScripts"/"DUMMY_QUEST_HAS_NO_SCRIPT").
  - Boundary: cards/rewards/objective text/cutscenes/region scripts/initial activation/save state =
    DATA; new quest control flow = NATIVE CODE => FSE (or DLL) required; CScriptInfo injection is the
    minimal correct seam.
  - BSim misnames: 0x00CB8110 "CHeroMorphDef" = CScriptBase ctor; 0x00CBFAB8
    "SetMiniMapRegionExitTextOffsetX" = SetScriptActiveStatus (FSE-verified).

- **2026-07-18 — Retail Fable.exe embeds NO Lua (open question resolved).** Full-binary regex scan
  of `Fable.exe` (16,666,624 bytes, scratchpad `scan_fable.py`): zero hits for `Lua 5.x`, `$Lua`,
  `Tecgraf`, `lua.org`, `LUA_*`, `lua_*`/`luaL_*` (a static Lua link always carries the version
  banner + error strings). Case-insensitive `lua` substring: only 4 hits — two `FrameLastEvaluated…`
  property names, two coincidental x86 instruction bytes at `0x579535`/`0x61c532` (mid-instruction).
  All Lua in the modding ecosystem is injected by FSE (vendored **Lua 5.4.8** + Sol2 per
  `FSE Vendor\lua\lua.h`) at the script-registration hook `0xCDB355`; TLC quest scripts are compiled
  C++ (`CGlobal_*Script`, 843 Quest API entries in the FSE manifest). `luadis.py`/`luadis51.py` are
  irrelevant to retail TLC; Fable 2's Lua lineage does not apply.
- **2026-07-19 — CGameScriptInterface: the native scripting API (891 methods).** `DecompByName.java`
  (decompile all functions matching name substrings) swept `CGameScriptInterface`/`CScriptManager`/
  etc. → `ghidra_out/scriptvm_decomp.c` (1,010 functions, 0 fail); catalog
  `ghidra_out/gamescriptinterface_catalog.tsv` (891 methods + retail addresses). Cross-referenced
  against `refs/fse_api_manifest.json`: **790 of 891 are FSE-exposed**; the **101 not in FSE** are
  `ghidra_out/native_methods_not_in_fse.txt` → spec'd in
  [SCRIPT_BINDINGS_CANDIDATES.md](../scripts/SCRIPT_BINDINGS_CANDIDATES.md). Representative surface:
  `GiveThingItemInHand`, `HeroGoDigging`, `OpenHouseDoors`, `SetTimer`/`RegisterTimer`,
  `EntitySetAsAllowedToFollowHero`, `WaitForCameraMessage`, conversation builders
  (`AddNewConversation`/`AddLineToConversation`/`AddPersonToConversation`), fishing
  (`GetHeroFishingLevel`/`DisableFishingSpot`), `ActivateMultipleQuests`, `AddLogBookEntry`,
  `GetTextString`/`GetFormattedString`.
- Cutscene macro-stream facts (verb census, interpreter architecture, 184-verb API, bug census,
  dispatch order) are re-homed in [CUTSCENES.md](CUTSCENES.md).
