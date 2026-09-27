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

## Continuation: the remaining ten (same day, second session)

Syntax failures across the inventory: **10 → 0** (369 files; 24 superseded first-generation lifts excluded,
each listed in its package's `SUPERSEDED.json` with an audited replacement). Unresolved diagnostics 2,390 → 2,000;
machine temporaries 6,034 → 4,749. All generator changes are A/B-checked against the pre-session generator over the
23 original units: only BookCollecting, Bordello, ChickenKicking, SickChild and OakValeRevisited change.

**OakValeRevisited** (now also has a typed TU: the exporter failed on a UTF-8 BOM in `define_addresses.txt`).
`OakValeFire` and the mission helper read like the retail logic (and like Aeon's port) with no TODOs:
the `OakValeFlag` member map is the cutscene flag map (`RunMacroWithFlags(..., quest:RetailFlags("OakValeFlag"))`),
the fire thread waits on `RetailFlags(...):Get("fire")` as a boolean, fills `FirePoint` from
`GetAllThingsWithScriptName`, sizes `Fires` (`StateListResize`) and stores each effect (`StateListSetAt`).
Generator: `this`-alias and member-list register resolution; bare member-list operands; `vector<CScriptThing>::resize`
(0xD34AC0); inlined `CScriptThing::operator=` into a list element; `map<CCharString,bool>` members (0x8ADF10) incl.
boolean char tests; the macro flag-map form; inline actor-map destructor on `._0_4_/._4_4_` fields.
`restore_stack_operands` no longer moves a counter phase across slots (bytes: OakValeFire's `[esp+0x10]`), only when
the old renaming would split it across different slots -- the first, broader version regressed Trader Conflict /
Trader Comment / Guild counters and was narrowed until the A/B was clean.

**BookCollecting**: `BookReactions` (vector<CConversation>) is an unmodified snapshot of definitions +0x4C8
(evidence `definitionSnapshots`, detected in the unit's own reached functions of the typed TU). Reads go to
`quest:GlobalConversations(0x4c8)` (sidecar, CConversation layout from `CConversation::Copy` 0x00E54CA0); the STL
assign/copy/destroy helpers that only the dropped fill reached are omitted and reported. Member resources taken as
a plain assignment (`pCVar5 = this + 0x58`, seh_Boy) no longer leak `this`.

**Six old clusters registered as units** (`register_unit.py`, the Gameflow recipe): beardy_baldy,
dragon_boss_fight, heros_old_house, singing_stones, statue_master, summoning_the_ship. Ranges end at the NEXT block's
first lifecycle function, not the next allocator (sick_child's allocator-bounded range swallows SingingStones'
threads). Generic fixes they needed: x87 -- a `__ftol2` operand duplicated with `fld st(0)` (`f_st0`), `_CIfmod`
operands read from the call-site bytes (`fmod(angle, 1.0)`), `fpatan` = `math.atan(y, x)`, a proven float return
for unit helpers the PDB does not name (StatueMaster 0xED43D0, `ghidra_typing_spec.proven_float_returns`),
`(float)(int / N)` truncation; a scalar float member array walked by pointer (AnglesToFaceList); the inlined
`CScriptThing::GetPos` through a register copy of the Data field (StatueMaster's SM_Center, lifted as `IsXbox`);
C comma sequences -- through any lvalue, in `else if` heads and one-line `if (...) break;`; `1U` literals;
`MsgOnRegionLoaded(&name)` returns the region name (BeardyBaldy compares it).

StatueMaster smoke is clean; so are DragonBossFight, HerosOldHouse, SingingStones. Open in the new units:
BeardyBaldy `extraout_EAX` returns in its hair/tash helpers, SummoningTheShip `in_stack_00000004` and boolean
arithmetic in two `Init`s. BookCollecting `DoConversation` still loses its speaker compares and the speaker resource
method calls; `BookReaction` is emitted as function `null` (unit JSON name drift noted earlier).

Sidecar patches (build on round 10 + member resources): `novi-zzzzzzzzzzzzzzz-state-list-resize.patch`,
`novi-zzzzzzzzzzzzzzzz-global-conversations.patch`; the Release x86 candidate builds. NOT installed; nothing here was
run in-game (the promoted scripts are not in a playtest bundle yet).

Validation: `test_round12_lowerings.py` (10) + `test_member_resources.py` (5); focused regression subset compared
with the morning commit in a separate worktree (see below); 23-unit A/B; smoke of the promoted units (new smoke
problems are mock artifacts: the mock returns nil for RetailFlags / GlobalConversations / positions).

Later the same afternoon: C bool-to-int in arithmetic (`ENGINE_BoolToInt`, SummonerMinion/Attacker Init) and
prototypes for unit functions the PDB does not name, from their `ret N` purge (`ghidra_typing_spec.stack_purges`:
STS_BriarRose `RemoveNeighbours(true)` -- `push 1` at 0x00DF1971 -- had read an unbound `in_stack_00000004`).
Applied only to the six re-exported new units' typing specs; the 23 existing units' typed TUs are unchanged, and the
final 23-unit A/B still changes only the five intended units. BeardyBaldy's hair/tash helpers stay open: Ghidra
lost the result byte (`mov al, [esp+3]` at 0x00E53AC6, set 0/1 by each clothing check) and prints
`CONCAT31(extraout_EAX >> 8, 1)` on every path -- no sound operand to lower.
