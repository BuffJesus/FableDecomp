# New Oakvale converter — night checkpoint, 2026-09-13

User requested a stop for sleep. Resume only when asked. The completion goal remains
unfinished; do not start Guild Training or treat low TODO counts as completion.

## Current state

- Authoritative readable package: `refs/script_recovery/lifted/NewOakValeIntro/readable/FSE/`.
  Build with `python -m tools.script_recovery.build_readable_new_oakvale`.
- Latest READABILITY_REPORT.json: disabled-incomplete, 51 inventoried functions,
  18/18 Lua syntax passes. Its 179 renamed / 168 semantic / 11 scratch counts describe
  recorded transformations, not a completeness percentage. Historical raw diagnostics
  remain 1,203; do not confuse raw output with the readable package.
- Exactly two explicit TODO(native) comments remain in readable Lua: quest state
  pointer in PostAttackStuff and destructor placeholder. Zero entity TODO markers
  does not prove every entity is behaviorally complete or integrated.
- Many entity bodies now use structured Lua, semantic names and verified resource
  adapters, including husband/wife, Theresa, father actors, victim and barrel scenario.
  Evidence includes original native-byte execution and compiled x86/real-Lua adapter
  tests. Phase-boundary tests prove only the tested boundaries, not whole gameplay.
- Main now binds all 16 entities, including DeadFather; its initial objective uses a
  verified atomic adapter. WatchBarrels and ManageQuestCoreMarkers are structured.
- PDB scope/local evidence is documented in `docs/scripts/NEW_OAKVALE_PDB_EVIDENCE.md`.
- Nothing installed or activated. Quests remains empty. Preserve the large dirty
  worktree and unrelated work; no broad reset, clean, stash or automatic commit.

## Regression result — first repair on resume

Suite16 has FINISHED; shell8977 consumed, exit1. Do not poll or restart that handle.
`work/converter_marathon_suite_20260913_16.log` and `.result.json` record:
1,488 tests in 1,680.980s, one failure and two errors (process elapsed1,690.615s).

1. test_readable_affair_man cannot import test_structure_affair_man_lua.
2. test_structure_affair_man_lua imports removed HUSBAND_LABELS from the shared
   builder. Complete husband generation moved this presentation responsibility out
   of the builder. Repair the tests' dependency using authoritative generator data;
   do not restore a misleading builder dependency just to turn the gate green.
3. test_bully_readable_integration.test_disabled_package_entry_and_main_ledger
   expects row['renamedLocals'] > 0 but gets 0. Inspect why the current structured
   candidate needs no generic renames and assert meaningful behavior/ledger evidence.

These are NOT the old known Bully two failures/two errors. Latest prior green full
suite15: 1,445 tests in1,285.285s. Suite16 discovery predates some later changes;
after targeted repairs and integration, run a new complete suite once appropriate.

## Runtime integration checkpoint

Latest common stage: `work/oakvale_timer_integration/`.
Reproduce with `python -m tools.script_recovery.run_oakvale_timer_integration_checks`.
It composes the marker stage with explicit nativeLifetime="NewOakValeIntro" registry
metadata, allocator policy propagation, pre-Init timer construction, transient timer
GetStateInt access and final-member cleanup before speech vectors. Default-off
preserves existing reconstructed ports that register their own timers.

- All three affected actual host translation units compiled.
- 28 actual-FSE/real-Lua timer policy cases passed; all six command exits0.
- x86 executable SHA256:
  980834ba6c6f2871de4cda2efcb85c50af5531d1b9ca6a692a768924fcee35ad.
- All common resource registration templates compiled; registration-result.json
  records exact inputs/object hash. Cumulative oakvale-timer-integration.patch passed
  git apply --check against D:/Code/ForgeFSE-retail-shadow. Still unapplied.
- Composer validates inherited hashes and preserves LuaManager.cpp as well as headers.
- Full DLL link/build, complete capability inventory/merging, scheduler teardown,
  state versus Lua VM destruction order, persistence/restore and live playthrough
  parity are still required. No claim of whole-runtime validation.

## Resume implementation in this order

1. Read this checkpoint and current files, repair the three regression outcomes.
2. Review worker checkpoint `work/start_barrel_timer/INTEGRATION.md`; integrate only
   after checking its reported tests and native evidence. Agent was asked to stop.
3. Finish PostAttackStuff atomic adapters before integrating the isolated structured
   `tools/script_recovery/post_attack_body.lua`. Missing capabilities:
   PostAttackStartIsAlive, TeleportToPostAttackStart, SetPostAttackVillageLimbo,
   PostAttackHeroNearTrigger. test_post_attack_dispatcher passes108 original-byte
   control-flow scenarios with explicit phase boundaries; it does not prove these
   missing adapter internals. Existing emitted movie-only recovery remains canonical.
4. Audit remaining quest helpers/DoMission/Main/lifecycle for raw labels, staging
   temporaries, omitted semantics and owner lifetimes. The two TODOs are not the
   complete work inventory. Do not erase the destructor placeholder without proof
   that timer, vector and base lifecycle responsibilities are accounted for.
5. Merge still-isolated entity capabilities into the common runtime stage (notably
   complete husband/wife, victim and DeadFather); derive full required API inventory
   from emitted Lua and reports. Compile/link the full staged DLL and validate host
   cleanup/restore/dispatch behavior before activation or live trace comparisons.
6. Final readable generation, targeted and full regression gates, then separately
   document engine/playthrough evidence or externally blocked live validation.

## Native details worth preserving

- Timer ctor registers ambient then watch at fields104/108; destructor closes watch
  then ambient via fresh singleton143E8F8, before eight speech vectors and base.
- PostAttack start lookup: key CString then owned lookup output; query uses returned
  pointer, closes owned output BEFORE key. Teleport and village-limbo use the same
  ownership ordering. Dadtrigger owned Thing persists across wait/movie/restoration;
  each distance check fetches fresh Hero, uses5.0, closes retained trigger at final join.
- Actual API stack returns matter: later call arguments may already be staged across
  intervening getters. Never assign staged arguments to the wrong call.
- Lupa Python exceptions can distort nested pcall error tests; use Lua error wrappers
  for primary-error precedence checks. Keep native and compiled evidence separate.

No root test processes remain live at checkpoint. Timer check19461, registration16542
and fullsuite8977 are terminal and consumed. Background worker final status is in
its INTEGRATION.md and the appended checkpoint note below.

Worker final: stopped, no live processes. StartBarrelTimer has414 native comparisons
and132 compiled host cases passed. Later stricter empty-literal verification and
harness vtable-mutation assertion are UNVALIDATED; refresh tests and staging on
resume. Worker session60738 completed exit0 before stop. No shared/runtime edits.
