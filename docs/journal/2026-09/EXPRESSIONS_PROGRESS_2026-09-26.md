# Expressions progress and packed flags - 2026-09-26

Continuation of `EXPRESSIONS_RECOVERY_2026-09-26.md`. Another active session owns
the teleport/resource checkpoint and its full-suite audit. This continuation
preserves that work and adds the numeric and flag recovery described here.

## Converter changes

`fold_reused_string_arithmetic` separates scalar arithmetic from a reused
`CCharString` slot when an explicit float read follows before the next address
use. It preserves mutable progress stores and decodes a float clamp's
`0x3f800000` as `1.0`. Integer-to-float reads preserve the integer random roll.
Plain integer/pointer arithmetic and unproven calls are excluded. An initial
broader version changed Arena; requiring the float read removed that change.
The read-only float constant fold now accepts Ghidra's `_DAT_` spelling too.

`fold_upper_byte_flags` recovers two boolean flags from a ushort widened to a
32-bit word, with explicit CONCAT writes to bytes 2 and 3. Unexpected whole-word
uses or arithmetic byte reads reject recovery. The excess register arguments
Ghidra attaches to the zero-argument CRT `rand` are ignored. No value is guessed
for the unobserved low word.

The decompile is corroborated by Steal's retail instructions at `0xEEBF66` and
`0xEEBF6B`, which clear the two bytes. `0xEEC0ED` through `0xEEC0FB` computes and
stores `1 - remaining / duration`; `0xEEC11B` stores the `1.0` float clamp.

## Validation

- Picklock and Steal execute progress, completion, button release, damage,
  witnessed deed, dead/null target, interruption, and ineligible-target paths in
  freshly generated draft and readable Lua. Integer and fractional durations
  are covered. `test_expression_progress.py`: **74 passed**, including rejection
  tests for ambiguous source patterns.
- Final combined focused run: **224 passed, 28 subtests passed**. This includes
  the other session's additional `GetPos` contract regression test.
- Expressions: 41/41 functions and 13/13 script files parse; **47 TODOs remain**.
  Readable: 14 files parse, no fallback; smoke: **13 scripts, zero problems**.
- The same numeric rule improves Pickpocket's three scalar slots. Its target
  copy and secondary timer still have unresolved logic. No whole-package
  gameplay claim is made.
- Final all-unit A/B against the pre-session converter: nine Expressions scripts,
  White Balverine WW, and Chicken Kicking's epsilon condition change. All other
  registered units, including the final Arena rerun, are unchanged.
- New Oakvale also matches the corrected baseline byte-for-byte: 18 Lua script
  files plus the disabled registration file; 51 functions, 1199 existing TODOs.
- Frozen test logs, comparison diffs, and module snapshots are under
  `work/expressions_progress_20260926/`. `ab_results.json` merges the final Arena
  rerun with the other 22 registered units.

The earlier broad-suite process belongs to the teleport checkpoint, started
before this continuation's edits. Its result is not a full-suite pass for these
new changes; see the other checkpoint's journal for that audit.

## Offline bundle

The resource-binding DLL was rebuilt from a fresh copy of `sidecar-abi-v13` plus
things-killed, cancel-using-ability, master-state-setters, and resource-vtable
patches. Release/x86 build passed; source and DLL are under
`work/lua_resume_20260926/sidecar/`.

`work/lua_resume_20260926/bundle/` derives from v29, adds Expressions and refreshes
White Balverine. All **168 manifest file hashes** match. Existing campaign and
new resource binding names are present in the DLL. This candidate was not
launched; the installed game and saves were not changed.

## Resume

1. This continuation is left in the working tree while the other session commits
   its isolated teleport checkpoint. A separate patch against that session's
   validated snapshot is saved as
   `work/expressions_progress_20260926/progress.patch`. Commit the progress changes
   separately after the teleport checkpoint; do not apply the patch over the
   already-modified working files.
2. Finish Pickpocket's target-copy and secondary-timer recovery, with behavioral
   tests, before treating the full Expressions package as playable.
3. Test the reviewed expression paths through the in-game runner using an
   isolated candidate and protected saves. The offline tests do not prove the
   new resource methods' runtime ABI or every remaining TODO path.
