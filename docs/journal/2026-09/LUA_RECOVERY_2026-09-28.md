# Lua recovery, 2026-09-28

Continuation of [the 2026-09-27 marathon](CODEX_LUA_MARATHON_2026-09-27.md). Corpus smoke (all 29 units,
readable stage): **21 → 15 failing functions, 0 new** (morning run at `ea7603e` vs. after promotion; scratch
in `work/codex_lua_20260928/`). Nothing installed or run in-game.

## Converter fixes

- **Literal string-vector members** (`native_literal_string_vectors.py`): a member `vector<CWideString|CCharString>`
  that Init resizes and fills slot by slot from UTF-16 `.rdata` literals, and that nothing else writes, becomes a
  file-level `local <PdbName> = {...}`; reads `*(this+OFF) + i*4` become `LOCALLIST_StringAt` → `Name[i + 1]`.
  CChickenSign::TextKeys (PDB +0x1c, Init 0x00E68DC0): the sign now reads `TEXT_QST_B17_SIGN_WON_*[PrizesWon + 1]`
  and passes it to `SetReadableObjectText` (retail vtable 0xB24 with the wide string; ForgeFSE wraps that slot).
- **Quest methods called through the entity's parent** (`quest_unit_evidence.parent_receiver_only`): the
  `mov ecx,[r32+0x14]` load may precede the argument pushes. The caller is now decoded linearly (capstone) and the
  last ecx write before the call must be that load. Four helpers move from entity to quest: V_BeardyBaldy
  SetWanderPointAndDistance (PDB member), V_TourGuide MoveToNextWaypoint 0x00EE6850, V_GuildMaster
  0x00E91F20 (speech by Gameflow stage, now `GetMasterGameState("PostSavePosition")`), Dragon 0x00D258C0
  (`DragonState`, `TargetNumMinions`, `TargetNumSummoners`). All call sites disassembled and checked.
- **Parent thing members by address**: `(CScriptThing_bv *)(parent + OFF)`, the bare sum once a rewrite stripped the
  cast, and `*(CV_X **)(this + 0x14) + OFF` (placeholder class = byte offset; `lea edx,[ecx+0x168]`) on a known
  parent member. TourGuideGuide's `IsDistanceBetweenThingsUnder(me, NextTourWaypoint, 2.0)` and both
  MoveToNextWaypoint calls.
- **By-value thing staged from a `CScriptThing const &` parameter** (`fold_thing_staged_from_reference`), bsim
  `class CScriptThing/C3DVector const &` parameters outranking Ghidra's `int`, and `*(C3DVector *)param` copies:
  SetWanderPointAndDistance is now exactly retail (centre, min 0x448, max 0x44c, state group 4).
- **Struct arrays with the base folded into the index** (`BASE + (IDX + 6) * 0xc` = WaypointInfo at +0x48) and the
  entity-parent int index form; local copy-construction from an element. TourGuide recovers every
  `WaypointInfo[WaypointCounter].locMarker/locTextOverheard/locTextRequested` and `RandomGuideResponse`.
- **Resource parameters**: a `CScriptGameResourceObjectScriptedThingBase &` parameter's vtable calls are
  `resources:<Method>(param, ...)`, also from entity context (MoveToNextWaypoint's MoveToPosition).
- **Discarded squared distance**: KickedChicken pops four GetSquaredDistanceBetweenThings results
  (`fstp st(0)`); the bare `(d ^ 2)` statement continued the previous Lua line as a call. Now the bare call.
- **IsDistanceFromThingToPositionUnder** (retail 0x00CBE4B7, fastcall, alive and strictly under): lifted like its
  Over twin to `thing:IsDistanceFromPositionUnder(pos, dist)`; new sidecar patch
  `novi-zzzzzzzzzzzzzzzzzzz-position-under.patch` (round-12 candidate builds; not installed). A literal in the
  helper's float slot is decoded (`push 0x40c00000` = 6.0 at ChickenMaster's site); `test_native_position_distance`
  previously listed raw bits as unproven and now accepts a literal there (a non-literal unknown is still rejected).
- **Vector component copies** (`native_vector_component_copies.py`, on the raw decompile): three loads of offsets
  0/4/8 from one `C3DVector_bv *` into three slots become `vec_<slot> = ENGINE_VectorCopy(P)`, and the slots'
  reads up to their next store / address use / object reuse become `.x/.y/.z`; for contiguous slots the x slot's
  address is the vector. KickedChicken's foul-line test is now retail's
  `(B.y-H.y)*(A.x-H.x) - (A.y-H.y)*(B.x-H.x) <= eps`, and ChickenMaster's call is
  `me:IsDistanceFromPositionUnder(vec_188, 6.0)`.

## Validation

- 29-unit A/B (`work/codex_lua_20260928/ab/compare.py`, baseline = HEAD copies of the four changed modules): only
  BeardyBaldy, ChickenKicking and TourGuide change; a targeted rerun for the Under change adds GuildMaster
  GameFlow and ChickenMaster's operand spelling. The four evidence moves were reviewed per file.
- Smoke harness: parameter kinds now come from the report's native signature (by name or retail address), with
  a vector kind; `GetDistance*` mocks return numbers. These removed three harness artifacts.
- New tests: `test_literal_string_vectors.py` (3), `test_parent_thing_member_address.py` (4),
  `test_thing_reference_parameters.py` (5), `test_vector_component_copies.py` (3); focused set 148 passed / 35 subtests.
- Full `tools/script_recovery` suite (before the vector fold and the distance-test update): the same 42 failing/erroring
  node IDs as the 2026-09-27 baseline (`work/codex_lua_resume_20260927/failed_nodes.json`), 2,485 passed; one extra
  failing subtest, the raw-bits distance case, since resolved by the reviewed test update above.
- Full suite after the commit (`a43c8e5`): the 42 baseline nodes plus four NOVI AffairWife tests; SUBFAIL lines
  identical to the baseline. Those four also fail at `0682f07` in a clean worktree: they read the install's
  `text.big`, which something outside this session rewrote at 06:51 between my two runs. The install was not touched.
- ChickenKicking re-promoted after the vector fold; Arena's Roth briefly changed (register temporaries) until the fold
  was restricted to stack slots.

## Open

- MoveToNextWaypoint: the initial waypoint copy is still a TODO (`nil`); equivalent, since it only runs when the
  waypoint is not alive.
- `(nil):GetName()` / `(nil):GetDataString()` statements in SingingStone and TraderEscort readables (lost things
  on paths the current control flow does not reach).
- Unchanged from yesterday: BordelloLady/Witch flag slots, Roth, SickChild helper_ECE460, Arena cell guards,
  BeggarBully/ChickenMaster `unaff_*`, BookCollecting BookReaction/DoConversation, OakValeFire.

Analysed, not fixed (next session):
- **Flag word in a reused string slot** (Spectator Main 0x00E63890; same family as BordelloLady/Witch): the
  `line` CCharString at local_68 is dead after its last use, then the slot becomes the cleanup-flag word
  (`mov ebx,[esp+0x14]; or ebx,1; mov [esp+0x18],ebx` at 0x00E63E00). Retail ORs the bits onto the stale string rep
  pointer, whose low three bits are clear (allocator alignment), so seeding the word with 0 where the reuse starts
  is faithful for the `& 1/2/4` tests. The lift also loses the first `| 1` update (a TODO on the string-typed slot),
  leaving `CVar11` nil on the MsgIsHitByHero path. Plan: a raw-decompile pass for `A = SLOT; B = SLOT | 1; SLOT = B;`
  after SLOT's last object use.
- **Arena cell guards** (ArenaCellDoorGuard Main 0x00F17C70): the hit tests' receiver is `ebp`, written only by
  `lea ebp,[esi+8]` (0x00F17D9D, `me`) and, inside the cutscene branch that exits elsewhere, `mov ebp,[esi+4]`
  (GSI). The typed export prints several unrelated values as `pCVar6`, so the Lua receiver is whichever branch
  assigned last (nil on the smoke path, the hero or a resource thing on others). Needs register reaching
  definitions at the call, not a text rule.
- **SickChild helper_ECE460**: an inlined `CCharString !=` between a constant and the new resource's name,
  printed as a four-operand `_stricmp`, plus an unresolved GSI +0x118 call whose receiver was dropped.
