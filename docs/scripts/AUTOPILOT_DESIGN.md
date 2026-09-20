# Hands-free script testing (autopilot) — design, 2026-09-20

**Goal:** run a converter bundle through a quest checklist with nobody at the keyboard, and get back a graded
report + the archived FSE log. Deterministic, not ML: the FSE log already says exactly what died; the missing
piece is *driving the game to the seam*.

## What exists today

| Piece | Where | State |
|---|---|---|
| Offline smoke (every generated file loads in a mock quest API) | `tools/script_recovery/smoke_run_unit.py` | gate, per regen |
| Scenario replays (drive a quest `Main` through state flips + a level reload in lupa) | `test_guild_woods_melee_converter.py`, `test_check_friendly_attacks_converter.py`, `test_cross_branch_goto.py` | 3 quests |
| Static audit (agents verify generated Lua against the typed C, adversarial verify) | Workflow `converter-operand-audit` | ad hoc, found 30+ silent bugs today |
| Launch + archive + timeline compare | `tools/script_recovery/ab_playtest.py launch/compare` | works |
| Unattended frontend driver (real input: title → profile → Continue → AutoSave → skip) + log waits + screenshots | `D:\Code\FableForge\tools\ingame\ingame_terrain_test.py`, `gamewin.ps1` | works (FableForge) |
| Log grading against a checklist | `tools/oakvale_reborn/grade_run.py` | Oakvale Reborn only |
| Sidecar diagnostics (cutscene commands, Speak keys, TryAcquire, lifecycle, quest info counters) | `NoviCompatibility.dll` | on |

## The gap: in-world control

Once the save is loaded, every step today is a human: walk to the Guildmaster, punch the dummy seven times, walk to
the woods, kill three beetles, answer YES. Three kinds of steps, three mechanisms:

1. **State-gated steps** (most of the Guild path): `DummyHits` → 7, `GenericTutorialCounter` → 5, `ScorpionsAlive`
   → false, `GameState` → n. These are quest-state writes. Needs a sidecar **exec channel**: the DLL polls
   `NoviCompatibility/autopilot/commands.lua` (or a named pipe) once per frame and runs each line *inside the named
   quest's VM* (`LuaQuestHost` owns one VM per quest, so `quest` is in scope):
   ```
   Q_GuildTrainingPreMelee: quest:SetStateInt("DummyHits", 7)
   Q_GuildTrainingWoodsMelee: for i = 1, 3 do quest:SetThingAsKilled(quest:GetStateListAt("GuildScorpions", i)) end
   ```
   plus `dump <quest>` (every state field the quest has written, via the state map the sidecar already keeps) and
   `eval <quest> <expr>`. Result lines go to the FSE log with an `[Autopilot]` prefix so the driver can await them.
   Cost: ~150 lines in `NoviUnitBindings.h` / `LuaManager.cpp` (file poll in the per-frame update, `sol::state::script`
   in the target host's VM, protected). No retail RE needed.
2. **Position steps**: "be at the woods door", "be next to the Guildmaster". `quest:EntityTeleportToThing(hero,
   quest:GetThingWithScriptName("MK_GTM_WD_GUARD"))` through the same channel; region transitions happen naturally
   when the hero is teleported past the door (retail streams the neighbour). `SetTimeOfDay` for night-gated steps.
3. **Message-gated steps** (`IsTalkedToByHero`, `MsgIsHitBy`, `MsgIsQuestionAnsweredYesOrNo`, `MsgIsGameInfoClickedPast`):
   these are engine messages we should NOT fake — they are exactly what we are testing. Drive them with real input
   from the FableForge harness: teleport the hero in front of the NPC (channel), then `gamewin.ps1` sends the key
   (`E`/mouse for talk, `Enter`/`Y` for the question, attack clicks with lock-on for the dummy). The driver already has
   `click_until_change` + screenshot diffing to confirm the frontend reacted.

## Driver (Python, extends `ab_playtest.py`)

```
autopilot.py run --bundle v6 --checklist checklists/guild_training.yaml --save f645456fds
```
1. `local_test.py --launch` (as today) → wait for `[NoviCompatibility] ... hosts enabled` in the log.
2. Frontend: `gamewin.ps1` sequence to the chosen profile/save (already scripted in FableForge; parametrise the profile).
3. For each checklist step: `do` = channel command(s) and/or input; `expect` = log regex + timeout; `on_fail` = screenshot,
   `dump` every active quest, archive, stop. Steps never guess: each `expect` is a retail-proven marker (cutscene macro
   name, Speak key, quest completion) taken from the retail-sequence tables in the journals.
4. Quit via the frontend (or kill after the last step), archive to `work/ab_runs/<bundle>-<ts>/`, grade.
5. `compare v6 v7` as today; a step table per run (`PASS / FAIL(at) / SKIPPED`).

Checklist format (one per quest unit, written from the retail sequence — the Guild one is in
`docs/journal/2026-09/GUILD_ARRIVAL_PLAYTEST_2026-09-19.md` and `V6_RETURN_CRASH_2026-09-20.md`):
```yaml
- id: punch
  do: [ "Q_GuildTrainingPreMelee: quest:SetStateInt(\"DummyHits\", 7)" ]
  expect: { log: "command=TEACHER.Speak TEACHER,'TEXT_CS_028_PREMELEE_STICK", timeout: 30 }
- id: woods_return
  do: [ "hero teleport MK_GTM_WD_GUARD", "Q_GuildTrainingWoodsMelee: <kill beetles>" ]
  expect: { log: "CS_GUILD_MELEE_WOODSWON", timeout: 60, forbid: "PREMELEE_PUNCH_10" }
- id: split_yes
  do: [ input: "Enter" ]
  expect: { log: "PlayAVIMovie", timeout: 20 }
```
`forbid` catches the replay class (PUNCH after the woods) that a pass/fail on the expected marker alone misses.

## Order of work

1. Sidecar exec channel + `dump` (the only new native-side code; half a day). Log prefix `[Autopilot]`.
2. `autopilot.py` step runner over the existing launcher + `gamewin.ps1`; checklist for the Guild path up to the
   beetles (all state/position steps; the two message steps — talk, YES — via input).
3. Grader = `grade_run.py` generalised (step table + the error/forbid scan `ab_playtest.py` already does).
4. Nightly: regen → smoke → scenario replays → audit workflow (Guild cap raised) → autopilot v6 and v7 → report.
   Anything red stops before the in-game half, so the game is only launched on a tree that passed the offline gates.

Not in scope: recognising failures from pixels (the log is authoritative; screenshots are for the human afterwards),
faking engine messages (that is what we test), retail RE (none needed — every marker is already in the log).

## Status 2026-09-20 (evening)

* **Exec channel built** — sidecar `a02ba57`, `FableScriptExtender/Autopilot.h` (header-only; registry from the
  `LuaQuestHost` ctor/dtor, `Poll()` from both `NewScriptFrame` forms, 200 ms throttle, reentrancy-guarded). Commands
  file `<bundle>/NoviCompatibility/autopilot/commands.txt`, consumed before execution; every result is an
  `[Autopilot] ...` log line (`batch <id>`, `ok`/`error`/`eval`, `state k = v`, `host`, `processed N line(s)`).
  In `tools/script_recovery/sidecar_patches/novi-unit-bindings.patch`; v6 / v7 rebuilt on it (preflight passed).
* **Driver** `tools/script_recovery/autopilot.py` (`send`, `run <checklist>`, `tail`): atomic batch write, waits for
  the batch's `processed` marker, checklist steps with `expect` / `forbid` / `timeout` / `repeat` + `interval`,
  dumps every live quest on the first failure and stops. `input:` steps are SKIPPED until the FableForge frontend
  driver is wired. Protocol proven offline against a fake sidecar (`test_autopilot_driver.py`, 4 tests).
* **First checklist** `checklists/guild_woods_return.json`: channel → punch 7 → stick 7 → teleport to the woods →
  kill beetles (repeat) → teleport back → expect WOODSWON, forbid `PREMELEE_PUNCH_10` → (YES = input, skipped).
  Markers are the ones the archived runs print.
* **NOT run in-game yet.** First live use, with the game free and a save at the punch stage loaded:
  `python tools/script_recovery/autopilot.py send v6 list` then `... run v6 tools/script_recovery/checklists/guild_woods_return.json`.
  Open questions the first run answers: does `EntityTeleportToThing` across the Guild/woods boundary stream the region
  (retail cutscenes teleport across regions, so expected yes); does `SetThingAsKilled` on a spawned beetle count for
  ScorpionHome's `GetAllThingsWithScriptName("GuildScorpions")` accounting (it removes the thing, so expected yes).
