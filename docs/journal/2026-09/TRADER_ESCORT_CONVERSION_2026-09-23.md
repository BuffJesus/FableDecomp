# Trader Escort conversion bootstrap

The verified Orchard checkpoint remains untouched: v12,
`adult_orchard_completed_2026-09-23`, stage 500. No game was launched and no
save files were modified during this conversion work.

Registered `trader_escort`: retail 0xE006D0 through 0xE0B020, bounded by
Q_TraderEscort's lifecycle vtable 0x12DF16C and Q_UndeadRising's Init.
Read-only Ghidra export expanded in three passes to 98 bodies. Native
factory/vtable inventory recovered 14 entities with no missing lifecycle
exports and no inventory errors. DIA exported 20,100 bytes of Trader Escort
PDB locals successfully. All quest helper names have matches; no PDB quest
members remain unused after the thread-name fix.

Two parent-quest threads registered by DarkwoodTrader were initially unnamed:
WatchForPickpocketing (0xE04F10) and TurnToBalv (0xE0A820). The instruction
inventory stopped at the nearest CCharString constructor, whose literal was
`ParentClass.`; retail concatenates this prefix with the earlier member name
through 0x99F570. The inventory now recognizes that exact concatenation shape.
Retail instruction stores, source string literals, and PDB members agree.
Previously both unnamed workers used the same null dictionary key, dropping
one body. Evidence generation now gives unknown workers distinct address-based
names and rejects conflicting names instead of silently overwriting bodies.

A separate cleanup-hoisting defect emitted an unconditional return immediately
before another branch's label, which is illegal Lua. The pass now wraps that
specific exit in a do block. An execution test verifies both cleanup and bypass
branches. Thirteen focused inventory/thread/cleanup tests pass.

Current generated draft: 15 owners, 65 functions, no missing bodies, 319 TODOs,
63/65 function syntax checks and 13/16 script file checks pass. The increase
from the initial 313 TODOs reflects recovery of the missing worker, not a
claim of improved behavior. Registration remains disabled. Readable output is
also generated; no generated Lua was hand-edited and v12 was not changed.

Draft smoke reports seven problems, including file-load errors and free
native globals. Remaining priorities: MakeTraderComment call/operand recovery
(including a bogus numeric GetDataString receiver), resource/vector operands
in DarkwoodTrader.Main, local-vector iteration in TraderComment, and lowering
the parent worker spawns with their captured CScriptThing arguments. Correct
names alone do not implement those spawns. Do not package or activate yet.

Validation: `trader_escort_conversion_evidence_2026-09-23.json` records hashes,
syntax results, focused tests, and compatibility comparisons; smoke details are
in `trader_escort_draft_smoke_2026-09-23.json`. Regenerated five established
units into work only: Orchard (11 Lua files), Wasp (10), Guardian Sister (4),
Trader Conflict (15), Guild (37). The first four match saved drafts exactly.
Guild has two pre-existing empty-guard removals; rerunning the affected scripts
with HEAD's cleanup implementation produces identical output to this pass.
Thus no new generated-output differences were attributed to these fixes.
Stored inventories for Guild, Orchard, Wasp and Guardian Sister also compare
unchanged. Trader Conflict differs only in its previously stale translation hash.

The broad suite was started, then cancelled in favor of focused tests and the
direct regeneration checks. No new full-suite result or equivalence to its
403-failure/45-error historical baseline is claimed.

Reproduction: `python -m tools.script_recovery.export_guild_training --unit
trader_escort`, inventory, quest_unit_evidence, ghidra_typing_spec,
infer_helper_prototypes, read-only ExportTypedTranslationUnit.java over the
registered range, convert_quest_unit, build_readable_unit, and smoke_run_unit.
PDB command uses `work/pdb_locals_20260913/pdb-locals.exe`, installed DIA SDK's
msdia140.dll, `debug_build/Ego_r.pdb`, and `*CQ_TraderEscort*`.


## Follow-up: dialogue prototype recovery

The signature parser lost `CCharString const` followed by a newline and `&`:
it trimmed spaces but left a newline in the base name, and the PDB's four-byte
reference was emitted as int. Whitespace normalization restores the pointer
type without changing parameter count or ABI width.

MakeTraderComment also incorrectly returned int. Its matched signature says
bool; all seven retail return tails independently set AL to zero or one and
pop 12 argument bytes. A conservative generic check now accepts bool only
when the matched qualified function name agrees and every exported return
tail explicitly produces a one-byte Boolean. It refuses mixed or full-EAX
return tails. Tests exercise wrapped types and rejection cases.

Regenerated typing, read-only Ghidra export, draft, readable, and draft smoke.
TODO count drops 319 -> 313 and smoke problems 7 -> 6; undefined high-byte
return globals disappear from the quest helper. File syntax counts remain
13/16 draft and 14/17 readable: invalid operand recovery still blocks play.
23 focused typing/parameter/inventory/thread/cleanup tests pass. No full
suite rerun, live launch, bundle change, or save modification.

Prototype-delta audit: one Trader Escort helper changes, two Orchard helpers
would gain corrected types on future typing regeneration; Wasp, Guardian
Sister, Trader Conflict and Guild prototypes are unchanged. Existing Orchard
typing/generated outputs and v12 were not regenerated. Native return-tail
proof, exact deltas, hashes, and reports are in
`trader_escort_typing_evidence_2026-09-23.json` and
`trader_escort_typing_smoke_2026-09-23.json`.

## Offline operand recovery (no playtesting)

User requested continuing without playtesting. No game was launched and no saves or live bundles were changed; verification
here uses Python and isolated Lua mocks only.

The native resource GetScriptThing call at 0x7E7490 was already classified by
address, but its lowering regex accepted only an optional CScriptThing* result
cast. DarkwoodTrader used void*, leaving both the hidden output and returned
alias unresolved. Accepting that pointer cast preserves the same proven hidden
result semantics. Regression cases cover typed/void pointers, the output alias,
and rejection of unknown targets and nonpointer casts. This removes 8 TODOs.

Complete native MoveToPosition calls carry position, radius, movement type,
and two flags (reviewed 0x7E72F0, RET 0x14; existing movement ABI audit). The
lifter's type-based argument pooling treated unknown vector pointers as scalar
operands, shifting the arguments and dropping a flag. For complete five-operand
entity calls in unit mode, preserve native order, remove the vector's native
address wrapper, and apply float/bool coercion by parameter position. An actual
Lua mock execution checks all seven observed values: vector xyz, radius, type,
and both flags. Partial calls still follow the existing diagnostic path.

Regenerated draft/readable and ran offline smoke: TODOs 313 -> 298; function
syntax 63/65 -> 64/65; draft file syntax 13/16 -> 14/16; readable syntax 14/17
-> 15/17; smoke problems 6 -> 5. DarkwoodTrader now compiles but still has
unrecovered globals; compilation is not behavior proof. MakeTraderComment
still contains an invalid numeric GetDataString receiver. Native evidence
shows real thing storage there, so substituting a dummy receiver would hide
the remaining operand/stack-alias bug. TraderComment still fails local-vector
iteration. Registration remains disabled.

96 focused lifter, parameter, typing, and resource tests pass. Established-unit
regeneration comparison is recorded in the operand evidence report.

Operand results: `trader_escort_operand_evidence_2026-09-23.json` and
`trader_escort_operand_smoke_2026-09-23.json`. Orchard/Wasp/Guardian/Trader
Conflict draft outputs match exactly. Guild differs only in the two previously
verified empty-guard removals; both files still match the prior control output.


## Bedtime checkpoint and final offline review

Implementation is paused for the next session. No game launch or playtest was
performed for this checkpoint. Current totals remain 298 TODOs and five smoke
problems; registration stays disabled. Resume priorities and regeneration
commands are in `docs/HANDOFF.md`; older handoff entries were preserved in
`docs/journal/HANDOFF_ARCHIVE.md`.

Final combined focused check: **137 tests passed** on 2026-09-23:

```powershell
python -m unittest tools.script_recovery.test_resource_movement_operands tools.script_recovery.test_resource_thing_result tools.script_recovery.test_lift_native_lua tools.script_recovery.test_native_function_parameters tools.script_recovery.test_ghidra_typing_signatures tools.script_recovery.test_native_cleanup_regions tools.script_recovery.test_quest_unit_thread_names tools.script_recovery.test_guild_training_inventory tools.script_recovery.test_checklists tools.script_recovery.test_autopilot_launch tools.script_recovery.test_autopilot_driver tools.script_recovery.test_global_definition_strings tools.script_recovery.test_wasp_combat_recovery
```

The harness's printed `guarded_menu: FAIL` is the expected simulated input
failure test; unittest completed successfully. Save staging tests use temporary
fixtures. The full suite was not rerun. Review covered lowering, typing, worker
identity, save/launch guards, sidecar patches, and generated/evidence scope.
Known conversion failures remain documented above; this is not a playable
Trader Escort release. Unrelated compile-gate outputs and scratch files are
excluded from the Lua commit.

Staged whitespace review found only generator-emitted blank EOF lines / empty
TODO argument suffixes in Trader Escort output, plus required blank context
lines in sidecar patch files. These were preserved to retain reproducibility
and patch applicability; handwritten source and documentation checks pass.
