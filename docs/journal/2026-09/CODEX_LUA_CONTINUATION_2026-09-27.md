# Lua workflow setup and readability continuation, 2026-09-27

Resumed `feat/novi-script-recovery` at `7421f72` in the main checkout from
`docs/HANDOFF.md`, the member-resources journal, and Desktop `lua.txt`.
The user confirmed the previous Claude Lua session was stopped and authorized
continuing its uncommitted converter files. Native engine work was left intact.
Owned scratch: `work/codex_lua_resume_20260927/`.

## Workflow setup

Inspected built-in, user/plugin and project skill names/descriptions. Reused the
installed `skill-creator`; no relevant existing project skill was present.
Created `.agents/skills/fable-script-recovery/SKILL.md` and
`.agents/skills/fable-readability-pass/SKILL.md` (77 and 73 lines).
Both pass `quick_validate.py`; every Markdown reference resolves.

These replace repeated lane/evidence discovery and scratch-generation/validation
setup. Session ownership and sidecar ABI checks are part of recovery; no separate
session/ABI/tooling skills were needed. Existing GOTCHAS remains canonical.
No generic token-efficiency skill, duplicated C++/Ghidra procedure, new conversion,
plugin installation or AGENTS.md routing change was added.
The readability skill calls out that `build_readable_unit --out` does not change
its draft input; use `build(unit, draft=..., out=...)` for complete scratch runs.

## Recovered behavior

- Preserved and validated the inherited string-return, parent-field, resource-call,
  call-site pairing and by-value string/boolean operand recovery. These restore
  BeardyBaldy's speech and hairstyle predicates, Bordello cutscene arguments and
  named flags, and related string/animation calls in other existing units.
- BookCollecting's unnamed worker now uses the evidence builder's stable
  `NativeThread_00e566f0` key instead of `null`; the retail/PDB name is BookReaction.
- Its two marker reads retain runtime indices through
  `ReadGlobalGameDataStringAt(0x4b0/0x4bc, reactionIndex)`.
- A generic, exact Data/Info/vtable/retain copy pattern preserves by-value
  CScriptThing actors. Four SetIsPushableByHero calls now receive the boy/girl
  handles instead of nil. Unmatched copies remain unresolved.
- Added the pending indexed-string binding to the smoke inventory and upstream
  requirements. The inherited round-12 sidecar is clean at `a7bb755` and builds
  Release x86; its release-unit-data and indexed-string patches are retained.

## Validation

- New behavioral tests: 6 pass, covering both draft/readable paths, actor identity,
  marker indices, early exits, and rejection of unproven copy/string patterns.
- Focused suite: 177 passed / 30 subtests (`focused.log`). Windows Unicorn emitted
  handled access-violation diagnostics; these are preserved in the log. A separate
  subprocess-run targeted suite exits 0: 33 passed / 2 subtests (`targeted.log`).
- The previously reported AffairMan checksum test now passes (1 test, exit 0).
- All 29 units regenerated against the inherited source snapshot with the same
  active-worktree evidence. Only BookCollecting changes from this session's new
  lowering. Against committed drafts, the combined continuation changes 20 draft
  and 20 readable Lua files across eight existing units; remaining units match.
- The eight-unit before/after smoke reports are retained in `smoke/`. They are
  not all clean: BeardyBaldy improves 4 to 2 failing callbacks; Bordello changes
  4 to 5, with Client's existing resource-as-thing expression newly reached by
  the recovered flag and Magicman's existing nil EH temporary reached after
  acquisition recovery. Client's numeric resource is also a smoke mock limitation
  (`NewResource` returns 1). Other units retain their existing problem counts.
  Do not describe this as no runtime regressions or as a runtime pass.
- Promoted files equal the tested scratch output; previous bytes are retained in
  `promoted_baseline/`. Full readability inventory: 369 files, 0 syntax failures,
  1,850 unresolved diagnostics. Ledger refreshed with `audit_readability`.
- Full suite: all 2,456 collected tests covered by three file groups (645 files).
  2,414 passed; 30 test cases failed and 12 errored. Pytest additionally reports
  442 failed subtests and 41,318 passing subtests. Exact logs: `full_shard_0/1/2.log`;
  all three exit 1. Do not call the full suite green.
  Matching replay of the 42 failing/error cases against current code and original
  HEAD code with pre-promotion refs and identical retail/PDB inputs is complete.
  Both runs: 36 passed, 6 parent cases failed, 320 failed subtests and 1,880 passing
  subtests (exit 1). Failure/subtest summary lines match exactly; no current-only
  failure reproduced. The other 36 failures/errors did not reproduce in either
  replay, so this does not establish that every full-suite failure is baseline.
  Logs: `failed_cases_baseline.log`, `failed_cases_current.log`; comparison:
  `failed_cases_comparison.json`. The original-source loader was self-checked
  against an existing passing test and the new indexed-marker regression.
  The two inherited output-filtered full-suite processes were stopped after
  30 minutes without a result. The initial sequential Codex full run was replaced
  by the completed three-group run; partial runs do not count as validation.

## Bundle and remaining gates

`work/new-oakvale-original-fse-20260912/local-candidate-v31` is staged, not installed
or run in-game. It preserves v30's override roster and updates six existing
packages plus the sidecar. BeggarAndChild and TourGuide source improvements are
outside that roster and were not activated. Offline checks: 210 Lua files parse,
215 manifest hashes match. Sidecar SHA256:
`71b3a6c1c43435eea493ea5105634a5687b574cca57aca1f1b7cc8159f93c5fe`.

BookCollecting's teacher still has unresolved native spawned-object dispatch;
conversation/state-vector paths and Bordello resource/EH temporaries remain open.
The new worker tests isolate conversation calls and do not prove those paths.
Candidate member-resource release on host destruction also needs runtime lifetime
proof. Do not launch automatically: the user's current continuation deferred playtesting.
