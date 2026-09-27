# Uninterrupted adult campaign and second Maze meeting, 2026-09-25

## Starting point

The earlier c/d progression reached stage 600 and passed a fresh reload,
but required a driver predicate correction and an Orchard runner reload.
This session starts `adult_chain_20260925e` from `adult_graduated` on the
unchanged diagnostic v16 bundle, with runner fixes already committed through
0cbeab0. No live driver/channel intervention is planned for this validation.
The launch backup is `save_backup_1234234_20260925-154532`.

Wasp accepted its real Guild card. The Lookout crossing at 1790372873.2 was
followed by the Picnic crossing at 1790372943.2, after arrival UI settling
(70 seconds between crossings). Its initial combat uses real hero attacks.

## Next story step prepared

`runner_quests/guardian_sister_info_2.json` targets the already converted
second Maze meeting. FinalAlbion.wad's OakValeWest_v2 TNG places MazeAtTavern
at world (2551.072998,1957.87439,8.742913); the proposed arrival is the nearby
walkable cell (2549,1954), terrain z 8.6532, map slot 336, region OakBay.
The native-derived script grants OBJECT_QUEST_CARD_BANDIT_CAMP after the
CS_GUARDIAN_SISTER_2 conversation. Gameflow's second-meeting handler advances
from stage 600 to 700 after completion. This configuration is not yet live
validated; it will run after the four-stage campaign releases the install.

## Converter loop-copy regression

Madame's Main emitted invalid Lua `while xStack_148_2 = iVar5, not bVar3 do`.
The comma-condition lowering recognized register assignments but excluded
stack copies. Known local/temporary sources can now be copied at each loop
head, with the destination declared outside the loop so its last value
survives the false predicate. Unsupported pointer stores remain unsupported.
The new runtime test fails with the original Lua syntax error and passes
after the fix, checking repeated evaluation and the initially-false case.
Focused tests: 96 passed, 35 subtests. All-unit A/B completed: only Madame changes across all 17 units.
Evidence: `work/loop_stack_copy_ab/summary.json`. Bordello now compiles
40/42 functions and 7/9 files (previously 39/42 and 6/9). TODO count remains
361; this is not a playable-unit claim. Regenerated its tracked draft.
The snapshot test also changes the source register before consuming the copy,
so an incorrect alias cannot pass as a real stored value.


Wasp completed at JSONL time 1790373251.8 with the stricter stage-300/next-Maze
predicate true. Its archived log confirms the queen host ran and the native
SetQuestAsCompleted callback fired for Q_WaspBoss, with zero Lua runtime or
resource errors. The driver harvested `adult_chain_20260925e_wasp_boss`,
restored its staged save, and automatically launched Guardian Sister from
that checkpoint. No manual channel command or input was sent during Wasp.


Guardian Sister completed at 1790373509.6, stage 400/CoreQuestWaiting true,
and harvested `adult_chain_20260925e_guardian_sister_info`. Its archived log
has zero Lua runtime/resource errors. Orchard launched automatically from
that checkpoint. The campaign is still uninterrupted.

Assistance remains enabled: movement teleports, hero health support, Wasp
enemy softening followed by real attacks, and ordinary-enemy clearing for
Trader Escort. Orchard uses real damage. Uninterrupted means no external
manual driving or live-code change during this replay, not unassisted play.

Orchard Good completed at 1790374575.6 with stage 500/CoreQuestWaiting true.
All three crates survived; Whisper was defeated through the runner's real
attack/flourish inputs. Its archived log has zero Lua runtime/resource
errors. The wrapper harvested `adult_chain_20260925e_orchard_farm_good`
and started Trader Escort automatically. No manual intervention or live
runner edit has occurred in this campaign.

## Clean campaign completion

`adult_chain_20260925e` completed all four stages from adult_graduated.
Trader Escort's strict stage-600/next-story predicate became true at
1790375429.6. The hero reached the exact bridge end marker through the
configured route, without manual rescue. All four archived stage logs have
zero Lua runtime/resource errors. Each stage harvested its own new AutoSave
and the wrapper completed successfully. No external input or command-channel
write, quest outcome assignment, or live runner change occurred in this run.
A read-only screenshot was taken during the escort's tutorial pages; the
existing runner cleared those pages itself.

Final checkpoint: `adult_chain_20260925e_trader_escort`.
Report: `work/runner/adult_chain_20260925e_campaign.json`.
Loop-copy converter commit: ebb6f91. Bandit Camp bootstrap and subsequent
recovery details are in [its journal](BANDIT_CAMP_CONVERSION_2026-09-25.md).

Fresh-process reload `adult_chain_20260925e_reload` passed at 1790375543.7:
all four quests complete, stage 600, second Maze quest active. The game
closed and all three protected AutoSave hashes match their pre-run values.
The next independent test starts the second Maze meeting from this checkpoint.

Second Maze run `maze2_20260925a` completed through the normal Maze interaction
and reached stage 700. The driver harvested `adult_maze2_completed_2026-09-25`,
archived its game log, closed the game, and restored the launch backup.
Fresh-process verification of this new checkpoint is still pending.

## Generated-output freshness

The latest generic short-circuit fix was compared before/after across all
18 registered units (`work/stack_sequence_v2_ab/summary.json`). Only Bandit
Camp, Book Collecting and Chicken Kicking changed. Their drafts were
regenerated in commit 3619ab6, together with Bandit Camp's readable stage.
Book Collecting and Chicken Kicking's generated readable stages were then
refreshed as well; their reports still record unresolved syntax errors.
These generated changes have not been deployed into the pinned v16 campaign
bundle. Conversion, offline validation and live-bundle promotion are distinct
steps; a successful regeneration is not a playability claim.
