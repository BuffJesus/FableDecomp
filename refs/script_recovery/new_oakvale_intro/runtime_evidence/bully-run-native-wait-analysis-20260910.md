# Bully RunTo native wait analysis (2026-09-10)

Authoritative retail interpreter evidence is `ghidra_out/runcutscenemacro_full.c`, function
`RunCutsceneMacro_Func` at `0x00CBFB7D`. The `.RunTo` branch:

- parses the destination, radius, and boolean operands;
- invokes scripted-resource vtable slot `0x14` with movement enum `1` (run);
- evaluates the authored wait operand with `IsFalse(local_3c)`;
- when that operand is false, repeatedly polls resource slot `0x68`
  (`IsPerformingScriptTask`), advances script frames, and checks cutscene skip state.

The compiled `CS_OAKVALEINTRO_BULLYRUN1` command is
`BULLY.RunTo MK_OIBR_BULLY2,0,FALSE,FALSE,FALSE,TRUE`; therefore retail requests the polling path.
The following `GamePause 0.5` is not intended to substitute for movement completion.

`analysis-bully-run-variance-20260910.json` measures all five archived Forge captures. One reaches
the marker exactly; four terminate at the identical point 11.759 m short after moving only
2.326-2.365 m. Starts differ by only centimetres. Because the same build and macro can fully succeed,
the actor map, destination, move enum, and global ABI are not categorically wrong. The repeatable
partial endpoint instead identifies a nondeterministic scripted-task/path completion seam. The scoped
Lua compatibility move is justified: it runs only outside retail's 2 m tolerance, targets the same
authored marker, retains run movement, and does nothing after native success.

The Forge boundary is now machine-audited by
`tools/script_recovery/audit_bully_cutscene_wrapper_boundary.py`. Retail's Bully call loads the macro
and actor map in `ECX`/`EDX` and pushes flags, input substitutions, setup condition, and skippable in
the order declared by Forge's `__fastcall` type. Forge forwards those six semantic arguments in the
same order, borrows an already-open caller movie sequence, and destroys a movie resource only when
it created that resource itself. The intermittent endpoint is therefore downstream of correct macro
entry and upstream of the native task poll becoming false; changing Forge's ABI or movie teardown is
not evidence-backed.
