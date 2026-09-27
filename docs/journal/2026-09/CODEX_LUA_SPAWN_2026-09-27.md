# BookCollecting parent worker dispatch, 2026-09-27

Continued the authorized Lua lane from `0c2a6f5`; scratch is
`work/codex_lua_spawn_20260927/`. Applied the project recovery/readability skills.
Other lanes' dirty native-engine work remains untouched.

## Evidence and change

The teacher donation helper `0x00E55CE0` allocates a 0x40-byte spawned function.
Retail instructions at `0x00E5646C` load its parent quest from entity +0x14;
`0x00E564B8` stores worker `0x00E566F0` at object +0x34, then +0x38 gets
that parent and +0x3c gets the reaction index from EBP. The name constructors
use `BookReaction` and `ParentClass.`, followed by the string append call.
The object is registered with that same parent at `0x00E564E2`.
Ghidra misleadingly types the direct parent/index stores as CCharString.

- Added a conservative entity-parent single-word capture matcher. It requires the
  complete constructor, parent load/store, object layout, name prefix and parent
  registration. Layout/owner changes and extra body effects remain unresolved.
- Captured operands go through normal expression resolution at the spawn site.
  The worker receives the original index even when the caller later changes it.
- Re-ran the existing inventory recovery: only the thread's previously null name
  changes. Updated unit metadata from `NativeThread_00e566f0` to `BookReaction`.
  Caller and exported worker now agree; no new sidecar binding was needed.
- Promoted seven generated Lua/report files after byte comparison with scratch.
  Teacher emits `quest:CreateThread("BookReaction", {args = {value}})` after
  clearing ReadingBook. The native allocation-failure machinery is abstracted
  by the existing Lua CreateThread mechanism, as in other lifted workers.

## Validation

- Focused suite: **129 tests and 28 subtests pass**, exit 0 (`focused.log`). Includes
  runtime capture/reassignment, negative matching, fresh retail naming evidence,
  prior thread/lifter tests and draft/readable worker regressions.
- Generated teacher tests exercise acceptance, rejection and termination against
  the actual exported worker name. Conversation/resource gaps and cutscene string
  construction are isolated with mocks; these are dispatch tests, not donation parity.
- **29 registered units** regenerated before/after with identical evidence inputs:
  only BookCollecting's quest and teacher Lua change (`comparison.json`).
- Before/after smoke has the same three failures: the teacher opinion helper's
  DAT_012448ec concatenation and nil global-data indexing in BookReaction and
  DoConversation. The worker name changes, but the error categories do not.
- Full readability audit: **369 files, zero syntax failures**, 1,844 unresolved
  diagnostics (previously 1,850). Ledger refreshed once after promotion.
- The preceding full-suite baseline/context failures are recorded in
  `CODEX_LUA_CONTINUATION_2026-09-27.md`. That whole suite was not repeated for this
  narrow matcher; focused coverage and complete unit A/B are the current gates.

## Candidate and next work

`work/new-oakvale-original-fse-20260912/local-candidate-v32` derives from v31 and
changes only BookCollecting. All **210 Lua files parse; 215 manifest hashes match**.
The override roster and sidecar (a7bb755) are unchanged. Neither candidate was
installed or launched in-game in this continuation.

The parent worker dispatch gap is closed offline. Next recovery targets are
BookCollecting's donation/reaction state vectors, conversation/global-data paths,
and the teacher's opinion helper string; Bordello resource/EH temporaries also
remain. In-game behavior and sidecar resource lifetime proof are still pending.
