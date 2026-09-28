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
