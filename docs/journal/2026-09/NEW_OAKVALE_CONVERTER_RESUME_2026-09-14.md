# New Oakvale converter resume — 2026-09-14

Resumed from the September 13 night checkpoint. Preserved the dirty workspace.

## Changes

- Repaired the husband test import to use `affair_man_complete.LABELS`, the
  authoritative generator mapping. The two import errors shared this cause.
- Replaced Bully's obsolete positive rename-count and unstructured-join assertions
  with checks of the actual structured helper mapping, entry condition, ledger
  accounting and absence of executable native jumps. Existing behavior comparisons
  remain intact.
- Validated both pending StartBarrelTimer changes: the empty native literal must
  be a zero byte, and timer callbacks cannot replace the previously captured HUD
  update method. Added a negative test for the literal witness.
- Added `readable_start_barrel_timer.py` and builder integration. Exact raw helper
  SHA is checked before replacement; native witnesses remain mandatory. This pass
  runs before WatchBarrels inserts its local helper functions, preserving the
  raw function boundary used by the hash guard.
- Added emitted-package comparisons covering 36 wait, cancellation, return and
  empty/nonempty retained-Thing combinations against original native execution.
- Composed all six timer HUD methods over the common timer-owner stage using
  `prepare_start_barrel_timer_extension.py`. The cumulative patch is unapplied at
  `work/start_barrel_timer_integration/start-barrel-timer-integration.patch`.

## Evidence

- Final consolidated selection: 17 tests passed; captured Python exit zero,
  process elapsed 86.245s. No test/build processes remain pending for this batch.
- Initial regression/native selection: 12 tests passed in 56.385s.
- Native timer gate after negative-test addition: four tests passed in 7.055s;
  the two comparison loops cover 414 native-versus-Lua scenarios.
- Emission/marker/Bully selection: seven tests reported OK in 74.959s. PowerShell
  treated redirected unittest stderr as NativeCommandError, so a final consolidated
  run captures Python's exit code directly in
  `work/converter_resume_20260914_verified.result.json` and its adjacent `.log`.
- Actual FSE/x86/real-Lua isolated helper: 132 policies passed, three compiler/run
  exits zero. Executable SHA256:
  `6e9eed66c92b7b4b18ab753ab55786fac6db5f7cc93ecb9dba553f24c273b8d0`.
- All combined resource registration templates compile as x86. Object SHA256:
  `6531e2766c88ebdc8abf6a7649793b750a23b9e403bebdbc23cc455525632d68`.
  Cumulative patch `git apply --check` passed against ForgeFSE-retail-shadow.
- Persistent readable package regenerated: 51 inventoried functions, 18/18 syntax
  passes, still disabled. Rename totals are transformation counts, not completeness.

## Next work and limits

Next: the four PostAttackStuff atomic adapters from the night checkpoint, followed
by the remaining quest/lifecycle audit and isolated entity capability composition.
Latest common stage is `work/start_barrel_timer_integration`.

The isolated timer harness and combined registration compile are separate evidence;
neither proves a full linked DLL or merged engine behavior. Full-suite rerun,
remaining capability integration, scheduler teardown, save/restore and live gameplay
validation remain open. Nothing was installed, activated or committed.
