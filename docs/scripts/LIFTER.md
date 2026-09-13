# Native-to-Lua lifter (`tools/script_recovery/lift_native_lua.py`)

Started 2026-09-11 to make script porting less manual. It turns the Ghidra decompile of a retail
script into a **first-draft ForgeFSE Lua package**. It is a converter, not an oracle: it emits Lua
only where the native shape is recognised and leaves `-- TODO(native): ...` everywhere else, so the
existing trace/fixture harness (`validate_reconstructed_package.py`) stays the arbiter.

## Inputs it builds on (nothing re-derived)

- `refs/script_recovery/native_clusters/<Script>.json` — the five lifecycle decompiles per script.
- `annotate_interface_slots.py` — proven `CGameScriptInterface` slot names (`GSI->Name(`) and, from
  `ghidra_out/script_recovery/cscriptthing_vtable_slots.tsv`, `CScriptThing` slot names with receiver
  aliases (`CScriptThing::Name(receiver, …)`; section below).
- `refs/fse_api_manifest.json` — Forge binding scope, parameter names and **types** (used to place
  operands), return types (used to bind results such as `GetActiveQuestName` into the next call).
- `refs/script_recovery/new_oakvale_intro/translation_unit.json` — entity-script decompiles for
  entity mode (`--tu ... --entity NAME --init 0x... --main 0x...`).
- Installed retail `Fable.exe` — reads `.rdata` strings behind unresolved literal pointers.
- Hand ports as scoring oracles: `work/aeon_lua_ports/<Pkg>` and our
  `refs/script_recovery/reconstructed/NewOakValeIntro/.../Entities/*.lua`.

## What it lifts

| native shape | Lua |
|---|---|
| entity-binding boilerplate (`operator_new(0x1c)` … `AddEntityScriptBinding`) | `Quest:AddEntityBinding("Name", "Pkg/Entities/Name")` |
| `GSI->PostAddScriptedEntities()` | `Quest:FinalizeEntityBindings()` |
| `CCharString` temporaries + argument-less `GSI->X()` | operands recovered in push order and placed by manifest parameter type |
| `CPersistContext::Transfer<T>(ctx,"Name",this+off,…)` | `OnPersist` body, and the offset→name map used to write `SetStateBool("Name", …)` |
| `*(T*)(this+off) = v` with an unknown offset | `SetState…("self_0xNN", …)` plus a TODO, never a guessed name |
| `while(!StartScriptingEntity(me,res,p)) NewScriptFrame()` | `if not me:AcquireControl(p) then return end` |
| `NewScriptFrame(); IsActiveThreadTerminating()` (also the bsim-mislabelled `0x00F35B30`) | `alive = quest:NewScriptFrame(me)` / `not alive` |
| `CScriptGameResourceObjectScriptedThingBase::_Method_…` | `me:Method(...)` |
| `CScriptThing::Method(thing, …)` | `thing:Method(...)` (TODO if not a Forge binding) |
| `(**(code **)(*(int *)pCVar1 + 0x54))()` through the entity's own thing (`this + 8` or a `pCVarN` aliased to it) | `me:MsgIsHitByHero()` — slot named from the CScriptThing vtable, operands lowered by the slot's decorated signature (below) |
| `piVar6 = (int *)GSI->GetThingWithScriptName(…); (**(code **)(*piVar6 + 0x28))(1)` | `local piVar6 = quest:GetThingWithScriptName("MK_OVID_DAD")` / `piVar6:GetAngleXY()` — a `*piVarN` receiver is a thing only while `piVarN` holds a thing-returning interface result (previously every `*piVarN` was assumed to be the interface: DeadFather's `GetAngleXY` was lifted as `not alive`) |
| `pCVarN = (CScriptThing *)(this + 8);` / `pCVarM = pCVarN;` | receiver alias: dropped, every later use becomes `me` (also as an operand: `quest:EntityTeleportToThing(me, uVar5)`) |
| `ppVar7 = apStack_c; GSI->GetThingWithScriptName(ppVar7, local_20); … GSI->MiniMapRemoveMarker(apStack_c)` | hidden return slot of a by-value CScriptThing/CCharString result: `local r1 = Quest:GetThingWithScriptName("…")` and later `apStack_c` → `r1` |
| `iVarN = **(int **)(this + 4);` / `piVarN = *(int **)(this + 4);`, the `CBaseIntelligentPointer` release block (`if ((p != 0) && (*p = *p + -1, *p == 0)) { (*(code *)p[1])(); operator_delete(p); }`) | dropped (interface-field caches and refcount bookkeeping, not script logic) |
| a `sol::optional<…>` parameter with no parsed operand | omitted, never filled from a stale temporary |
| a bool-returning slot/interface result `cVar6` (`_N` decoration or manifest `bool`) | `cVar6 == '\0'` → `not cVar6`, `cVar6 != '\0'` → `cVar6` (Lua booleans never equal 0) |

### CScriptThing slots (`ghidra_out/script_recovery/cscriptthing_vtable_slots.tsv`)

The CScriptThing vtable at `0x01238C8C` (RTTI complete-object-locator at VT-4 → `0x1333444` →
typedesc `0x1378a3c` `.?AVCScriptThing@@`; `CScriptThing::CScriptThing` `0x004ABE90` stores the
immediate at +8, 1571 `.text` occurrences), 79 slots `0x00..0x138`, every one named from
`ghidra_out/engine_api.tsv` (78/78 `@CScriptThing@` rows land in it) and `rebuild/manifest/functions.tsv`;
`0x13c` is string data, so the table ends at `0x138`. `annotate_interface_slots.load_thing_slots()`
returns `offset -> (name, mangled)`; the decorated name is the operand contract the lifter applies:

| decoration | meaning | lowering |
|---|---|---|
| `UBE_NABVCCharString@@@Z` | `bool(const CCharString&)` | one string operand; when Ghidra dropped it (`MsgIsHitBy()` after `CCharString::CCharString(aCStack_54,"SCRIPT_NAME_HERO",-1)`) it is taken from the string temporaries in push order |
| `UBE_NW4EHeroAbility@@ABVCCharString@@@Z` | `bool(EHeroAbility, const CCharString&)` | `(0xe,&local_60)` → `me:MsgIsHitByHeroSpecialAbility(0xe)` |
| `UBE?AV1@XZ`, `UBE?AVCCharString@@XZ`, `UBE?AVC3DVector@@XZ` | class returned by value through a hidden first operand | that `&stack…`/`&pos`/`auStack_48` operand is dropped (`me:GetHomePos()`) |
| `UBEABV…@@XZ`, `UBEMXZ`, `UBE_NXZ`, `UBEJXZ` | reference / float / bool / long, no parameters | every operand is dropped (`GetAngleXY`'s `(1)` is a decompiler artefact) |
| `UBE_NAAVCCharString@@@Z`, `UBE_NAAJ@Z` | out-parameter the callee fills | dropped; Forge returns it (`me:MsgIsPresentedWithItem()`) |

Forge binds only the hero-specialised message forms, which the hand ports use, so a recovered
`"SCRIPT_NAME_HERO"` operand selects them: `MsgIsHitBy` → `MsgIsHitByHero`, `MsgIsTalkedToBy` →
`IsTalkedToByHero`, `MsgIsHitBySpecialAbilityFrom` → `MsgIsHitByHeroSpecialAbility`,
`MsgIsHitByAnySpecialAbilityFrom` → `MsgIsHitByAnySpecialAbilityFromHero`, `MsgIsTriggeredBy`,
`MsgIsUsedBy`, `MsgIsKnockedOutBy`, `MsgIsHitByWith{Flourish,Decapitate,Weapon,ProjectileWeapon}` →
`…ByHero…`. Any other name keeps the raw slot with the "not a ForgeFSE binding" TODO.

Validation against the hand inventories (`refs/script_recovery/new_oakvale_intro/entities/*.json`):
`0x54 MsgIsHitBy` + `0xa8 MsgIsHitByAnySpecialAbilityFrom` (NOVI_Villager seq 8), `0xa4
MsgIsHitBySpecialAbilityFrom(0xe, hero)` (Villager `EXCLUDED_HIT_ABILITY`), `0x6c MsgIsTalkedToBy`
(Villager seq 17, NOVI_Guard seq 41), `0x1c GetHomePos` (Guard seq 4), `0x8c MsgIsPresentedWithItem`
(NOVI_Bully/TeddyGirl/Theresa) all agree with the retail table; the DeadFather `GetAngleXY` lift now
matches `refs/script_recovery/reconstructed/.../OVI_DeadFather.lua` (`marker:GetAngleXY()`).

Receiver classification is one streaming pass over the decompile: alias definitions
(`pCVarN = (CScriptThing *)(this + 8)`, `piVarN = (int *)GSI->GetHero()`, `piVarN = *(int **)(this + 4)`,
copies, and any other reassignment) and call heads are applied in text order, so a variable
reused for the interface after holding a thing (`0x00DBEB20 piVar5`) is classified correctly at each
call. Unknown receivers (`pCStack_108` with no visible definition) are left as they are.
| `operator_new(0x3c)` + `CSpawnedFunc<…>` | `Quest:CreateThread("Name")` with the thread body symbol/address recorded in the report |
| `if/else/while/do-while/break/return`, `&& || ! !=`, casts | Lua control flow |
| `if (x) goto LAB` | `if x then return end`; the TODO is suppressed only when `LAB` is the final destructor-only epilogue with no interface call or state write |

## Scoring

`tools/script_recovery/benchmark_lifter.py` lifts every script we have a hand port for, writes the
drafts under `refs/script_recovery/lifted/`, and regenerates `LIFT_BENCHMARK.md` (interface-call
recall/precision against the port, TODO counts). Unit tests: `test_lift_native_lua.py`.

## Thread-body decompiles (`refs/script_recovery/native_threads/<Script>.json`)

The clusters only carry the five lifecycle decompiles, so `CreateThread` bodies (recorded in
`LIFT_REPORT.json` `threads[].body` as a Ghidra label or `LAB_` address) had no offline source.
`tools/script_recovery/export_native_threads.py` closes that gap in two phases:

1. **Offline address resolution (retail bytes, no Ghidra).** The thread allocator stores the body
   pointer as `mov dword ptr [reg+0x34], imm32` (`C7 4x 34 imm32`) inside `Init`/`RegisterMain`/
   `Main`. The stores are scanned in code order from the installed `Fable.exe`, restricted to the
   script's own address range (min..max of its lifecycle functions, ignoring the shared empty
   `OnPersist` `0x00CBD4E0`), and matched to the recorded threads. Every `RegisterMain` also stores
   the shared Main-thread trampoline `0x00CDD440`, which is dropped by the range filter. Cross-checks:
   a `LAB_` body must equal its store; a symbol body must equal `functions.tsv` at that address.
   All 12 benchmark scripts resolve with zero problems (e.g. `WatchForBooty` → `0x00D8A240`,
   `WatchForNewHairdo` → `0x00E50150`); the bsim labels in the decompile are *not* trusted.
2. **Ghidra headless, read-only.** `tools/ghidra_scripts/ExportScriptThreadBodies.java` takes the
   job TSV (`ghidra_out/script_recovery/native_threads_job.tsv`), adds every direct callee of the
   lifecycle functions whose entry lies inside the script range (one script class is emitted
   contiguously, so range == class without trusting module labels), decompiles the set and writes
   `{script, class, range, functions:[{name, address, namespace, size, selectedBy, calls, decompile}]}`.

```
python tools/script_recovery/export_native_threads.py --build-job   # phase 1 only (offline)
python tools/script_recovery/export_native_threads.py               # phase 1 + headless export
# the headless command it runs:
D:\Subuwu\tools\ghidra-public\support\analyzeHeadless.bat D:\Documents\FableTLC\ghidra_proj FableTLC ^
  -process Fable.exe -readOnly -noanalysis -scriptPath tools\ghidra_scripts ^
  -postScript ExportScriptThreadBodies.java ghidra_out\script_recovery\native_threads_job.tsv
```

The Ghidra GUI must be closed (it holds the project lock); `-readOnly` guarantees no DB change.
Log: `ghidra_out/script_recovery/native_threads_job.log`.

## Known levers (next work)

1. **Thread bodies — first pass done (2026-09-11).** When
   `refs/script_recovery/native_threads/<Script>.json` exists, each recorded thread is matched by
   `selectedBy`, address, or exact terminal symbol name, annotated, and fed through the same lifter.
   The package now emits `function <Name>(questObject)` bodies and records per-thread calls/TODOs.
   Nested worker threads found inside a lifted parent are queued too; their literal address (`LAB_`
   and Ghidra's post-export `FUN_` spellings normalize identically) or unique
   in-range `functions.tsv` symbol drives the next read-only export. Dragon's `RunEnemySpawning`
   (`0x00D26BA0`) and `JackTaunts` (`0x00D26A50`) raise its final recall to 0.45. Benchmark recall
   rose from 0.10 to 0.53 for SummoningTheShip, 0.06 to 0.45 for DragonBossFight,
   0.51 to 0.72 for BeardyBaldy, and 0.87 to 0.97 for HerosOldHouse. Next: reduce the newly exposed
   thread TODO classes rather than treating body availability as the bottleneck.
2. **`CScriptThing` message slots — done (2026-09-11).** The thing vtable table and receiver aliases
   above took Villager from 118 TODOs / 0.74 recall to 93 / 0.84 and DeadFather from 10 TODOs to 6
   with `GetAngleXY` correct; no script lost recall or gained TODOs. Left: `MsgWhoHitMe`-style
   results used as receivers are chained, but a thing whose definition is invisible
   (`pCStack_108`) or a `*piStack_N + 0x18` on `[CScriptThing+4]` (a CThing, not a CScriptThing) is
   deliberately not named.
3. **Field naming without `OnPersist` â€” local inventory join done (2026-09-11).** Reviewed entity
   inventories now supply byte-proven class-local names and types. Parent fields, stack locals, and
   rows without a provable type remain deliberately unnamed.
4. **Junk operands.** Where Ghidra guessed a prototype, non-string operands can still be decompiler
   locals; the typed placement fixes string, thing and optional slots, but a dropped CScriptThing
   operand still falls back to the most recent unconsumed thing result (DeadFather's
   `quest:MiniMapRemoveMarker(piVar6)` should be `me`; BeardyBaldy's `MiniMapRemoveMarker(r1)` is
   right by the same rule), and a dropped string operand to the most recent string temporary.
   `Speak` / `AddNewConversation` / `AddPersonToConversation` are not in the Forge manifest, so they
   stay TODO in Villager (the hand port's `me:Speak` calls are its remaining recall gap).

### String-temporary closure (2026-09-11)

Default `CCharString` construction, copy construction/assignment, and the common `pcVarN =
"literal"` followed by `CCharString(dest, pcVarN, -1)` now remain typed operand temporaries. These
operations have no independent Lua side effect, but their exact values feed the next manifest-typed
interface call. The benchmark drops TODOs without changing call precision: Beardy Baldy 114 to 111,
Rock Troll 14 to 13, Dragon 75 to 73, and Villager 93 to 90. The complete recovery suite is 353/353.

### Recovered helper call graph (2026-09-11)

In-range functions selected as callees of lifecycle or thread bodies are now emitted as Lua helpers,
and calls are resolved by their exported target address. Duplicate terminal symbols receive an
address suffix rather than being conflated (the two distinct Beardy Baldy `GoTalkToBeardyBaldy`
bodies are the first real case). Six previously ignored bodies are now present: two for Beardy
Baldy, `DoMission_HeroUIInitialize` for Summoning the Ship, and Dragon's `DoMission_Setup`,
`SpawnMinions`, and `SpawnSummoners`. The benchmark reports helper counts separately because honest
body coverage can increase the raw TODO total while exposing more retail behavior. Beardy Baldy
interface-call recall rises 0.72 to 0.78; Dragon rises 0.45 to 0.46. Flattened direct CScriptThing
symbols such as `_IsAlive_CScriptThing__UBE_NXZ` also normalize through the proven method/manifest
entry.

### Reviewed binding-manifest closure (2026-09-11)

Five bindings present in the reviewed ForgeFSE implementation but absent from the checked-in tooling
manifest are now added reproducibly by `update_fse_manifest_2026_09.py`: `AddNewConversation`,
`AddPersonToConversation`, `MsgOnRegionLoaded`, `GiveHeroQuestCardDirectly`, and `IsXbox`. Their
scope, return types, and operands are transcribed from `LuaManager.cpp`, `LuaQuestState.h`, and the
native API typedefs; no signature is inferred from a hand port. `sol::this_state` is correctly
excluded as an injected C++ parameter rather than emitted as a Lua operand. This closes sixteen
false missing-binding TODOs in the current benchmark. The native overlay is regenerated at 952 FSE
functions and tooling SDK validation passes with the intentionally stale external mirrors skipped.
Full recovery tests: 357/357.

### Anchored entity-body join (2026-09-11)

`write_package` now checks `refs/script_recovery/native_entities/<Package>.<Binding>.json` before
creating an entity stub. An export is consumed only when it has exactly one body or exactly one
function explicitly named `Main`; ambiguous sets remain stubs. This currently replaces two stubs
with recovered bodies: `HerosOldHouse/GhostFisherman` at `0x00D8ABF0` and
`GuardianTrophyDealerInfo/GTDI_Maze` at `0x00E27E90`. The benchmark has a separate `entity bodies`
column so package coverage is visible independently of main-script call recall. Full recovery tests:
358/358.

### Reviewed local-field inventory join (2026-09-11)

Entity mode now loads `localFields` from the reviewed New Oakvale inventory matching the requested
entity. Only a hexadecimal class offset, Lua-safe field name, and a type proven either by the
explicit native `type` or the JSON type of `initValue` are accepted. Parent-field lists are not
merged, and Theresa's explicitly documented `stack` local is rejected by the offset check. This
maps Villager `0x1c` to `HeroDidHitMe`/`Bool` and removes three field-name TODOs from its benchmark.
Direct `CCharString::operator=` writes to quest fields are also emitted as `SetStateString` instead
of opaque native statements; unknown names retain a review marker and `self_0xNN`.

### Opaque token-initializer filtering (2026-09-11)

Calls to native target `0x0099A2E0`, labelled `NHeroInformationScreens::CBase::CBase`, are omitted
from Lua. This is grounded in `native_helper_operation_ir.json`, which classifies all 101 observed
consumers as a complete `opaque-vtable-token-initializer` with no Lua emission. It removes 22 false
TODOs from the benchmark without changing recovered gameplay calls. Full recovery tests: 362/362.

### Spawned-thread ownership collapse (2026-09-11)

The thread recognizer now retains the native variable receiving each `operator_new(0x3c)`
`CSpawnedFunc`. A later `CGuiVarTransferStruct::Add` is removed only when its transferred object is
that exact captured variable: the already-emitted `CreateThread` represents the script operation,
while the `Add` merely gives the native container ownership. Transfers of any other object remain
TODOs. This closes 17 false TODOs across the benchmark without changing thread counts or recovered
interface calls. Full recovery tests: 363/363.

### Thiscall entity-binding closure (2026-09-11)

The entity-binding recognizer now accepts the second instruction-equivalent MSVC spelling present
in the retail corpus: `::operator_new(0x1c)` plus a casted `CScriptBase *` receiver. It still
requires one allocation variable to flow to the matching `AddEntityScriptBinding` call and takes
the binding name only from that constructor's literal. This recovers 12 previously missed bindings
(Beardy Baldy 1, Summoning the Ship 6, Singing Stones 2, Dragon 3), removes 58 TODOs, and raises
Summoning interface-call recall from 0.53 to 0.60. Generated packages now contain explicit stubs for
those recovered retail entity scripts rather than silently reporting zero entities. Full recovery
tests: 365/365; tooling SDK validation passes at 952 FSE functions.

### Locally paired termination-alias closure (2026-09-11)

Anchored entity exports do not currently carry call-target metadata, so the known bsim label
`CCreatureAction_TrollWhackGroundBase::Initialise` is not discarded by name. It is collapsed only
when its result is immediately tested as `extraout_AL[_N] ==/!= '\0'`; that consumer is already
lowered to the `alive` result from `NewScriptFrame`. Unpaired occurrences remain review TODOs. This
removes 48 redundant TODO lines from the two generated anchored bodies (Ghost Fisherman retains 5;
GTDI Maze retains 3). These entity-body TODOs are reported separately and therefore do not change
the benchmark row's top-level TODO column. Full recovery tests: 367/367.

### Anchored-body benchmark visibility (2026-09-11)

`benchmark_lifter.py` now reports `entity TODO` separately from the package lifecycle/thread/helper
TODO total. The first honest baseline is 113 for HerosOldHouse/GhostFisherman and 122 for
GuardianTrophyDealerInfo/GTDI_Maze; previously the table showed only that one body existed, which
made body-level converter work invisible.

### Reviewed `Speak` binding and scalar operands (2026-09-11)

The manifest now includes the Lua name `Entity.Speak`, whose binding-table overload delegates to
`Speak_Blocking`. Its target, text key, selection method, and three flags come directly from
`LuaManager.cpp` and the seven-operand `Speak` typedef in `EntityScriptingAPI.h`. Resource-method
lifting retains the explicit target, trims only Ghidra's trailing phantom operand by that ABI, and
constant-propagates casted `0/1` staging locals. All eight recovered calls now contain their retail
text keys and scalar values: three Ghost Fisherman, three GTDI Maze, and two NOVI Villager. Villager
recall rises from 0.84 to 0.87; its TODOs fall from 83 to 80, while GTDI Maze entity TODOs fall from
122 to 114. Manifest-typed boolean operands are normalized from native `0/1` to Lua `false/true`
(where `0` would otherwise be truthy), while enum/integer slots remain numeric. Full recovery tests:
369/369. The manifest and native overlay now contain 953 FSE functions.

### Template-cast scalar staging (2026-09-11)

Pure local assignments whose value is a literal behind one parenthesized native type cast now
remain operand temporaries even when the cast contains templates (for example
`(pair<enum_...,class_...> *)0x0`). The cast is stripped only inside literal recognition; arbitrary
expressions are unchanged. Combined with typed boolean normalization, this removes six top-level
TODOs and ten anchored-body TODOs while preserving exact null/zero values. Ghost Fisherman entity
TODOs fall from 113 to 109 and GTDI Maze from 114 to 108. Full recovery tests: 370/370.

### Assignment-in-condition sequencing (2026-09-11)

The exact Ghidra form `if ((left) && (v = GSI->Call(...), right))` is now split into a manifest-
typed call assignment followed by the Lua condition, removing the C comma-expression syntax. The
2026-09-12 correction below adds the required short-circuit guard; the first version called the
right-hand side unconditionally. This conversion recovers Ghost Fisherman's timer query with its
previously registered timer handle and raises Dragon Boss Fight call recall from 0.47 to 0.49.

### Typed refcount cleanup closure (2026-09-11)

The existing native refcount-release filter now accepts typed pointer casts in addition to
`undefined **`, while retaining the strict same-variable `--refcount; refcount == 0` shape. These
blocks only release native temporaries and have no Lua operation. Unmatched assignment expressions
remain visible rather than being broadly suppressed. Full recovery tests: 372/372; tooling SDK
validation passes at 953 FSE functions.

The interface annotator accepts both hexadecimal and decimal vtable offsets. This matters for
Ghidra's `+ 300` spelling of retail slot `0x12C`; the name still comes exclusively from the proven
slot table rather than an inferred call label.

### Repeated termination-query conditions (2026-09-11)

The exact compiler/decompiler form
`if ((!v) && (v = CScriptBase::IsActiveThreadTerminating(...), !v))` now lowers to
`if alive then`. Both operands are the same pure scheduler-termination fact, which the lifter
already obtains from Forge's `NewScriptFrame` result. This removes invalid Lua assignment syntax
such as `true = CScriptBase::IsActiveThreadTerminating(...)` without generalizing over unrelated
comma expressions. Full recovery tests: 373/373; tooling SDK validation passes at 953 FSE
functions.

### Short-circuit execution and block closure (2026-09-12)

For `if ((left) &&/|| (v = GSI->Call(...), right))`, the converter now evaluates `left` once,
guards the call according to the operator, and evaluates `right` only after the assignment. The
result variable stays visible after the guard and retains its previous value when the call is
skipped. Generated condition variables avoid identifiers present in the native source.

Lua execution tests also exposed two structural defects: an `else`/`else if` followed an already
emitted `end`, and declaration stripping removed all trailing braces instead of just the function's
outer brace. Both are corrected. Tests execute the generated Lua for both operators, both left-hand
outcomes, both right-hand outcomes, staged/existing result variables, overwritten boolean conditions,
and nested terminal `if`/`elseif`/`else` branches.

All 16 benchmark drafts were regenerated. Call recall, precision, and TODO counts are unchanged:
these fixes preserve execution semantics that call-count scoring does not measure. Drafts still
require review of unresolved native constructs and validation before use in game.

Syntax-only inspection of the regenerated tree passes 47/59 Lua files (including entity stubs).
The other 12 still contain unsupported pointer expressions, comma expressions, or native control
flow; this is not a gameplay-readiness count. Next work should make those failures visible in the
benchmark and lower supported expression shapes without hiding unresolved behavior.

Validation: converter tests 61/61, full recovery suite 378/378. SDK validation passes with
`--skip-mirrors`; the full check reports overlay drift in the external FableForge and ForgeFSE
mirrors, which this continuation did not modify.

### Marathon continuation: syntax scoring and executable data flow (2026-09-12)

`benchmark_lifter.py` now compiles each recovered Lua body with `lupa.lua54`, matching Forge's
vendored Lua 5.4.8 language version, without executing it. The Markdown table adds a passing/checked
column; JSON rows and the aggregate `syntax` object include paths, line numbers, and compiler errors.
Empty entity stubs and `quests.lua` registration loaders do not count. `--require-syntax` makes the
benchmark return nonzero while any recovered file fails. Baseline artifacts are retained under
`work/lifter_marathon_20260912_baseline/`.

The baseline is **6/18 recovered files**, not 47/59: the earlier tree-wide count included stubs and
loaders. The current result is **8/18**, with Barrel and Dead Father newly compiling. Ten files
still have unsupported native expressions; all are listed in `LIFT_BENCHMARK.md` and JSON.

Changes grounded in existing exports/inventories:

- Entity parent reads/writes through `*(int *)(this + 0x14)` join the owning quest's reviewed
  persistence-layout offsets to the entity's declared parent-field names. Qualified owner classes
  must agree; arrays, aggregate regions, and undeclared fields remain unresolved. Parent reads are
  typed, so boolean waits reload the shared flag and test it as a Lua boolean.
- Reviewed numeric constants with one explicit `DAT_` anchor and `retail-rdata` evidence are
  available to entity expressions. This resolves Villager's `_DAT_0122dedc` health threshold to
  `0.0`; numeric `float10` casts no longer leak into Lua. Ambiguous or inferred constants are excluded.
- Exact-target termination calls immediately followed by `v = extraout_AL_N` become typed boolean
  assignments, including Ghidra's wrapped `) ;` spelling. Unverified callees retain their TODOs.
- Mutable scalar staging variables retain Lua storage instead of being substituted forever with
  their initial constant. Their declarations are hoisted within the generated function so branch
  and loop updates stay visible. Reassignments invalidate old operand temporaries; numeric values
  in manifest-typed boolean slots become explicit `~= 0` tests.
- Unknown scalar field loads now use the same `GetState...("self_0xNN")` storage as their existing
  `SetState...` writes, retaining unnamed-field review markers. A typed, never-reassigned
  `CScriptBase *in_ECX` input in an exported quest worker is normalized to the native quest receiver.
  An untyped or reassigned ECX input is not treated as evidence of ownership.
- Boolean field comparisons against native hexadecimal `0x0`/`0x1` use boolean semantics rather
  than comparing Lua booleans to `nil` or numbers.

Tests execute generated loops, branch updates, shared-parent waits, and boolean operands in Lua.
The actual lifted Barrel Main also executes its already-instructed path, both PC/Xbox instruction
text branches, and termination while the instruction prompt remains open. This verifies Main only;
entity callbacks and unrelated generated scripts still need their own coverage.

Call recall does not regress in any of the 16 rows: ScytheInfo rises 0.81 -> 0.89, BeardyBaldy
0.82 -> 0.85, SummoningTheShip 0.60 -> 0.70, SingingStones 0.73 -> 0.77, and DragonBossFight
0.49 -> 0.62. These are call-multiset scores including state accessors, not behavioral parity.
Some precision scores decrease because the hand ports hide state access behind helpers; newly
exposed missing-operand/unnamed-field TODOs are retained rather than suppressed for the score.

Validation: full recovery suite **394/394**, including **77 converter/benchmark tests**; canonical
SDK validation passes with `--skip-mirrors`. External mirror drift remains as recorded above.

Next substantive recovery targets are the inlined string-comparison loops, CThing/CScriptThing
temporary ownership and return slots, nested master-data/vector accesses, and remaining comma
expressions. Their syntax failures remain explicit; a syntax pass alone never enables a package.

### Literal loops, call operands, and labeled returns (2026-09-12, parallel runtime session)

The runtime/retail-porting handoff is
`docs/journal/2026-09/NEW_OAKVALE_PARALLEL_SESSION_PROMPT_2026-09-12.md`; it includes the Bully
run-off/camera investigation and the newly reported camera-dependent quiet dialogue. This converter
continuation does not modify the runtime implementation, installed game, or reconstructed packages.

`native_literal_loops.py` recognizes the exact bounded byte-comparison loop over two ASCII literals.
It preserves the resulting count and equality flag, and only removes pointer updates when neither
pointer is used elsewhere except its declaration. It rejects escaped/unknown bytes, mismatched loop
updates, reused pointers, counts outside signed-int range, and any read beyond either literal's
terminating NUL. Matches inside comments or quoted data are not treated as code. This recovers two
Beardy Baldy comparisons: Main and WatchForQuestCardConditions. Each function's report now records
`literalByteComparisons`; the formerly invalid dereferences of string literals are gone.

`native_arguments.py` splits call operands by top-level commas while respecting quoted literals,
parentheses, brackets, braces, and C comments. All interface/resource/thing/helper call emitters use
it. Commas inside template pointer casts are protected by their surrounding parentheses; comparison
operators are not mistaken for template delimiters. Malformed operands remain explicit review
items. Template pointer cast spellings are removed with quoted text preserved; numeric and other
non-pointer casts are not generalized by this helper. Execution tests verify comma-containing
strings, nested operands, and casted 0/1 values in boolean API slots.

Native code can have a return followed by a label and more instructions. Lua requires a return to
end its block. The converter now encloses such a return in `do return end`, preserving the return
and retaining the later labeled code. Labels/gotos remain review items; this does not claim to
reconstruct their control flow.

Validation: **87 converter/support/benchmark tests** pass; the full shared-workspace recovery suite
passes **406/406**. Regenerated all 16 benchmark rows with unchanged call-recall, precision, and TODO
counts from the preceding checkpoint. **8/18 recovered files** compile; ten still fail. The first
remaining failure in Beardy Baldy is now its conditional indirect call at generated line 150 rather
than the literal-comparison loop. Statue Master and GTDI Maze also advance past template-cast syntax
to their still-unrecovered native operands. See the generated benchmark for exact current locations.

Current converter-only handoff: `docs/journal/2026-09/SCRIPT_CONVERTER_HANDOFF_2026-09-12.md`.

### Verified thing predicates and repeated conditions (2026-09-12, next marathon)

Guardian Trophy Dealer Info's `WaitForPieceOver` now recovers its Maze lookup and repeated
`IsAlive` predicate. The generated quest file compiles, raising the benchmark to **9/18 recovered
files**. The anchored GTDI Maze entity still fails independently; this is not a gameplay-complete
port. Call-recall, precision, and TODO counts remain unchanged in all 16 benchmark rows.

`native_thing_predicates.py` and `native_thing_predicate_witnesses.json` form a deliberately narrow
reviewed recovery registry. This pass covers one exported thread, not general stack analysis. It
does not infer receivers from reused stack names or apply CScriptThing's vtable to arbitrary CThings.
It verifies SHA-256 digests of the original and annotated export, the reviewed caller region and
native methods, plus two vtable targets. Missing or changed evidence leaves the source untouched
and records the rejection in `thingPredicateEvidence`.

Primary evidence is the installed retail executable, cross-referenced with the CScriptThing and
GSI slot tables in `ghidra_out/script_recovery/`, `ghidra_out/engine_api.tsv`, and
`rebuild/include/engine/CScriptThing.h`:

- CScriptThing has its vptr at +0, implementation pointer at +4, and counted-pointer info at +8.
  The copy constructor at `0x004ABE90` confirms this layout. `IsAlive` at `0x004AB130` loads +4,
  rejects null, dispatches implementation slot `0x12c`, and normalizes the boolean result.
- At `0x00E27B1C`, `GetThingWithScriptName` receives a hidden return buffer at stack-base +8.
  Its native implementation returns with `ret 8`; the intervening string destructor preserves
  the stack position. At `0x00E27B30`, the caller loads stack-base +12, exactly that wrapper's
  implementation pointer, and executes the inlined IsAlive sequence.
- Both false edges reach `0x00E27B65`; the frame/termination path returns to the predicate. The
  witness hashes 233 caller bytes through the final ret; the export's reported size of 230 omits
  the final instructions. Similarity-derived function names do not establish identity.

The rewrite binds the lookup to a collision-free local and emits a nil-safe IsAlive loop. It refuses
to collapse the native result assignment if the temporary has another use. Reports include verified
addresses, digests, and the explicit stack association. Beardy Baldy's negative OR condition still
needs separate receiver evidence; Guardian's witness does not cover it.

GSI assignment conditions also now lower in `while ((left) &&/|| (target = GSI->Call(...), right))`.
The left operand executes at each loop head, the RHS call only when needed, and the right predicate
after assignment. The target stays outside the loop so skipped calls, exits, and explicit breaks
preserve its value. This does not recover unknown indirect callees or arbitrary comma expressions.

Validation: **418/418 full shared recovery tests** pass. Lua 5.4 execution tests cover the actual
Guardian thread with immediate completion, absent/dead Maze, multiple PieceOver/IsAlive frames,
and termination during either wait. Additional tests exercise both short-circuit operators over
repeated loop heads and explicit breaks. Negative tests cover missing/changed bytes and targets,
source/annotation drift, live result temporaries, ambiguous witnesses, and invalid byte ranges.
These are offline tests, not an in-game run.

Baseline: `work/lifter_thing_predicates_20260912_baseline/`. Only Guardian's quest draft/report and
the two benchmark summaries differ after regeneration. This pass did not modify runtime files,
the installed game, reconstructed packages, or shared API manifests.

### Beardy's barber predicate and spaced calls (2026-09-12)

The reviewed predicate registry now also covers Beardy Baldy Main (`0x00E4FA20`). Its negative OR
condition tests the nearest barber, not the separately fetched Beardy object. At `0x00E4FFCA` the
caller pushes the string argument before calling GetHero. GetHero (`0x00891CA0`) returns a borrowed
wrapper pointer in EAX and uses plain ret, so that argument stays on the stack. After pushing EAX,
the caller computes the hidden result address with `[esp+0x28]`, putting the returned barber wrapper
at stack-base +0x20. GetNearestWithScriptName (`0x0089A510`) uses ret 12; the load at `0x00E4FFE9`
reads base +0x24, that wrapper's implementation pointer. Null and false predicate edges both reach
`0x00E5002C`. The distinct Beardy wrapper is base +0x2c and is tested directly at `0x00E50030`.

The new witness pins the raw/annotated source, the 178-byte lookup/predicate window, GetHero and
GetNearestWithScriptName implementations, shared wrapper helpers, and four vtable targets. It is
a reviewed native window, unlike Guardian's complete caller region. Lifecycle Init/Main now pass
their original export metadata into the same verification pipeline as threads and record evidence.

The negative rewrite retains its conditional result assignment because cVar1 is reused elsewhere.
Structured if/while assignment lowering now accepts direct CScriptThing methods as well as GSI
methods. The target remains unchanged when the left operand skips the call; the method's signature
supplies boolean result typing before the right predicate is translated. An absent Lua wrapper is
handled by the null guard. Tests execute the actual recovered lookup/guard slice with distinct
Beardy, hero, and barber objects, plus absent/dead/live barber cases. This is deliberately slice
coverage: the rest of Main and its worker functions are not claimed to execute correctly.

Interface-call recognition also accepts whitespace between the method name and opening parenthesis.
This recovers a previously raw GetActiveQuestName expression in Beardy and six formerly unparsed
Ghost Fisherman calls. Operand recovery remains separate: Ghost Fisherman's question arguments and
some thing-name/marker operands still expose guessed or incorrect ordering. Their reduced parser
TODO count is not evidence of behavioral correctness; the anchored entity remains an invalid draft.

Validation: **426/426 full recovery tests** pass. The benchmark remains **9/18 compiling files**;
Beardy's first error advances from its indirect condition at line 150 to the refcount comma
expression at line 258. All recall scores are unchanged. Beardy's lifted interface-call count rises
182 to 183 from GetActiveQuestName; precision changes 0.841 to 0.836. Ghost Fisherman's TODO count
falls 110 to 104. Baseline: `work/lifter_beardy_predicate_20260912_baseline/`. Runtime/game files,
reconstructed ports, and shared manifests were not modified.

### New Oakvale behavioral comparison: CreatedBeetle (2026-09-12)

The working port now provides an executable comparison in `test_novi_converter_parity.py`.
The test loads the actual reconstructed CreatedBeetle script and its actual common/fields/deeds
modules, then compares its host-call trace with the complete generated Main under Lua 5.4.
The host is mocked; this is offline parity for the tested sequence, not an in-game certification.
Scenarios cover an already-expired timer, multiple lifetime frames, cancellation on the first
frame before registration, and cancellation on two later wait frames. Both implementations must
register/set/read the timer in the same order, remove only on normal expiry, and deregister exactly
once after registration. An explicit expected trace checks the five-second lifetime and removal
flags, so agreement alone is not the only oracle.

This caught a behavior bug that compilation and call-multiset scoring missed. Retail Main at
`0x00DB80C0` calls `RemoveThing(me,1,1)`; the port passes `true,true`. The shared API manifest has a
legacy one-argument RemoveThing entry, causing the converter to drop both flags. The current
runtime's `LuaQuestState::RemoveThing` accepts optional immediate/removeFromWorld booleans and uses
defaults false/true. Therefore the dropped immediate flag changes behavior. The retail signature
is also present in `ghidra_out/engine_api.tsv` at `0x008910D0` as thing,bool,bool.

`converter_signatures` now adapts only that exact legacy Quest/void/CScriptThing* entry in a copied
in-memory manifest. It leaves complete or differently shaped signatures alone and does not alter
the shared SDK files. Runtime evidence was read from
`D:/Code/ForgeFSE-retail-shadow/FableScriptExtender/LuaQuestState.h` (declaration),
`LuaQuestState.cpp` (implementation/defaults), and `LuaManager.cpp` (binding).
Optional bool parameters now receive the same 0/1 and typed numeric-result conversion as required
bool parameters. Absent optional flags stay absent instead of consuming stale staged values.

Validation: **431/431 full recovery tests** pass. The parity test deliberately removes the flags
again and verifies that the comparison detects the former bug. Additional tests cover explicit
false/true combinations, hexadecimal literals, typed numeric results, omitted flags, signature
idempotence, input immutability, and nonmatching signature preservation. The generated beetle now
uses `RemoveThing(me, true, true)`. Optional boolean correction also affects generated DeadFather
animation flags and a few conversation flags in Villager, SingingStones, and GhostFisherman.

All benchmark metrics remain byte-for-byte unchanged: **9/18 recovered files compile**. That is
expected for a behavior fix in already compiling code. Baseline:
`work/lifter_novi_parity_20260912_baseline/`. Runtime, installed game, working port modules, and
shared manifests were read only. Other unresolved arguments and control flow remain review items.

### Aeon sample execution comparisons (2026-09-12)

`test_aeon_converter_parity.py` now executes generated Fisherman and MeetSister Main functions
against Aeon's unchanged sample scripts under Lua 5.4. It compares bindings, objective arguments,
frame waits, quest activation, and deactivation. Logs are excluded. State/region queries are pure
reads of a frame-driven mock world, so Fisherman's extra region query in the sample does not invent
an extra world transition. The tests do not compare Init/OnPersist or entity callbacks. In particular,
Fisherman's sample uses an Int FishermanTasks field where the current native persistence recovery
uses Bool; that difference is not silently treated as a converter defect or normalized away.

Fisherman's complete Main comparison exposed a missing deactivation delay in the generated draft.
The sample passes zero; the current runtime requires an unsigned delayFrames argument, so nil is
not a runtime default. Native Main (`0x00E7B4E0`) confirms the zero independently: EBP is zeroed at
`0x00E7B4EB` and stays unchanged until `push ebp` at `0x00E7B775`. GetActiveQuestName is called at
`0x00E7B77B` with a hidden string-return buffer; its implementation at `0x00891880` uses ret 4 on
both branches, leaving the zero on the caller stack. The returned string is pushed at `0x00E7B784`,
followed by DeactivateQuestLater at `0x00E7B785`. Ghidra had attached zero to the getter's operands.

`native_call_operands.py` recovers this one reviewed adjacent-call pair using
`native_call_operand_witnesses.json`. Raw/annotated export digests, complete Main bytes, getter ABI,
deactivation dispatch bytes, and vtable targets must match. Missing or changed evidence leaves
the input untouched and is recorded as a rejection; a match is recorded in Main's `operandEvidence`.
The rule does not guess that arbitrary extra getter operands belong to the next call.

Validation: **435/435 full recovery tests** pass. Fisherman comparisons cover immediate transition,
waiting for the rescue and region exit, and cancellation during either wait. They require activation
before deactivation with the exact active quest name and zero delay. A mutation test restores the
old nil argument and confirms failure. MeetSister checks both entity bindings and the complete
objective/region tuple. Evidence tests reject missing/changed bytes, targets, and source digests.

Benchmark remains **9/18 compiling files** with unchanged call scores. Fisherman's TODO count falls
4 to 3. Baseline: `work/lifter_aeon_parity_20260912_baseline/`. Sample scripts, New Oakvale/Bully port,
runtime files, installed game, and shared manifests were not modified.

### Full NewOakValeIntro conversion objective (active)

`convert_new_oakvale.py` now enumerates all 17 reviewed inventories and emits 51 native functions,
including quest helpers and entity callbacks. It reads the translation unit with a native-decompile
file fallback for shared entries outside its range, resolves available local helper names by exact
callee address, and reports coverage aliases separately. Generated registration remains disabled.
Per-function and complete-file Lua 5.4 diagnostics appear in `CONVERSION_REPORT.json/.md` under
`refs/script_recovery/lifted/NewOakValeIntro/`. No working port is used as generated source.

The initial complete-scope baseline was 23/51 functions and 2/17 files compiling with 2085 TODOs.
The first recovery pass reaches 32/51 and 3/17 with 2015 TODOs. Native SetIsPushableByHero takes
CScriptThing by value (`engine_api.tsv`, 0x008A6DD0). `native_self_wrapper.py` recognizes the exact
embedded wrapper copy and reference increment, replacing its argument with me while rejecting
changed layout/lifetime shapes. This restores calls previously discarded as destructor noise.
Current runtime declarations also verify optional forceDead on EntitySetAsKillable and optional
wander/wave/continueAI on SetThingHasInformation; converter-only legacy signature corrections retain
those flags. Entity receiver aliases using param_1+8 now work alongside this+8.

The full generator gives each entity module private state storage for native entity-local fields;
parent quest fields remain on quest. Tests verify that two entity instances cannot overwrite each
other's DoneIntro. This is intentionally separate from the older single-entity benchmark's storage
convention. Actual generated Bully Init host effects match the unchanged working port, including
pushability and all killability/information flags. This does not cover its Main or the live visual bug.

Validation at this checkpoint: 443/443 full shared recovery tests. The complete conversion goal is
active and incomplete. Generic helper signatures still require parameter plumbing, and many native
control-flow, aggregate, resource, and operand shapes remain unsupported. Compilation counts are
progress diagnostics, never completion criteria or permission to enable these generated drafts.


### Helper values, retail RNG, and current termination queries

The complete generator now parses inventoried member signatures, removes the native receiver,
and gives explicit scalar parameters distinct Lua names. This prevents helper param_1+8 from being
mistaken for the entity wrapper. Local helper calls preserve assigned return values and check arity.
Native value returns are emitted, and function-scoped locals first assigned inside a Lua block are
hoisted so a loop result remains available after break. Formal parameters are never shadowed.
Comparison-to-integer masks of the form ~-(uint)(a != b) & value return value or zero in Lua.

`native_random.py` folds an adjacent rand-result/modulo pair only when the raw temporary is dead
and exported rand calls target 0x00BFEB16 with matching x86 relative-call bytes. The runtime's
RetailRandModulo calls this same CRT thunk, preserving the game RNG stream. The supported domain
is positive moduli (including the villager's native vector counts). Comments, live temporaries,
unknown targets, mismatching bytes, and extra unmatched calls are rejected. Converter-only host
signatures include this binding; the shared SDK manifest remains untouched.

The full generator enables live termination queries. The current LuaManager.cpp entity-VM binding
captures pEntityHost and calls 0x00F35B30; quest VMs use the quest predicate. Thus reviewed native
termination calls refresh alive at the original check, including before the first frame. The legacy
benchmark retains its older frame-status convention. Unresolved native calls/control flow elsewhere
are still unresolved; this change does not establish whole-quest cancellation correctness.

`test_native_random.py` executes the actual generated GetVillagerSpeechIndex under Lua 5.4 and
compares its draw/frame/write effects to the unchanged working-port helper. It covers immediate
selection, repeat retries, and retry cancellation. Additional native tests check termination before
drawing and before/after the final state write. Retail returns zero on cancellation while the port
returns nil; this intentional difference is asserted explicitly. The generated helper has zero TODOs.

Validation: 452/452 full recovery tests pass. Full conversion remains 51 functions across 17 owners,
zero missing sources, 32/51 function syntax passes and 3/17 file passes, with 2019 TODO diagnostics.
The higher TODO count includes now-visible helper arity mismatches; it is not a completion score.
Legacy benchmark remains 9/18 compiling files. Generated registration stays disabled.

Next work: cross-owner parent helper calls (Bully GivenTeddy calls AddBadDeed at 0xDAEA70;
TeddyGirl GivenTeddy calls AddGoodDeed at 0xDB0660) need a valid shared Lua module/call boundary,
not merely a renamed global across separate entity VMs. TeddyGirl also needs its master-data B
string write. Villager Main still has unresolved vector-count/index/string-resource operations.
All runtime, reconstructed port, installed-game, inventory and shared manifest files were read only
throughout this pass.


### Shared native quest helpers and deed operands

The generator now resolves entity-to-parent helper calls from exact exported call targets, requiring
an explicit parent pointer at this/param_1+0x14 and the reviewed explicit parameter count. It emits
calls through NewOakValeIntro.native_quest_helpers, not cross-VM globals. The generated shared module
contains AddBadDeed and AddGoodDeed, with local forward declarations and a returned table. Its bodies
are lifted again from native source with quest field layout but entity execution context; frame waits
receive me. Root quest copies retain quest execution context. Transitive helper dependencies are
included. Conflicting short call labels are rejected when building the parent-helper map.

The shared artifact has separate per-helper TODO diagnostics and is included in complete-file syntax
checks. The package now has 18 Lua implementation files for the same 17 owners / 51 native functions.
Bully GivenTeddy has no direct TODOs and tests verify gold, teddy removal, both parent flags, and the
shared AddBadDeed call with deed ID 3 in isolated entity VMs. The test substitutes the deed body to
verify the call boundary; it does NOT claim the still-incomplete actual deed body is usable. Loading
the real generated shared module is tested without executing those unresolved bodies.

Argument placement now permits equal values from distinct consumed native temporary slots. This
restores both empty region strings on AddGoodDeed's objective update. Exact-target alias 0x00CBE9EE
recovers AddLogbookTutorialEntry from the misleading CSubtitleRenderer::SetText label. Its one ECX
string operand is resolved before the noise filter (the spurious decompiler argument list contains
unaff_EDI). Tests cover the real string argument, unresolved slots, and an unknown target. Existing
native logbook audit and current runtime verify the tutorial helper's category/title/yield behavior.

Validation: 456/456 shared recovery tests pass. Full generator: 32/51 functions compile, 4/18 files
compile, zero missing native sources, 1994 TODOs in the 51 primary bodies. Shared-helper diagnostics
remain visible separately. Legacy benchmark stays 9/18 files compiling, with lower missing-argument
counts in several scripts after retaining repeated temporary values. All drafts remain disabled.

Exact next recovery target: AddGoodDeed now has only the native morality-load statement and missing
GiveHeroMorality argument as TODOs. AddBadDeed additionally needs its indexed WhichBadDeedsPerformed
array access and phantom register string-wrapper alias. Native amount comes from SCRIPT_DEF
OVI_MoralityChangePerDeed (0.001 in pinned installed script.bin), proven by
`audit_new_oakvale_morality_amount.py`; do not copy a port constant without preserving that data evidence.
The bad-deed decompile misidentifies the indexed enum as unaff_retaddr; inspect instruction evidence
before substituting native_arg_param_1. Working-port deeds.lua is available for execution comparisons.
Runtime and working-port files were read only during this pass.


### Deed bodies recovered and executed against the working port

`native_deeds.py` and `native_deed_witnesses.json` now recover the morality operand for both native
deed helpers. Recovery checks complete native function bytes and both raw/annotated source hashes,
then decodes SCRIPT_DEF entry 597 from the hash-pinned retail script.bin. Its float is
0.0010000000474974513; AddGoodDeed passes it unchanged and AddBadDeed negates it. Native instructions
DB0670..DB067F load global+0xd64 and push its bits into interface slot 0x270. DAEA7A..DAEA8E uses
fld/fchs/fstp before the same slot. Changed source, bytes, or unavailable/changed data reject recovery.
No working-port source or constants feed the generated output.

Bad-deed indexing is also instruction-proven: after push esi/push edi, DAEA98 reads the explicit
stack parameter from [esp+0xc] into EDI. DAEB4B, DAEB67, and DAEBE2 write/read [ESI+EDI+0xfc]. The
pinned recovery replaces Ghidra's erroneous unaff_retaddr index with the actual renamed enum argument
and removes two phantom aliases used only by incorrect string cleanup/logbook signatures. Typed
byte-array loads/stores now lower through quest state, using bool-array metadata from persistence
inventory and the established WhichBadDeedsPerformed_0..4 keys.

Signed 32-bit host int/long parameters now emit -1 for 0xffffffff, preserving the native bit pattern
without passing an out-of-range positive Lua integer. Unsigned slots retain the positive value.
This fixes the deed counter sentinel and the same native animation loop-count idiom.

Both generated AddGoodDeed and AddBadDeed have zero TODOs. `test_native_deeds.py` executes the actual
shared generated module and unchanged reconstructed deeds.lua/common.lua/fields.lua under Lua 5.4.
It compares state writes, host calls, frame waits, tutorial/objective updates, and counter values
across five good-deed scenarios and twenty bad-deed scenarios (all five categories; first, repeat,
new category, and wait cancellation). Morality comparison applies the host's real float32 conversion;
native/port Lua return conventions are not equated. Initial or between-call termination is not
claimed as parity with the port merely because tested wait cancellation agrees.

Validation: 459/459 recovery tests pass. Full conversion remains 32/51 functions and 4/18 files
compiling, zero missing native bodies, 1986 TODOs. The two shared deed bodies also have zero TODOs.
Legacy benchmark remains 9/18 files compiling. Draft registration is disabled. Runtime, installed
application and working-port files remain untouched.

Next: exercise the complete generated Bully GivenTeddy callback through these actual deed bodies,
then recover TeddyGirl's master-state string write. Broader remaining families include native
control-flow joins, aggregate resource wrappers, and the villager/quest speech vectors. Current
source/data pinning deliberately rejects altered retail exports/data instead of guessing operands.


### Teddy callbacks through actual generated deed dependencies

The full generator now loads unambiguous string master-field write mappings from each entity's
native inventory. Exact CCharString assignment through the entity's master pointer at +0x18 maps
to SetMasterGameState; unknown offsets and non-entity/parent-pointer variants stay unresolved.
TeddyGirl GivenTeddy's MASTER+0x54 = B now emits TeddySolution = B, matching the current host's
literal-string assignment binding. Its Main's corresponding C write is also recovered.

An immediately overwritten `local_4 = this` staging assignment is removed only when the very next
statement constructs a literal CCharString into that exact slot. Different slots or an intervening
use retain diagnostics. This recovers the last phantom assignment in TeddyGirl GivenTeddy.

`test_native_teddy_callbacks.py` executes both generated GivenTeddy callbacks through the actual
generated shared deed module, comparing unchanged working-port callbacks and their actual
common/fields/deeds dependencies in isolated Lua 5.4 VMs. Four scenarios per callback cover immediate
completion, two-frame wait, repeat deed, and cancellation during the deed wait. Effects include gold,
item removal, parent flags, morality, logbook, objective/counter, information clearing and master-state
write. The private FoundTeddy state is explicitly checked. Native and port callbacks both continue
their authored post-deed operations after a cancelled deed wait; the converter does not invent an
extra cancellation guard. These tests cover GivenTeddy, not Bully Main's visual skip bug.

Validation: 462/462 recovery tests pass. Full generator remains 32/51 function syntax, 4/18 file
syntax, zero missing native sources, now 1983 TODOs. Both GivenTeddy callbacks and both deed helpers
have zero direct TODOs and execution comparisons. Legacy benchmark remains 9/18 compiling files.
Generated registration remains disabled; runtime, port and installed game files were read only.

Next priority: broad native control-flow/resource expressions in entity Main. Current first failures
include comma-assignment while predicates (AffairWoman, Theresa, Victim, WatchBarrels), native wrapper
pointer tests, and subregister/aggregate values (LiveFather/TeddyGirl). Guard Init is also still invalid.
WatchForGotGold's short body compiles but still has argument staging issues around GetActiveQuestName
and SetQuestCardObjective; use its native source at 0xDBE2E0 and working thread as comparison.


### Comma predicates and counted-loop structure

The lifter now lowers while predicates containing top-level scalar assignments separated by the C
comma operator. Balanced operand splitting preserves nested call commas. Every assignment executes
inside the loop before the final predicate, including the exit check. Prefix targets are mutable
function-scoped locals rather than folded compile-time temporaries. Pointer writes and unsupported
prefix expressions are not discarded or treated as supported scalar assignments.

Canonical for loops with optional scalar assignment initialization and scalar assignment update
now lower to Lua while loops with the update at the corresponding block tail. Pending updates are
tracked by block depth; nested loops retain separate updates and break skips its loop's update.
This fixes WatchBarrels' previous structural failure: unsupported for openings had been left as
comments while their closing braces emitted real Lua end statements. Its pointer-wrapper destructor
calls remain explicit TODOs; balanced blocks do not imply native cleanup has been recovered.

`test_native_comma_conditions.py` executes final false checks, multiple ordered assignments,
call-result predicates, counted-loop final values, breaks, and nested loops under Lua 5.4.
Validation: 468/468 full recovery tests pass. Full generator now compiles 34/51 functions and 5/18
files (Victim Main and WatchBarrels newly compile), zero missing native bodies, 1981 TODOs. Neither
new syntax pass is a complete behavioral parity claim. Legacy benchmark remains 9/18 files compiling.
Draft registration remains disabled, and runtime/port/game files remain untouched.

Next blocking families remain resource wrapper/aggregate expressions, pointer and vector accesses,
and control-flow joins represented as gotos. WatchBarrels still treats GetAllThingsWithScriptName's
aggregate result as a scalar count, has native collection pointer arithmetic, unresolved destruction,
and a branch-to-next-frame goto incorrectly approximated as return (already marked TODO). These must
be recovered before its new syntax pass is usable. Read generated functions using anchored
`^function Name(` matches: unanchored searches can match an earlier CreateThread comment naming the
function and accidentally inspect a different body.


### Scope-checked native jumps

`native_goto_scopes.py` inventories native block ancestry and accepts only jumps to a label in the
same or an enclosing lexical scope. Entering a nested/sibling block remains unresolved. Identical
jump text occurring in multiple scopes is accepted only when every occurrence is safe, because the
current emitter keys on normalized statement text. Duplicate labels or unbalanced block structure
reject the analysis. The full generator enables this mode; legacy benchmark behavior is unchanged.

Accepted jumps emit actual Lua goto/labels instead of early-return approximations. Function locals
are hoisted, and assigned native scalars are kept mutable so forward jumps cannot enter Lua local
scope or incorrectly substitute a skipped assignment. Existing return legalization keeps labels
after native returns parseable. Per-function nativeJumps/nativeLabels expose each recovered edge.

Execution tests cover forward joins skipping assignments, backward jumps, and branch-to-loop-tail
jumps which preserve later iterations. Negative tests reject entering nested scopes and ambiguous
repeated text. An actual WatchBarrels assertion confirms its iVar4 <= count-4 branch goes to
LAB_00dbeabd (the frame wait) instead of returning from the thread. Its sibling-scope cleanup label
LAB_00dbeb07 is explicitly not certified. Resource/count/destructor defects remain in that function.

Validation: 473/473 shared recovery tests pass. Full conversion emits 285 distinct jump statements
and 111 target labels. It remains 34/51 functions and 5/18 files compiling, zero missing bodies,
1632 TODOs. This removes many previously explicit control-flow approximations without asserting
whole-function parity for the still-incomplete entity Main bodies. Generated registration stays
disabled; runtime, working-port and installed game files remain untouched.

Next: native collection and CScriptThing/resource wrapper expressions remain the major syntax and
behavior blockers. Cross-scope C jumps will require restructuring or explicit control-flow lowering;
do not force them into Lua goto or claim cleanup-only based on pointer-looking code alone.


### Guard Init: entity aliases, repeated copies, and hidden vector result

Staging a known embedded-entity alias now retains the actual Lua me value instead of an undefined
native variable. This fixes Guard Init's first three calls, whose operands were pushed via pCVar1.
The exact by-value self-wrapper rule now covers reviewed wander centre/distance and scripting-state
methods in addition to pushability. Repeated passes remove a later dead copy before checking earlier
copies that reuse its locals; a remaining real use still blocks removal. Reviewed decorated signatures
prove float operands for SetWanderMinDistance/MaxDistance (despite Ghidra's long guess), so native
argument words 0 and 0x40c00000 become 0.0 and 6.0. Layout/refcount/unknown-signature guards remain.

By-value C3DVector results from reviewed CScriptThing signatures now retain their hidden return slot
and a Lua result local. SetWanderCentrePoint's pos parameter has converter-only vector metadata,
so it can consume the GetHomePos result and cannot use stale scalar staging as a position. Guard
Init now calls SetWanderCentrePoint(me, homePosition) instead of discarding GetHomePos's result.
This is not general native vector-memory or collection support.

Guard Init has zero TODOs and executes against the unchanged working port in Lua 5.4. Tests compare
all init host effects and a nontrivial position vector. EntitySetAsKillable's omitted third argument
is normalized through the current host's false default in this comparison. Additional tests cover
reused copy locals, live-tail rejection, staged me, and rejecting scalar values for vector parameters.
Native ABI sources: engine_api.tsv at 0x008A23B0, 0x008A2690, 0x008A2950, 0x008A36D0;
CScriptThing GetHomePos vtable slot 0x1c has decorated C3DVector-by-value return. Current runtime
SetWanderCentrePoint/MinDistance/MaxDistance uses retained by-value copies and vector/float operands.

Validation: 477/477 full recovery tests pass. Full converter is now 35/51 functions and 5/18 files
compiling, zero missing bodies, 1624 TODOs. Legacy benchmark remains 9/18 compiling files; Ghost
Fisherman loses one missing-argument TODO. Runtime, port, shared manifests, and installed game remain
untouched. Draft registration remains disabled. Continue with the unresolved NPC Main resource and
collection expressions; Guard Main is still incomplete despite Init now matching the port.


### Conditional declarations, controlled calls, and literal byte slices

Fixed an interaction between hoisted variables and conditional call lowering: an already-hoisted
new variable must not emit a bare Lua expression statement. If no prior value requires assignment,
its function-level declaration is sufficient; it is not reset to nil at each loop/condition check.
This repairs BarrelThug Main syntax without changing the short-circuit call behavior.

Conditional CScriptThing calls now go through the same statement dispatch as standalone calls.
Reviewed CScriptGameResourceObjectScriptedThingBase methods therefore bind to me rather than being
called on a raw stack address. The test executes IsPerformingScriptTask only when the left condition
requires it, verifying both zero calls on the skipped path and the returned boolean on the live path.

`native_subregisters.py` evaluates Ghidra ._byteOffset_byteCount_ expressions only for complete known
integer-literal inputs, after temporary substitution. Zero, nonzero byte positions, and negative
integer bit patterns are tested. Unknown values, floating inputs, invalid widths and quoted strings
remain untouched. This removes malformed constant slices in LiveFather/TeddyGirl but does not recover
their remaining aggregate resource state or justify folding unknown subregisters.

Validation: 481/481 full recovery tests pass. Full conversion is 36/51 function syntax, 6/18 file
syntax, zero missing sources and 1622 TODOs. BarrelThug Main now compiles but remains behaviorally
incomplete. Legacy benchmark remains 9/18 compiling files. No runtime, working-port, shared manifest
or installed-game changes; generated registration remains disabled.


### Parent byte arrays and decisive constant conditions

Entity parent-state loading now expands explicitly declared bool arrays when the owner's reviewed
persistence layout proves their exact byte extent. Guard's WhichBadDeedsPerformed[0..4] declaration
maps parent+0xfc..0x100 to the same WhichBadDeedsPerformed_0..4 keys used by generated AddBadDeed.
Ownership, declaration, element type and inclusive extent must agree; conflicting offsets reject.
Tests execute all five reads before/after shared-state changes and reject mismatched owners/ranges.

`native_constant_conditions.py` folds only a decisive literal equality/inequality on the left of
and/or, with one balanced parenthesized RHS. For example, a known-null equality in an OR makes the
reference-count decrement RHS unreachable. Unknown pointers, nondecisive tests, mixed nil/numeric
comparisons and outer operator combinations are left unchanged. Tests verify short-circuit effects
and folding after native literal staging. This is not a live reference-count/cleanup implementation.

Validation: 486/486 full recovery tests pass. Full converter remains 36/51 functions and 6/18 files
compiling, zero missing native bodies, 1612 TODOs. The existing scalar-parent test was updated to
include its now-supported, explicitly declared bool-array fixture. Legacy benchmark remains 9/18.
Runtime, working port, shared manifests and installed game remain untouched; drafts stay disabled.

Guard Main now reaches a live piStack_13c decrement/cleanup condition before its first syntax failure
(around generated function-relative line 622). That case needs native ownership/resource evidence;
do not remove it using the constant-null rule. Other remaining families are native collection counts,
resource wrappers, aggregate subregisters, and cross-scope control-flow joins.


### Native speech vectors and collection return contract

`native_speech_vectors.py` recovers eight speech tables / 42 ordered keys from the native Init
construction export, checking its full source hash, 3306 native bytes, and agreement with reviewed
inventory offsets/keys. No port source supplies generated literals. The full Villager module emits
these immutable tables and records staticVectors evidence in its report. Known parent vector
(end-begin)>>2 expressions become exact lengths; vector-base reads become local table references;
CCharString assignment from base+index*4 becomes a 1-based Lua lookup whose result is tracked as a
string operand. This recovers all eight native selector call/count/base pairs and their shared read.
It does not claim the entire native Init body or all Villager Main logic has been converted.

`test_native_speech_vectors.py` executes every index through the actual eight native selector-call
pairs plus indexed string-copy expression, verifies all 42 selected lines, and compares table order
to the unchanged working-port speech module. Changed native bytes reject table recovery. Full source
hashes likewise reject changed construction evidence. Full recovery suite: 488/488 tests pass.
Conversion remains 36/51 functions and 6/18 files compiling, zero missing sources, 1587 TODOs.
Draft registration remains disabled. Runtime, working port, installed game and shared manifests
were read only during this pass.

Collection contract finding for the next WatchBarrels work: native GetAllThingsWithScriptName at
0x008A8570 returns a signed count and fills vector<CScriptThing>&. The host's LuaQuestState.cpp:836
returns only a dense Lua table, and skips failed heap allocations or rejected WrapScriptThingOutput
results. Its length is therefore not an unconditional proof of the raw native count. The existing
working port uses table length; record the compatibility limitation or expose raw count before
claiming exact equivalence for failure/filtered-wrapper cases. Current generated WatchBarrels still
incorrectly treats a Lua table as the native scalar result and also has native vector pointer/count
and cleanup operations. This finding is not an external blocker while other converter work remains.


### Villager conditional RNG, parent aliases, and Theresa wait conditions

The Villager Main conditional rand/modulo check is now recovered with exact raw-source,
annotated-source and native-body SHA256 witnesses. Native 0xDAE635 calls the retail rand thunk;
0xDAE63A..0xDAE642 computes/tests signed remainder 100. Both outgoing paths overwrite the raw
random result before reuse. The converter preserves the short-circuit draw inside the condition,
using RetailRandModulo(100). Tests execute draw/no-draw paths and reject changed source or bytes.

Entity parent-pointer aliases now resolve reviewed parent scalar fields and speech-vector bases.
Branch entry snapshots restore aliases for sibling alternatives; invalidations merge conservatively
at joins, including nested branches. Conditional aliases do not escape their defining scope, and
literal, unresolved-call, address and skipped-noise assignments invalidate stale aliases. This is
structured-flow tracking, not a proof for arbitrary native gotos or complete Main behavior.

native_conditions.py replaces the parenthesis-sensitive conditional-call regex with balanced
recognition of one outer short-circuit operator and a two-part call-assignment comma expression.
It preserves nested/quoted call arguments and rejects ambiguous multiple outer operators. The
actual Theresa Main animation-wait header now uses the existing controlled-resource dispatch.
Lua execution tests cover initially near, animation completion, repeated frame checks and arrival
after a frame. Theresa's next syntax failure is function-relative line 108 (native :: expression);
the whole Main is not yet usable. Native task-query call target is 0x007E7450, first at 0x00DB9954.

Validation: 507/507 full recovery tests pass; legacy benchmark remains 9/18 compiling files.
Regenerated full intro: 17 owners, 51 functions, zero missing bodies, 38/51 functions and 8/18
files compiling, 1583 TODOs. Compilation still does not establish behavior. Live resource cleanup,
collection count/vector semantics, string comparisons and unsupported control flow remain work.
Runtime, working port and installed game were not edited; generated registration remains disabled.


### Embedded termination queries in native conditions

native_termination_expressions.py recovers a reviewed member termination call immediately followed
by its extraout_AL zero/nonzero test inside a comma expression. Only this/param_1 receivers and
known termination names are accepted; quoted/commented text, unknown callees, other receivers and
intervening effects remain unchanged. Live mode emits a small Lua expression which updates alive
and returns the tested result, retaining the original short-circuit position and saved result.
Legacy mode uses the saved frame status as before.

The actual Theresa Main OR condition is covered by Lua execution tests: an already-terminated
left side skips the second query; a live left side queries once; the saved status reflects its
result. All matching native metadata calls resolve to 0x00F35B30. known_callee_aliases now rejects
labels associated with any unknown, missing, invalid or differently reviewed target, independent
of call order. This prevents trusting a similarity label merely because one occurrence is known.

Validation: 512/512 full recovery tests pass. Full conversion remains 38/51 functions and 8/18
files compiling, zero missing native bodies, 1583 TODOs; legacy benchmark remains 9/18. Theresa's
first syntax failure moves from function-relative line 108 to line 212, an inline literal byte
comparison. The existing literal-loop rule declines it because the pointer locals are reused
elsewhere; do not remove their updates without proving subsequent reads cannot observe them.
Generated drafts remain disabled; no runtime, installed-game or working-port files were edited.


### Literal comparison pointer lifetimes across native control flow

native_local_liveness.py supplies a conservative read-before-overwrite proof over normalized native
statements. It follows conditional/unconditional native jumps, branch alternatives, loop back edges,
break exits and returns. It rejects escaping local addresses, unknown control structures, missing
jump targets and unbalanced scopes. Extra branch paths may reject safe cases; they cannot justify
removing a potentially observed pointer value. Re-entering the comparison marker kills its old
pointer values because the recognized loop initializes both pointers before reading them.

The literal-loop folder can now use this proof when locals are reused elsewhere, while retaining
the original no-outside-use fallback. It still requires the exact ASCII byte-loop shape, distinct
locals, signed count and reads within each literal's terminating NUL. The actual Theresa comparison
of OBJECT_CHOCOLATE_BOX_UNGIVEABLE against the empty string folds to count 31 / equality false.
Adding a subsequent read of either pointer rejects the transformation. Tests also cover branch
bypasses, loops, back edges, conditional breaks, address escape and unknown control flow.

Validation: 519/519 recovery tests pass; legacy benchmark remains 9/18. Full output records
literalByteComparisons per function: one each in TeddyGirl Main and Theresa Main. Overall 38/51
functions and 8/18 files compile, zero missing bodies, 1580 TODOs. Theresa's next syntax failure is
function-relative line 258, a conditional CCharString__NotEqual assignment. Its string operand
recovery needs native evidence before lowering. Draft registration remains disabled; runtime,
installed game and working port were not edited.


### Presented-item comparison operands recovered from native instructions

native_presented_items.py recovers Theresa and TeddyGirl's argumentless CCharString__NotEqual
calls using pinned raw/annotated source, complete native function bytes, comparison callees and
retail string literals. At 0xDBA4D1 / 0xDAF729 slot 0x8c receives [esp+0x10] as the output string.
The following push supplies 0x12D8F24 (chocolate) / 0x12D8958 (teddy); ECX=[esp+0x14] then points to
the same output after the push. Calls at 0xDBA4E8 / 0xDAF740 target 0x99E960, which negates the exact
byte equality routine at 0x4115A0. Both callee byte ranges are included in the witnesses.

The recovered explicit CCharString comparison lowers inside the existing conditional-assignment
emitter. It keeps the saved boolean and native short-circuit path. Only this reviewed global-name
operand plus a literal is supported; arbitrary CCharString pointers remain unresolved. The host
contract inspected was D:/Code/ForgeFSE-retail-shadow/FableScriptExtender/LuaEntityAPI.cpp:648:
it returns bool and publishes g_PresentedItemName before returning true. The function text hash
(normalized Python read_text newlines) was 3a12cad01557628d8865fdf451b3396a510481d9b311d400d5419c4e3ce832b3.
There is no intervening call/yield before the generated comparison reads the global. This is the
inspected host contract; no second Lua return value was assumed or runtime binding changed.

Tests execute both actual recovered conditional headers for absent/present messages, matching,
other, case-different, empty and nil names. They verify saved results, exactly one message query,
and short-circuit protection from stale globals. Changed source, bytes or item literals reject.
Full recovery suite: 521/521 pass; legacy benchmark remains 9/18. Full conversion stays 38/51
functions and 8/18 files compiling, 1580 TODOs, zero missing bodies. presentedItemEvidence is now
recorded per function. TeddyGirl next fails at function-relative line 328 (repeat/end structure),
Theresa at 291 (aggregate/subregister expression). Neither Main is complete. Drafts remain disabled;
runtime, working port and installed game were read only.


### Preserve structural conditions during native-noise filtering

TeddyGirl's repeat/end failure came from dropping `if (unaff_EDI != (undefined4 *)0x0) {`
while retaining its closing brace. Generic substring noise filtering now preserves structural
statements and records unresolved native control operands. Guessed register names and destructor
operands alone do not prove that an enclosing branch can be erased. Existing explicitly recognized
reference-count block handling remains separate. Tests execute both alternatives and a conditional
loop, and require unknown native function conditions to remain visible diagnostics.

The new execution tests also exposed whitespace-sensitive null conversion: removing a pointer cast
left extra spaces before 0x0, bypassing the existing comparison normalization. It now accepts those
spaces, preserving nil-pointer branch behavior. Full recovery suite: 524/524 pass. Full intro still
38/51 functions and 8/18 files compile, zero missing bodies; TODOs rise to 1584 because four formerly
silently dropped conditions are reported. TeddyGirl's next syntax failure is function-relative
line 331, an unresolved distance-call argument. Legacy syntax remains 9/18; previously hidden
conditions also increase diagnostics in StatueMaster, BeardyBaldy and DragonBossFight. No runtime,
working-port or installed-game edits; generated registration remains disabled.


### TeddyGirl cached Bully distance operands

native_teddy_distance.py reconnects the initial NOVI_Bully lookup to TeddyGirl's 10-unit distance
check. At 0xDAF106..0xDAF110, slot 0x120 receives the script-name string at original ESP+0x10 and
an output wrapper at original ESP+0x28. At 0xDAFAB1 the distance helper receives that wrapper as
ESP+0x2c after pushing float 0x41200000; ECX is entity+8. Ghidra's uStack_14c operand association
was misleading and the old output lost both the entity and distance placement. A converter-owned
hidden output slot now connects the lookup result to the reviewed use. It does not replace the
later AffairWife lookup or infer other wrapper uses.

Recovery pins raw source, annotated source after presented-item recovery, full 5498 native bytes,
and the retail script-name literal. Tests execute both distance outcomes, checking exact entity
handles, one NOVI_Bully lookup and threshold 10. Changed source, bytes or strings reject recovery.
The report records teddyDistanceEvidence. All 526 recovery tests pass; legacy benchmark remains
9/18. Regenerated full intro now compiles 39/51 functions and 9/18 files, zero missing bodies,
1583 TODOs. TeddyGirl Main compiles but still contains unresolved behavior; this is not full Main
parity or authorization to enable it. Runtime, working port and installed game remain untouched;
generated registration stays disabled.


### Theresa unsigned three-byte condition

native_subregisters.py now lowers explicit Ghidra `(uint3)` casts on scalar atoms to a 24-bit
mask, preserving truncation before later shifts. It does not just erase the cast. Tests exercise
the actual `(char)((uint3)uVar30 >> 0x10) == 0` condition with zero, bit 16 set, high-only bits,
negative and full-width values. Calls, compound expressions, floats, indexed/member expressions,
postincrements and quoted/commented text are excluded. The native source assigns uVar30 as 0 or
0x10000 in this routine; its remaining byte-slice test now compiles.

Validation: 528/528 recovery tests pass; full intro stays 39/51 functions and 9/18 files compiling,
1583 TODOs, zero missing bodies. Theresa's next failure is function-relative line 599, a cached
marker operand in a distance call. Native evidence for the next pass: GetThingWithScriptName at
0xDB9844 fills original ESP+0x2c (LEA ESP+0x30 after the name push) for M_TriggerOutro. At 0xDBA3E8
retail pushes 0x40000000 (2.0), obtains the hero at 0xDBA3ED, and at 0xDBA3F3 addresses that same
wrapper as ESP+0x30; ECX=hero and EDX=wrapper feed 0xCBE2FF at 0xDBA3F9. This is the same missing
cached-output association family as TeddyGirl, suitable for a shared reviewed recovery rule.
No runtime, installed-game or working-port changes; generated registration remains disabled.


### Theresa cached outro marker recovered

The cached-distance recovery now shares its validation/replacement implementation between the
TeddyGirl and Theresa witnesses. Theresa's initial M_TriggerOutro lookup gets an explicit hidden
output slot; the reviewed distance use consumes that same result alongside the current hero and
fVar26=2.0. Native sites 0xDB9844 and 0xDBA3F9, complete 7010 function bytes, raw/annotated sources
and the string at 0x12D9930 are pinned. The source witness is after presented-item recovery, as for
TeddyGirl. The unrelated entity lookups and other distance checks are not inferred from this rule.

Execution tests check exact lookup/hero/distance order, handles, 2.0 threshold and both boolean
results; changed threshold source or unavailable native bytes rejects recovery. The report records
theresaDistanceEvidence. Full recovery suite: 530/530 tests pass; legacy benchmark remains 9/18.
Full intro now compiles 40/51 functions and 10/18 files, zero missing bodies, 1582 TODOs. Theresa
Main now compiles but remains behaviorally incomplete (including reused-temporary/aggregate and
resource cleanup issues). Compiling Main bodies must not be treated as gameplay-parity evidence.
Runtime, working port and installed game remain untouched; generated registration stays disabled.


### Invalidate stale operand values after unresolved writes

Lifter.forget_value centralizes invalidation of temporary values, operand ordering, receiver/parent
aliases, result kinds and staging facts. Unresolved stack writes, skipped noise writes, unknown
local-call assignments and unresolved address assignments now discard the previous known value.
Known address copies capture their referent before invalidation. This prevents a previously staged
chocolate string from surviving a later unlifted bitmask assignment and entering a bitwise test.
It does not claim to recover the missing assignment; subsequent operands remain unresolved.

Tests cover stale stack strings, unknown calls, ignored bitmask writes, receiver/kind invalidation
and preservation of a valid address copy. Full recovery suite: 534/534 pass. Regenerated intro stays
40/51 functions and 10/18 files compiling, zero missing bodies, with 1600 TODOs (formerly 1582):
invalidating stale values exposes additional missing operands rather than silently substituting old
ones. Legacy syntax remains 9/18. No runtime, working-port or installed-game edits; drafts disabled.

Viability assessment shared with user: native-to-Lua conversion is supported by real recoveries and
execution tests, but unattended whole-script parity is unproven. Remaining work includes replacing
function-specific recovery patches with stronger general control-flow/type/value tracking, native
resource lifetime handling, and readable output beyond decompiler locals and raw labels.


### Shared control-flow IR and reaching definitions

native_flow_ir.py extracts the existing conservative control-flow construction into immutable
NativeStatement/NativeFlowGraph records with source text, normalized code, statement identity and
successor edges. Native literal-pointer liveness now consumes this shared graph instead of keeping
a private graph implementation. Unsupported structural syntax rejects graph construction. The graph
explicitly reports exact=False: alternate-branch overapproximation remains and must not be mistaken
for a fully reconstructed executable CFG.

native_reaching_definitions.py adds fixed-point analysis of which scalar-local assignments can
reach each statement. It joins definition sets across branches and loop back edges, ignores
unreachable definitions and tracks conditional/compound/subregister writes. Unknown entry values
and address-escaped locals remain unknown. This analysis is not yet used to rewrite arbitrary
operands: expression typing, alias handling and precise control edges still need development.
Tests cover joins, loops, unreachable assignments, conditional definitions, escape and compound
writes. Full recovery suite: 540/540 pass; full output unchanged at 40/51 functions, 10/18 files,
zero missing bodies, 1600 TODOs.

Actual-native analysis artifact: work/script_converter_native_flow_analysis.json. Graphs accepted
CreatedBeetle Main (26 statements), TeddyGirl (821), Theresa (906) and Villager (431). Results record
ambiguous incoming facts and structural reachability, not behavior validation or completion. This
is the first shared analysis foundation for replacing source-order temporary guessing; existing
native-specific witnesses remain active. No runtime/port/game edits; drafts remain disabled.


### Typed scalar facts from reaching definitions

native_value_ir.py adds immutable typed scalar values and resolves literals/copies through all
incoming definitions. Copies consult their defining program point. Agreement is required across
all paths; cycles, unknown writes, escaped locals and differing values remain unknown. Bool,
integer and string types remain distinct. Native narrowing/casts and general arithmetic are not
yet folded here. Reaching definitions now conservatively treats locals mentioned in calls as
potential outputs until API effect signatures are supplied, and recognizes indexed/compound/
subregister mutations rather than trusting an earlier constant.

Tests cover agreeing/disagreeing branches, copy timing, types, calls, partial writes, escapes and
loop mutations. Full recovery suite: 544/544 pass. Work artifact:
work/script_converter_native_value_analysis.json. Actual raw-native analysis finds 21 known
incoming values in TeddyGirl, 18 in Theresa and 9 in Villager, zero in CreatedBeetle. No additional
literal copy substitutions were proven in this first pass. Those are incoming facts, not a count
of corrected instructions or behavioral coverage. No new Lua rewriting is enabled from these facts;
call read/write effects and more precise branch modeling are the next prerequisites. Full conversion
remains incomplete at 40/51 compiling functions and 1600 TODOs. Runtime, port and game unchanged;
drafts remain disabled.


### Correct alternative edges and scalar API input effects

Native flow construction now sends a completed if/else-if arm past all sibling alternatives.
The previous fall-through into else could erase a real true-path definition; it was not a sound
conservative approximation. Nested chains and unmatched alternatives are covered by tests, as is
the corresponding pointer-liveness failure where an else-only overwrite hid a true-path read.
The global exact flag remains false because other structural approximations still exist.

native_call_effects.py recognizes standalone annotated GSI calls with required scalar-only Quest
parameters/returns, matching arity and atomic operands. Their input locals are passed by value;
world effects and returned values remain unknown. NativeValues/reaching_definitions optionally
consume this classifier. Hidden outputs, optional parameters, pointers, nested expressions and
unknown calls fall back to conservative invalidation. Escaped locals stay unknown even with a
known scalar API. Tests verify timer-ID preservation independently from its unknown return value.

Actual annotated-native measurement: work/script_converter_api_effect_analysis.json. All four
sample Main graphs construct; incoming known facts are 0 CreatedBeetle, 21 TeddyGirl, 19 Theresa,
7 Villager with corrected branches. The initial scalar API classifier does not increase those
counts yet; richer entity/result/reference effects remain necessary. These facts are not newly
emitted Lua or behavioral parity. Full generated intro remains 40/51 functions, 10/18 files,
zero missing bodies, 1600 TODOs. Drafts remain disabled; runtime, port and installed game untouched.


### Symbolic API results and first IR-driven readable locals

ProducedValue records a call's result kind, producing statement and callee without inventing its
runtime value or replaying the call. NativeValues carries these facts through copies at their
original definition points. Distinct producers do not merge. Producers on graph cycles are rejected
because one static statement could represent multiple runtime instances. The initial result profile
covers scalar API results plus the reviewed direct-pointer, zero-argument GetHero binding.

native_readable_names.py is the first emitter consumer of the new analysis. It names RegisterTimer
and GetHero locals only when all uses retain that producer, with no reassignment or repeated producer.
The full converter enables it; legacy defaults remain unchanged. It avoids native/generated name
collisions and preserves quoted strings and comments. The conversion report records readableLocalNames.
Current actual output: CreatedBeetle Main uVar1 -> timerId; Bully Init uVar4 -> hero; Villager Init
uVar3 -> hero. Uncertain locals retain their native names. The symbolic-results work artifact records
four proven uses of CreatedBeetle's registered timer and no new symbolic facts in the three large
Main routines tested. Richer reference/call effects remain necessary there.

Validation: full suite 558/558 passed before adding the final focused emitter test; all four readable
name tests then passed, including Lua execution verifying timer registration result, set/remove calls
and return value after renaming. A no-candidate early exit avoids unnecessary analysis; regenerated
full output remains 40/51 functions, 10/18 files, zero missing bodies, 1600 TODOs. Full generation now
takes about 7 seconds in this run versus roughly 1-2 before analysis. No runtime, port or installed-game
edits; generated registration stays disabled. Completion and whole-script behavioral parity remain
unproven.


### Shared entity output-slot and const-reference effects

The call-effect profile now treats a supported explicit GetThingWithScriptName hidden return slot
and literal key as an entity definition. Its output address alone is not classified as an escape;
any other unrecognized address use still is. NativeValues resolves such definitions into ProducedValue
facts without requiring a textual assignment. Unsupported arity, missing contracts, variable keys
and unknown functions remain conservative.

A second profile recognizes the const-reference distance helper only when every occurrence of its
native label resolves to 0x00CBE2FF and the reviewed parameter contract agrees. Its input bindings
survive the call. The checked native_support/0x00CBE2FF.c shows validity/position queries and a squared
distance comparison over two const CScriptThing references. Conflicting targets or unknown names do
not receive this effect. This profile feeds both analysis and proven readable-local naming.

Tests exercise both entire actual recovered TeddyGirl/Theresa native bodies: the shared analysis now
connects local_native_cached_bully and local_native_outro_marker from their original hidden-output
producer to the distance use inside the loop. Unknown effects correctly lose the loop fact. Additional
tests cover escape and unsupported output forms. Full suite: 564/564 pass. Regenerated intro remains
40/51 functions and 10/18 files compiling, zero missing bodies, 1600 TODOs. This improves general
provenance tracking after operand recovery, not yet elimination of the native operand witnesses.
No runtime, working-port or installed-game changes; generated registration remains disabled.


### Native call-setup IR and automatic instruction windows

native_call_setup_ir.py symbolically evaluates straight-line x86 MOV/LEA/PUSH argument setup,
recording ECX, EDX, call target and stack arguments. Values include constants, memory/address
expressions, window-relative stack addresses and explicitly profiled intervening-call results.
Unknown calls/control transfers, unsupported stack mutations and memory writes reject recovery;
unmodeled partial-register writes invalidate the whole register fact. Stack offsets are relative
to window entry, not automatically globally comparable native frame locations.

Automatic window discovery decodes from the aligned function entry, stops at prior calls/branches,
and respects incoming branch targets so it cannot use skipped setup instructions. Both cached-distance
rules now consume this shared decoder: lookup output address must agree with distance EDX, the target
must be 0xCBE2FF, and the pushed float must match the reviewed threshold. The report includes decoded
lookupSetup/distanceSetup snapshots. Manually supplied window boundaries were removed from witnesses.
Theresa's intervening GetHero call still has an explicit reviewed zero-cleanup/result profile; call
identity and relationships between window stack baselines are not yet inferred generally.

Metadata finding: TU size 5498 for TeddyGirl stops inside the final ADD ESP; actual stack-restore/RET
ends at 0xDB0600. Theresa's 7010 likewise ends inside ADD ESP; the tail ends at 0xDBB305. Earlier
'complete function bytes' wording for these pinned ranges was too strong: they omit 6 and 3 tail
bytes respectively. Decoder extension accepts only observed ADD ESP immediate plus RET, rejecting
arbitrary undecoded tails that could hide incoming branches. Existing source/call-region evidence
remains unchanged. Capstone is now used by production cached-argument validation as well as audits.

Validation: 570/570 full recovery tests pass, including actual native windows, automatic boundaries,
incoming-jump bypasses, partial registers and rejection cases. Full generation remains 40/51 functions
and 10/18 files compiling, zero missing bodies, 1600 TODOs. This moves operand verification into reusable
instruction analysis but does not prove whole-script parity or eliminate all per-function witnesses.
Runtime, working port and installed game remain untouched; generated registration remains disabled.


### Post-attack availability wait cleanup

`native_post_attack_cleanup.py` removes only the reviewed temporary thing-wrapper
bookkeeping from the opening `PostAttackStuff` loop. The witness pins the original
and annotated source, all 1095 native function bytes, and the base-vtable restoration
helper. Aligned native decoding establishes the hidden lookup output at ESP+0x10,
its reference-info pointer at ESP+0x18, and the IsAlive result controlling the wait.
String cleanup, frame yields, termination checks, and the loop backedge are retained.
The existing nil-safe IsAlive emitter lets missing lookups wait instead of crashing.

The report records `postAttackCleanupEvidence`, including an explicit unresolved
native zero-reference destructor limitation. This is script-control recovery, not
proof that Forge garbage collection reproduces retail wrapper destruction. Tests
execute the actual recovered loop for immediate availability, missing/dead retries,
and cancellation, and verify changed evidence rejects cleanup removal. Later
PostAttackStuff statements still prevent the whole function from compiling.


### Post-attack scalar operands

`native_post_attack_scalars.py` validates the three reviewed music/camera calls
with shared `read_call_window` analysis: receiver is the script interface at
ESI+0x40, the indirect target is the expected vtable slot, and the sole stack
argument is an independently decoded constant. Native call sites DBEBEB, DBECE4,
and DBECFC supply CacheMusicSet(45), CameraResetToViewBehindHero(0.0), and
CacheMusicSet(57), respectively. The camera operand is decoded as float32;
music operands use signed int32. API contracts, native vtable entries, source
correspondence, and full function bytes are checked before all three replacements.
The rule runs after availability cleanup; changed upstream source fails closed.
`postAttackScalarEvidence` includes decoded setups and typed values. These calls
remove three missing-argument TODOs but do not prove the full post-attack sequence.


### Numeric post-attack logbook call

The BSim name CSubtitleRenderer::SetText at CBE87F is not a usable prototype.
The reviewed native body converts its ECX integer into a story text key and the
runtime exposes it as AddLogbookStoryEntry(int). `native_post_attack_logbook.py`
pins caller/source and all 225 callee bytes, checks the numeric/string manifest
contract, and uses call-window IR to require direct target CBE87F, ECX=20, and
zero ABI stack arguments at DBECC1. The emitted call is AddLogbookStoryEntry(20);
`postAttackLogbookEvidence` records the rejected prototype and decoded setup.

Argument placement now accepts explicit integer literals for a single int|string
parameter before string classification, including signed int32 hex conversion.
This prevents an unrelated string temporary from replacing the numeric overload's
argument. Existing string overload placement remains covered. No broad inference
from the misnamed helper's decompiled parameter list is introduced.


### Post-attack teleport operand recovery

`native_post_attack_teleport.py` replaces the reviewed pointer-staging block with
lookup, GetHero, and EntityTeleportToThing(hero,target,false). The witness pins
caller/source plus full native lookup/GetHero/teleport wrappers. Their reviewed
ABIs justify lookup stack cleanup of 8 bytes, GetHero cleanup of zero, and preserved
callee-saved registers. Shared call-window IR separately verifies both intervening
call targets/receivers, then verifies the final stack contains hero result, lookup
result, and zero in that order. The lookup string and vtable entries are checked.
Native cleanup following teleport remains unchanged by this rule.

The converter signature overlay now includes the runtime optional third teleport
bool; old two-thing manifests gain that optional parameter without editing the
shared manifest. Lua tests check lookup-before-hero evaluation, target identity,
and explicit false. Changed native bodies, source, dependency targets or swapped
operands reject recovery. This removes the invalid pointer dereference and SUB41
staging expressions but does not establish complete post-attack behavior.


### Repeated cached-distance checks

The shared cached-distance recovery accepts multiple reviewed native checks for
one cached lookup. Existing one-check witnesses retain their behavior. Each check
must match the cached wrapper address, direct distance target, and float threshold;
post-attack checks additionally require ECX to contain the profiled GetHero result.
All checks pass before either source occurrence is replaced. Reports preserve the
original distanceSetup field and add distanceSetups for the complete checked list.

The post-attack witness covers lookup DBED2C and checks DBED51/DBED8D with GetHero
DBED45/DBED81, common wrapper window-baseline ESP+28, and threshold5.0. Source
replacement caches MK_OVI_DADTRIGGER once and uses it in both checks. Tests execute
the actual recovered wait, verifying current hero on each query, cached target,
frame retries, and cancellation without another query. Mutating either check's
wrapper, hero or distance rejects the entire recovery. Window-baseline equality
remains a reviewed per-function relationship, not general global frame inference.

PostAttackStuff now passes Lua syntax, but later resource/control/cutscene operations
remain unresolved. Syntax acceptance is not whole-function runtime or retail parity.


### Post-attack movie flags and control limitation

The scalar-call recovery is shared between initial music/camera arguments and
post-attack movie flags. The latter witness runs after distance recovery and pins
three calls: DBEE47 PauseAllNonScriptedEntities(true), DBEE54
FixMovieSequenceCamera(true), DBEE8F FixMovieSequenceCamera(false). Each receiver,
vtable target and pushed word is decoded and checked before any source changes.
Boolean manifest types drive Lua true/false emission; stale fVar11 no longer
supplies the pause flag. The final pause(false) call is not covered by this witness.

Control is still unresolved: native DBEDC8 uses hero, resource at baselineESP+28,
and priority4. Native slot20 (89B5B0) can yield internally at89B684, but also returns
false on failed-priority/termination paths. PostAttackStuff does not branch on its
return. Runtime AcquireControl retries failed native acquisition; substituting it
and assuming identical failure behavior is unproven. Generated me:AcquireControl
is also the wrong receiver in this quest function. Neither syntax acceptance nor
the recovered movie flags resolves resource ownership or this control mismatch.


### Post-attack village limbo transitions

`native_post_attack_limbo.py` recovers both SetVillageLimbo calls from their staged
lookup result and native flag. Call-window IR proves entry DBEC6E uses lookup
DBEC64 with true, and exit DBEEE7 uses lookup DBEEDD with false. The native lookup
body/RET8, interface vtable targets, village string, source and caller bytes are
checked. Replacements name the corresponding decompiler result locals explicitly;
the exit performs a fresh lookup. Existing wrapper cleanup is retained.

Lua tests execute both recovered lookup/call sequences with distinct returned
objects, asserting true/false and actor identity. Changed result or flag evidence
rejects both replacements. The report includes postAttackLimboEvidence for each
transition. This resolves the exit missing flag, not cutscene/resource parity.


### Implicit entity receiver requires an entity execution context

The interface emitter no longer invents me for StartScriptingEntity or Entity-scope
bindings in a quest execution context. These calls now produce an unresolved
receiver/resource TODO and do not count as converted API calls. Assigned results
are explicitly unresolved nil values, not invented success/failure booleans.
Entity threads and shared helpers with execution_entity=True retain their explicit
me parameter; explicit typed thing calls use their separate receiver-aware path.

This removes the phantom me:AcquireControl(4) from PostAttackStuff. It does not
replace the native resource with a nearby GetHero result or claim the runtime's
retry behavior matches native control. The missing resource conversion remains
visible and must be solved before the disabled draft is ready for use. Tests cover
quest calls with a recent hero, unresolved result assignment, an entity-context
shared helper, and the actual PostAttackStuff input. Existing entity retry-loop
and typed thing-call tests remain in place.


### Post-attack resource relationship report

`native_post_attack_resources.py` maps ten native resource events through the
shared call-window decoder: acquisition, actor-map construction/entry/assignment,
movie construction/start, macro invocation, and movie/map/actor destruction.
Caller/source and profiled GetHero/map-entry callees are pinned. Actor key HERO
and macro CS_OAKVALEINTRO_HESDEADJIM are checked against native strings.

postAttackResourceEvidence records status=mapped and loweringStatus=unresolved.
Its resource relationships connect the priority4 hero handle (window offset40)
to the actor-map entry, the map (offset16) to macro EDX, and the movie (offset56)
to StartMovieSequence and its destructor. Macro stack flags are null flag map,
null input map, setup=false, skippable=true. Offsets are decimal and cross-window
relationships are reviewed for this pinned function, not general frame inference.
Tests check those relationships and reject every mutated event target, profiled
callee body, or native string. This report does not remove ownership TODOs or
claim Lua equivalence. Runtime RunCutsceneWithSetup reuses caller movie handles
but performs additional actor acquisition, termination checks and cleanup.


### Memory stores in call-setup analysis

With an explicit ABI argument count, the instruction decoder can continue past
scalar MOV memory stores by discarding all staged stack contents and every
memory-derived register fact. It preserves only independent register identities,
constants, results and computed addresses. It does not infer what was written or
assume a store cannot alias an argument. A missing argument still rejects at the
final call, and fresh pushes after a store can establish new arguments. Without
an explicit ABI count the previous store rejection remains. Implicit memory
writers, exchanges and unsupported stores remain rejected.

This recovers DBEDAD's actor constructor ECX=stack40 despite the preceding
DadFound field write. The post-attack resource report now includes eleven events,
linking actor construction to acquisition, map assignment and destruction. Tests
cover byte/word/dword writes invalidating arguments, nested memory-derived address
invalidation, independent fresh pushes, and the actual native constructor window.


### Lowering the post-attack movie lifetime

`native_post_attack_movie_lifetime.py` consumes the verified resource graph to
remove the movie constructor placeholder and emit EndMovieSequence at the exact
native movie-destructor position, before actor-map/hero-resource cleanup. The
native constructor6E7B60 and destructor6E7B80 bodies are pinned separately. Runtime
StartMovieSequence constructs with99A380/vtable1260EF4 and an empty class; its
EndMovieSequence invokes the same6E7B80 derived destructor. Both bindings are
checked against the manifest and the empty native string is checked.

The rule requires construction/start/destruction on the same movie slot56decimal
and ordered cleanup. Actor construction and unresolved actor/map/macro statements
remain. Report postAttackMovieLifetimeEvidence identifies the recovered movie
only; whole resource graph still has loweringStatus unresolved. Tests check one
start/end in Lua, native source cleanup order, preserved actor constructor, and
rejection on helper changes, wrong resource or reordered cleanup. These tests do
not assert engine behavior for allocation/API failures or whole cutscene parity.


### Literal pointers must remain within file-backed section bounds

RData.string_at now uses raw section sizes and requires the NUL terminator within
the same raw section and file bounds. Previously virtual section padding could
be interpreted as an offset into unrelated file bytes, and scans could cross a
section boundary. The512-byte scan limit, printableASCII restriction, and empty
string behavior are retained. Tests include a synthetic virtual-padding address
that formerly returned a fake literal and a terminator just beyond raw bounds.

The native GetPos fallback143E8E0 is inside the data section virtual tail but
outside all raw section data. RData.bytes_at/string_at therefore provide no runtime
value for it. This distinction is now tested against the retail image; it does
not prove the fallback vector's runtime contents or settle the empty-table mismatch.


### Checked indirect jump tables in argument windows

The call-window decoder now recognizes contiguous CMP index,max; JA default;
JMP [index*4+table] dispatches with a32-bit index and at most1024 entries. Table
bytes must be present outside decoded instructions, and every entry must target
a decoded instruction boundary. A direct or table branch into JA/JMP bypassing
the CMP rejects the function. All enumerated case targets participate in window
trimming, so argument setup skipped by a case cannot be borrowed. Other indirect
branches remain unsupported and reject analysis.

BarrelMan's DB551D CMP eax,3 / DB5520 JA / DB5526 JMP tableDB6B24 meets these
requirements. Its first lookup DB55E2 can now be analyzed by shared call-window
IR: hidden wrapper stack172, string stack160. Previously any indirect jump in
that function rejected even this straight-line lookup. Tests cover skipped setup,
guard bypass, signed condition, invalid/missing entries, and the real native case.
This enables operand recovery; it does not resolve the position fallback or emit
new Lua for the barrel-man position queries by itself.


### Position ownership evidence and instruction-boundary checks

Call-window analysis now rejects direct branches into instruction interiors within
the decoded function, including an extended epilogue. Such branches would invalidate
the switch guard and incoming-edge model. Tests cover entries into CMP, JA, table
JMP and argument-setup instruction interiors.

`native_barrel_position.py` adds barrelPositionEvidence for the first BarrelMan
inlined GetPos. The source/caller and lookup/string-cleanup/GetPos wrappers are
pinned. Shared analysis checks lookup slot120, interface receiver, and hidden
output stack172; the reviewed following implementation load is stack176=wrapper+4.
Native lookup string and binding are verified. The report records status mapped,
loweringStatus unresolved, fallback address143E8E0, and remaining vector consumers.
Only this first query is mapped; later queries cannot inherit its actor by proximity.

### BookTrader intermittent timer

`native_book_trader_timer.py` recovers the shared parent timer ID at +0x104 and
reset value 3 after BookTrader's pause and random passes. Both native call setups,
source and caller hashes, and API contracts must match. The generated calls use
`quest:GetStateInt("TalkIntermittentTimer")`; the report includes
`bookTraderTimerEvidence`. Shared call-setup analysis supports immediate 32-bit
register ADD/SUB for these address calculations, with constant wraparound and
unknown-value propagation. Lua tests verify the timer ID and reset value;
conversation arguments and resource cleanup still need separate recovery.

`native_book_trader_line.py` recovers the intermittent sales line after the timer
pass. At DB4E55, native arguments are conversation ID, text, false, self, hero;
the host API puts the flag last. The recovery explicitly names the hero result
and supplies self as speaker, correcting the previous hero-speaker/missing-listener
output. Caller/source hashes, decoded five-argument setup, native vtable binding
and host manifest contract gate this rewrite. `bookTraderLineEvidence` records it.

`native_book_trader_health.py` recovers seven health-query actors through the
reviewed control resource at stack20. All eight native acquisitions target self;
each GetScriptThing result is checked through its GetHealth consumer. Caller,
getter and binding bytes plus source and API contracts gate the rewrite to
`GetHealth(me)`. Wrapper ownership/cleanup remains unresolved and is retained.
The report records `bookTraderHealthEvidence` separately from cleanup readiness.

`native_book_trader_allies.py` restores both directed ally calls: self/hero and
hero/self. The second native call stages self before GetHero; checked call setup
preserves it across that getter. Source recovery retains two getter evaluations
and the second result assignment. `bookTraderAlliesEvidence` records caller/source
hashes and the binding/contract checks. Lua trace tests verify call order and
both actor positions.

The movie lifetime checker can track reviewed ECX resource selections across CFG
joins. Other receiver writes (including partial writes) and intervening calls
invalidate the selection. This supports BookTrader's shared destructor sites,
which can receive either stack152 or stack44. The BookTrader movie witness pins
the complete 4042-byte body, both starts/ends and five receiver-selection sites;
native paths balance one movie handle. `recover_book_trader_movies` shares the
AffairMan lowering machinery and records `bookTraderMovieEvidence`. Only the two
verified movie destructors become EndMovieSequence; misnamed actor destructors
and unresolved cleanup jumps remain separate work.

`native_book_trader_cleanup.py` expands 21 reviewed terminal jumps into their
unpause/movie-end effects followed by the existing actor-cleanup label. Native
suffix traversal verifies the effect order and terminal destination; source and
caller hashes restrict the transformation. Conditional jumps become guarded
blocks, so skipped branches have no cleanup effects. Lua tests exercise every
expanded jump. Actor resource destruction remains unresolved, as do normal
continuation jumps outside these terminal suffixes.

The cleanup recovery also handles DB4BC5 (terminal unpause/movie-end) and DB4CE1
(normal movie-end followed by the DB4CE6 timer check). There are now 24 expanded
jumps. Native traversal checks a separate destination for each suffix; the normal
path resumes the script rather than returning through actor cleanup.

`native_book_trader_hits.py` recovers ability ID14 and the nested hit-classification
branch. The result is direct hero hit OR (any hero special hit AND NOT ability14),
with queries evaluated only on the native paths. Source recovery preserves each
conditional string constructor and its cleanup-mask assignments. Lua tests cover
all eight predicate combinations, call order, the final query result and masks.
The reviewed native/source hashes and host contracts gate this rewrite.

`native_book_trader_termination.py` captures two loop-carried termination results
whose decompiled assignments are separated from the call by a pure scalar copy.
This prevents undefined extraout_AL globals from controlling loop entry and
continuation. Native/source hashes gate the recovery; Lua traces cover immediate
termination and termination after one or two iterations without extra queries.

`map_book_trader_resources` records 18 checked call setups for the single stack20
control resource: eight acquisitions, four presence checks, four resets and two
destructor calls. The mapping stays unresolved for lowering. Native acquisition
failure can copy a fallback resource before returning false; resource presence
therefore cannot be represented by the acquire-result boolean. Shared destructor
receiver selections and inline reference-counted exit cleanup are documented in
the witness. The report exposes `bookTraderResourceEvidence`.

`native_book_trader_acquisition.py` uses that checked map to restore priority3
for BookTrader's initial acquisition and retry; later acquisitions remain4.
The original frame/termination/cleanup source between attempts is preserved.
Resource identity and runtime retry scheduling remain unresolved independently
of these operand corrections; `bookTraderAcquisitionEvidence` states that limit.

Quest Init's reviewed speech-vector construction is now materialized as the same
eight immutable Lua tables used by Villager consumers. `recover_init_vector_construction`
checks both source versions, native bytes and all42ordered keys before replacing
the C++ allocation/copy tail. The quest initialization prefix remains unchanged.
The caller must supply and emit the matching static tables; absent/reordered
tables reject recovery. This is the existing immutable speech-data model, not a
general mutable-vector or repeated-append implementation. Init now compiles;
its other unresolved operations still require recovery.

`native_init_scalars.py` restores Init's TalkIntermittentTimer operand and signed
GUIBullyHealthCounter sentinel -999. Native caller/source hashes, decoded timer
setup, API contract and typed state mappings gate both edits. Call-setup IR now
recognizes full-register XOR zeroing; partial writes still invalidate the whole
register, and ESP zeroing rejects the window. Lua traces verify timer ID/value
and signed state write in native order.

`recover_init_bad_deeds` expands Init's packed four-byte and one-byte zero stores
at FC..100 into five indexed WhichBadDeedsPerformed bool writes. These use the
same keys as AddBadDeed/Guard consumers, replacing the unrelated self_FC/self_100
keys. Source/native hashes and the typed array mapping gate the rewrite. Lua tests
verify all five flags reset and neighboring state is unaffected.

Run `python -m tools.script_recovery.audit_generated_init` to compare the complete
generated Init's scalar effects against decoded native writes before speech
construction. The audit tracks native zero registers, typed stores (including
the packed bool array) and the timer call, then executes freshly lifted Lua with
nondefault state. All30state writes and one timer reset must match in order.
This does not establish mutable speech-vector or Init reentry equivalence.

`native_affair_wife_route.py` corrects two actor operands around the first husband
position query. Native DB345C enables movement on self; DB3491 clears self's
information flag. Explicit operands prevent staged lookup results from displacing
self. Source/caller/binding hashes and decoded call setups gate both edits.
The position query, null fallback and movement command remain unresolved.

`recover_affair_wife_distance` connects the husband lookup at DB3445 to both
native distance calls DB34A2/DB3586 through their shared wrapper at stack48.
The existing cached-distance recovery emits explicit self, cached husband and
3.0 operands. Lua traces verify one lookup and the conditional repeated check;
an altered cached slot rejects both edits. Other route position/movement operands
remain unresolved.

`native_hero_distance.py` recovers Wife's nested GetHero distance operands at
DB32FB (5.0) and DB35E4 (15.0). The latter threshold was incorrectly attached to
GetHero by the decompiler. Caller/callee/vtable and source hashes plus decoded
call setups gate the correction. Each original hero query remains in place;
Lua traces verify distinct hero results and thresholds despite a stale float
temporary. Changed native threshold or actor rejects the entire recovery.

`recover_affair_wife_hits` shares BookTrader's reviewed hit recovery machinery.
The native branches test ordinary hero hit, then any hero special ability, then
exclude ability14. It preserves short-circuit order and conditional temporary
cleanup masks. The stack-byte result becomes an explicit boolean local, including
its later branch use. Generated native argument locals now participate in mutable
scalar detection, preventing branch assignments from being folded to the last
literal. Tests cover all eight predicate combinations, query order, masks and
changed native ability rejection. This does not recover the hit movie lifetime.

`recover_affair_wife_pause` uses the shared saved-interface recovery with a
separate native/source witness. Four native sites pause and thirteen unpause;
missing arguments and indirect saved-interface calls become explicit booleans.
The second scene saves its interface at stack36 before reusing EBP. Its shared
cleanup receives either that saved interface or EBP from an earlier branch.
Tests verify all17 emitted calls, ignore unrelated staged actors, and reject
changed saved-interface operands. Movie lifetime and cleanup jumps remain separate
unresolved work.

`recover_affair_wife_movies` checks the full4890byte native body through its final
RET. Four starts use resources164/64/64/116 and seven ends close the active movie;
the CFG checker proves no overlapping lifetimes and matching cleanup on exits.
Shared ends DB32A5/DB3E11 use five reviewed predecessor receiver selections.
Only the seven actual movie destructors become EndMovieSequence. Misnamed actor
destructors are preserved. Source cleanup gotos still need separate recovery.

`recover_affair_wife_cleanup` expands19 reviewed movie-cleanup jumps after the
pause/movie passes. Native suffix decoding checks call order and destination:
normal completion reaches32AA, terminal scenes reachactorcleanup3E16, and the
later scene reachesactorcleanup3D6A. Every source condition remains in place.
Lua tests execute each expansion with true/false guards and check ordered effects
and destination. Actor cleanup and its remaining unsupported jumps are unresolved.

`recover_affair_wife_argument_hits` repairs the later hit-classification branch
DB36FE/DB3725/DB3751. Ability14 was already present, but a sibling goto became
an early return in Lua. The structured result preserves all eight predicate
combinations and short-circuit query order. Saved-register aliases in the adjacent
temporary-cleanup mask remain explicitly unresolved.

`recover_affair_wife_mask` follows the argument-hit recovery and removes spurious
EBP mask aliases. Native EBX carries the mask through conditional clears4/2/1;
the recovered copy uses the current uVar14 directly and retains destructor bodies.
Repeated Lua checks verify upper-bit preservation and clearing of all three
temporary bits. Since the first query always setsbit1, the prior alias did not
prove a final-mask corruption; this recovery removes unresolved register plumbing.

`recover_affair_wife_facing` restores allthree facing calls to self and the cached
husband, with native snap flagsfalse/true/true. Caller/callee/vtable hashes and
decoded call setups gate the rewrite. A converter-local signature correction adds
the runtime's optional snap boolean, preserving newer contracts and leaving the
shared SDK manifest intact. Tests verify cached-target reuse and explicit flags.

`recover_affair_wife_allies` shares the reviewed ally-call recovery with
BookTrader. Both Wife hit branches set self ally-of-hero, then hero ally-of-self,
with separate GetHero evaluations. Four decoded setups and source/native/binding
checks gate the edits. Lua tests use four distinct hero results and preserve the
later pair's uVar11 result. Source branch placement remains unchanged.

`map_affair_wife_resources` checks26 native operations on control resource16:
ten acquisitions, five presence checks, five resets and six destructors.
`recover_affair_wife_acquisition` uses this evidence to emit explicit self/resource
operands and priorities3/3 followed by eight4s. Tests preserve surrounding source
and verify retries in all five pairs. Presence is a native handle check, not the
acquisition result; runtime scheduling and release equivalence remain unresolved.

`recover_affair_wife_termination` captures the loop-entry and backedge queries at
DB2B8C/DB339E into cVar5. Pure mask assignments had separated these calls from
their decompiler AL result aliases. The recovery retains the mask assignments,
checks native caller/callee and source hashes, and preserves one query per site.
Lua tests cover immediate termination and repeated live iterations before exit.

Wife's resource evidence also covers14 task-status queries at7E7450, all on
resource16. The native helper returnsfalse for a null implementation and otherwise
dispatches slot68. Existing Lua self-query calls are retained; tests verify all14
source sites keep self even with a staged hero, and preserve true/false results.
The resource map now contains40 checked operations. Handle-lifetime equivalence
remains outside this operand audit.

Wife's timer recovery uses the same parent TalkIntermittentTimer as BookTrader:
DB32BD reads it, DB332B resets it to3. The following reviewed random recovery
maps DB32D2 to RetailRandModulo(500), retaining the timer-zero and forced-first
speech short-circuits. Tests cover all500 residues across both guards and verify
a nonzero stored timer ID is used for read/reset.

`recover_affair_wife_conversation` restores the Where's Husband conversation:
create with self,false,false, add a fresh hero participant, then speak the reviewed
text key from self to another fresh hero result. Source/native/vtable/contract
checks and decoded participant/line setups gate the rewrite. Lua traces verify
the conversation ID, flags, ordered queries and listener. Later conversations
remain outside this recovery.

The running-to-husband line is recovered separately: create(self,false,false)
and speak from self to an absent listener. Native constructs an empty CScriptThing
wrapper; the runtime maps Lua nil listeners to that ABI value. Explicit typed
`(CScriptThing *)0x0` operands now participate in actor argument placement without
being filled from staged actors. Plain numeric zero is not reclassified. Tests
verify the absent listener despite an unrelated hero result staged beforehand.

The argument-conversation recovery gives native stack132 a dedicated saved ID,
initialized tozero and updated from AddNewConversation. IsConversationActive uses
that ID; creation uses self,false,false and adds the cached husband. Tests verify
ID reuse while active and replacement after completion. Numbered dialogue text
construction is recovered by the following bank-specific pass.

`recover_affair_wife_text_sequence` specializes the numbered argument lines for
the pinned installed English text bank. Its index contains10/20/30/40 and no50;
the independent native counter starts0, advances10, and resets10 on missing text.
Lua constructs the key from that counter, preserving the missing-key termination
check and wife-to-cached-husband line operands. The bank SHA gates this conversion;
changed or unavailable banks reject it. Tests cover repeated wraps and cancellation
at the missing-key boundary.

`recover_affair_wife_reply` recovers the optional husband reply after the numbered
wife line. The pinned native body proves signed remainder modulo 2, a termination
check only on the zero edge, and husband-to-wife dialogue operands. It uses the
host retail random operation; Lua's random generator is not involved. Only native
string temporaries are removed; the existing actor-cleanup destination is retained.
Tests execute emitted Lua for both remainder outcomes and cancellation, including
negative signed remainders, and reject changed native listener evidence.

The shared health-operand recovery also covers AffairWife: six queries now read
self health, with the native float32 zero threshold recovered from pinned data.
It verifies all ten self acquisitions of resource16 and each resource-wrapper
getter/health-consumer pair. Tests cover positive, zero, and negative health while
an unrelated hero is staged, plus rejection of changed getter/consumer operands.
Native wrapper ownership remains unresolved.

`recover_affair_wife_animation_random` replaces the final raw Wife `rand()` with
the retail modulo-2 operation, preserving the two animation branches verbatim.
Tests compare emitted Lua selection against the native signed-mask/sign-fix
instructions, including negative values and integer boundaries. Full animation
flag recovery remains separate, particularly the mutable `DAT_01375748` operand.

The `PlayAnimation` signature overlay now matches `LuaManager`'s entity binding
to `PlayAnimation_NonBlocking`: animation name plus seven optional flags, without
waiting in the wrapper. It replaces only the recognized legacy three-flag
signature and leaves newer/custom entries intact. A regression checks all 128
flag combinations through emitted Lua. This preserves supplied operands; it does
not infer omitted native flags or resolve mutable native globals.

The subsequent Wife animation-operand pass pins the caller and native forwarding
wrapper, recovering both animation keys and all seven flags. The fifth flag stays
`DAT_01375748`: its installed value is one, but writable storage is insufficient
evidence for constant folding. Regression tests supply both boolean values and
verify all arguments. Runtime access to that global and resource ownership remain
explicitly unresolved in the conversion evidence.

`affairWifePositionEvidence` maps the husband's implementation-present and absent
position branches plus the movement ABI. The current host `GetPos`/`MoveToPosition`
composition returns an empty table for an absent implementation, then substitutes
zero coordinates. Retail instead reads a global fallback vector at `0143E8E0`.
Its runtime value is unproven, so this mapping does not enable position lowering.

The converter supplies the missing `ClearAllActions` entity signature for the
existing runtime binding. It is distinct from `ClearCommands`. A Wife regression
verifies both native resource16 receivers, the separate forwarding slots, and
emitted Lua call order despite decompiler-invented extra arguments.

AffairMan also uses the shared health-operand recovery. Its witness verifies six
self acquisitions of resource16 and eight hidden-wrapper health consumers.
The generated queries explicitly use the husband; distance-related wrapper
queries and native ownership remain separate. Tests execute all eight health
statements with an unrelated actor staged beforehand.

The husband's eight health thresholds are recovered as float32 zero from pinned
read-only data. Regression extraction preserves seven positive-health predicates
and the separate nonpositive-health predicate; native wrapper cleanup between
the query and branch remains outside that isolated test's claim.

The husband's hit predicate uses the shared native hit recovery: ordinary hero
hit OR (any special ability AND NOT ability14). Recovery preserves short-circuit
query order and cleanup masks, names the saved boolean result, and removes the
misstructured success-label jump. Tests execute all predicate combinations and
reject altered native ability evidence.

The husband's cleanup mask now has a dedicated scalar local. Recovery changes
only its six stores and loop-back load, preserving the unrelated address-taken
dialogue expression with the same decompiler stack name. Tests verify that clearing
bits1/2/4 preserves upper bits and restores the saved value after an intervening
overwrite of the working variable.

Both husband ambient-conversation creation paths now explicitly use self with
false/false flags and add the cached affair woman using the returned ID. Tests
execute both recovered source fragments with an unrelated hero staged first.
The persistent ID is initialized to zero, updated by both creation paths, and
used explicitly by the active-conversation query. Numbered dialogue-line
construction remains unresolved.

The close-range husband/woman dialogue pass builds both numbered keys from the
native BadgerIndex. At10 the woman speaks first; at20–50 the man speaks first.
Both calls retain the conversation ID, opposing listeners, and false subtitle
flag. Branch-local cancellation is preserved before either line. The existing
counter/wrap logic remains outside this pass; ranged lines and actor cleanup
remain unresolved.

The subsequent ranged-line pass recovers the distinct man/woman text prefixes,
always queues the man first, and removes the shared string-cleanup jump. Both
dialogue modes now construct numbered keys; actor cleanup and the surrounding
distance/counter behavior still require broader verification.

The dialogue-sequence regression now runs actual recovered creation/counter/line
fragments across 12 alternating near/ranged exchanges, including two wraps. It
compares their line IDs, keys, speakers and listeners with the working port's
actual Lua helpers. Separate cancellation cases verify that a wrap interruption
leaves the counter at60 after conversation creation and queues no lines. Distance
gates, parent-state side effects and resource lifetime are outside this comparison.

The affection-setup operand pass restores the float32 0.4-second pause and both
snap-facing calls (man toward cached woman, then woman toward man). It matches
the whole consecutive call block to avoid changing an unrelated unresolved
facing call elsewhere. Tests verify float bits, actor order and snap flags.

The same pass separately recovers the cowering branch's facing call: self toward
cached wife with snap=false, before the retail modulo50 check. Its distinct
source context and native stack44 operand prevent confusion with the woman-facing
calls. Tests retain the facing-before-random order with an unrelated hero staged.

The earlier conversation-facing call also explicitly uses cached woman toward
the freshly fetched hero, snap=false. Its original hero lookup remains in place;
the regression checks actor order and exactly one lookup.

The husband animation-operand pass recovers all seven flags for cowering, kiss
and hug. Kiss/hug set the second flag; cowering clears it. All retain the dynamic
fifth flag and explicit final false. Tests cover both supplied dynamic values and
reject changed native final-flag evidence. Live access to that global and control
resource ownership remain unresolved.

The husband hero-distance pass verifies both nested GetHero/retail-distance call
pairs and emits explicit self/hero/5 or self/hero/13 operands. Its regression
checks fresh hero results and near-first branch selection without reimplementing
the native distance calculation or claiming geometric boundary parity.

The legacy yes/no-question signature is corrected to the current void return
and optional fifth boolean. Husband question operands are pinned and emitted as
question, yes, no, empty third label, true. The answer is still obtained through
the native message-query loop; opening the question does not return an answer.

The husband question-cleanup pass expands nine conditional jumps to the equivalent
native unpause/movie-end suffixes before the existing actor-cleanup destination.
Tests verify both outcomes of every expansion and the actual answer-wait loop:
cancellation after a frame performs cleanup before another answer poll. Actor
resource release at the final destination remains unresolved.

That cleanup recovery also covers six decompiler jumps to the equivalent1085
suffix. Native paths either push false inside1CF3 or prepare it before1178's
branch. The regression rejects an altered prepared flag and tests both outcomes
of all15 expanded conditional jumps.

The husband home-position pass preserves the second GetHomePos snapshot and
uses it for movement and both distance-over-2 retry checks. The first home query
and its 0.1 gate remain separate. Native movement arguments are radius 0,
move type 0, false, true. The converter's legacy MoveToPosition signature now
matches the existing-handle, nonblocking Lua binding, including both optional
flags and the typed vector parameter. Newer/custom signatures are preserved.
Tests execute the recovered loop through retries, task waits, cancellation,
and the already-in-range path. Resource acquisition/release remains unresolved.
The stricter vector parameter also exposes previously hidden missing positions
in other entity drafts; those diagnostics must not be suppressed with zero vectors.

Husband cleanup now expands all 19 reviewed conditional movie-cleanup jumps,
including four remaining source references to 1CF3. The question-entry pass
restructures the shared 1243 destination so normal completion of the intro
speech opens the question. Positive health selects the optional speech; the
native unordered/nonpositive branch bypasses it. Cancellation during or after
the speech unpauses and ends the movie before actor cleanup. The existing
question/answer body follows the optional speech without duplication.

The husband talk-continuation pass recovers three jumps to shared suffixes
1144/1153. Successful SOME_NERVE and HOW_FIND_OUT paths clear information on
self and continue at1593 instead of falling into cancellation cleanup. The
1144 route performs its native termination check first; direct1153 routes add
no query. Cancellation unpauses and ends the movie before actor cleanup.

Husband speech mapping explicitly reports eight unresolved scheduling mismatches:
retail7E7390 dispatches without waiting, while the current me:Speak binding uses
Speak_Blocking. The emitted caller task loops and their mocked tests do not prove
runtime parity when the speech method itself waits. The native mapping pins each
fresh hero query, dialogue key, resource16, selection0 and false/true/false flags.
Correct lowering requires explicit resource acquisition/use/destruction together.
Current read-only runtime source exposes LuaRetailResources::Speak and task
queries, but lacks resource MoveToPosition, PlayAnimation, ClearCommands and
ClearAllActions needed by the complete husband function. The report deliberately
keeps these speech calls unresolved pending that work.

A reviewable extension for those four resource actions is prepared by
`python -m tools.script_recovery.prepare_man_resource_extension`. It writes a
candidate header, patch and source/native-byte metadata under
`work/man_resource_extension`, without changing the runtime checkout. Exact
native dispatch wrappers are checked before preparation. The candidate keeps
all arguments explicit and performs no acquisition or waiting. A C++ harness
compiles the actual method fragment against stub types and tests forwarding,
resource selection and empty/invalid-resource behavior. This is not an x86
engine ABI or complete runtime-build validation; speech diagnostics stay active.

Husband resource mapping proves one constructor, three preparation pairs, six
acquisition call sites and one destructor for the same stack16 local. The native
CFG checker now supports non-mutating resource-use events and requires a matching
live resource on every incoming path. Constructor/preparation/acquisition/end
paths are verified, including termination before construction. This does not yet
prove all action or temporary Thing lifetimes, and does not enable a partial
mix of explicit-resource and cached-entity operations.

That map now covers57 resource events: the original14 plus8 speech,19 task,
3 animation,1 movement,2 clear and10 Thing-getter calls. Every mapped receiver
is baseline16. The argument IR recognizes a zeroed register followed by an
unsigned low-byte memory load, keeping animation byte1375748 dynamic. Wider or
unproven partial-register loads remain rejected; memory writes invalidate this
fact. Separately, ten resource-returned Things have verified create/query/destroy
paths and operands, including the two home-distance queries. Cached lookup and
hit-condition wrappers and movie lifetimes still need their own lowering.
