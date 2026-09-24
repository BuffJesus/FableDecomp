# Wasp quest-card acceptance and completion handoff

Continuation of `WASP_COMBAT_RECOVERY_2026-09-23.md`, using the same v11 DLL
and generated quest Lua. The earlier assisted queen defeat called native
completion but left `IsQuestCompleted('Q_WaspBoss')` false and gameflow at 200.

## Native evidence

Read-only Ghidra exports are in `work/wasp_completion_native_20260923.log` and
`work/wasp_completion_lookup_20260923.log`. Retail bytes were also disassembled
with `RData`/Capstone to check argument order.

- Wasp WatchForTermination at `0x00E0F1D0` passes **true, false, false** to
  interface slot `0x480`; the converter preserves those arguments correctly.
- Interface `SetQuestAsCompleted` at `0x00892F80` calls quest manager
  `0x004B1D30`, then rests. An ENTER/EXIT log is evidence of the call, not of
  persistent completion.
- Quest manager completion looks up the registered script, emits completion
  events, and obtains the active quest card through `0x004B0C80`. The card is
  part of reward/completed-card bookkeeping.
- `IsQuestCompleted` at `0x004B0FC0` scans the completed-card list at manager
  offset `0x60`, comparing the card's script name. It does not read Lua's
  `MissionSucceeded` flag.
- `MsgOnQuestCompleted` at `0x00893570` observes native message type `0x33`;
  the completion manager distinguishes that from the pre-screen message `0x35`.

## Accepted-card replay

Loaded `adult_graduated`, initially gameflow stage 150. The actual guild UI
listed **Wasp Menace**, its 500 gold / 200 renown reward, and three dummy quests.
Selected Wasp Menace, then **Take Quest**, then closed the UI. Wasp disappeared
from the available list and its Lua host started without an `ActivateQuest`
command. The quest table was reached by teleporting beside the actual Wasp
card; preliminary attempts to speak to the Guildkeeper were unsuccessful.

The first Lookout helper dialogue required one manual click on its visible
Next control; `input: clear`'s green-pixel heuristic selected scenery instead.
That input-detection limitation remains separate from quest logic.

The subsequent entry/initial-wave checklist passed **9/9**:
`work/wasp_card_combat_report.json`. The combat remains assisted: teleporting
near targets, replenishing hero health, and issuing real sword/lightning inputs.
No quest-success, enemy-health, or queen-phase flags were assigned.
The bow driver stalled at queen health 40 despite a target query naming her;
repositioning and a camera reset did not make those shots land. A real lightning
cast reduced health to 34, and subsequent casts reached the next minion phase.
Bow attempts are retained in `work/wasp_card_queen_drive.json`; the subsequent
driver writes `work/wasp_card_queen_magic.json`.

The checklist now accepts the guild card rather than calling `ActivateQuest`.
A separate `wasp_boss_handoff.json` requires the persistent completed-card query,
gameflow stage 300, and an active Guardian Sister Info quest. Its checks do not
set those states. Checklist validation
now recognizes the already-supported click/move input verbs.

## Live handoff result

**Passed 3/3** in `autopilot_wasp_card_handoff_2026-09-23.json`: persistent Wasp
completion is true, gameflow is 300, and `QS_GuardianSisterInfo` is active. Its
host entered Main and bound `MazeAtTavern`. The native completion screen showed
Wasp Menace, the Wasp Queen's Head, and the 500 gold / 200 renown quest rewards.
No Lua runtime errors occurred. Archive:
`work/ab_runs/v11-20260923-144311/FableScriptExtender.log`.

The earlier third probe tried to enumerate the Guardian card as a world thing;
that returned none despite the quest already running. Accepted cards can be in
limbo. The failed probe is retained in
`autopilot_wasp_card_handoff_probe_2026-09-23.json`; the corrected gate reads
native `IsQuestActive` and does not create or activate anything.

The acceptance path resolves the earlier completion boundary with **no changes
to v11's quest Lua or DLL**. Starting the script directly skipped the accepted
card needed for the normal completion/reward path.

## Checkpoint and replay isolation

The stage-300 AutoSave was copied into a new profile,
`adult_wasp_completed_2026-09-23`, without overwriting `adult_graduated`.
Native saves can omit the optional `.qs.hs` snapshot. The staging helper now
copies only existing companions and removes stale destination companions;
restore preserves their original absence as well as their bytes. A missing
backup directory is rejected before touching the live save. Regression tests
cover both directions and the missing-backup case. **Fourteen tests pass** across
the checklist, driver-entry-point, and launch/save-isolation modules.

Checkpoint reload passed **4/4** in
`autopilot_wasp_checkpoint_reload_2026-09-23.json`: after a fresh process/load,
Wasp remains completed, gameflow remains 300, and Guardian Sister Info remains
active. Reload archive: `work/ab_runs/v11-20260923-144506/FableScriptExtender.log`.

Fresh-load automatic card acceptance remains **unresolved**. Proximity checks,
post-teleport facing/camera resets, and tutorial dismissal still selected the
Guildmaster rather than reliably opening the map table. These changes are not
claimed as a fix. The canonical combat checklist now requires manual acceptance
through the real card UI, checks active state without changing it, and then
continues the scene-guarded replay. Active state alone does not prove card
bookkeeping; the separate handoff checklist checks persistent completion.
The experimental UI sequence is retained in `work/wasp_card_accept_experimental.json`.

`clear_all` clears up to eight detected tutorial pages; a bounded `wait` supports
frame boundaries. Input errors abort subsequent clicks. These helpers have
regression coverage, but do not guarantee the correct interaction target.
An attached acceptance check passed 2/2 after manual recovery; another passed
3/3 attached. Neither establishes fresh-load automation. The failed fresh report
is `autopilot_wasp_card_accept_2026-09-23.json`; the final manual targeting probes
are archived in `work/ab_runs/v11-20260923-151810/FableScriptExtender.log`.

The test game was closed and the original protected profile restored byte for
byte from `save_backup_1234234_20260923-142438`. The completed-Wasp checkpoint
remains separate for continuing the Maze conversation.

Evidence and checkpoint hashes: `wasp_card_handoff_evidence_2026-09-23.json`.
The earlier 1,689-test suite and 16-unit converter gates remain applicable to
unchanged v11 quest generation; this follow-up changes only the harness/tests
and documentation, so it runs the targeted harness/save-isolation tests.
