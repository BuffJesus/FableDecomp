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

## Afternoon: dead cleanup flags, split counters (after `c3046c0`)

Corpus smoke **15 → 8** failing functions (21 → 8 since the morning), none new; 7 real (ScorpionHome is intentional). Remaining: BookCollecting
BookReaction/DoConversation, OakValeFire, the two Arena cell guards, Roth, SickChild helper_ECE460, and
ScorpionHome (intentional).

- **Dead flag bookkeeping** (`lift_native_lua.drop_dead_flag_statements`): cleanup-flag words that share a slot with
  another object (a dead string, a resource member, the `ebp` register) cannot be started clear, but their bits only
  select epilogue destructors, which Lua does not have. The cluster grows backward from tested / bit-produced values and
  forward only from bit-produced ones (plain copies are leaves, so the shared slot's own life stays); flag ops are
  `| K` with small K and `& K` with near-all-ones K (a high-byte boolean's `& 0xffffff` is not one); `if ((v & 0x80) ~= 0)`
  counts. Statements drop only when every surviving read of every member is textually preceded by a
  non-flag definition, not a flag write (a first version checked only the next mention and dropped New Oakvale
  AffairMan's mask, which goes back to the caller through its parameter and result; `test_native_affair_man_mask`
  caught it). Loop back-edges are not modelled. Fixes Spectator, BordelloLady Main, Witch, BeggarBully; elsewhere it
  only removes dead bookkeeping (Arena guards/Whisper/Flick/Needle/Shadow, BanditCamp boss-battle entities,
  BordelloGuard, Madame, GuildMasterGameFlow, FishermansWife, ManWithDoorName, SickChild, TalkingTrader2, KG_Chief,
  LookoutPointBeggar, ArtifactThief): every removed line is flag-shaped, nothing is added.
- **Counters split from string registers** (`ctr_CVarN`): the lifter's local-name filter and definition counter skipped
  the prefix, so ChickenMaster's score (`= 0`, `+ 0x64` ...) and BS_Teacher's counter were TODOs (and a single
  definition would have been constant-propagated). ChickenMaster's high-score logic now runs.
- **Dword stored as four bytes** (`native_vector_component_copies.fold_split_dword_stores`): ChickenMaster's by-value
  `me` copy for SetIsPushableByHero left `piVar2 >> 8` residue on nil.
- Smoke harness: recorded native types also apply to readable entity files (leading `me`), are skipped when the bsim
  prototype's parameter count disagrees with the export (see below), and the integer game-data mock returns 1.
- A/B (29 units, baseline `a43c8e5`): only the units above change; twelve promoted (GuildTraining draft only; its
  readable is hand-reviewed).

## Evening: AskForBook, member-resource speech (after `cb6bc76`)

Corpus smoke **8 → 5** failing functions, none new; 4 real: the two Arena cell guards, Roth, SickChild
helper_ECE460 (ScorpionHome is intentional).

- **Lost parameter from the reviewed prototype** (`native_function_parameters`): BS_Teacher AskForBook 0x00E55CE0 is
  `AskForBook(long, class CCharString)` in ego_r (RET 8) but exported with one `CCharString_bv param_1`. When every
  reviewed parameter is one dword and each missing one's this-call stack slot is referenced, it is appended under its
  slot name (`stack0x00000008` → `native_arg_param_2`, renamed before the stack-slot passes) and the reviewed scalar
  type replaces the export's; the refusal line reads the caller's key.
- **Callers' pushed operands** (`native_local_helper_operands.py`): a local helper call printed with fewer operands than
  its recovered signature gets the missing ones decoded from the pushes before it (in-place by-value CCharString
  literal, immediates, `this` / parent member loads), only when every missing one decodes. AskForBook's callers now
  pass `(LastBookRequested, "TEXT_QST_B16_BOOK_REFUSE_AGAIN")` and `(value, "TEXT_QST_B16_BOOK_REFUSED")`; before, the
  first passed nothing and `value < booksWanted` would have compared nil.
- **Member-resource vtable calls**: slot 0x34 is `Speak`, 0x30 `GetScriptThing` (hidden result slot →
  `resources:ScriptThing`), also directly on the member (`*(this + 0x34)`), through a local copied from a member alias,
  and through a vtable temporary. SickChildsMother/Witch now wait on `IsPerformingScriptTask()` (was `while nil`),
  WomanToAttract speaks its laugh lines, BS_Teacher speaks its refusals.
- **Getter operands**: Ghidra gave Speak's pushes to the preceding operand-less GetHero (0x118); they move to the call
  that consumes the result, the old register value saved as `<reg>_pushed` when it is also an operand.
- Harness: `RetailFlags` / `GlobalConversations` mocks shaped like the sidecar bindings (BookReaction, DoConversation and
  OakValeFire were harness failures).
- A/B: only BookCollecting (BS_Teacher) and SickChild (Mother, Witch, WomanToAttract) change; each changed file has
  fewer `missing` / `unresolved` / TODO placeholders than before. Tests: `test_ask_for_book_recovery.py` (4).

## Late: Roth's high-byte flag (after `8d92536`)

Corpus smoke **5 → 4** failing functions (Arena cell guards ×2, SickChild helper_ECE460, ScorpionHome intentional).
Arena Roth Main (0x00F21880) keeps a byte flag at `[esp+0x13]` (`mov byte ptr [esp+0x13], 1`; cleared when
GetHealth <= 0; read back), which Ghidra prints under two dwords: `pCVar15 = CONCAT13(1,(int3)pCVar22)` stores, the
test reads `pCVar22 >> 0x18`, and one site stores `CONCAT13(bVar3, ...)`. `fold_cross_variable_high_byte_flags`
(in `normalise_typed_decompile`, after the existing `if (C) { X = Y & mask }` shape and before tests become `X_b3`)
makes the store, its `& 0xffffff` alternatives and the next high-byte test of either name one boolean
`hb_stk_fNNN`, when nothing else in a 45-line window mentions them. A/B: only Roth changes.
Tests: `test_cross_variable_high_byte.py` (2).

Open, analysed: **SickChild helper_ECE460** builds `CCharString("NULL")` (0x012393E4) and sets the macro string map's
`$ARG1` to a CCharString member of its scripted-thing resource (`local_4`, resource object +0xC) unless that equals
"NULL" (the four-operand `_stricmp` is the inlined compare). Ghidra also hands the operands of the GSI +0x20 call
(StartScriptingEntity: `&resource, 4` and GetHero's result) to GetHero. The resource member's meaning needs the
scripted-thing resource layout before this can be lifted faithfully.

## Candidate v35 (staged, not installed or run)

`local-candidate-v35` = v34 + 23 roster Lua files refreshed from the readables at `26e5fa9` (same rule as v33/v34:
in-game-pinned packages unchanged) + `FableScriptExtender.dll` from the round-12 sidecar candidate at `e83c39f`
(v34's `a7bb755` + position-under). `work/codex_lua_20260928/stage_v35.py`, validation
`package_validation_v35.json`: 210 Lua parse, 216 manifest hashes match; mock smoke of the ten changed packages vs v34:
7 fixed, the one "new" entry (SetWanderPointAndDistance) is the package-dir harness lacking recorded parameter types.
Branch pushed at `26e5fa9`.

## Night: receivers by reaching definitions (after `4a30c2b`)

Corpus smoke **4 → 2** failing functions (SickChild helper_ECE460; ScorpionHome intentional).
`native_receiver_reaching.py`: for an entity's thing vtable call, ecx is traced to its source register and every
definition of that register reaching the call is collected over a CFG decoded from the machine code (direct jumps,
fall-through, and switch tables bounded by a preceding `cmp reg, N`; anything else, no claim). When all are
`lea r,[esi+8]` with esi = this for the whole body, the call is respelled on `this + 8` (without a receiver argument,
as Ghidra prints calls on `me`). The Arena cell guards' hit tests now test the guard (were the merged `pCVar6` /
`pCVar11`); BanditCamp Gate1GuardOuter's talk check was on the hero and is now on the guard (starts the gate
cutscene). A/B: only those three files. Tests: `test_receiver_reaching.py` (2).

### Found: Trader Escort's trader comments never run (MakeTraderComment 0x00E01900)

Every lift since v15 (the build that completed Trader Escort in-game) returns early from MakeTraderComment: the first
`IsAlive` on its speaker copy is unresolved, so `if not nil then return false`. Cause: the `CScriptThing const &speaker`
is copied into `local_30` with its Info word split into `local_28`; the dead-speaker fallback re-seats it with an inlined
`operator=`; and a second IsAlive goes through the copy's own vtable (`local_30._0_4_ + 300`). A fold for these forms
(run after `restore_stack_operands`: running before it changed the printed call heads and broke the callOrder pairing)
recovered the speaker, both IsAlive checks, AddNewConversation, the speaker's data string and the first comment line.
**Not landed**: the `comment_type` 1/2 branches reuse the parameter slots (`speaker` as a byte flag and a string
temporary, `comment_type` / `comment_to_make` as string temporaries: a raw `CONCAT31(...)` call and
`r2:IsEqualTo(r3._4_4_)` would become reachable). Enabling comments in an in-game-proven package needs the whole
function right; next step is parameter-slot reuse for this function. Kept from the attempt: `CScriptThing::GetDataString
(X, &slot)` → `slot = X:GetDataString()` (and GetName / GetDefName).

## Night: `me` through an int register (after `d82fd83`)

A survey of promoted readables for conditions on a literal `nil` (dead code behind an unresolved value) found 23;
17 were one shape: `piVar1 = (int *)(this + 8); (**(code **)(*piVar1 + SLOT))(piVar1, ..)`, the hit / talk tests on
`me` through an int register. `respell_me_register_calls` (entity lowering, after `resolve_this_aliases`) respells those
calls up to the register's next assignment. MsgIsHitBy..., MsgIsHitByAnySpecialAbilityFrom..., the 0xa4 special-ability
test and IsTalkedToByHero now run in LookoutPointBeggar, BordelloClient, BordelloLady, GuildMasterGameFlow, the Witch and
BanditCamp's Assassin1 (BanditCamp is pinned in v35, so the bundle is unchanged). A/B: only those six files. Six nil
conditions remain (TraderEscort MakeTraderComment, BS_Teacher `uStack_14` IsAlive, TourGuideFollower, STS_BriarRose,
WomanToAttract).

### Vector copies from dword pointers (after `7169e81`)

`fold_vector_component_copies` also takes a `undefined4 *` / `float *` source read as `*P`, `P[1]`, `P[2]`, with at most one
unrelated literal store between components (kept, after the copy). V_TourGuide TourGuideFollower's waypoint distance
test (`IsDistanceFromPositionOver(vec_28, 3.0)`, was unresolved) and its MoveToPosition (was given only the x
component) now use the whole position. GuildTraining SkillTarget's four positions are respelled as vectors with
identical values (draft only; its readable is hand-reviewed). The A/B snapshot set now includes the helper modules
(`native_vector_component_copies`, `native_receiver_reaching`, `native_local_helper_operands`,
`native_literal_string_vectors`), so later edits to them are compared too.

## Candidate v36 (staged, not installed or run)

v35 + 6 roster Lua files (BordelloClient, BordelloLady, Witch, BanditCamp Gate1GuardOuter, TraderEscort ×2), sidecar
unchanged. 210 Lua parse, 216 hashes match; mock smoke vs v35 unchanged. Gate1GuardOuter's talk check moves from the
hero to the guard (retail), in a package completed in-game with the old line.

### Split Data words and the numbered-children loop (after `5f7f305`)

`native_split_thing_words.merge_split_thing_words` (raw decompile): a stack CScriptThing (`undefined1 X [4|8|12]`, used as
a CScriptThing) whose Data word Ghidra printed as the local four bytes above it becomes `X._4_4_` when that local is only
stored, null-tested or used as a vtable base. BS_Teacher 0x00E56D10 now posts its opinion deed to boy0, boy1, ... and
girl0, ... while each exists (was a `while nil`); Global_DebugCycleThroughSpeech gains a nil guard. The smoke mock now
returns an empty thing for numbered script names from 4 upward, so such loops end. `native_receiver_reaching` takes
`this` from the prologue's `mov R, ecx` (any callee-saved register) instead of assuming esi (no new sites proven in the
corpus; V_SickChild WomanToAttract's 0x6C call reloads its receiver from a stack spill, which is not tracked).

### TourGuide stop markers; next: ChickenMaster `csargs` (after `9ef80ff`)

V_TourGuide Init never set `WaypointInfo_N_locMarker` (a struct element's address is its offset-0 string member), so
every `GetThingWithScriptName(WaypointInfo_N_locMarker)` looked up an empty name. The string-array rule now covers a
struct array's first string member: all 18 stops are set. A/B: only V_TourGuide.lua. Focused tests pass; the full
suite was not rerun for this commit (usage limit).

Next, verified in scratch but NOT landed: ChickenMaster's 33 `csargs` map statements lift to
`resources:SetString(resources:MemberStringMap("csargs"), "$LINE", ...)` once `_MEMBER_CAST` accepts `( map<...> *)` (a
space after the parenthesis). Needs the 29-unit A/B and full suite before landing.

### ChickenMaster cutscene lines, SickChild expression reactions, BeggarBully belch flag (after `96246cf`)

- **ChickenMaster `csargs`**: `_MEMBER_CAST` accepts `( map<...> *)`. All 33 cutscene-line statements lift to
  `resources:SetString(resources:MemberStringMap("csargs"), "$LINE", pcVarN)`. The key was checked against the native
  `CCharString("$LINE")` at each site, and every site sets its own `TEXT_QST_B17_*` value first.
- **String compares per stretch**: `fold_name_compare` folds a pointer loaded once (`pvVar11 = *(void **)pCVar19`)
  for the compares that follow it, up to its next write. When the pointer has several writes in the function, the
  stretch may not contain a label or a write of the string. This clears all 26 WomanToAttract expression compares.
- **`(undefined3)` high-byte flags**: `fold_high_byte_flags` now takes the `(undefined3)` spelling and `X._3_1_` reads.
  It still refuses a slot whose dword is copied out or used in arithmetic (WomanToAttract `uStack_a8`: a cleanup mask
  and a string). It rewrites a clear done through a copy (`uVar2 = (uint)X; ...; X = (uVar2 & 0xffffff)`) as the
  slot's own clear, which is exact because CONCAT13 keeps the low three bytes. A byte test inside a compound
  condition is the boolean itself, and a whole-dword constant store sets the flag too.
  - WomanToAttract's alive check becomes `hb_stk_a8 = true; if health <= 0 then hb_stk_a8 = false end`, and its
    reaction flag is set.
  - BeggarBully's first-belch flag was `if (1 ~= 0) or TaughtBelch` (always true, so the belch cutscene was always
    skipped). It is now false at the start and set after the cutscene.
- A/B (29 units): only ChickenMaster, WomanToAttract and BeggarBully change. SickChild's TODOs drop from 205 to 142
  in the draft; the CONCAT/Compare clusters drop from 56 to 5.
- **Open (pre-existing)**: WomanToAttract Main's final `~CTimer(&pCStack_ec)` lifts as `DeregisterTimer(<wrong local>)`.
  It was `pCVar6` (only ever `0x1`) and is now `hb_stk_a8`; it should be `x_stk_d0`, the `RegisterTimer()` result. The
  destructor's stack operand resolves to the wrong slot. ManInLove `xStack_170` (a refcounted pointer that is read)
  correctly stays unfolded.

### Codex continuation: WomanToAttract timer destructor (base `118fb6c`)

- Retail Main `0x00ECF7F0` ends with `lea ecx,[esp+0x14]` / `call 0x00CD4470` at
  `0x00ED0AE8` / `0x00ED0AEC`. Its epilogue restores 0xE4 bytes, proving timer slot -0xD0.
  The typed export incorrectly reports `ecxStack=-0xCC`; that spelling became the reaction flag.
- `restore_stack_operands` now corrects this known timer-destructor receiver from bytes when an adjacent
  LEA and straight-line epilogue prove the frame. Interior branches, overwritten receivers and missing bytes
  are refused. Counting only the initial prologue was insufficient: BanditKing saves EBP later. That initial
  attempt and its comparison are superseded by `ab2.log` / `ab/comparison.json`.
- Promoted only WomanToAttract draft/readable and the readable report. Timer registration, reads and cleanup
  now share the same handle. No generated Lua was hand-edited. No candidate installed or game launched.
- Validation in `work/codex_timer_20260928/`: all 29 units regenerated before/after against a converter snapshot
  of `118fb6c`; only WomanToAttract changes. Scratch readable: 14/14 files parse. Before/after smoke JSON is
  identical, including the pre-existing `_stricmp` failure in helper_ECE460 and pending MemberResource bindings.
- Behavioral mock: both generated stages release exactly handle 731 once on immediate termination after
  registration and termination after the distance check. The committed baseline fails the latter with
  `wrong timer handle`. Focused suite: **58 passed, 7 subtests passed** (`tests_final.log`); full suite not rerun.
  Current readability audit: **371 files, 0 syntax failures**; ledger refreshed without narrowing inventory.
- Reproduce: `python work/codex_timer_20260928/ab/compare.py`; tests include
  `test_timer_destructor_operand`, `test_round12_lowerings`, `test_skill_target_and_friendly_actor`,
  `test_member_resources`, `test_high_byte_flag_slots`, `test_resource_member_zero_init`,
  `test_native_loop_timer`, and `test_native_entry_timer` under `tools/script_recovery`.
- Next from desktop transcript: GuildMaster's remaining goto structure, then stage candidate v37 before
  playtesting. Existing helper_ECE460 and TraderEscort parameter-slot recovery gaps remain open.

### Offline marathon: shared attack guards and nested Boolean comparisons (base `581c1ff`)

User requested background work: no game launch and no shared-install writes. The timer fix is checkpoint
`581c1ff` (local only). This pass fixes the shared readability generator, not individual Lua scripts.

- Empty nested guards combine with short-circuit AND; alternative guards keep OR. Private 0/1 jump chains
  retain numeric type when needed, while Boolean uses lose their number round trip. GuildMasterGameFlow's
  two attack checks (native labels 0x00E914B3 and 0x00E91742) now express hero hit OR non-healing special hit;
  six jumps and their scratch flag disappear. Other GuildMaster cleanup/quest-card gotos remain unresolved.
- The numeric truth simplifier now balances parentheses with strings/comments masked. Both 0/1 directions
  and both equality operators preserve Boolean result type and call order. It refuses larger RHS operands
  such as 0.5 and 0 + 1. Negation rewrites keep grouping: `not not (a == b)` previously could become
  `not a ~= b`, which changes the result. Some regenerated functions retain explicit termination checks
  where the old optimization combined them with NewScriptFrame; these account for extra mock calls.
- 29-unit before/after regeneration, reviewed diffs in `final_review.diff`: 78 changed Lua files in 19 units.
  **75 files / 18 units promoted**, byte-for-byte from tested scratch. GuildTraining's three changed files
  are excluded: its existing readable_converter differs from a clean baseline in three files (including
  ArtifactThief/FinalMaze), so that lineage needs separate review. The hand-reviewed training tree stays intact.
- Audit: **371 files, zero syntax failures; 40 fewer gotos, 44 fewer generic temporaries**. Unresolved count
  stays 1,498. This is a readability checkpoint, not certification of full retail equivalence.
- Broad suite: **268 passed, 529 subtests passed**, five failures and three setup errors identical to baseline
  (older Oakvale native/candidate tests). Same-order baseline run: 185 passed, 48 failed, three errors,
  529 subtests; the additional 43 baseline failures are newly added chain/guard tests. An isolated replay
  of the eight old cases hit Unicorn access-violation noise and was stopped; `baseline_style_suite.log`
  supersedes that partial run. No full script-recovery suite rerun.
- Final bounds/semantics checks: **165 passed / eight subtests**. Final source reproduces GuildMaster scratch
  exactly. Whole-zero hardening has no matching materialized expressions in existing corpus inputs/outputs;
  targeted tests cover numeric prefixes and compound operands. All affected-unit before/after smoke JSONs
  reviewed: no new errors. Existing helper_ECE460 `_stricmp`, Bordello helper argument faults and pending
  member-resource bindings remain. Beardy's unchanged entity has random-dependent call-count variation.
- Reproduce from `work/codex_lua_marathon_20260928/`: `ab/compare.py` does full before/after; `ab/refresh.py`
  reuses the frozen before trees. `style_tests_final.log`, `test_comparison.json`, `bounds_tests.log`,
  `final_smoke_comparison.json`, `promotion.json`, and `readability.json` contain results. Source snapshots
  and intermediate comparisons are retained; final diff supersedes checkpoint-1 output.

**Next native recovery evidence:** SickChild helper 0x00ECE460 is not blocked on a guessed resource-string
member. Its bytes prove **two by-value CCharString parameters**: `ret 8` at 0x00ECE7E9; after popping saved
registers, destructors on `[esp+0x38]` and `[esp+0x3C]` with frame size 0x34 (entry +4/+8). Caller 0x00ECDBDB
constructs two strings on the outgoing stack, including literal `NULL` at 0x012393E4 and macro pointer
0x012F2600. At 0x00ECE4EE the helper loads the second parameter from `[esp+0x48]` (frame 0x40), compares it
against NULL with `_stricmp` at 0x00ECE501, and at 0x00ECE52F takes that same parameter's address for `$ARG1`.
Next: recover the generic dropped by-value string signature and caller operands, then the local map/resource
construction. Do not patch just the `_stricmp` symptom or invent a resource member. No helper change landed.

### Candidate v37: offline staging complete

Source checkpoint **98fa73a** (timer **581c1ff**); both local, not pushed. Bundle:
`work/new-oakvale-original-fse-20260912/local-candidate-v37`, derived from v36. **45 Lua files refreshed**,
including later BookCollecting/ChickenMaster fixes and WomanToAttract's timer. Fifty-six differing candidates
retain their pinned source lineage; no ambiguous mappings. All nonupdated payloads are byte-identical to v36.
No new override registrations: GuildMasterVillage is outside this existing roster, so its source improvements
are not in v37. Its remaining goto recovery and any roster change are separate work.

Validation: **210 Lua parse, 216 manifest hashes match, 38 override entries**. Before/after whole-package smoke
has **zero new errors**; 17 existing mock failures remain (full identities in `package_validation_v37.json`).
Sidecar stays **e83c39f**, DLL SHA-256
`f0ded2603690b8325fb0565d8aa0829b83887300f62c5553b03b828513e7df2c`.
`stage_v37.py` and `validate_v37.py` in marathon scratch record the staging/validation procedure.
**Not installed or run in-game.** User's background-only constraint remains active.

Next offline work: recover helper_ECE460's two by-value strings and its map/resource lowering from the
native evidence above; continue GuildMaster's shared cleanup tails without hiding unresolved jumps.

### SickChild continuation: import-thunk inference and recovered helper prototype

Background-only continuation after `57f29a0`; no game launch or install changes. Fixed
`infer_helper_prototypes.py`: an entry jump through the import table now terminates purge inference,
instead of scanning the adjacent unrelated routine for a `ret N`. Direct thunks still resolve;
cycles return unproven. Rebuilding a spec revalidates old `inferred ret` rows and removes unsupported
ones, while retaining reviewed/API signatures. A read-only audit found **22 unsupported signatures
in 17 registered units**, mostly `rand`; this does not regenerate those units automatically.

Native evidence confirms `_stricmp` at 0x00BFEBA8 is an import jump through 0x014400EC. Its SickChild
caller pushes two pointers and cleans eight bytes at 0x00ECE506. Baseline inference incorrectly
returns `(16, False, False, False)`. Added its reviewed cdecl/two-pointer signature, plus helper
0x00ECE460's two by-value CCharStrings (`macroName`, `argument`). The helper destructors at
0x00ECE7D8/0x00ECE7E1 and `ret 8` establish parameter ownership; 0x00ECE6FF passes macroName to
RunCutsceneMacro, and 0x00ECE544 assigns argument to `$ARG1`. The old witch-initializer autoname
is untrusted and is not evidence of the helper's role.

Validation: **22 focused tests pass**, covering import boundaries, direct thunks/cycles, stale-spec
rebuild, retail instruction bytes, existing call pairing and timer cleanup. Two broader attempts
including native-string generation encountered the existing Unicorn `mem_map` access-violation
loop; stopped only the owned pytest processes. Those attempts do not count as passes.

Scratch `work/codex_sickchild_20260928/` contains `typing_spec.json`, the read-only Ghidra export
`translation_unit_typed.json`, `typed_export.log`, `draft/`, `import_audit.json`, and
`focused_tests.log`. The experiment changes only the two reviewed prototypes in its input spec.
The corrected export now has stable string parameters, hero acquisition and separate actor/string
maps. It also exposes missing lowering: CCharString null/data extraction around `_stricmp`, the
`pOther = &argument` alias, and inline string-map construction/destruction. The draft still has
syntax failures (48/49 functions; 11/13 files pass), including `piVar2 = *"NULL"`.
**No experimental evidence or Lua promoted; v37 stays unchanged.** Shared generator Lua A/B/smoke
promotion is deferred until those lowering gaps are fixed; this checkpoint changes prototype
inference only. Existing evidence specs/TUs remain unchanged pending reviewed regeneration.

Next: finish generic string/map lowering against the corrected helper export, recover and test
both callers' macro/argument operands, then compare the full SickChild regeneration to its frozen
baseline. Review other fresh-export changes separately; do not replace the entire TU blindly.
Only promote syntax-clean, behavior-tested output with unchanged unrelated-unit comparisons.
GuildMaster cleanup tails remain the subsequent offline task.

### SickChild marathon: string arguments, maps and caller cleanup recovered

Base `a0a257d`; offline/background only. Promoted exactly five SickChild Lua files in each of draft/readable:
quest root, shared quest helpers, SickChildsMother, Witch and WomanToAttract. No shared-install changes.

- The earlier corrected Ghidra export supplies only **three reviewed functions** to the typed TU:
  0x00ECD9E0, 0x00ECE460 and 0x00ECE7F0. All other function records remain identical. Typing spec now records
  the two reviewed signatures (helper's two by-value strings; CRT `_stricmp` cdecl/two pointers). The full
  fresh export's other changes remain excluded. Correct stack tracking also repairs the callers' pause and
  movie cleanup operands and recovers the mother's potion possession lookup. Broader unresolved branches
  remain; these functions are not certified completely recovered.
- General lowering recovers null/character-data CCharString reads, zero-only `_stricmp` tests, and complete
  inline string-map construction/destruction when the proven 0x009AC700 index operation establishes its type.
  Missing/wrong destructor, mismatched head links and nonempty fallback buffers are rejected. String-map
  address aliases retain runtime values; char-literal pointers keep existing literal resolution.
- Native caller recovery now includes ordered sites without exported stack-slot metadata. Parent receiver
  identity and the outgoing constructors establish all four helper operands: CS_SICK_INTRO/OUTRO/WITCH +
  NULL, and CS_SICK_POTION + runtime definition offset 0x744. The CDefString-created slot is reused, avoiding
  a duplicate read. Constructor length, branch barriers and the final ECX receiver are checked.
- The same rules recover WomanToAttract's leaving-cutscene string map and its runtime `$ITEM` at offset
  0x738. Both helpers now preserve their actor bindings and actual map/resource lifetimes.
- Smoke accepts `--conversion-report` so scratch/package runs can use their actual native parameter types.
  Without it the name heuristic passes a thing table to the new string argument; that early smoke result
  is superseded by the typed report and the final ordinary `--unit sick_child` smoke.

Validation: **100 focused tests pass**, including four full caller branch fixtures in both draft/readable,
all four native call sites, NULL/null/mixed-case/empty/missing/nonempty arguments, both helper contexts,
leaving-cutscene lifetime, unsafe-shape rejection and existing BookCollecting/timer regressions. The potion
branch fixture seeds existing entity progress; it does not validate persistence. No full-suite claim.
**371 current Lua files parse**, zero syntax failures. Draft TODOs 245 -> 210; readable corpus diagnostics
1498 -> 1439. These are mechanical counts, not proof of game equivalence.

29-unit A/B: only the five SickChild files differ; **28 unrelated units unchanged**. Intermediate alias rules
regressed four static literals in Arena/ChickenMaster; those outputs are superseded and were never promoted.
Final char-literal guard was checked against all **1,973 exported function inputs**: only those two functions
changed. Both units regenerated back to byte-identical baseline. Final SickChild regeneration exactly matches
its tested scratch draft. Reports: `ab/final_comparison.json`, `literal_guard_corpus.json`, `readable_review.diff`,
`final_tests.log`, `promotion.json`, `source_hashes.json`, `readability.json` under
`work/codex_sickchild_marathon_20260928/`. Readable generation uses explicit scratch `draft=` inputs.

SickChild smoke problems **4 -> 1**: helper `_stricmp` failure and two free-global sets resolved; no new
failures. Remaining `unaff_EBP` is in another WomanToAttract path. MemberResource remains a reported pending
binding in the generic harness. No game launch or runtime proof. Candidate v38 staging follows this source
checkpoint, preserving v37's sidecar and override roster.

GuildMaster follow-up: native 0x00E91898 unpauses/destroys its movie and continues; 0x00E91DB4 also continues
through the frame loop. The 0x00E91EE8/0x00E91EF9 path unpauses, destroys the movie, releases the controlled
resource at 0x00E91F02 and returns. Do not merge these into one exit. Evidence is in
`guild_cleanup_evidence.txt`. GetGuildMasterSpeech 0x00E91F20 still exports a pointer return with an
`in_stack_00000004` hidden CCharString result, while ego_r declares a by-value CCharString return. Its hidden
return and cleanup control flow are the next recovery target. Witch persistence also remains suspect:
OnPersist currently serializes ThingsForPotionGot as a quest bool instead of the local integer progress;
that is separate from this tested helper/branch recovery.

### Candidate v38 staged offline

Source checkpoint **62a0685**, local only. `work/new-oakvale-original-fse-20260912/local-candidate-v38`
is v37 plus exactly five reviewed SickChild roster Lua files. All other payloads match v37 byte-for-byte
apart from manifest/STAGING metadata. Existing pinned lineages and 38 override registrations are preserved;
no ambiguous mappings. Sidecar remains **e83c39f**, DLL SHA-256
`f0ded2603690b8325fb0565d8aa0829b83887300f62c5553b03b828513e7df2c`.

Validation: **210 Lua files parse, 216 manifest hashes match, no new package smoke failures**. Sixteen
existing package mock failures remain; SickChild's old helper failure is resolved. Both before/after bundle
smokes use the same explicit SickChild parameter report, so its new string argument is not supplied a
thing table by the name heuristic. `stage_v38.py`, `validate_v38.py`, `package_validation_v38.json` and
`bundle_payload_diff.json` in marathon scratch preserve the procedure and results. No installation or
in-game run. Next: GuildMaster hidden string-return/cleanup recovery, then separate Witch persistence.

### GuildMaster hidden result and returning cleanup checkpoint

Offline continuation from `9512faa`; scratch `work/codex_guild_marathon_20260928/`.
Recovered `GetGuildMasterSpeech`'s hidden output argument and fixed the general cleanup-region pass
so a region ending in a value return propagates that value. Previously several terminating speech
branches called a local function returning the empty string and then returned nil themselves. A
return now ends the region immediately, before any unreachable jump or subsequent side effects.
The expression is evaluated after cleanup statements. Empty regions and delegated emitted tails
retain their value returns too. Existing loop/ambiguous-control exclusions remain.

Native evidence: helper `0x00E91F20` has a by-value CCharString ego_r declaration, one hidden output
word, and `ret 4` exits at `0x00E922E2` / `0x00E924E9`. The output appears only in result constructors
and returns. Recovery accepts the omitted entry-stack declaration and explicit CCharString_bv
export spelling, but rejects escaped outputs, wrong purge/signature, alternate returns and collisions.
`abi_evidence.txt` records retail instructions. No generated Lua was hand-edited.

Promoted exactly two draft and two readable Lua files (quest body and shared speech helper), plus
reports. GuildMasterGameFlow is unchanged. **100 focused tests and eight subtests pass**: 74 return/
cleanup cases plus 26 existing string/readability tests; eight unrelated generated/native string cases
were deliberately deselected. Tests exercise both helper contexts in both stages, 11 phase results,
three terminating branches, cleanup-before-return behavior and rejected evidence. Optional random
information selection is disabled in phase fixtures: cache correctness/random-call counts are NOT
claimed. The original in-process generator fixture hit the known Unicorn mem_map access violation;
the final fixture isolates constructor emulation in a subprocess and passed. No full-suite claim.

All **29 units** regenerated before/after with unchanged evidence: only GuildMaster's two Lua files
differ; **28 unrelated units identical**. Baseline snapshots match HEAD. Final draft equals the A/B
result, and promoted Lua equals tested scratch. **371 current Lua files parse**, zero failures.
Readable diagnostics 1439 -> 1419, mechanical only. Smoke JSON is byte-equivalent in content before/
after: two existing `in_stack_fffffff0` free-global reports remain, no new errors. Reports and source
hashes are in `ab/comparison.json`, `source_hashes.json`, `return_tests.log`, `focused.log`,
`review.diff`, `promotion.json`, `readability.json`, and `smoke_{before,after}.json`.

Further ABI findings are scratch-only, not merged into registered evidence: CRT rand import
`0x00BFEB16` is a cdecl zero-argument tail thunk, not the inferred four-argument stdcall. Literal
CCharString constructor `0x0099EBF0` returns its receiver in EAX (mov eax,esi / ret 8); typing it void
loses the source of GuildMaster's immediately following copy constructor. A read-only Ghidra export
with those corrections and the explicit hidden output succeeds (`typing_spec.json`,
`translation_unit_typed.json`, `typed_export.log`), restoring local_10 stack identity. It exposes
remaining lowering for the constructor-result alias, inline string equality/cache assignment, and
Hero Souls master-state field at +0x70. The registered export and specs remain unchanged.

Next: finish those generic lowering paths and test repeat-selection/random counts and Hero Souls
branches before merging the corrected export. Then recover GuildMasterGameFlow's distinct movie
cleanup continuations versus the true return epilogue; native evidence in the previous marathon
must remain separate. Witch progress persistence remains another task. v38 is unchanged: GuildMaster
is outside its roster. No new bundle, installation, game launch or push.

### GuildMaster cache / Hero Souls recovery and BeardyBaldy string stores

Continued offline from `8914c8e`, scratch `work/codex_guild_cache_20260928/`.
GuildMaster's speech helper now reads and writes LastDialogueSaid as a string, compares the selected
line by value, calls rand only for a repeated line, and updates the four one-time information flags.
Hero Souls selection now reads CurrentHeroSoulsPosition (+0x70) before reusing its master-pointer
register. The empty-string constructor/copy no longer loses the constructor's returned receiver.

Generic lowering changes are conservative: constructor-copy folding rejects pointer escapes;
string equality requires the entire identity/null/length/content test and a dead comparison-result
temporary on every later path; raw string-member loads require a recovered value consumer; master
pointer self-reuse is limited to known master fields. Incomplete pointer/memory operations stay
unresolved. First-pass Bordello/BookCollecting changes were rejected by these tighter guards and
are superseded, never promoted. Four genuine BeardyBaldy stores also recover RequiredBeard/RequiredTash
from definition strings at 0x468/0x460/0x478/0x470. Tests read those actual native decompile fragments
and execute their lowered setters with distinct string values.

Reviewed ABI overrides now persist for rand (0x00BFEB16, cdecl/no args), the literal CCharString
constructor (0x0099EBF0, receiver return in EAX), and GuildMaster's hidden result (0x00E91F20, ret 4).
Only the reviewed speech-helper function record was merged from the scratch typed export; the other
33 function records and the base export header remain unchanged. The three signature records are
updated. No generated Lua was hand-edited. Native helper instructions and the earlier export/ABI
artifacts remain available in this and the previous GuildMaster scratch directories.

Validation: **693 targeted tests pass against the registered evidence**, including 672 comparisons
of draft/readable and quest/shared helper output against retail x86 control flow, four 11-call repeat
sequences, conservative rejection cases, ABI bytes and four BeardyBaldy string stores. Compared values
include returned speech, cached speech, flags, query order and random-call counts. Native execution
mocks string primitives, rand and quest APIs; it is not an in-game test. **81 compatibility tests pass**;
a broader focused run also passed 133 tests (overlapping coverage; not a full-suite claim). Initial
pytest-native runs printed handled Windows memory-fault diagnostics despite passing; the final native
worker runs outside pytest's fault handler, completes cleanly, and checks process exit. An independent
Python 3.12 standalone native comparison also passed.

Final **29-unit A/B** changes only two GuildMaster files and BB_BeardyBaldyMan; **27 unrelated units
are byte-identical**. Before outputs were reused only after the first full comparison completed; final
after outputs regenerate every unit with the tightened guards. Promoted exactly three draft and three
readable Lua files plus reports. Final draft equals A/B and promoted Lua equals tested scratch.
**371 current Lua files parse**, zero failures. Readable diagnostics 1419 -> 1391, mechanical only.
GuildMaster smoke's two free-global reports are resolved: zero problems. BeardyBaldy has zero problems
before/after with the native parameter report. The scratch smoke without that report supplied a number
for SetWanderPointAndDistance's vector; `beardy_smoke_after_typed.json` supersedes that harness error.

Reports: `ab/comparison.json`, `ab/initial_comparison.json` (superseded), `source_hashes.json`,
`guild_final_tests.log`, `registered_guild_tests.log`, `compatibility_tests.log`, `focused.log`,
`promotion.json`, `review.diff`, `readability.json`, and smoke JSONs. Candidate v39 staging follows this
source checkpoint: only the BeardyBaldy roster file should change from v38. GuildMaster remains outside
the existing override roster. No installation, game launch or push. Next: GuildMasterGameFlow's distinct
movie cleanup continuations versus its true return epilogue; Witch integer-progress persistence remains
separate. Do not merge those cleanup paths merely because they unpause and destroy the same movie.

Final label-entry guard rejects comparison-result reads when a jump can bypass the replacement
assignment. Compared both guard versions on all 1,973 exported function inputs: no generated changes
(`label_guard_corpus.json`). Final targeted rerun: 693 passed. Source hashes refreshed after this guard.

### Candidate v39 staged offline

Source checkpoint **3a59649**, local only. `work/new-oakvale-original-fse-20260912/local-candidate-v39`
is v38 plus exactly BB_BeardyBaldyMan.lua with its four recovered requirement stores. GuildMaster remains
outside the 38-entry override roster. All other payloads match v38; only manifest/STAGING metadata also
changes. Sidecar remains **e83c39f**, DLL SHA-256
`f0ded2603690b8325fb0565d8aa0829b83887300f62c5553b03b828513e7df2c`.

Validation: **210 Lua files parse, 216 manifest hashes match, no new package smoke failures**; the same
16 old package mock failures remain. Both package smokes use the existing SickChild parameter report.
`stage_v39.py`, `validate_v39.py`, `package_validation_v39.json`, and `bundle_payload_diff.json` in the
current scratch preserve the procedure and exact comparison. No installation, game launch or push.
Next: GuildMasterGameFlow's movie cleanup continuations versus its true return epilogue; keep the separate
Witch progress-persistence issue on the list.
