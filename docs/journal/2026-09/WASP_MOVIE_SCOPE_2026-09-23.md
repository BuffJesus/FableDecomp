# Wasp Menace movie scope investigation (2026-09-23)

Resumed from the September 22 handoff and run 37. The Lua package is still the
generated WaspBoss readable output; this investigation changes instrumentation
and the driving checklist first.

## Additional movie owner

Yesterday's inventory of three movie sites covered only `WaspBoss.lua`.
`Entities/WaspHelper.lua` also starts a movie at lines 48 and 62 for the helper's
follow-me dialogue and destroys it at the normal or interrupted exits. In the
sidecar, `NoviUnitBindings.h` returns one `LuaRetailResources` per quest state.
The helper is therefore another candidate owner of the movie that prevents
`WaspIntro` from starting. The new run confirms it shares the same scope with
the intro; yesterday's uninstrumented collision cannot be attributed conclusively.

## Changes under validation

* `wasp_boss.json` queries `IsInCutscene` and `IsInMovieSequence` before each
  crossing. A busy scene is polled without arming a transition. The single
  transition command checks both queries again before it arms the crossing.
* `novi-z-movie-trace.patch` is generated from the scratch sidecar's actual source
  diff. Movie start/refusal records include the resource-scope address, handle,
  and Lua caller stack. Destruction records include the same scope and handle,
  including destruction during scope cleanup.
* `wasp_entities` now waits for a Picnic Area entity Main, since registration
  already happened at Lookout Point. `wasp_intro` confirms the Quest Start card
  with Enter and queries `QuestStartScreened`, rather than matching any dialogue.
* The checklist validator rejects `eval ...: return ...`: the exec channel wraps
  an expression in `return tostring((...))` itself. The first run caught this
  mistake before either crossing; it was corrected and the loaded game resumed.
* The checklist, resource member zero-init, and fresh-launch isolation tests pass:
  10 tests (the two new launch tests were run separately from the already-running
  full-suite discovery).

## Live evidence

The Release/x86 sidecar built successfully (`work/sidecar_movie_trace_build.log`).
Bundle `local-candidate-v10` has the same generated quest Lua as v9 plus tracing.
The attached replay reached Picnic Area and played the intro without a Lua runtime
error. Archive: `work/ab_runs/v10-20260923-081653/FableScriptExtender.log`.

* Lines 230-232: WaspHelper.lua:62 starts movie 3 in scope 520126780.
* Line 246: movie 3 is destroyed.
* Lines 255-264: the guarded Picnic Area crossing is armed once.
* Lines 366-369: WaspIntro starts movie 7 in the same scope.
* Line 746: movie 7 is destroyed.
* Line 756: the state query returns `WASP_INTRO_FINISHED`.

`autopilot_wasp_intro_2026-09-23.json` records the final state check. The attached
check needed a manual Enter at the Quest Start card; that action is now in the
checklist. The card displayed corrupted artwork and empty quest details, a
separate open issue. No combat/completion parity is claimed, and the checklist's
later forced mission-success steps were not executed.

## Uninterrupted verification

The fresh adult-save replay passed **9/9 steps**, with **zero Lua runtime errors**,
including confirming the Quest Start card without manual input. Report:
`autopilot_wasp_entry_2026-09-23.json`. Archive:
`work/ab_runs/v10-20260923-082058/FableScriptExtender.log`. The compact extracted
movie/crossing/state evidence is `wasp_movie_trace_2026-09-23.json`.
The staged save was restored by the driver's `finally` block and the test game
was closed after collection.
SHA-256 checks confirm all three restored AutoSave files match their pre-run
backup, and all 98 Lua files in v10 match v9 byte-for-byte.

The verification checklist is the first nine steps of the committed
`tools/script_recovery/checklists/wasp_boss.json`, ending at `wasp_intro`, saved to
`work/wasp_entry_verify_20260923.json`. Command:

```text
python tools/script_recovery/autopilot.py run v10 work/wasp_entry_verify_20260923.json --launch --save adult_graduated --report docs/journal/2026-09/autopilot_wasp_entry_2026-09-23.json
```

The full suite completed: **1,677 tests in 1,821.176 seconds; 403 failures and
45 errors**, matching the September 22 baseline. All **448 failure/error
identities**, including subtest cases, match after normalizing PowerShell line
wrapping and the `tools.script_recovery` import prefix introduced by `-t .`.
Comparison artifact: `wasp_regression_baseline_2026-09-23.json`; raw log:
`work/converter_suite_20260923b.err`. The two fresh-launch tests were added after
discovery and passed in the separate 10-test focused run. This is an unchanged
failing baseline, not an all-green suite. No converter changes were made during
this investigation.

## Next converter targets (offline evidence)

`refs/script_recovery/wasp_boss/translation_unit_typed.json` gives concrete next
targets, cross-checked against the generated `WaspBoss.lua`:

* `Main` 0x00E0EA40 constructs the `WatchForCutscene` thread with a body labelled
  `CScriptGameResourceObjectScriptedThingBase_HandleQueenHornetCutscene`. This is
  still TODO text in Lua; the generated function exists but Main does not start it.
* `DoMission` 0x00E12580 converts definition strings at global offsets 0xE3C and
  0xE40 via 0x00415D70 (calls 0x00E12698 and 0x00E1298A). The first feeds the five
  HornetDrone spawns and the second feeds the queen spawn. The Lua drops the first
  creation and emits a suspicious three-argument `CreateCreature` for the second,
  using the attacker list as its final argument. Recover the definition-string
  operands and the native CreateCreature signature before any queen-phase run.
* DoMission also constructs `GuildmasterHelp` through a body labelled
  `CScriptGameResourceObjectScriptedThingBase_HandleQueenHornetDialogue`; the Lua
  leaves this thread construction as TODO text too.

The clean replay log also has one pre-checklist channel error from a stale,
read-only intro-state query left in `commands.txt` by the prior attached run.
It runs before WaspBoss is created and changes no state. `launch_and_load` now
removes pending commands before starting a fresh game, after refusing a launch
if Fable is already running. The two launch-isolation tests prove that stale
commands/logs are cleared for a new session and preserved when a game is live.
This last harness change is unit-tested; the 9/9 replay preceded it. Zero Lua
runtime errors in that replay does not mean zero channel diagnostics.
