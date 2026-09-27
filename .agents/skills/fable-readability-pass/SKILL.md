---
name: fable-readability-pass
description: Validate FableDecomp Lua generator and readability changes with scratch regeneration, behavioral tests, and unrelated-unit comparisons before promotion. Use for existing ports; excludes hand-editing generated Lua and new conversion selection.
---

# Fable readability pass

## Preflight

Resume ownership and authorization through [fable-script-recovery](../fable-script-recovery/SKILL.md).
Read [style plan](../../../docs/scripts/READABLE_STYLE_PLAN.md) for generator boundaries,
and [ledger](../../../docs/scripts/CURRENT_LUA_READABILITY_REVIEW.md) for current inventory.
Use the current journal for baseline claims; do not embed changing counts in this skill.
Snapshot the relevant dirty files and record the base commit in an owned `work/<task>/` directory.
Existing uncommitted changes are part of the baseline, not automatically yours to replace.

## Choose the correct layer

- Native operand/field/call errors belong in evidence or general lowering.
- Presentation changes belong in the readable pass; retain the faithful draft and diagnostics.
- Do not remove an unresolved operation, jump, cleanup or yield merely to reduce a metric.
- Inspect native control flow before merging conditions or moving declarations.
  Preserve random-call counts, short-circuit order, termination checks and resource lifetimes.
- Lua zero is truthy; C numeric/boolean conversions require explicit semantics.
  A nil receiver or by-value argument can be an ABI fault even when syntax succeeds.

## Scratch generation

Run from the resolved repository root; check each tool's arguments before reuse.
These are existing command shapes, with task/unit placeholders replaced:

```powershell
python -m tools.script_recovery.convert_quest_unit --unit <unit> --out work/<task>/draft
python -m tools.script_recovery.build_readable_unit --unit <unit> --out work/<task>/readable
python -m tools.script_recovery.smoke_run_unit --unit <unit> --package-dir work/<task>/readable/FSE --json work/<task>/smoke.json
```

Important: `build_readable_unit --out` changes only its destination; its draft input is
the registered unit's normal draft tree. It does not automatically read the scratch draft above.
For a complete scratch pipeline, inspect its `build` API and pass the intended source explicitly,
or use an owned isolated checkout with the required evidence copied read-only.
Do not temporarily overwrite the shared draft just to redirect an input.
The module roots derive from `__file__`; check any reused journal helper for root assumptions.

## Validation ladder

1. Add or run a focused behavioral regression for the actual failure.
   Include a negative case where an ambiguous pattern must remain unresolved.
2. Parse every affected generated Lua file and inspect its conversion/readable reports.
   Read smoke JSON, distinguishing unavailable mock bindings from actual faults.
3. Compare before/after calls, arguments, state changes and exits for affected paths.
   Reduced TODO/goto counts are evidence to inspect, not an acceptance gate alone.
4. Regenerate unrelated registered units with both generators when shared lowering changes.
   Compare contents, including added/deleted files. Explain each changed unit.
   Compare source hashes and evidence availability before attributing a difference to code.
5. Run broader tests only as justified by impact. For pre-existing failures, compare the
   same tests in the baseline with equivalent inputs; missing ignored files invalidate comparisons.
   Preserve summaries and failure identities. Do not suppress or relabel new failures as baseline.
6. Reuse completed reports only when source/evidence inputs still match. Empty logs or live
   test processes are not completed validation. Do not edit inputs while owned tests read them.

## Promotion and reporting

Promote only the reviewed affected output, then verify it equals the tested scratch result.
Refresh the readability ledger using its existing audit procedure after successful validation.
Record exclusions/superseded artifacts explicitly; do not hide syntax failures by narrowing inventory.
Update only the lane journal and relevant handoff status with commands, results and limitations.
Package with an explicit new bundle name; inspect `build_unit_playtest_package.py` arguments.
Check package Lua syntax and manifest hashes, and identify the sidecar DLL actually included.
Do not equate packaging with installation or a successful in-game run.
Keep runtime proof and assisted paths separate from offline evidence.
Stop after acceptance is met; retain unresolved behavior and the next concrete gate.
Do not run a full corpus audit repeatedly without a change or an unresolved question to justify it.
