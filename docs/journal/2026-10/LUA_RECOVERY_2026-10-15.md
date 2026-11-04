# Lua recovery and authorized runtime continuation — 2026-10-15

User resumed Lua work until asked to stop and explicitly authorized game launches
and testing while away at work. This supersedes the October 14 bedtime pause and
older screen restrictions for this Lua lane. Native lane work remains separate.

Active checkout: D:/Documents/FableTLC, feat/novi-script-recovery, base 366c08e.
Existing dirty paths are inherited; no reset, broad staging or commit performed.

## W143 resumed validation

Read October 14 pause receipt and both Fable skills. All three owned root hashes
and all 17,664 prior corpus input hashes reverified unchanged. Prior 1,425-test
regression remains reusable under those pins. Preserved interrupted corpus/.
Created resume_20261015.py and versioned corpus_v2.py under
work/codex_lua_maze_cleanup_flag_20261014/. Fresh corpus session 13817 is active;
outputs corpus_v2/, corpus_inputs_v2.json, corpus_progress_v2.json and
corpus_results_v2.json. Do not promote before terminal pass.
An initial PowerShell piped Python setup was stopped at its exact owned Python
PID 7256 and replaced by a filesystem script; no result was accepted from it.

## Integrated W140 DLL actual runtime, W142 Lua payload

Workspace: work/codex_lua_runtime_20261015/. runtime.py owns preparation, input,
command channel and restoration. Copied only manifest-listed accepted W142
payload plus manifest to runtime-bundle; sealed parent remains untouched.
All payload hashes verified. DLL e6627611fb48c1e3d4183484e2b992c628622b1e9a70634e3ff67152e3f75aaf.
Fresh Fable/Fable2/Xenia process guard passed. Backed up all 563 original save
files to original-saves and pinned original-save-hashes.json BEFORE staging
codex_trader_integer_repair_20261005 into the disposable frontend slot.
Installed payload is unchanged by launcher. Actual game PID 14224.

Observed: saved game loads with eight Lua hosts. Assisted GoToMap to OrchardFarm
and fishing-rod gift succeed. Rod presentation and tutorial explicitly dismissed.
Inventory use starts generated Expression_Fish; screenshot fishing_start.png
and runtime log establish actual cast/native fishing interaction.

First cancellation/completion path PASSES: after tutorials and Escape, log shows
PauseAllEntities(false), DeactivateQuestLater, RetailMovie destroy, Lua Main
returned, and LuaQuestHost destructor. Subsequent host list excludes Fish;
assert(quest:IsHeroControlledByPlayer()) succeeds. Physical W movement works,
with normal HUD/camera in control_return.png. No runtime error observed so far.
This proves this empty-completion/cancellation path, not successful catch/reward
or full script equivalence. Catch test is ongoing. Game still live; original save
restoration has NOT yet occurred. Close game before runtime.py restore.

Runtime session now closed normally. actual_runtime_results.json records five
actual Fish Main entries, five returns and five host destructors, zero Lua errors.
Snapped-line failure also restores player control. Successful catch/reward remains
unverified: slow manual input lost the fish; screen-driven input reached near-catch
but snapped the line, and another attempt timed out/escaped. Input-helper failures
are not classified as script failures. No successful reward claim.
All 563 original save hashes verified restored; four changed files archived/restored,
no new save files. Runtime 204 payload hashes and sealed parent verified unchanged.
Logs, screenshots and bounded real-input observations retained under runtime workspace.

Prepared versioned promote_v2.py, finalize_v2.py and external verify_v2.py; Python
syntax checks pass. Not yet run. Next finish fresh W143 corpus, guarded promotion,
package and external verification, then test Maze with an appropriate natural
gameflow checkpoint. BookCollecting snapshot/global-stability contract remains open.

## W143 accepted; fresh Maze gameplay started

Corpus session13817 terminalPASS29units/534Lua; only2 Maze files changed,28
unrelated units/reports exact. Reuse verifier confirms1943 source pins for prior
1425 tests. promote_v2 terminalPASS;644 other productionLua unchanged,ledger860.
finalize_v2 rejected stale assumption: W142 has no Guardian payload. Preserved
partial original bundle; no substitution occurred. Reviewed finalizer_v3 adds4
generated files and2 existing registry entries with unique IDs; only profile and
README existing payload change. No new converter registrations.203Lua/208hashes
parse/preflightPASS; unchanged W140 DLL. Seal19067inputs. External verify_v3
terminalPASS632native/128generated/18negative/21focused; acceptance JSON written.

New independent runtime copy staged natural adult_wasp_completed_2026-09-23
checkpoint after backing up563 original saves. PID13412 launched normally;
frontend session17886 active. Workspace work/codex_lua_maze_runtime_20261015.
Maze gameplay not yet verified; original saves not yet restored for this new run.

## Actual Maze completion after copied-checkpoint schema repair

{
  "pid": 15460,
  "status": "live_after_numeric_checkpoint_load",
  "first_pid": 13412,
  "first_result": "native save load crash 009E5C63",
  "original_saves_restored_after_first": true,
  "original_saves_currently_staged": true,
  "maze": "natural active quest, wave and TAB conversation, unskipped finish, quest complete/inactive, host teardown, control assertion and movement PASS",
  "next": "BookCollecting teacher interaction running",
  "schema_repair": "Only CoreQuestWaiting FALSE -> 0 in copied AutoSave and AutoSave.qs; original source checkpoint unchanged"
}

Crash dump and exact register/stack evidence in numeric_checkpoint_review.json.
All original checkpoint hashes unchanged. Prior W100 native token counterexample
corroborates current fault. No script or engine patch used for this save repair.
Archived maze-completed.log; screenshots maze_approach/maze_talk/maze_complete.
Native Gameflow remains active; Lua-only GetMasterGameState lookup returns nil.
Used existing saved progression plus live completedWasp/activeGuardian prerequisites;
never forced activation or completion. Bowerstone arrival scene skipped, Maze
conversation itself unskipped. Same resource1 reacquired priority3->4 in trace.

## Closed runtime and TourGuide progression failure

Runtime closed; all563 original saves restored and verified. Evidence:
work/codex_lua_maze_runtime_20261015/actual_runtime_results.json.
Maze unskipped conversation/completion/cleanup/control PASS. BookCollecting first
donation/item removal/unskipped reading/control PASS; snapshot contract unresolved.
Raw checkpoint crashed at009E5C63; copied CoreQuestWaiting FALSE->0 repair loaded.
Original checkpoint unchanged. TourGuide repeats waypoint0: suspected signed
sentinel mismatch (0xffffffff store versus -1 comparison). Native proof and generic
lowering recovery underway in work/codex_lua_tour_sentinel_20261015.
Sealed W143 bundle unchanged. No game currently running.


## Signed sentinel candidate and actual tour retest

Tour sentinel recovery: original Init writes FFFFFFFF; Main compares the same
DWORD against FFFFFFFF at00EE59BA. Generic lowering now preserves signed reads
beside negative comparisons, while unsigned/ambiguous reads stay unchanged.
27 bounded original-byte cases, generated Init/Main prefix baseline counterexample,
90 focused tests, and1428 regression tests plus192 subtests pass. Smoke has two
identical baseline limitations (region-unload mock and invalid helper argument).
Full29-unit scratch corpus still running: TourGuide and Orchard TeamSpawn differ.
TeamSpawn initializes its affected signed field to2/3; extra normalization is identity.
No production output promoted yet. Explicit experimental package retains W140 DLL.
First test PID4288 crashed at007002D3 in CReactionMatch_AsymmetricPyramid::Match
after forced GoToMap loading. Dump preserved; all563 original saves restored.
Resource diagnostic numbers match prior stable logs; they do NOT prove corruption.
Second test PID7824 CURRENTLY LIVE, same payload via GoToMapSlotRetailTransition.
Guide has naturally progressed through waypoints1,2,3,5; no outcome-state writes.
Player control intact. Runtime work/codex_lua_tour_transition_runtime_20261015.
563 original saves backed up; close game then runtime.py restore before other tests.
Prior forced-load crash remains unresolved; no full-tour success claimed yet.


## W144 accepted and next actual Dig test

# Lua continuation - 2026-10-15

User authorized Lua work and actual game launches until asked to stop. Earlier
bedtime/native-only restrictions are superseded for this lane. No commits or pushes.
Current journal: docs/journal/2026-10/LUA_RECOVERY_2026-10-15.md.

W143 Maze cleanup accepted offline AND actual unskipped Maze conversation,
quest completion/inactivation, resource cleanup and player-control checks pass.
BookCollecting first donation/item removal/unskipped reading also passed. Definition
snapshot stability remains unresolved. Raw adult-wasp checkpoint required copied
CoreQuestWaiting FALSE->0 repair; source checkpoint and all original saves preserved.
Fishing: five actual runs verified cancellation/snapped-line cleanup; reward unverified.

W144 signed field sentinel fix accepted: work/codex_lua_tour_sentinel_20261015.
Bundle tour-signed-sentinel-candidate-20261015;203Lua/208hashes; unchanged W140 DLL.
27 original-byte cases, generated baseline counterexample/candidate prefix,
1428tests+192subtests,29units/534Lua pass. Four generated files changed across
TourGuide and Orchard TeamSpawn;642 other production Lua unchanged. Orchard limit
normalization is identity for native2/3 values.27 unrelated units/reports exact.
Ledger unchanged371files/zero syntax/860unresolved.18,853sealed inputs reverified.
Actual native-transition tour test PID7824 completed route through17->0, physical
Tab BAR dialogue, control/movement, region-unload Main/OnPredicateFail/timer cleanup.
Game closed;563 original saves restored. Runtime evidence:
work/codex_lua_tour_transition_runtime_20261015/actual_runtime_results.json.
Earlier forced-GoToMap PID4288 crashed at007002D3 engine reaction matching;
dump retained in work/codex_lua_tour_runtime_20261015; causal link unresolved.
Closing-time/no-followers tour branches unverified. No whole-package equivalence claim.
Sealed prior bundles immutable; current root pins supersede W143 root-source hashes.

NEXT ACTIVE: actual Expression_Dig failure/success/reward/cleanup tests from existing
registered conversion, using W144 bundle and copied saves. Workspace:
work/codex_lua_dig_runtime_20261015. setup.py created runtime driver; launch pending.
Use native region transitions, not forced GoToMap, for assisted travel. Keep current
live_state.json and save-restoration receipt current; no production save mutations.


## Dig runtime results and diagnostic exit

Dig runtime completed: work/codex_lua_dig_runtime_20261015/actual_runtime_results.json.
Three actual inventory-triggered runs: empty ground, Upper Dress reward, consumed
spot repeat. Three Main returns/destructors/unpauses; control and movement pass.
Guild Seal menu used native cullis travel; converted teleport host NOT executed.
Process4216 exited during later diagnostic query of guessed chest definitions;
no crash dump found. Cause unresolved. Game gone;563 original saves restored.
NEXT ACTIVE: verify real chest definitions/binding behavior before actual OpenChest
tests. Avoid guessed GetAllThingsWithDefNameByDistanceFrom definition names.
No source or bundle changes from Dig tests. W144 accepted bundle remains current.


## Verified chest assets and runtime launch

CURRENT LIVE chest test PID7256, work/codex_lua_chest_runtime_20261015.
Accepted W144 bundle copy;563 originals backed up; numeric checkpoint staged.
Installed FinalAlbion.wad SHA51271b65... extracted read-only: OrchardFarm chest
is OBJECT_CHEST_OPENABLE with OBJECT_GEMSTONE_JET. Guild locked chest ScriptName
TwentyKeyChest uses OBJECT_SILVERKEY_CHEST_50. First two previous guessed names
were not verified; do not repeat those queries. WAD-backed valid-name retest running.
After game closes run this workspace runtime.py restore. live_state.json tracks PID.


## Actual OpenChest positive and negative key paths

Chest runtime CLOSED;563 originals restored. Evidence:
work/codex_lua_chest_runtime_20261015/actual_runtime_results.json.
Three actual Global_OpenChest runs: ordinary Jet reward;20-key rejection with0;
Murren Greathammer reward with20 assisted keys. Three Main returns/destructors,
both successful paths unpause and empty containers; possession false->true,
keys retained20, physical movement/control assertions PASS. Zero Lua errors.
Verified WAD-backed OBJECT_CHEST_OPENABLE query works. Earlier guessed-name
diagnostic exit remains separate, no dump/causal proof. No source/bundle changes.
NEXT ACTIVE: choose next existing converted runtime path after reviewing evidence;
Fishing success/reward, Follow/Wait and other expression branches remain candidates.
No game currently running. W144 remains latest accepted bundle.


## Assisted Follow and Wait runtime paths

Follow/Wait runtime CLOSED;563 original saves restored. Evidence:
work/codex_lua_follow_wait_runtime_20261015/actual_runtime_results.json.
Four explicitly activated real-game runs: Follow live NPC, Wait with Follow,
Follow nil target, Wait without Follow. Four Main returns/destructors, control
and physical movement restored; no Lua runtime errors. Three diagnostic guards
rejected positioning while scene/transient NPC state changed. Normal menu/hotkey
did not instantiate converted quests; natural activation remains unverified.
NPC following/release visual proof only; cross-host KillSpawnedFunction not
isolated from subsequent DeactivateQuest teardown. No source/payload changes.
Next: continue existing unverified conversions. No game currently running.


## Picklock runtime process exit

Theft expression test PID460 exited after Picklock Main entry, no return.
No Windows dump/event found. Cause unresolved; Pickpocket/Steal never executed.
563 original saves restored. Evidence work/codex_lua_theft_runtime_20261015/actual_runtime_results.json.
Next: debugger-monitored reproduction to locate failing call; no source changes yet.


## W145 reproduced nullable target binding crash

Picklock debugger reproduction CLOSED;563 originals restored. PID4480
reproduced AV c0000005 at NoviCompatibility.dll+45BFAC, reading0xC.
Matching DLL/PDB maps to LuaQuestState::IsEntityPickLockable line6393,
shared_ptr::get before null guard. Lua nil passed to shared_ptr reference is
invalid. Dumps/symbols: work/codex_lua_picklock_debug_20261015/crash_review.json.
ACTIVE W145: work/codex_lua_nullable_predicates_20261015 owns isolated SDK
worktree from W140 commit49b969d including inherited RetailThingVector.h bytes.
Candidate generic nullable predicate adapter covers Picklock/Pickpocket/Steal;
no generated Lua changed. Focused actual sol2 tests/native rejection proof,
build, package and real-game retest remain required. No game running.


## W145 accepted nullable-target predicates and runtime evidence

# Lua continuation - 2026-10-15

User authorized Lua recovery and actual game launches until asked to stop.
Earlier native-only/no-launch restrictions are superseded for this lane.
No root commits or pushes. Isolated SDK fix committed as required for export.
Journal: docs/journal/2026-10/LUA_RECOVERY_2026-10-15.md.
NO GAME RUNNING. All563 original saves restored and hash-verified after every session.

LATEST ACCEPTED W145: work/codex_lua_nullable_predicates_20261015.
Bundle nullable-target-predicates-candidate-20261015;203 unchanged W144 Lua,
208 payload hashes; SDK commit5a26a4c9313c35e608f778925776ecb23cebbd6b.
DLL31c9260c6f0b3111180e129709470528edae90ede8fff6304a401fbf4b4b8189.
Fix three nullable predicates using existing sol::object-to-Thing helper:
IsEntityPickLockable, IsEntityPickPocketable, IsEntityStealable. Only signatures
and local coercions changed; native forwarding/guards unchanged.19,332 sealed
inputs verified; separate replay confirms3 old-binding nil AVs,3 candidate nil
passes,45 exact sol2 cases,18 original-byte empty-handle rejection cases.
Actual runtime:3 nil-target quest returns plus2 physical F1 Picklock attempts,
five Main returns/destructors, two movies cleaned, true/false native door queries,
owned NPC and borrowed hero handles safe. Held attempt produced witnessed crime
fine; physical payment and control/movement pass. Successful unlock/theft/pickpocket
remain unverified. Existing null diagnostics retained; no Lua runtime errors.
Evidence: work/codex_lua_nullable_predicates_runtime_20261015/actual_runtime_results.json.
Original crash PID460 had no Windows dump; debugger reproduction PID4480 captured
DLL+45BFAC / IsEntityPickLockable line6393 reading0xC before null guard:
work/codex_lua_picklock_debug_20261015/crash_review.json.

Earlier completed runtime evidence (all originals restored):
- W143 unskipped Maze conversation/cleanup and first BookCollecting donation/read:
  work/codex_lua_maze_runtime_20261015/actual_runtime_results.json.
  Book definition snapshot remains unresolved. Copied checkpoint required
  CoreQuestWaiting FALSE->0; original source checkpoint unchanged.
- W144 TourGuide signed sentinel fixed;29units/534Lua,1428tests+192subtests;
  four Lua changed in TourGuide/Orchard TeamSpawn,642others exact,ledger unchanged.
  Actual full tour17->0 and native region-unload cleanup PASS:
  work/codex_lua_tour_transition_runtime_20261015/actual_runtime_results.json.
  Closing-time/no-followers unverified. Earlier forced-GoToMap crash007002D3
  remains causally unresolved in work/codex_lua_tour_runtime_20261015.
- Dig empty ground, Upper Dress reward, consumed spot repeat PASS:
  work/codex_lua_dig_runtime_20261015/actual_runtime_results.json.
  Later guessed-definition diagnostic exit unresolved. Do not query guessed defs.
  GuildSeal menu uses native cullis travel; converted teleport was not exercised.
- Chest Jet reward,0-key rejection and20-key Murren Greathammer reward PASS:
  work/codex_lua_chest_runtime_20261015/actual_runtime_results.json.
- Assisted Follow live NPC/Wait pair and nil Follow/standalone Wait PASS:
  work/codex_lua_follow_wait_runtime_20261015/actual_runtime_results.json.
  Normal expression inputs did not instantiate these quests; cross-host kill
  dispatch not isolated from subsequent quest deactivation.
- Fishing cancellation/snapped-line cleanup PASS; successful catch remains a gate.

NEXT ACTIVE: investigate existing BookCollecting definition snapshot diagnostics
against native Init evidence; retain successful donation proof and avoid generated
Lua hand edits. Continue other unverified runtime paths as appropriate.
Sealed W143/W144/W145 workspaces and their evidence must remain immutable.



### W146 in progress: owned BookDef string snapshot

Workspace `work/codex_lua_book_string_snapshot_20261015`. User's continuous Lua-work and launch authorization remains active. No game is currently running; prior 563-save restoration remains intact. Accepted package is still W145.

Candidate generic quest string-vector bindings in isolated SDK commit `2fe60a432053e6ca61d43238584fa05b26665da9`, parent W145 `5a26a4c9313c35e608f778925776ecb23cebbd6b`. Actual committed patch exported into workspace and root sidecar patch catalog. Candidate DLL build passed, SHA256 `91c0326ba178385c9f833178793d7e0fde46c9972fb21989bd80d532f056d976`.

Native Init E54990 copies global BookDef into quest+94. New generic lowering emits resize/copy; teacher reader lowering uses owned storage. Both lifter string-expression classification sites recognize the new synthetic read. Scratch conversion has 47 TODOs (baseline50), all14 functions/3files parse. Three native Init TODOs resolved; conversation snapshot remains separate and unchanged.

98 existing focused BookCollecting tests and14 new tests passed. New tests cover global replacement, repeated Init, shrinking/empty definitions, duplicates, and ambiguous lowering rejection. Exact SDK bindings/table/release with real LuaQuestState and vendored sol2/Lua passed25 assertions and two-owner/repeated-release checks. Resource class is a no-op fixture, never instantiated: this probe does not prove actual engine host teardown. Original-byte Init replay passed60 cases, including32 changed-global cases now matching retained snapshot; symbolic native string identities, empty conversations, allocator/free/memmove boundaries retained. Full corpus and broader regression runs are underway. No production Lua promotion or game launch yet.


### W146 accepted: owned BookDef snapshot with actual donation and lifetime proof

 work/codex_lua_book_string_snapshot_20261015.
Bundle book-owned-string-snapshot-candidate-20261015:203Lua/208payload hashes.
SDK2fe60a432053e6ca61d43238584fa05b26665da9; parent W145 retained exactly.
DLL91c0326ba178385c9f833178793d7e0fde46c9972fb21989bd80d532f056d976.
BookDef is now quest-owned copied strings: generic resize/set/get storage and
evidenced Init/reader lowering. Both lifter expression-kind sites updated.
29units/534Lua compared: only4 BookCollecting draft/readable files changed.
1442tests+192subtests;60native Init/32global-replacement cases;25 exact binding
assertions;3148 instructions checked against retail; smoke3files/0problems.
Separate replay verified20,437 sealed inputs and exact one-header SDK delta.
Promoted4Lua plus reports;731 baseline Lua/report files unchanged. Ledger371files,
0syntax failures,857 unresolved diagnostics (was860). W145 nullable-predicate
committed patch now also preserved in root patch catalog for future rebuilds.

Actual runtime: work/codex_lua_book_string_runtime_20261015/actual_runtime_results.json.
All40 saved definitions match globals before/after reactivation. Physical teacher
interaction/YES, gifted first book removed, count1/flag0, full unskipped classroom
reading, ReadingBook cleared, control and movement PASS. Region unload teacher
exit, explicit quest deactivation/destructor, reactivation with old test vector
absent and fresh40 definitions PASS. No Lua errors. Incidental Maze conversation
completed unskipped before teacher; arrival scene skipped; six tutorial pages
cleared. Accepted message required normal Next click. Original saves all restored.
Only first donation tested; later awards and full campaign remain outside scope.
Global replacement retention is native/fixture evidence, not live global mutation.


Acceptance: `work/codex_lua_book_string_snapshot_20261015/acceptance_verification.json`. Separate replay: `work/codex_lua_book_string_snapshot_20261015_verify/verify.py`. Runtime PID13332 closed and563 saves restored. Next: native nonempty BookReactions copy, separately from the accepted BookDef fix.


### W147 in progress: nonempty conversation snapshot recovery

IN PROGRESS W147: work/codex_lua_book_conversation_snapshot_20261015.
BookReactions script-visible conversation snapshots.648 original-byte native
assignment/reuse/grow/shrink/destructor/self-assignment cases pass,192 retained
source-replacement cases. Fixtures are unpooled CDefClassBase records; allocator,
free and memmove remain explicit boundaries. Captured destructor tail jump to
9FC550 separately; actual execution resolves the bounded CFG frontier.
SDK candidate58182377795548b2f171750e1575ca0c18b06138, parentW146; new value-reader
header and two generic snapshot bindings, existing global reader shares helper.
Built DLL445fcfff4327b05525f2007e544f9a45f3e70370d9e039a54cb4b6c95e8eb198.
28 exact binding assertions plus release pass.53 focusedtests+768subtests pass;
20 new lowering/composed snapshot tests pass. Conversion14functions/3files parse,
47TODO unchanged. Full corpus/regressions running; no W147 promotion or launch.
Root converter/evidence/tests now contain owned W147 edits; W146 sealed scratch
and bundle remain immutable. Runtime must exercise nonempty BookReaction lines;
query live stored conversation counts and select a corresponding book if needed.


### W147 accepted: owned conversation snapshots and live pupil reaction

LATEST ACCEPTED W147: work/codex_lua_book_conversation_snapshot_20261015.
Bundle book-owned-conversation-snapshot-candidate-20261015:203Lua/208payloads.
SDK58182377795548b2f171750e1575ca0c18b06138, parent W146 retained.
DLL445fcfff4327b05525f2007e544f9a45f3e70370d9e039a54cb4b6c95e8eb198.
BookReactions now owns script-visible Lines and four string vectors per quest.
Generic SnapshotGlobalConversations/GetStateConversations and evidenced lowering.
648 native assignment/destructor/self-assignment cases;192 source replacements;
72 full Init/generated comparisons;28 binding assertions;1464tests+960subtests.
29units/534Lua: only2 master BookCollecting Lua changed;532 others identical.
695 conversation instructions checked against retail; smoke3files/0problems.
Late mock/report corrections regenerated byte-identical Lua; input pins recorded.
Promoted2Lua and reports;733 baseline Lua/json unchanged; ledger371files,
0syntax failures and857 unresolved diagnostics. Separate replay verifies21,607
sealed inputs, exact two-file SDK content delta, native648+72 and bindings28.
Acceptance: work/codex_lua_book_conversation_snapshot_20261015/acceptance_verification.json.

Actual runtime: work/codex_lua_book_conversation_runtime_20261015/actual_runtime_results.json.
All25 records/fields match globals before and after host reactivation; returned
Lua table mutation isolated. Gifted book2 OBJECT_BOOK_STORY_13 (Balverine Slayer),
physical TAB/YES, unskipped classroom, six pupil speech keys in order, seven
DoConversation returns and one BookReaction return. Donation count1/flag2,
item removed, ReadingBookfalse, control/movement pass. Region unload entity exit,
explicit deactivation/destructor/reactivation, old probe absent and fresh25
conversations/40BookDef verified. No Lua runtime/C++ errors. One diagnostic
assertion ran before Maze finished and passed later; menu/popup timeouts resolved.
Entry/return instrumentation wrapped original parent-VM functions, forwarding all
arguments/results; package files unchanged. Assisted time/position/item. Native
fixtures cover unpooled bases, not pool identity, OOM or exceptions. Live globals
not mutated; source replacement proof is native/binding replay.

NEXT W148: continue actual runtime with TourGuide closing-time and no-followers
paths, previously unverified. Use W147 bundle unchanged, disposable numeric adult
checkpoint and full563-save backup/restore. Inspect existing tour actions/native
evidence first. Continue until user stops; acceptance of one increment is not stop.


### W148 TourGuide adverse runtime and closing-time boundary proof

W148 runtime verified: work/codex_lua_tour_adverse_runtime_20261015/actual_runtime_results.json.
PID10792 closed;563 original saves restored. W147208payloads and21,607 sealed
inputs unchanged. Original closing watcher EE4A00 is inert except time/state
reads, termination checks and frame yields:162 original-byte/draft/readable cases
match,38instructions verified against retail,ABI/state bytes preserved. In-game
23:00 kept guide/four followers active; no invented closing behavior added.
Assisted fade/kill of four followers caused live WatchForNoFollowers to set
AllFollowersDead; four entity exits and timers32/28/29/31 released. Control and
movement pass. Exit marker M_TG_ClosingTimeExit absent; guide remained alive/idle,
then exited on region unload with timers21/20 released. Walk-to-exit/fade branch
still unverified. No Lua runtime/C++ errors; two diagnostic failures retained:
premature assertion after background command timeout, and missing nil guard for
absent marker. Successful focused resend and guarded follow-up recorded.
No production Lua/converter/SDK changes in W148.
NEXT W149: successful fishing reward using accepted W147 bundle. Earlier session
work/codex_lua_runtime_20261015 verified cancellation/snapping only; inspect its
reel_visual.py/controller observations before improving input and relaunching.

### W149 successful special and ordinary fishing rewards

W149 successful fishing runtime: work/codex_lua_fishing_reward_runtime_20261015/actual_runtime_results.json.
PID9076 closed normally;563 original save hashes restored. W147208payloads and
21,607 sealed inputs unchanged. Two natural inventory Fishing Rod Use activations,
two Main returns/destructors/unpause calls, control and physical movement pass.
First catch awarded Elixir of Life (screenshot); potion inventory5->6; nearest
special fishing spot disabled, other two still enabled. Second cast at consumed
spot awarded OBJECT_FISH_CARP_MEDIUM_01,18cm/13g; native gift log, inventory
possession assertion and screenshot verify ordinary catch. Spot remained disabled.
No Lua/C++/diagnostic errors. Modal popup timeouts resolved after normal Next.
Input correction: earlier controller only watched red meter pixels and clicked
while Reel In was gray. New real-input helper gates on white enabled text;42 and
63 clicks in successful casts, zero while initial tutorials blocked. No minigame
state changes. Assisted transition/standard rod gift/position only. Menu startup
helper initially reached quit prompt; stopped, No selected, menu-only load passed.
No production/source/SDK edits. Other fish sizes/competition awards remain untested.
NEXT W150: successful physical Picklock path using W147 bundle; prior W145 proved
nil-target safety and witnessed/cancel cleanup, but never successful unlocking.
Inspect native held-input duration and isolate witnesses through disposable
scene positioning if needed; do not force success flags or replace quest logic.

### W150 successful physical Picklock and witnessed-crime path

W150 runtime verified: work/codex_lua_picklock_success_w150b_20261015/actual_runtime_results.json.
Earlier setups (PID16100, retry PID12732) closed/restored without success: trespass
from inside-building teleports, window targeted instead of door, and door lock
flipping. W150b PID9232 closed normally; 563 original saves restored; W147 208
payloads and 21,607 sealed inputs unchanged; no production/source/SDK edits.

- Targeting: 1.4 units outside along +door forward with -0.3 tangent offset gives
  native GetHeroTargetedThing == OBJECT_BUILDING_DOOR_3 (on-axis picks the window).
- Attempt 1 (no isolation): Main ran 32s, native IsDeedWitnessed abort, crime modal
  "1 x Lockpicking (200 gold)" paid with physical Yes; door still lockable,
  morality unchanged.
- Native door finding: the shop door is pick-lockable only at night with the
  shopkeeper (owner) inside. Day hours or teleporting the owner away unlocks/opens
  it; CloseDoor+EntitySetAsLocked is reverted by the schedule. Returning the owner
  inside restores lockability. This explains the retry's lockable true->false flips.
- Attempt 2 (non-owner creatures moved to 3808,4288): held F1 64.5s, matching
  15fps * 8.0 / ((0+1)/(7+1)) = 960 frames. Bar full, door no longer lockable and
  swung open, morality 0.50575 -> 0.50425, +6 evil popup (screenshots).
- Lifecycle: 2 Main entries/returns/destructors, movie start/destroy pairs; the
  line-33 traceback is the RetailMovie diagnostic (also W145). Control assertion and
  physical movement (S held 1.5s, 6.6 units) pass. No Lua/C++ errors; 2 expected
  diagnostic assertion failures (verify after witnessed abort; hold precondition
  refused F1 while owner was away).
- Limits: assisted positioning/time/witness isolation; partial-progress random roll
  untested (progress 1.0 always passes); pickpocket/steal success still unverified.

NEXT W151: successful physical Pickpocket and Steal with the W147 bundle.

### W151 successful physical Steal and Pickpocket

W151 runtime verified: work/codex_lua_steal_pickpocket_w151_20261015/actual_runtime_results.json.
PID17028 closed normally; 563 original saves restored; W147 208 payloads and 21,607
sealed inputs unchanged; no production/source/SDK edits. No Lua/C++ errors and no
diagnostic assertion failures.

- Steal: the checkpoint's PicnicArea has TNG `Stealable TRUE` green apples. A teleport
  onto one apple auto-collected it (ordinary pickup: no Expression_Steal, morality
  unchanged). With EXPRESSION_STEAL gifted, targeting a second apple from 1.6 units
  and holding F1 ran Main 16.4s (GetStealDuration 2 * 15 / (1/8) = 240 frames); item
  gone, hero possesses it, morality 0.50575 -> 0.50375. No creatures within 60.
- Pickpocket native predicate (0x88e220 -> 0x665e90) = def flag + hero state + 2.0f
  geometric test, so all creatures read false at range; within 1.0 the villager is
  true from every angle.
- Retail pickpocket (0xeeaef0) rolls RetailRand%100 < 10 every script frame
  (threshold reset to the counter at 0xeeb3ed-0xeeb3f1) and catches only if F1 is
  still held 15 frames after a spot; release rolls success < progress. The converted
  Lua matches. Real 0.7s F1 holds after an in-process native target precondition:
  two clean misses, third succeeded with native "You find nothing interesting."
  Victim then reads unpickpocketable at 1.0 while three same-def villagers stay true.
- Lifecycle: Steal 1 and Pickpocket 3 Main/returns/destructors; pause restored;
  control and physical movement pass.
- Limits: assisted positioning/expression gifts; victim carried nothing (gold/item
  transfer and the caught event-19 path untested); witnessed steal untested.

NEXT W152: remaining unverified runtime paths (pickpocket caught/transfer, witnessed
steal, TourGuide walk-to-exit) or the next unconverted/unverified script from the ledger.

### W152 pickpocket caught path and witnessed Steal

W152 runtime verified: work/codex_lua_caught_witnessed_w152_20261015/actual_runtime_results.json.
PID12948 closed normally; 563 saves restored; W147 bundle and 21,607 inputs unchanged;
no Lua/C++/diagnostic errors; no production/source/SDK edits.

- Pickpocket with real 4.0s F1 holds (past the 15-frame grace), frames every 0.5s:
  5 Main entries/returns/destructors, pause true/false balanced. Caught: victim turns
  to the hero mid-hold, then an arms-up angry reaction after release; no find
  message, gold unchanged. A later 4s hold still succeeded ("You find nothing
  interesting"), consistent with retail re-rolling the spotted window every frame.
- HUD finding: native IsEntityPickPocketable can be true while the context HUD offers
  only F2 (fart) / F3 (flirt); F1 then starts nothing. That explains Main-less attempts.
- Witnessed Steal: Guild TNG potions are absent in this save, so used an
  OrchardFarmEast stealable apple with a CreateCreature villager 3 units beyond it
  (assisted). Real F1 hold; Main returned at 6.45s of 16s; "!" over the witness with a
  partial bar; apple still stealable; morality unchanged; no guards so no fine. An
  operator slip spawned a second witness after the attempt (recorded).
- Gold 570 -> 575 between the last pickpocket and the orchard is unattributed.

NEXT W153: pickpocket loot transfer needs a victim carrying items/gold; otherwise
TourGuide walk-to-exit or the next unverified ledger script.

### W153 Expression_Follow / Expression_Wait runtime

W153 runtime verified: work/codex_lua_follow_wait_w153_20261015/actual_runtime_results.json.
PID6140 closed normally; 563 saves restored; W147 bundle/inputs unchanged; no errors.

- EXPRESSION_FOLLOW and EXPRESSION_WAIT are retail expressions (names.bin, with
  _DUMMY items); gifted. Ordinary villagers offer no context slot, so both quests
  were started with explicit ActivateQuest while a creature was natively targeted.
- Follow: Main fades, spawns FollowThread and returns; quest stays active. The
  thread reads GetHeroTargetedThing after the 1s fade, so it captured the teacher
  (target changed from the staged girl). Teacher stayed 2-3 units through two
  physical 3s walks (~16 units of hero travel).
- Wait: TryAcquire hero, movie, DeactivateQuest('Expression_Follow') -> Follow
  destructor -> DeactivateQuestLater(Wait) -> Wait destructor. Teacher then no
  longer tracks the hero (18 -> 32 units over D/S walks). Control/movement pass.
- Limits: explicit activation; fade visuals not caught on frame; follower was a
  V_BookCollecting entity (no errors).

NEXT W154: V_ChickenKicking (no runtime mention anywhere) or pickpocket loot transfer.

### W154 V_ChickenKicking (partial, stopped at usage limit)

See work/codex_lua_chicken_kicking_w154_20261015/actual_runtime_results.json. Ghost
questline completed with real input; organiser lines/state transitions verified; paid round
started. No successful kick yet; resume from CK_HeroStart with short W run + LMB.
