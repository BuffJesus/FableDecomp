# Maze conversation and Orchard Farm handoff ? 2026-09-23

The unchanged v11 Guardian Sister Info Lua completed its normal conversation
from the verified `adult_wasp_completed_2026-09-23` save. Entry checks passed
5/5, native completed-card state is true, and gameflow advanced from 300 to
400 with `CoreQuestWaiting` true. Handoff checks passed 3/3. No outcome flags,
quest activation, or completion calls were injected by the driver.

## Live path and evidence

The replay crossed once to Bowerstone Slums (slot 339), skipped the separate
Bowerstone arrival movie, and waited for idle scene state before teleporting
beside Maze. Maze waved and queued `TEXT_QST_027_MAZE_CALL_HERO_OVER_10`.
The first interaction exposed two Expressions tutorial pages. After clearing
those, a short leftward movement and TAB initiated the real conversation.
The Maze movie was allowed to finish without skipping. Its commands include
`TEXT_CS_027_BOWERSTONE_20` through the later dialogue and teleport-out ending.

The log records native `SetQuestAsCompleted('QS_GuardianSisterInfo')`, stage 400,
and independent read-only completion/state queries. The gameflow source's
`RunStage_MazeMeeting` adds both Orchard Farm cards before returning stage 400.
Archive: `work/ab_runs/v11-20260923-170540/FableScriptExtender.log`.
Reports: `autopilot_maze_entry_2026-09-23.json` and
`autopilot_maze_handoff_2026-09-23.json`.

A preliminary attempt to count the guild card world entities while standing
in Bowerstone returned zero; that probe was stopped. World-entity enumeration
is not evidence of the native card-manager state while the guild is unloaded.
The final handoff uses persistent completion, stage, and CoreQuestWaiting.
Direct visual verification of the two available cards remains separate.

## Harness and remaining limits

The old checklist force-activated Guardian Sister Info and repeatedly crossed
regions. It now verifies natural prerequisites, guards a single crossing with
both scene queries, waits for the arrival scene, and approaches Maze once.
The interaction sequence is recorded from the attached run; the entire revised
checklist has not yet been replayed from a fresh launch. A guard prevents its
second input sequence from running if the first interaction started a movie.
The separate handoff checklist is read-only and does not consume completion
messages needed by gameflow. Seventeen targeted harness tests pass, including
regression checks against forced progression and repeated crossings.

Assistance: region transition, hero teleport, and manual interaction recovery.
This is normal conversation completion, not an unassisted walking-route test.
The arrival movie briefly displayed incomplete character geometry; this pass
does not establish asset-rendering correctness. Existing generated resource/EH
flag aliases and hit-reaction paths remain unaudited; no generated Lua or DLL
was changed for this result.

A new engine-written AutoSave is preserved as `adult_maze_completed_2026-09-23`.
Fresh-process reload passed 4/4: both completed quests, stage 400, and
CoreQuestWaiting remain intact. Reload report: `autopilot_maze_reload_2026-09-23.json`;
archive: `work/ab_runs/v11-20260923-170724/FableScriptExtender.log`. Both archives
contain zero Lua runtime errors. The game is closed and the original protected
profile restored byte for byte (all three files). Hashes and event excerpts are
in `maze_handoff_evidence_2026-09-23.json`.

The next Orchard checklist also contained direct activation, repeated crossings,
and an injected MissionSucceeded flag. Those were removed: actual card acceptance
is a prerequisite, the crossing is guarded once, and completion is read-only.
Real Orchard combat still needs implementation/live validation; this correction
is not an Orchard completion claim.
