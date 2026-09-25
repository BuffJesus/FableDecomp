# Quest runner continuation, 2026-09-25

Final result: progression from graduation through Wasp, Maze, Orchard Good,
and Trader Escort persisted in `adult_chain_20260925d_trader_escort`.
A fresh-process reload verified all four completed quests, stage 600, and
`QS_GuardianSisterInfo2_SisterInBanditCamp` active. This was a resumed campaign:
Wasp's driver predicate was corrected between c and d, and Orchard received
a live runner targeting fix. It is not an uninterrupted four-stage replay.
No quest outcome or engine pause flag was assigned by hand. New Game and
Guild training remain outside the runner campaign.

Resumed after commit `45cc120`, which added card acceptance, real boss fights,
and the Wasp/Guardian Sister runner configurations.

## Launch and save lifecycle

The earlier `gs1` process never reached `play`: its output ended with
`the save did not load (no quest host within the timeout)`. There is no
`work/runner/gs1.jsonl`. The final frontend capture showed the correct
1234234 Load Game screen; the evidence does not establish why loading failed.

The `gs2` retry successfully loaded `adult_wasp_completed_runner_2026-09-25`
on v15, confirmed Guardian Sister active, and crossed into Bowerstone.
It failed to target Maze (six empty OCR target results), then closed and
restored the staged profile. This establishes that the Wasp checkpoint can
load; it does not establish Guardian Sister completion.

Fixed runner lifecycle issues found during this review:

- Check for an existing Fable process before save staging or cleanup. Previously,
  a refused `--launch --close` could stage over a live save and then kill the
  existing game from its `finally` block.
- Require `--launch` with `--save`.
- Measure the harvest baseline after staging. A newer staged checkpoint must
  not count as an autosave written by the current run.
- Automatically close staged runs before restoring the original profile,
  including failed launches. Leaving the game open could overwrite the restore.

Focused validation: `test_ingame_runner_lifecycle.py`, `test_ingame_runner_ocr.py`,
`test_autopilot_launch.py`, and `test_autopilot_driver.py`: 14 tests and
5 subtests passed. Tests use mocked process/input boundaries and temporary
save files; the OCR test uses Windows.Media.Ocr on a captured label fixture.
They do not launch the game.

## Maze targeting

`gs3` separated teleport and facing commands. Maze's name was visible in a
capture, but full-screen OCR returned no label. `gs4` tried a processed crop:
the outlined name was read as `(vlaze` and the runner refused interaction.
All these failed attempts closed the game and restored the profile.

The runner now tries three contrast thresholds on the top-centre target
label, and requires an exact normalized match. The `gs4` image returns
`Maze` at threshold 210. A cropped fixture in
`tools/script_recovery/testdata/runner/maze_target.png` covers this regression.
Approach screenshots are retained individually instead of overwriting one
file across all six positions.

## Successful Guardian Sister run

`gs5` loaded the Wasp runner checkpoint on v15, crossed to Bowerstone, waited
for the arrival movie, recognized Maze on the second approach, and pressed
TAB. The real Maze conversation ran through `TEXT_CS_027_BOWERSTONE_*` and
`SetQuestAsCompleted('QS_GuardianSisterInfo')`. Gameflow persisted
`PostSavePosition = 400`. No manual intervention or quest-state writes were
used. Assistance is automated teleporting, interaction input, and the runner's
usual health protection; this is not an unassisted walking playtest.

Evidence: `work/runner/gs5.jsonl`, per-approach screenshots and `gs5_end.png`,
and `work/ab_runs/v15-20260925-123952/FableScriptExtender.log`.
The log has no Lua runtime errors or autopilot errors.

Harvested `adult_maze_completed_runner_2026-09-25`. This run deliberately
omitted `--close`: automatic closure with `--save` worked, and all three
protected save files (including companion-file absence) matched
`scratchpad/save_backup_1234234_20260925-123951` byte for byte afterwards.

Fresh-process reload `gs5_reload` passed all four conditions: Wasp completed,
Guardian Sister completed, stage 400, and `CoreQuestWaiting = true`.
The condition is recorded in `work/runner/maze_reload_check.json`, results in
`work/runner/gs5_reload.jsonl`, and the log in
`work/ab_runs/v15-20260925-124359/FableScriptExtender.log`.
The reload also closed automatically and restored the original profile.
Orchard Farm subsequently passed with automated real input, as recorded below.

## Orchard Farm runner

`of1` completed Protect Orchard Farm on v15 from the verified Maze checkpoint:
real card acceptance, guard-side arrival, bandit waves, Whisper's actual
flourish gate, sword combat, reward sequence, and stage 500 with
`CoreQuestWaiting = true`. No quest outcome flags or enemy-health changes were
injected. Hero health protection and positioning assistance remained enabled.
Whisper acknowledged the flourish with
`TEXT_QST_051_WHISPER_HAS_BEEN_HIT_WITH_FLOURISH_10`.

The farm config keeps combat targets and teleport destinations inside the
farm boundary to avoid chasing a bandit into the region exit. It uses real
damage throughout and target-specific sword/flourish inputs. The shared
runner retains its previous default combat behavior for other quests.

Evidence: `work/runner/of1.jsonl`, finished `done=true, failed=false`, zero
Lua errors. Harvest: `adult_orchard_completed_runner_2026-09-25`.
The game closed before restoration; all three protected AutoSave files match
`scratchpad/save_backup_1234234_20260925-132802` byte for byte.

Added `run_campaign.py` and `runner_campaigns/adult_good.json` to run Wasp,
Guardian Sister, Orchard Good, and Trader Escort through fresh processes,
using only a successful stage's newly harvested checkpoint. The wrapper
stops on a failed run or missing harvest. This starts at graduation, not New
Game. The first full chain, `adult_chain_20260925a`, is under test.
Focused runner, OCR, save lifecycle, and campaign tests: 24 passed, 5 subtests.

## Marathon follow-up: assassin distance

Commit `8f4faae` fixes a mismatch between the lowering and lifter passes.
`DarkwoodAssassinSpawn.Main` (0x00e02e50) reads SCRIPT_DEF offset 0xe14
(`TE_AssassinSpawnDistance`, retail value 30), converts the integer to a
float, and keeps it in a register Ghidra typed as `CCharString`.
The lowering pass correctly isolates that lifetime as `f_CVar3`, but
`RE_LOCAL_ASSIGN` rejected that generated name. Its assignment became a
TODO and the distance call read a free global.

The lifter now accepts the generated float-register spelling. The regression
executes the resulting Lua and checks that 30 reaches the consuming API.
All 85 lifter tests and 28 subtests passed. A/B conversion of all 17 registered
units, using before/after regexes in separate processes without modifying
source files, changes only `DarkwoodAssassinSpawn.lua`.
Evidence: `work/assassin_ab/summary.json` and the per-unit logs/outputs there.

Regenerated Trader Escort draft/readable reports and Lua. Readable smoke:
16 files, zero problems (previously one free global). Draft smoke: 16 files,
one pre-existing `extraout_EAX_00` free global in DarkwoodTrader. Other native
TODOs remain, including assassin position and sound operands. Zero smoke
findings does not establish full behavior recovery or live assassin coverage.
The v15 playtest bundle has not been changed for this fix.

## Entity position recovery and campaign comparison

The assassin's own entity address was cast as `CCharString` by Ghidra.
Normalizing that explicit `this + 8` address in entity scripts lets the
existing alias analysis name both GetPos calls. Camera visibility and creature
creation now receive `me:GetPos()` instead of missing operands. An explicit
`(void *)` receiver is also removed before matching method arguments, keeping
Madame's special-ability ID at 0xe instead of accidentally passing `me`.

A/B conversion of all 17 units changes only DarkwoodAssassinSpawn and Madame:
`work/assassin_position_ab/summary.json`. Tests: 125 passed, 30 subtests;
Trader Escort readable smoke: 16 files, zero problems. Bordello's existing
39/42 function and 6/9 file syntax pass counts are unchanged; its TODO count
falls from 370 to 361. Its incomplete draft is not a playable release.
Neither live bundle was changed for these converter fixes.

Campaign `adult_chain_20260925a` stopped during Wasp on v15. Several drones
reached zero health while reporting IsAlive true and IsDead false; the queen
did not spawn. No outcome was injected and no checkpoint was harvested. The
owned driver/game were stopped and the protected AutoSave files restored
from `save_backup_1234234_20260925-134523`.

`adult_chain_20260925b` repeats the chain on the existing diagnostic v16 bundle.
The two bundles' Lua files compare byte-identical. This is a diagnostic
comparison, not yet evidence that the DLL explains the stall. Early v16
PauseDiag records show WaspIntro's pause and unpause both executing.

The v16 attempt also stalled. Read-only process inspection found the entity
manager's non-scripted pause byte at +0x54 clear and its all-entities pause
byte at +0x55 set, including after tutorial screens closed and the runner's
script frames resumed. Retail evidence: PauseAllEntities 0x890b00 stores
+0x55; PauseAllNonScriptedEntities 0x890ab0 calls 0x51efc0, which stores
+0x54. The manager accessor at 0x49e1b0 is `mov eax,[ecx+0x50]; ret`.
The inspector validates those accessor bytes and only uses ReadProcessMemory.
No pause flag was changed to make the test pass.

Both attempts crossed Lookout Point in approximately six seconds, allowing
first-time tutorial pages to arrive after the next region transition. The
next test (`adult_chain_20260925c`, v16) adds a travel settling step: dismiss
arrival pages and require two consecutive clear, scene-ready observations
before crossing. Failed arrival/scene waits now stop the run instead of
continuing. Three focused tests cover delayed pages, scene waits, and timeout.
This is a hypothesis under test, not yet an established cause of the pause.
The stopped v16 attempt restored from backup
`save_backup_1234234_20260925-135729` and harvested nothing.

In `c`, Lookout Point lasted about 69 seconds; the helper's own dialogue ran
before the Picnic crossing. The pause trace
`work/runner/adult_chain_20260925c_pause_flags.jsonl` shows +0x55 clear after
the intro and throughout combat. The wave cleared and Wasp reached
SetQuestAsCompleted without changing either pause flag by hand. This supports
the travel sequencing fix; the exact engine write that leaked the pause in
the earlier attempts has not been localized.

The first stricter Wasp predicate incorrectly required `CoreQuestWaiting`:
that flag remains false for the directly granted Maze quest. Gameflow's
source confirms the correct boundary is stage 300 with
`QS_GuardianSisterInfo` active. After stopping the stale driver, a read-only
query returned `true stage=300 maze=true`. Harvested that actual autosave as
`adult_chain_20260925c_wasp_boss`, closed the game, and restored the protected
files from `save_backup_1234234_20260925-140604`. The c campaign report remains
failed because its driver was stopped; it is not evidence of an uninterrupted
campaign success. No quest state or pause flag was edited.

Continuation `adult_chain_20260925d` starts at Guardian Sister from that
checkpoint, using the corrected predicates and travel settling. The runner
tests now total 27 passed and 5 subtests.


Guardian Sister in continuation `d` completed automatically and harvested
`adult_chain_20260925d_guardian_sister_info` at stage 400. Its preserved log
is `work/runner/adult_chain_20260925d_guardian_sister_info_FableScriptExtender.log`.
The campaign then launched Orchard from that checkpoint.

Orchard's first wave stalled with one living bandit at (3295.0, 3277.2),
health 5, just outside the runner boundary ending at x=3293. A read-only
query was issued with the driver temporarily suspended to avoid competing
channel writers. The targeting fix allows enemies within three units of an
interior landing while still clamping the hero inside the boundary; the
exit spawn at (3301,3292) stays excluded. Five combat tests pass. Reloaded
this runner code into the live test without assigning quest state. The
second wave then began. This run includes that live runner-code update.

Commit 1769bd5 archives each stage's full game log synchronously before
restoring saves, so the next launch cannot overwrite its evidence. The
archive-failure regression also verifies save restoration still runs.


Presented-item output (`031fa58`): LuaEntityAPI's MsgIsPresentedWithItem
returns a boolean and publishes its CCharString output as the VM global
`g_PresentedItemName`. The converter previously discarded that output.
It now copies it immediately on success and preserves the prior local value
on failure, including an un-emitted string constructor. Runtime tests cover
success, stale global on failure, ignored boolean, and the staged constructor.
The final source-snapshot A/B (`work/presented_item_ab/summary.json`) covers
all 17 units and changes only Madame and MansLover. Their tracked drafts
and reports were regenerated. Syntax counts remain Bordello 39/42 functions,
6/9 files and SickChild 46/49 functions, 9/13 files; neither incomplete unit
is being claimed playable. Combined focused checks: 113 tests, 32 subtests.


Orchard in `d` completed at JSONL time 1790368763.5: all three crates,
real bandit damage and Whisper flourish, then stage 500/CoreQuestWaiting.
The completion capture shows Whisper's Brooch, 750 guild reward plus 30
extra gold, 400 renown and 500 experience. No boast was selected.
Checkpoint `adult_chain_20260925d_orchard_farm_good` was harvested and passed
automatically to the Trader Escort stage. The synchronous archived log is
`work/runner/adult_chain_20260925d_orchard_farm_good_FableScriptExtender.log`;
its Lua runtime/resource error count is zero.


Trader Escort in `d` accepted its card through the Guild UI and moved all
three traders through Darkwood1-6. The troll did not leave the party scared.
At JSONL time 1790369518.5 the hero was at the end marker
(2643.9,2167.2,25.7), and ENDTRADER_GREETINGS triggered automatically.
No manual route, teleport, quest-state change, or live-code update was used
in this escort stage. This exercises the exact-height bridge leg that had
needed manual recovery in the earlier te8 test.


Continuation `d` finished successfully with exit code 0; its report records
Guardian Sister, Orchard Good, and Trader Escort all done. The final saved
checkpoint is `adult_chain_20260925d_trader_escort`. All three stage logs
have zero Lua runtime/resource errors. The escort stage had no intervention.

Fresh reload `adult_chain_20260925d_reload` used
`tools/script_recovery/runner_checkpoints/adult_good_completed.json` and
returned done=true, failed=false at JSONL time 1790369705.1. Its single
predicate checks all four completions, Gameflow stage 600, and the active
next story quest. This tests persistence in a new game process, not merely
live state from the run that completed the quests.

Final cleanup: the game and campaign drivers are closed. All three protected
AutoSave files compare byte-for-byte with the reload launch backup
`save_backup_1234234_20260925-145346` and retain the original SHA-256 prefixes
87414c4b277aaa7c / fc68b46f36dd5ed8 / 47a667d9afd068e4. The reload's archived
log also has zero Lua runtime/resource errors. No changes were pushed.
The other session's reconstruction edits, including its HANDOFF paragraph,
remain outside these commits.
