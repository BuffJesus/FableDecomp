# Script converter checkpoint — 2026-09-13

Superseded for resumption by [the night checkpoint](NEW_OAKVALE_CONVERTER_NIGHT_HANDOFF_2026-09-13.md). User stopped for sleep.


Continues [the 2026-09-12 handoff](SCRIPT_CONVERTER_HANDOFF_2026-09-12.md). That file's
chronological evidence still applies; this one records today's step and the resume point.

## What was done

- **Separate disabled resource-aware husband candidate now exists.**
  `python tools/script_recovery/generate_affair_man_resource_candidate.py` writes
  `refs/script_recovery/lifted/NewOakValeIntro/candidates/NOVI_AffairMan.resource_candidate.lua`
  plus a JSON report. It is not registered (`FSE/quests.lua` still `Quests = {}`), not installed,
  and the converter's `Entities/NOVI_AffairMan.lua` draft is untouched.
- The generator re-verifies `native_affair_man_resources_witness.json` (57 events, ten
  temporary Things, lifetime CFG) and the speech witness against native bytes, then applies
  fourteen counted rewrites (3 preparations, 6 acquisitions, 8 speeches, 19 task polls,
  1+1 clears, 3 animations, 1 move, 8 health Things, 2 home-distance Things, 1 construct,
  1 release, 1 dropped address-take). A count mismatch raises; nothing partial is written.
- One `man_resource` id spans Main inside `quest:WithRetailResources`; `resources:Speak` is
  the native non-waiting wrapper followed by the draft's existing `IsPerformingScriptTask`
  poll, which is retail's wait. Release sits at the DB1D8E/DB1D92 join reached by every
  constructed native path; termination before construction creates nothing. In the Lua
  candidate, errors use scope Close instead of this join (see review follow-up below).
- The prepared extension (`work/man_resource_extension/`, still **unapplied and unbuilt**)
  gained `ThingIsDistanceFromPositionOver` (retail helper 0x00CBE45C, prologue bytes checked).
  Note: the witness value 13362268 IS 0xCBE45C, the same helper the runtime's cached
  `IsDistanceFromPositionOver` binding calls. The harness covers the new method.
- Animation byte 0x01375748 is emitted as a raising stub (`__native_byte_01375748()`), never
  frozen; the three PlayAnimation sites therefore fail loudly if reached.

## Gates

- `python -m unittest tools.script_recovery.test_generate_affair_man_resource_candidate` — 6/6
  (counts, no cached control, hit-path event order, two cancellation paths, changed-draft rejection).
- Focused resource set (`test_retail_resource_actions`, `test_native_affair_man_resources`,
  `test_native_resource_lifetime`, the candidate tests) — 19/19.
- Full suite: 878 tests, only the known Bully two failures/two errors (`work/converter_resource_candidate_tests.log`; also noted in
  `docs/HANDOFF.md`). The known Bully two failures/two errors belong to the parallel session.

## Resume next

1. Cached woman/wife lookups (`GetThingWithScriptName`) and the hit-condition wrappers still
   use the draft lowering inside the candidate; lower their ownership from native evidence.
2. Movie/pause locals are not modelled as resources (`StartMovieSequence` / `EndMovieSequence`
   remain the draft's calls); check the native movie local lifetime before changing them.
3. Provide a reviewed runtime read for byte 0x01375748 (a narrow accessor, not a memory peek),
   then replace the raising stub.
4. Coordinate applying `work/man_resource_extension/resource-actions.patch` (five methods) with
   the runtime owner; build the full x86 runtime; only then run the candidate against native
   traces for normal, retry and cancellation paths.
5. Continue the four syntax-failing entity files (AffairWife, AffairWoman, BarrelMan, Bully).

## Recovered review and fixes (2026-09-13)

Workflow `wf_fa71fce5-a35` completed 11 verifier verdicts; 13 agents failed at the
spend limit, including all five research agents and the critic. The critic produced
no findings. Recovered complete verdicts are saved in
`work/man_candidate_review/recovered_verdicts.json`. Three completed lifetime reviewers
(`a51129eec605ef108`, `a5e0bd53ad7d220d1`, `a6581ef12296d72ec`) independently identified
the following concrete bugs, now fixed in the separate candidate generator:

- Hit movie pause: native DB0C88 pushes 1, DB0C8C calls game slot 0x5EC (379).
  Call-setup IR independently confirms receiver `[esi+4]`, target slot and true operand.
  The one hit-start pause now emits true; cleanup pauses remain false.
- Talk branch: three undefined `SUB41` calls assigned only to dead `uVar6` values.
  All executable occurrences in the current draft are assignments, with no consumers.
  Remove those three calls and reject future drafts that read uVar6. Tests execute both
  acquisition success and retry through clears and HOW_FIND_OUT speech, then cancel;
  another test cancels during talk acquisition.
- Generator now rejects failed Lua syntax before writing either output artifact.

The regenerated candidate uses sixteen counted rewrites. Candidate tests pass 11/11;
focused resource/extension/lifetime/candidate tests pass 24/24. The 878-test full-suite
result above predates these fixes; this follow-up did not rerun the full suite.

Corrected claim: the native CFG verifies the destructor join; Lua normal exits use it,
but errors (including the retained animation-byte stub) use `WithRetailResources` Close.
Runtime `LuaRetailResources.h` confirms Close destroys live entries and only unpauses
state set by its own Pause method. It cannot undo the candidate's quest movie/pause calls.
That error-cleanup gap remains open. Reviewers also found that Add appends entries and
Destroy only marks them dead, so repeated Thing queries grow storage until Main exits.
Resolve both with the runtime owner before live integration; do not naively reuse IDs
and allow stale handles to refer to new objects. The animation byte remains unresolved.
The workflow prompt also misnamed the task poll: it is 0x7E7450; 0x7E74D0 is the resource
destructor. The candidate's existing resource witness already has the correct addresses.

Runtime source, extension proposal, registered draft and installed game were not changed
by this follow-up. The candidate remains disabled and has no gameplay-parity claim.

## Movie resource continuation (2026-09-13)

The movie/pause error-cleanup gap above is now resolved in the disabled candidate's
offline model. `verified_movies` rechecks the existing movie witness's native source
and body/helper hashes, fourteen call setups, empty class string at 0x122D70E, start
binding (game slot 0x5C8 -> 0x89B110), and single-active-resource CFG. Starts at DB0C71
and DB0E3D use stack slots 96 and 72 respectively; twelve destructor sites call 0x6E7B80.
The two lifetimes never overlap, allowing one Lua movie-ID variable.

The generator now emits `man_movie = resources:StartMovie("")`,
`resources:DestroyMovie(man_movie)` and `resources:Pause(...)`. There are two starts,
33 expanded Lua destructor tails and 35 pause sites. These are layout counts, not
claims that native has 33 destructor call sites. Total counted rewrites: nineteen.
The existing bridge already supports these methods; no extension or runtime edit was
needed. StartMovie constructs and initializes the movie and calls the native start;
DestroyMovie calls the native destructor. Close unpauses then destroys live entries
in reverse order before WithRetailResources rethrows a Lua error.

Recording Lua mocks now track live entries, reject double destruction and overlapping
movies, and model Close on success and error. Tests inject health and speech failures
on hit and talk branches and verify unpause, temporary Thing destruction when needed,
movie destruction, resource release, and error propagation. Repeated hit movies destroy
distinct IDs exactly once. Changed destructor bytes and a rejected movie CFG fail closed.

Validation: candidate 15/15; combined resource/action/lifetime/candidate and related
movie recovery/destructor tests 36/36. Full recovery suite was not rerun. Native CFG
and mocked execution establish offline evidence, not full x86 ABI or gameplay parity.

Remaining: cached woman/wife and hit-wrapper ownership, animation-byte runtime access,
and storage retained by dead Thing **and movie** entries until Main exits. Runtime-owner
integration and native trace comparisons remain required. Candidate regenerated;
registration is still disabled; runtime checkout and installed game remain untouched.

## Active completion marathon (2026-09-13)

User requested a plan, sustained work until New Oakvale is complete, and human-readable
Lua. The active goal is not complete. Plan: `docs/scripts/NEW_OAKVALE_CONVERTER_PLAN.md`.

- `readable_lua.py` and `build_readable_new_oakvale.py` produce a separate disabled
  readable package with per-function diagnostic/readability ledgers and reversible
  native-local name maps. Strings, comments, members and table keys are protected.
  Consistent call/state results get semantic names; conflicting/reused values remain
  honestly identified scratch values. Actual husband trace comparisons pass.
- `ReadAnimationArgument5` reads the relocated volatile byte 0x01375748 each time,
  returning a bool. Three native read instructions and fifth-argument IR are checked.
  The raising stub is gone; tests run the cowering path with false then true. The
  argument's gameplay meaning/writers remain unknown, but no constant is assumed.
- `IsHitByHeroExceptAbility(me,14)` preserves native short-circuit slots 0x54/0xA8/0xA4
  and reverse string destruction. DB0B30..DB0C1C owns CCharStrings, **not condition
  objects**. New byte/hash and Lua correspondence witness records the lowering.
- Cached woman/wife Things now use explicit lookup IDs and reverse destruction at
  DB1D7C. Their lookups, names, receivers, destructor sites and independent CFG
  lifetimes are rechecked. The bridge resolves IDs or owned/borrowed actor userdata
  for distance, facing and conversation calls without exposing Thing pointers to Lua.
- `retail_resource_storage.inc` and `prepare_resource_storage.py` combine the action
  extension with bounded storage. Explicit destruction erases entries; IDs increase
  monotonically and never repeat; exhaustion rejects instead of wrapping. Close
  retains reverse creation-order destruction. A 100,000-cycle C++ test verifies
  bounded allocation count, stable live pointers, stale handles and exhaustion.
- Combined proposal: `work/man_resource_integration/resource-integration.patch`;
  eleven bound methods. `git apply --check` passes against the untouched runtime.
- `run_resource_candidate_checks.py` compiles actual proposed headers, vendor sol/Lua
  and MSVC x86 against engine doubles. Successful latest run:
  `work/new_oakvale_converter_resource_x86_20260913_04/result.json` (binary SHA-256
  `08cd566aa9762c2c87870eb95f6ee2a8667fe49f1da87ff65471d63c5cfbfef5`). It tests hit
  truth tables/string lifetimes, dynamic-byte boolean conversion, captured Thing
  distance/facing/conversation arguments, owned and borrowed userdata, invalid IDs,
  10,000 Thing/movie cycles, error cleanup and the existing resource smoke cases.
  Earlier run 01 failed linking the harness's missing g_fableBase; the dedicated
  candidate harness now supplies a relocated test byte. Runs 02/03 passed prior scopes.

Current candidate tests: 17/17. Last combined Python/C++ focused batch: 39/39 before
the final cached-Thing pre-loop cancellation/lookup-error test was added. No whole
recovery-suite rerun or full runtime DLL build yet. Remaining native draft diagnostics
are not removed just because the candidate fixed their behavior. Raw conversion still
has four syntax failures and 1,296 diagnostics; readable output does not mask them.
Continue the active plan rather than marking the goal complete here.

### Marathon continuation: readable data flow and position recovery

- Added conservative reaching-definition grouping in `lua_local_versions.py`.
  Reused native locals split only when no read joins their definitions; loop-carried
  values stay joined. Unsupported control-flow/closure shapes reject splitting.
  Dead literal-only staging can be removed, with original lines in the source map.
  Lexer handling now protects long strings/comments, table keys, members and `..`.
  Actual barrel platform/instruction branches and husband paths retain their traces.
- `native_quest_vector_fields.py` verifies the barrel death callback's 49 native
  bytes and maps BarrelBrokenPos to three transient Float keys. The persistence
  manifest's string representation is not changed. The callback copies XYZ, and
  WatchBarrels passes that snapshot to CreateCreature. Its byte-verified health
  argument is 2.0 with set-current=true, not integer 0x40000000.
- Added proposed global `RetailThingPosition`: query implementation slot 0x18 or
  copy the current relocated vector at 0x0143E8E0. Lua nil represents Forge's empty
  lookup wrapper and takes the same fallback. Snapshot copying, changing fallback,
  present/empty/nil actors pass the actual MSVC x86/sol/Lua engine-double harness:
  `work/new_oakvale_converter_resource_x86_20260913_06/result.json`.
  Combined proposal now has eleven resource methods plus this global binding.
- Wife position recovery preserves the cached husband and MoveToPosition arguments
  (position,2.0,1,false,true). Unused interface/vtable loads are removed only when
  their entire chain has no surviving use; removed statements are recorded.
  The wife file now compiles. Resource ownership and other diagnostics remain.
- Woman run-off recovery preserves the marker snapshot, self movement/pushable
  flags (both true), and movement (position,0.0,1,false,true). The native joined
  loop label is normalized without changing its target. Woman now compiles;
  resource distance queries, cleanup, camera/removal and other branches remain.
- First BarrelMan walk-off recovery uses the existing native actor/lookup evidence
  and rechecks movement setup. Both distance checks use the same snapshot and 2.0
  threshold; movement uses radius0,type1,false,false. Found and fixed `= me;` being
  discarded as noise before alias tracking. Later positions and switch dispatch
  still need recovery.
- Full suite before these last position changes: 909 tests, two failures and two
  errors, all stale Bully cancellation camera expectations. Inspected DBCC00..41:
  cancellation unpauses and destroys resources, bypassing normal DBCC76 camera
  unfix. Updated expectations and added direct native branch/call checks. All six
  Bully byte-audit tests pass. Second full-suite log is
  `work/converter_marathon_suite_20260913_02.log` (check completion/result).

Latest completed pre-BarrelMan generation: 49/51 functions and 16/18 files compile,
1,282 diagnostics. Remaining syntax failures: BarrelMan and Bully. Readability
generation previously had 566 semantic names and 388 scratch names; regenerate
after the current raw conversion. Goal remains active; nothing deployed or committed.

### Next checkpoint: control-flow backend and camera choice

- Second full suite completed: **918 tests passed**, log
  `work/converter_marathon_suite_20260913_02.log`. PowerShell's stderr redirection
  reported NativeCommandError for unittest progress despite the test result OK;
  the next run uses Python subprocess redirection and propagates its exit code.
- BarrelMan phase 2 now snapshots the primary marker position, checks the camera,
  and chooses primary when offscreen or alternate when onscreen. Both native
  termination checks are retained. Normal-branch tests prove the chosen actor and
  false teleport flag. Cleanup jumps and switch dispatch remain separate gaps.
- Latest canonical generation: **49/51 functions, 16/18 files compile; 1,277
  diagnostics**. Readable output regenerated: **570 semantic names, 387 scratch
  names**, 957 renamed locals. Wife and woman compile; BarrelMan's remaining
  position expression is in phase 3 (cached M_WHouse_ManStart), Bully starts with
  unresolved native Thing dispatch at slot 0x6C.
- `native_flat_control.py` and opt-in converter `--flat-control` preserve native
  switches, fall-through, loops, break/continue, arbitrary label destinations,
  comma conditions and short-circuit evaluation in a single label scope.
  Six isolated execution/parser tests pass; existing goto tests pass. Canonical
  generation is protected from being overwritten by this experimental mode.
  First full experiment: `work/new_oakvale_flat_control_20260913/`; **not a usable
  package**. It exposes native refcount pointer conditions previously omitted by
  the structured emitter. Unresolved condition and alias-data-flow limitations are
  explicit in the new metadata. Re-run to include the latest comma-condition work.
- Third full suite: **925 tests passed**, exit 0, 288.898 seconds:
  `work/converter_marathon_suite_20260913_03.log`. A final experimental-mode numeric
  literal truthiness edge case was added afterward; all six flat-control tests pass.
  Latest runtime proposal still passes apply --check; x86 bridge run06 remains
  current. No runtime/game files were changed.

Prepared but not wired: `native_barrel_return_position_witness.json` records phase
3's cached ManStart lookup, position copy, two resource distances and movement.
Lookup DB54B5 writes baseline36; query DB5915 reads its implementation40. Acquisition
DB58B2/DB58E4 passes EDI=self, resource20, priority4. Repeat getter DB5A04 has resource20
and output244; repeat distance DB5A12 uses vector196 and float2. First getter DB5957
and first distance DB5965 currently exceed read_call_window's supported setup and
return None; their inspected instructions show the same stack relationship, so do
not invent a successful IR result. Movement DB59A8 uses vector196,radius0,type1,
false,false. Existing actor lowering only covers phase 1; extend with verified phase
3 acquisition evidence. Bully's seven unresolved calls need exact native actor/argument
recovery; teddy dispatch bytes already exist in ghidra_out. Goal stays active.


## Marathon continuation: Barrel return, switch and pause/hit control

- Phase 3 now queries the cached `M_WHouse_ManStart` marker and shares its copied
  position across both distance polls and movement. Reacquisition, repeated getter,
  distance and movement call-window checks guard actor/vector relationships. The
  first getter/distance pair is documented as inspected whole-function native evidence;
  the call-window decoder does not recover that pair, and the report says so.
- `native_structured_switch.py` lowers only switches whose cases have terminal
  break/goto/return, with one selector evaluation. A single-iteration loop preserves
  case breaks inside the surrounding loop. Fall-through, outer-loop continue, and
  unsupported case forms reject recovery. Four focused behavior/rejection tests pass.
- `native_barrel_pause_witness.json` recovers five saved-interface calls: one true,
  four false. DB61DA..DB61ED explicitly guards the true operand and saved receiver;
  false paths retain call-window checks and DB6AE0..DB6AF4 join instructions.
- `native_barrel_hits_witness.json` recovers the short-circuit hit result without
  a nested-entry jump that the draft replaced with return. All eight boolean
  combinations and query order are tested. String mask/destructor ownership remains
  visible and unresolved; this is not a resource-aware Barrel candidate yet.
- Canonical converter and readable package regenerated: 51 functions, 49 function
  syntax passes, 16/18 file syntax passes, 1,255 diagnostics; 572 semantic names and
  387 scratch names (959 renamed locals). BarrelMan still fails at a raw interface
  load, and Bully at an unresolved native message call. Husband candidate remains
  disabled and passes its generation syntax check.
- Focused return/switch/flat-control/pause tests: 20 passed; hit tests: 2 passed.
  Full-suite run is recorded in `work/converter_marathon_suite_20260913_04.log`;
  check its final result before treating that gate as passed.


## Marathon continuation: full syntax coverage, behavioral work remains

Canonical generation and the readable package now compile all 51 functions and all
18 files. There are still 1,227 diagnostics; 28 function bodies have none. The
readable package reports 577 semantic names and 383 scratch names, with reversible
source maps. Registration remains disabled. This is a milestone, not completion.

- Barrel dispatch cleanup removes nine specifically reviewed interface/vtable
  assignments while preserving earlier uses of the same decompiler names.
- Seven Barrel health queries now consume the actor returned from self-resource20.
  All ten acquisitions, seven getter/query pairs and float-zero bytes are checked.
  Three packed-byte results become `native_arg_barrel_has_health`; x87 unordered
  comparison is false, tested with NaN as well as zero, negatives and infinities.
- Barrel departure fades out for 1 second/holds 1 second, pauses for 2 seconds,
  teleports hero to `M_WHouse_GuardPoint` and self to `M_BarrelManHiddenPos`, both
  with false native flags, then fades in. Native byte region guards exact order,
  arguments, marker strings and the opaque black color used by the host wrapper.
- Bully's five hero talk/hit slots now have verified self receivers and ability14.
  The teddy-offer predicate preserves talk -> optional hero -> optional inventory
  order. The existing runtime inventory binding was added to the converter's SDK
  overlay, preserving any newer supplied contract.
- Bully's two presented-item calls remain separate. Exact teddy comparison replaces
  the pointer comparison and impossible empty-string match; failed query results
  guard global reads. Native `ToConstChar` at99E4C0 returns the pinned empty string
  for an empty wrapper, so the existing host publishes empty as well as nonempty
  names. Tests cover stale globals, failed queries, empty strings and other items.
  Native output/string lifetime is still an explicit draft limitation.
- Bully's hit result now uses a dedicated boolean instead of a decompiler's reused
  pointer/high-byte slot. All eight combinations preserve query order and exclude
  ability14. String construction/mask cleanup is not claimed integrated yet.
- Parent field loads/stores accept decimal offsets as well as hex, but still require
  the entity inventory's declared owner and field mapping. This fixes Bully's
  `GUIBullyHealthCounter` offset100/0x64 without inventing an entity-local field.
- Frame bridge dispatch is handled before operand placement, keeping its implicit
  execution actor distinct from native operands. Current SDK already has no frame
  parameters, so this does not account for a diagnostic-count reduction.
- Missing-argument diagnostics now name the API and missing parameter names.

Validation chronology: `_04.log` passed 935 tests. `_05.log` ran949 and failed only
because `test_new_oakvale_conversion` still asserted that package syntax must fail.
That expectation now requires successful syntax while still requiring visible
remaining diagnostics and `Quests={}`. Its10 tests pass. Lifter/frame/candidate
focused batch99 passed; the newer hit-result tests2 passed; lifter diagnostics
batch80 passed. `_06.log` is the next full suite; inspect its final result.

Next meaningful work: the compiling bodies still have unsupported or substituted
cleanup jumps, implicit cached-control operations, and missing native arguments.
BookTrader's home movement is a small concrete next target: its return-home getter
and distance vector remain unresolved, and its movement argument placement is
wrong. Resource maps already exist for BookTrader, wife and PostAttackSequence, but
only the husband's separate candidate has explicit resource/movie/Thing ownership.
Do not promote the experimental flat backend before its operand/ownership gaps are
resolved. Runtime checkout and installed game remain unchanged.

Full suite `_06.log`: **953 tests passed**, 308.257 seconds, exit0.

User asked whether the debug PDBs can help. Direct DIA extraction confirms original
function-local names/types exist in Ego_r.pdb and richer lexical scopes/locals in
FableWin.pdb. BarrelMan examples: seh_me, guard_hero_start_position,
guard_man_start_position, walk_away_pos, return_pos_1/2, local_position, speech_movie,
PauseMyEntitiesPlease and WaitTimer; FableWin also has local_movetype/local_range.
Ego_d has no matching BarrelMan::Main in this query. Tool source:
`runtime_checks/pdb_locals.cpp`; compiled extractor and initial TSVs under
`work/pdb_locals_20260913/`. Whole-quest extraction is in progress. PDB offsets and
addresses are cross-build evidence, never automatic retail replacements.


## PDB catalog and BookTrader/BarrelMan continuation

Whole-quest PDB extraction finished: Ego_r100 functions/317 data symbols and
FableWin133/465. Original qualified-name matches cover49/51 converter functions;
Barrel Init and CreatedBeetle Init have no match in these query results. Retained
TSVs and scope-preserving `pdb_locals.json` are under the intro evidence directory.
Readability reports link this catalog without asserting debug-to-retail local or
address equivalence. See `docs/scripts/NEW_OAKVALE_PDB_EVIDENCE.md` for reproduction.
New parser rejects truncated output, mismatched counts and impossible scope depths.

BookTrader home recovery now proves the initial0.1 distance snapshot separately
from the repeated2.0 distance/movement snapshot; movement is position,0.0,0,false,true.
Health recovery proves seven ordered health>0 gates, including NaN rejection, and
recovers controlled-actor aliases while retaining native temporary cleanup.
Existing movie evidence already covers its two movie locals. Two remaining source
MovieBase destructor names are decompiler errors: native DB41C7/DB4F5A release the
controlled entity resource at frame20; do not emit extra EndMovieSequence calls.

BarrelMan Init now resets the actual parent WatchTimer to0, copies all three home
position components to transient WarehouseMeetPoint Float state, supplies all
native killable/information flags explicitly, and sets sight radius10.0 (not integer
bit-pattern0x41200000). Whole167-byte Init, source, contracts and parent ownership
are guarded. Init now has zero diagnostics; tests vary timer IDs/coordinates and
check source-table mutation cannot alter the copied position.

Current generation:51/51 functions and18/18 files compile;1,213 diagnostics;
29 functions have none. Readable:963 renamed locals,581 semantic and382 scratch.
Focused checks:17 catalog/home/conversion tests,7 health/vector tests,12 Init/conversion
tests passed. Full suite `_07.log` is running; previous953-pass `_06.log` remains the
last completed full gate. No runtime, game, registration or commit changes.

User explicitly requested keeping one background agent on other incomplete ports.
After delivering BookTrader, that agent moved to ScytheInfo: isolated new modules,
byte evidence, disabled output and tests under its own paths. Root continues New
Oakvale and owns shared converter integration. ScytheInfo has missing entity bodies
and misassigned objective/timer operands; do not copy the old draft as correct.


Full regression `_07.log` finished: **966 tests passed**,343.792 seconds, exit0.
PowerShell's redirected stderr adds a NativeCommandError formatting prefix for
unittest's progress stream; the process exit and final unittest result are both
successful. No restart or duplicate suite was launched.

## Main mission and attack transition operands

The next pass pins DoMission DBDE40 and AttackStuff DBE3C0 native bytes, annotated
source, host signatures, relevant literal bytes and GetHero/GetActiveQuestName ABI.
GetHero returns an interface-owned reference with plain ret; GetActiveQuestName
consumes only its hidden output buffer with ret4. This proves the other staged
arguments belong to the subsequent call.

DoMission fixes initial/final fades2.0/0.0 and0.5/0.0, music set46, midday12.0,
hero killable false/false then true/false, sleeping false then true, the actual house
lookup for unlock/open, quest screen false/true and completion false/false/false.
Two existing runtime house bindings are added to the converter's missing-signature
overlay without replacing newer manifest entries. AttackStuff fixes immediate
pre-attack deactivation, time23.0, zero-duration theme transition and explicit
objective/empty-region arguments. These fixes also remove silent wrong numeric
values, not just visible missing-argument diagnostics.

A provisional time-stop token diagnostic was investigated and removed: runtime
LuaQuestState.cpp9596 forwards its per-instance m_stopTimeIndex (header1080) to the
native API on both stop/resume calls. Both mission helpers share that quest host.
Do not report this as an unimplemented bool-only output again. Native temporary
Thing/string lifetime and full runtime/gameplay parity remain separate concerns.

Validation: mission/lifter/conversion93 tests passed; expanded mission/attack
behavior and mutation tests5 passed. The966-test full gate predates this newer pass.

Current regenerated mission batch:51/51 functions and18/18 files compile;1,203
diagnostics;31 functions have none. All reported recovery passes accept their
evidence. Readable counts remain963 locals,581 semantic and382 scratch.


## BookTrader complete native lifetime map

New `native_book_trader_lifetime.py` and witness recheck the existing acquisition
map, then all54 controlled-resource events and24 temporary-Thing events (8 lifetimes).
Construction is DB3FFC/frame20. Four exits are DB41C7, DB4F5A and inline pImp release
followed by base destructor DB4EB6/DB4EFF. The early entry cancellation skips
construction. Native CFG proves exactly one destruction on every constructed path.
The conditional ECX selection at DB41A1 is followed through the branch to task query
DB41AB. Coverage separately inventories all resource helper calls so omitting a
speech/use cannot pass merely because start/end still balance.

Seven health Thing temporaries use ordinary destructors; the distance Thing at
frame136 has inline callback/free/weak-handle cleanup ending DB4147. Each getter,
query and destructor is checked against call-setup IR and full native CFG. Native
helper/vtable byte regions include the zero-argument GetHero reference-return ABI.
Three new tests cover complete maps, omitted uses/exits, wrong branch selection,
wrong temporary identity, missing temporary destruction and altered cleanup/ABI.
Combined lifetime/CFG/mission test batch12 passed. Report field is
`bookTraderLifetimeEvidence`; this does not itself rewrite the Lua candidate.

Next: use the54-event map to generate a separate disabled BookTrader candidate,
following the existing AffairMan resource scope conventions. Preserve one actor
resource across4 preparations/8 acquisition call sites; lower7 speech calls,
16 task polls,1 move,1 animation (live animation byte),8 temporary Things and
all4 destruction routes. Existing BookTrader movie witness already proves2 movie
scopes; retain their explicit pause/cancellation order. Review the two Theresa
lookup/facing wrappers and hit-string lifetime too; do not declare full ownership
parity from the controlled-resource map alone.


## BookTrader candidate and readability follow-up

The disabled BookTrader candidate now lowers the verified resource/Thing/movie map
and is substituted into the readable package. Nine candidate tests cover hit event
ordering, cancellation, movement, health/NaN, live animation reads and error cleanup.
The native DB4234..DB430B hit-string scope now uses IsHitByHeroExceptAbility(me,14).
Three Theresa facing sites preserve false/true/false snap flags. Getter8A7D60 ret8
leaves the previously pushed facing flag on the stack; facing88E620 consumes ret12.
Thing/name lifetimes around those three lookups remain open, as do conversation
and entry-predicate integration. Do not mistake this operand correction for cleanup
parity. Candidate plus runtime-action tests:12 passed; readability plus candidate:20.

Readable husband labels now have reversible purpose names in the source map, and
owned named Things infer affairWoman/affairWife. Latest build:18/18 Lua files compile,
583 semantic locals/380 scratch locals. User explicitly clarified that the endpoint
is structured Lua/helpers and actual ForgeFSE APIs, not just renamed goto labels.
The completion plan now records that distinction. No runtime/game files changed.


## Husband structured control flow

`structure_affair_man_lua.py` now runs after local-name recovery in the readable
builder. It extracts actor/control lifetime bodies into local functions, using
returns to reach original destruction sites. The native backward jump becomes a
while loop; processInteraction returns false on cancellation and true to advance
exactly one frame. Shared conversation tails become finishConversationMovie and
facePartnerAndFinishConversation. There are no executable goto statements or
labels in the readable husband file. Candidate/native evidence remains unchanged.

Two focused tests compare exact traces with both the resource candidate and its
renamed predecessor for225 scenarios:30termination boundaries across7 scenarios,
plus15injected-error cases. They include hit/talk/retry/idle/acquisition failure and
woman/wife absence. The shared mock was extended locally to record information
clearing and select actor availability. This is control-flow equivalence evidence,
not full gameplay validation. All18readable files compile. Full suite is running
under `work/converter_marathon_suite_20260913_08.log`; do not record it as passed
until the live process completes. Operand/scratch cleanup and runtime parity remain.


## Literal staging, named-facing scopes, and condition audit

`lua_literal_values.py` removes a temporary only when a single literal assignment
dominates every read in the supported CFG. Conditional/zero-iteration/uninitialized
reads, effectful assignments, reassignment, unsupported closures and targets are
retained. It records prior lines for exact reversal. Builder applies this after
version splitting and before semantic naming/structuring. Six targeted tests pass;
the225husband trace comparisons still match. Latest package:806renamed locals,
545semantic/261scratch (157fewer locals,119fewer scratch values),18/18syntax passes.
Declaration-only lists wrap to100columns. This changes no initializer evaluation.

BookTrader's three Theresa lookups now invoke the Scythe agent's reusable proposed
quest:FaceThingByScriptName(me,name,snap), preserving key?Thing?facing?Thing cleanup?key
cleanup. Snap flags remainfalse/true/false. Exact fragment is
`tools/script_recovery/scythe_thing_adapters.inc`, SHA256
14b3707e393753000455b106c8549387a075526dd363a70fe6e699bfb113f1c9;
compiled x86 gate metadata is `work/scythe_converter/runtime_proposal/proposal.json`.
This quest extension is required explicitly by the BookTrader report; runtime is
unmodified. Ten BookTrader tests pass, including facing errors during both plain
resource and active movie scopes. Conversation/entry conditions remain open.

Full suite08 finished1010tests/318.527s with4Scythe cleanup fault-injection failures.
The agent corrected Python/Lupa exception bridging in those tests to inject actual
Lua errors; root reran cleanup plus literal/structure/readability21tests successfully.
Combined full suite09 is running under a Scythe source freeze, session88175; do not
claim a full pass until completion. Agent is doing read-only audit while frozen.

Next shared semantic gap: work/new_oakvale_condition_audit.json inventories15native
entry registrations among the actual NewOak inventory (translation unit also contains
neighboring unrelated quests, excluded from this audit). Fourteen usevtable12C2FE8:
0xCDDAB0 calls Thing.IsAlive slot12C then rejects IsUnconscious slotF4;
cloneCDDAE0 creates16bytes and counted-copies the Thing; BookTrader registrationDB3FCF
precedes first frameDB3FE6. DeadFather alone uses existence-only vtable12C32F0 among
these15. Restore native cloned condition registration, not Lua health polling. Existing
RegisterBoundAliveCondition handles only the latter predicate; the conscious condition
needs a reviewed binding/adapter and site-by-site entry ordering. Remaining entity
entries outside these15need separate inspection; this scan does not prove absence.


## Shared native entry conditions restored

Full suite `_09.log` passed 1,019 tests in 324.897 seconds, exit 0. Scythe source
freeze ended and the background agent moved to isolated V_MazeResearch recovery.

The readable builder now restores native conditions in 15 entity mains: 14 use
RegisterBoundConsciousCondition; DeadFather uses the existing RegisterBoundAliveCondition.
Each reviewed entry copies bound self at entity +8, calls F35B10 to clone/register,
and destroys the caller copy before its first frame. Native helper, predicate,
clone, destructor and entry bytes are pinned by native_new_oakvale_conditions_witness.json.
Lua entry prefixes are pinned separately for raw and resource-candidate variants.
Other pre-frame behavior cannot silently drift past the insertion point.

`prepare_new_oakvale_conditions.py` stages an unapplied LuaManager/header patch under
work/new_oakvale_conditions. The real MSVC x86 harness executes the original 44-byte
retail predicate at its actual calling convention against controlled Thing vtables.
All four alive/unconscious combinations and short-circuit behavior pass, as do clone
retention, replacement, missing-thread rejection, registration failure cleanup and
real sol/Lua binding calls. The patch passes git apply --check. No DLL/game install.

Four Python tests cover all 15 first-frame/cancellation orders, resource candidates,
changed bytes/prefixes and duplicate registration rejection. Condition, structure,
and readability focused batch: 17 passed. All 18 readable files compile with 15
condition entries reported. Full suite09 predates this new condition batch.

Guard is the remaining entity Main: complete 5,864-byte scan found no F35B10 call.
It constructs a control resource at DAC776 before checking termination at DAC78D.
work/new_oakvale_guard_condition_audit.json records this exception; no condition is
invented for Guard. Additional mid-body condition replacements elsewhere still need
audit; this pass establishes entry behavior only. BookTrader conversation operands
and ownership, other actor resource lifetimes, and full runtime/playthrough checks
remain open.


BookTrader follow-up validation: 12 candidate tests now pass. Added purchase flow
coverage for gold 0/2/3/99, exact object?gold?objective?GivenSweets?information order,
No answer, cancellation while a question remains pending, and a second interaction
that neither grants nor charges twice. These exercise previously untested generated
branches; native conversation ABI/string ordering still needs its explicit audit.
No additional game/runtime changes were made.

Final focused condition/candidate/structure/literal/readability batch: 52 tests passed in 11.625 seconds.

## BookTrader structured readability and conversation audit

The readable builder now applies `structure_book_trader_lua.py` after local analysis.
BookTrader has no executable goto or native labels: `prepareAndReturnHome`,
`processInteraction`, `finishTrade`, and `tryIntermittentLine` preserve the phase
boundaries and release the resource after the outer loop. Its original scoped
cleanup helper is retained. Before/after event traces match for 240 scenarios
covering purchase, refusal, hit, movement, acquisition and cancellation, plus
three injected-error cases. Shared local analysis now understands inline loop
breaks and names known conversation/random/distance/Thing-alive results.

Current generated package: 51 functions, 18/18 files compile, 861 semantic local
names and 210 unresolved scratch names. Raw diagnostic count remains 1,203.
The focused readability/BookTrader/husband/entry-condition set passes 44 tests.
The last full suite remains `_09.log` (1,019 passed), predating this batch.

Conversation audit: `work/book_trader_conversation_audit.json` records native
call operands and byte hashes. Creation uses self/false/false, and the subsequent
person addition consumes that conversation and the native GetHero result.
A remaining ordering gap is concrete: native constructs the line key before
GetHero, whereas current Lua obtains the listener before the host constructs its
FableString. A scoped line-to-hero binding needs native GetHero ownership review
and runtime validation before this gap can be closed. Host participant-validity
checks also require parity review. Runtime proposals remain unapplied; full DLL
and gameplay validation remain outstanding.

## Scoped BookTrader conversation line

The native intermittent line now lowers to `quest:AddConversationLineToHero(...)`.
`native_book_trader_conversation.py` checks pinned call windows, vtable bindings,
text literal, and symbolic call operands. GetHero returns the interface-owned
Thing at interface+0x30 (891CB8/891D46), so the binding borrows it without a Lua
copy or caller destruction. The scoped binding constructs the key first, queries
the hero, queues the line, and destroys the key in native order.

Staged proposal: `work/book_trader_conversation/conversation-integration.patch`.
It passes git apply --check and the actual MSVC x86/API-type/vendor sol+Lua harness,
including populated/empty hero values and lookup/line exceptions. Native evidence,
BookTrader candidate/readability and entry-condition tests: 21 passed.
All 18 readable files compile; 860 semantic names and 210 scratch names remain.
One temporary disappeared with the scoped binding. Runtime is unchanged; the
creation/person validity guards, full DLL build and gameplay parity remain open.

## Conversation setup runtime proposal

The staged BookTrader adapter now also provides `StartConversationWithHero`:
native CreateConversation -> GetHero -> AddPerson -> return conversation ID.
It avoids the existing host IsNull query and preserves participant addition for
zero/negative IDs and actual empty hero values. Updated two-method patch passes
git apply --check. The x86/API-type/vendor Lua harness verifies both flags,
IDs -1/0/73, populated/empty participants, call-stage errors, and no extra host
validity query. See `work/book_trader_conversation/proposal.json` and build logs.

The converter still emits the separate setup calls pending the running full
suite10 source freeze. Next: strengthen native setup callee pins, lower the
three-call setup to this binding, add caller tests, regenerate readable output.
Full suite10 is confirmed live at this checkpoint (session25209); poll that
handle and read its log rather than starting another suite.

## Conversation setup lowering and BookTrader readability

BookTrader now uses both tested scoped bindings: `StartConversationWithHero` and
`AddConversationLineToHero`. Native create/person callees are pinned in addition
to the caller windows and GetHero/line callees. Creation/person tests preserve
both flags, empty participants, IDs -1/0/73, error propagation and cleanup. The
converter no longer emits the separate host calls with extra IsNull queries.
Both runtime methods remain staged in the conversation integration patch.

BookTrader's readable output has no executable labels/gotos, TODO markers, or
unresolved scratch local names. Question-answer and guarded-distance results
now get semantic names; a second checked literal cleanup removes staging stores
made provably unused by literal inlining. All 18 files compile; current package
counts are 873 semantic names and 192 unresolved scratch names. Historical raw
diagnostics remain 1,203 and do not equal current surviving TODO markers.

Full suite10: unittest reports 1,042 tests / 262.565s / OK. The PowerShell wrapper
returned 1 after wrapping stderr as NativeCommandError; the separately recorded
result JSON distinguishes that from the unittest verdict. After the setup and
readability changes, 47 focused tests pass. Runtime/gameplay parity is unproven.
Next substantial work: remaining ownership/control-flow gaps in the other New
Oakvale actors, then combined runtime integration and live trace validation.

## AffairWoman loop and departure operands

Readable builder now applies `native_affair_woman_control.py` against the pinned
2714-byte Main and exact raw draft. It repairs the loop's unresolved extraout_AL
values using the already computed termination boolean. The two camera queries
now consume the bound woman's current GetPos result (native DB290D/DB293D ->
DB2914/DB2944), and removal uses self/false/true atDB2965. The prior draft passed
missing camera positions and removed the run-off marker instead of the woman.

Nine focused control/position/entry-condition tests pass, including18 departure
camera/cancellation traces and mutated native/draft/binding rejection. Resource
reset and final temporary cleanup are explicitly outside those trace assertions.
Woman resource/movie/named-Thing ownership, home/run-off controlled distance
queries, partner distance operands and remaining body branches are still open.
Do not treat readable syntax or these partial operand checks as completed parity.

## AffairWoman controlled-resource map

`native_affair_woman_resources.py` now verifies all39 native resource events,
including6 acquisitions, five temporary-Thing getters, action/task calls and
cleanup. Full aligned CFG validation proves one stack+16 constructorDB1F5C and
one destructorDB298A on every constructed path; entry cancellation bypasses both.
`work/affair_woman_resource_audit.json` records the verified operands and map.
The map does not yet lower the Lua or prove temporary/named-Thing/movie lifetimes.

The shared argument verifier now retains literal/address arguments across writes
to provably disjoint stack slots; overlapping slots and memory-derived arguments
remain invalidated. This recovers DB280B's output stack136 and resource stack16
without relaxing unknown-pointer alias rejection. Twenty call-setup tests pass,
including disjoint/overlapping writes and memory-loaded argument rejection.
Next: map five temporary getter/query/destruction lifetimes, movie and named-Thing
ownership; generate the separate disabled woman resource candidate, then structure.

## AffairWoman temporary and movie lifetimes

The resource verifier now checks five temporary getter/query/destruction lifetimes
against the full CFG and pinned resource helper bodies. Three distance queries
use the returned Thing with threshold2; two health queries also consume the
returned Thing, not the cached husband currently emitted by the draft. The home
temporary's inline strong cleanup and final weak cleanupDB20EE are accounted for.
Changed output/missing temporary/early destructor evidence is rejected.

`native_affair_woman_movies.py` verifies two nonoverlapping movie scopes (stack148
and96), ten constructor/start/destructor events, and eight pause calls. Separate
CFG checks prove pause pairing and that unpause occurs while its movie is live,
before destruction. Both starts pass true; all six normal/cancel exits pass false.
The raw draft's false starts and missing destructors remain to be lowered.
Resource/movie tests:5passed; temporary-map/call-setup set:23passed.
Artifacts: `work/affair_woman_resource_audit.json`, `work/affair_woman_movie_audit.json`.
Next: retained wife/man/run-off Thing scopes and their operands, hit predicate,
then explicit resource candidate using these maps and structural/readability pass.

## AffairWoman retained actors and hit predicate

`native_affair_woman_actors.py` pins three named-Thing lookups and checks each
retained lifetime against the native CFG. Wife stack48 is the radius5 run-off
trigger partner. Man stack36 faces the hero then self atDB24A5/DB25C1 (snapfalse),
not the draft's hero/wife pair. Run-off marker stack72 supplies the copied position
and is destroyed before man then wife; the removed actor remains bound self.

The hero-hit byte window DB217C..DB224B proves the existing scoped helper pattern:
direct hit OR(any hero ability AND NOT ability14), with conditional CString
construction and reverse destruction before consuming the result. The byte-backed
witness retains the exact old Lua block for counted replacement. Two actor tests
pass, including wrong partner/omitted use/destructor rejection.

Next: generate a separate woman resource candidate from control, resource,
temporary, movie and actor maps. It must repair home snapshot/movement, health
queries, hit result, movie flags/cleanup, retained facing/distance operands and
all release joins; owned run-off Thing position needs a scoped position binding.

## Separate AffairWoman resource candidate generated

`python -m tools.script_recovery.generate_affair_woman_resource_candidate` now
writes the disabled candidate to `work/affair_woman_candidate`. Counted rewrites
consume all four native maps before output: one resource, five temporary Things,
three retained Things, two movie scopes, native hit/facing/distance/health/movement
operands, live animation reads, pause flags and normal/cancel cleanup joins.
No unresolved TODO/missing/extraout/SUB41 operands remain in this intermediate
candidate, but structured readability and full operand/runtime parity remain open.

Four candidate tests pass, including203 cancellation scenarios, hit/talk movie
ordering, reversed partner release, home/run-off behavior, animation flags and
injected errors. The mock was corrected to accept the woman's documented second
animation flag true (BookTrader's is false); source animation flags were unchanged.

Owned-marker position now has a staged12-method resource proposal at
`work/woman_resource_integration/resource-integration.patch`, generated from the
hash-checked combined11-method proposal. New `ThingPosition` resolves the live
owned wrapper and invokes existing ReadRetailThingPosition, returning only a vector
copy. git apply --check passes; compiled validation of the new member remains next.
Do not add woman candidate to readable builder until this binding and entry
condition insertion are verified. Runtime checkout/game remain unchanged.
Full suite11 completed:1,065 tests/347.828s/OK, captured Python exit0. It predates the new woman candidate tests.
