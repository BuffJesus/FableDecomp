# Script-member resources: twelfth readability pass (2026-09-27)

Continues the [readability review](ALL_SCRIPT_READABILITY_REVIEW_2026-09-26.md). Artifacts:
`work/readability_marathon_20260927_round12/`.

## Result

Syntax failures across the Lua inventory: **16 → 10** (359 files now; two new
`native_quest_helpers.lua` files). Unresolved diagnostics 2,561 → 2,390, native names
5,856 → 5,832, machine temporaries 6,089 → 6,034. Newly loading: Bordello `V_Bordello` and
its helpers, SickChild `V_SickChild`, helpers and `WomanToAttract`, ChickenKicking `ChickenMaster`.
The [ledger](../../scripts/CURRENT_LUA_READABILITY_REVIEW.md) is refreshed.

## The gap

Five of the 16 failures were the same thing: retail scripts hold resource objects (`seh_*`)
and a cutscene-argument string map (`csargs`) as **script members** that outlive every callback.
Bordello's entities acquire an actor and copy it into their quest's member
(`parent->seh_Guard = res`, operator= `0x8ABD10`) and write `csargs["$DIALOGUE"]`; the quest's
`PlayCutscene` (`0x00E3E720`) builds its actor map from the four members, runs the macro with
`csargs`, then clears it (`0x9AACE0` = `std::map<CCharString,CCharString>::clear`, disasm).
The evidence builder dropped the members entirely (`expand_member` recursed into the resource
class, found only padding/pointers, emitted nothing), so the refcount dances stayed as C.

## Changes

- **Sidecar** (user decision: a sidecar member store): `resources:MemberResource`,
  `MemberStringMap`, `AssignResource`, `ClearStringMap` in the quest's long-lived
  `RetailResources` scope, which the quest and each entity VM share (entity VMs receive the
  parent's `LuaQuestState*`). Patch `novi-zzzzzzzzzzzzzz-member-resources.patch`; Release x86
  candidate builds. Not installed, not in-game validated.
- **Evidence** (`quest_unit_evidence.py`): `resourceFields` (retail offset → PDB name) for the
  quest and entities. A helper that only one entity calls is a quest method when every call
  site loads the parent as `this` (`mov ecx,[r32+0x14]`, checked in retail bytes): ChickenKicking
  `0x00E68B20`, BookCollecting `0x00E55C60`, Bordello `0x00E44A40` / `0x00E44CC0`.
  Only `resourceFields` and these four moves were merged into the unit JSONs; a full regeneration
  also shows unrelated drift (master bool-array offsets, `null` → `NativeThread_00e44980`,
  `NativeThread_*` additions in Tour Guide / Book Collecting) that still needs review.
- **Lowering** (`native_evidence_lowering.lower_member_resources`, before the actor-map fold):
  member operands (`this + N`, parent `*(this+0x14) + N`, a parent cached in a reused local only
  while it holds the parent) become `RESOURCE_MemberResource("seh_X")` / `STRINGMAP_Member("csargs")`;
  never a dereference. `operator=` (called or inlined as node-self + refcount dance) folds to
  `AssignResource` only when the source is a resource the function constructs.

## Validation

- `test_member_resources.py` (5, incl. the real Bordello `PlayCutscene` body) plus the layout /
  string-matrix tests: 104 passed.
- A/B of all 23 units against the pre-change generator (separate tree): 19 byte-identical; only
  Bordello, SickChild, ChickenKicking, BookCollecting change, all in member sites.
- Smoke before/after for the four units: no regressions, six files newly loading. New methods
  are listed in `smoke_run_unit.PENDING_BINDINGS`.
- Promotion changed exactly the A/B file set; every other file equals yesterday's.

## Still open (not fixed here)

- Magicman `Main` (`0x00E40E80`): Ghidra merges many objects into stack slot 0x364; the lifter
  zeroes the resource (`PrepareResource(0)`, `AssignResource(seh_Boss, 0)`). Pre-existing.
- Bordello entities' calls to `PlayCutscene` lost the by-value macro-name argument (TODO).
- SickChild `V_SickChild`: the HERO actor store's `&local_1c` stays a TODO; a
  `quest:PauseAllNonScriptedEntities(<seh_Mother>)` call looks like a bsim mislabel of a resource method.
- The quest resource scope is never closed; members outlive a finished quest.
- Remaining 10 syntax failures: BookCollecting `Init`'s 92-byte record vector copy from global data
  `+0x4c8` (reachable, needs a real lowering); OakValeRevisited's thing-vector walk; and 8 old
  first-generation cluster lifts with no registered unit (BeardyBaldy, DragonBossFight, GTDI_Maze
  (superseded by the unit port), HerosOldHouse ×2, SingingStones, StatueMaster, SummoningTheShip).
  Registering those clusters as units is the likely route.
