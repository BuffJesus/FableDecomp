# Expressions recovery continuation — 2026-09-26

Resumed from the Desktop `lua.txt` transcript, on `feat/novi-script-recovery`.
The checkout also contains unrelated native reconstruction work; it was preserved.

## Converter changes

The previous session's pending changes recover quest-owned resource vtable calls,
the string overload of `MakeHeroCarryItemInHand`, empty-string comparisons, and
the end of a destroyed string temporary's lifetime. They also recover read-only
float constants and vector operands/results for guild-seal recall. The resource
bindings are in `novi-zzzzzzzzzzz-resource-vtable-methods.patch`.

This continuation fixes the remaining `Global_TeleportToHeroGuild` conversion gaps:

- Three zero stores to a declared `C3DVector` immediately consumed by address are
  preserved before counted-pointer cleanup deletes zero stores. The recall-clear
  call now receives `{x = 0, y = 0, z = 0}` and angle `0.0`.
- Resolve a local list's out-pointer alias before canonicalizing its begin/end
  fields. Recognize `GetFollowingEntityList` as a list fill. The follower loop now
  iterates the returned Lua table instead of reading native layout fields.
- Recover `PerformExpression` on a proven resource before the generic noise pass
  drops calls with Ghidra's excess register arguments. Retail Main `0x00CDD6B0`
  calls it once per follower with `EXPRESSION_WAIT`.
- The previous session's first recall-location and teleport-position operands
  were already correct in the working tree; they are covered by behavioral tests.

The early resource-call recovery is deliberately limited to `PerformExpression`.
An initial broader version also emitted `Expression_Follow`'s `FollowThing` with
an unresolved `pcVar5` distance. The all-unit comparison caught this, and that
extra emission was removed. Do not infer an operand merely from a method name.

## Validation

- `test_expressions_recovery.py`: 19 passing cases, exercising freshly converted
  draft and regenerated readable teleport code. Departure with 0/1/3 followers,
  return from near the marker or inside the Guild, negligible recall positions,
  interruption cleanup, and non-vector/non-adjacent zero-store rejection.
- Together with the lifter, vector-register, thing-release/flag, moving-dummy,
  AffairMan home, and BookTrader home tests: **159 passed, 53 subtests passed**.
- Expressions readable: 14 Lua files parse, no readable fallback. Smoke runs 13
  script files and reports two existing failures: Picklock and Steal free globals.
- White Balverine readable smoke: 11 files, zero problems.
- Chicken Kicking's epsilon is also `DAT_0129BA3C`: the float fold changes only
  that condition in its draft. Its existing readable problems remain (six smoke
  findings); this is not a playable unit claim.
- Final isolated sources, comparison artifacts and test logs:
  `work/lua_teleport_checkpoint_20260926_9ad7/`. Earlier full-suite logs are in
  `work/lua_resume_20260926/`.
- All **23** registered units compared against HEAD: exactly eight draft files
  differ (six Expressions files, WhiteBalverineWW, KickedChicken). Every diff was
  reviewed. The initial broad-resource comparison was superseded by a rerun of
  Expressions after narrowing the early recovery rule; `ab_results.json` contains
  the corrected final list.
- The three installed-asset audit failures examined so far reproduce with the
  HEAD converter loaded: barrel reward component, dead-father cutscene assets,
  and final barrel gold release. The installed `StartOakValeWest.tng` and
  `names.bin` differ from their audit baselines; no game assets were changed here.
- Resource sidecar Release/x86 build passes in the previous session's scratch
  `sidecar_v21` directory. Reverse patch check confirms the resource-method patch
  is present. Build log: `work/lua_resume_20260926/sidecar_build.log`.
  The DLL is preserved as `work/lua_teleport_checkpoint_20260926_9ad7/NoviCompatibility.dll`
  with its SHA-256 in `sidecar_artifact.json`; it has not been deployed.
- An additional New Oakvale A/B exposed a regression in the inherited broad
  `*Pos`/`*Position` return tagging: adding metadata to `GetPos` invalidated the
  AffairMan/BookTrader home evidence contracts and their downstream recovery.
  Restrict the new return tag to **GetGuildSealRecallPos**. After that correction,
  all 18 New Oakvale Lua files match HEAD byte-for-byte (51 functions, 1199 TODOs).
  The dedicated regression test pins the unchanged `GetPos` contract.

Another active session began editing packed-flag/numeric lowering in the same
file and reusing the earlier comparison directory. Those edits were preserved.
This checkpoint uses isolated copies of the tested modules and generated files;
the other session's subsequent additions are not included in it.
It is saved on `checkpoint/lua-teleport-20260926-9ad7`, based on `ed32330`, using
a separate Git index so concurrent staged work and the shared branch are preserved.

Pytest's Windows fault handler prints repeated access-violation diagnostics while
Unicorn handles its own native exceptions. The allegedly crashing isolated file
`test_affair_man_complete.py` actually completes (4 passed). Run the broad suite
with `-p no:faulthandler`; changing Python 3.14 to 3.13 alone does not remove the
diagnostics. The first diagnostic-filled attempts were stopped, not passed.

The full run completed in 29m10s: **1798 passed, 475 failed, 3 errors, 42681 subtests
passed**. These are pytest's raw counts, including hundreds of failed subtest
cases, and describe the version before the `GetPos` contract correction.
There were 38 unique test nodes with failures/errors. Rerunning them on the final
isolated converter produced **22 passed, 455 failed, 3 errors, 1184 subtests passed**.
Twenty formerly failing nodes were completely repaired by the contract fix.
The remaining **18** nodes (including two whose parent passes but subtests fail)
were rerun with the HEAD converter: the exact same 18 nodes fail/error there,
with **455 failed, 2 passed, 3 errors, 1023 subtests passed**. Their issues include
installed-asset hashes, runtime-source audit expectations, and existing
LiveFather/Bully/WatchBarrels harness mismatches. No full green suite is claimed.
Lists and logs: `initial_failed_nodes.json`, `remaining_failed_nodes.json`,
`failure_comparison.json`, `full_initial.log`, `failure_rerun.log`, and
`baseline_remaining.log` in the isolated checkpoint directory.

## Resume

1. Fix the numeric/packed-flag conversion shared by `Expression_Picklock` and
   `Expression_Steal` before activating the whole Expressions package. Steal has
   more gaps than its two free names: `CCharString`-typed numeric stores are dropped,
   packed `CONCAT13`/`CONCAT12` operations survive, and a float bit pattern can be
   used as an integer in a condition. Do not just initialize the free names.
2. Stage the preserved resource-binding DLL with reviewed scripts and test in-game
   through the runner. No new live bundle or in-game success is claimed here.
3. Earlier queue remains Arena, Trader Rescue runner, and the unplayed side units;
   see `CHAIN_AFTER_BANDIT_CAMP_2026-09-26.md`.
