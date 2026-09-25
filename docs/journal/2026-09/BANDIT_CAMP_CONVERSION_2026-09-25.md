# Bandit Camp conversion bootstrap ? 2026-09-25

Offline recovery; no Bandit Camp override has been deployed or activated.

The retail lifecycle anchors bound the family at 0x00D006D0 through
0x00D12D00 (the next Bounty Hunt Init). The three scripts are Q_BanditCamp,
Q_BanditCampBossBattle and Q_BanditCampHoldingScript. Read-only Ghidra
export converged after three passes: 131 function bodies, all entity
lifecycle exports present, and no inventory errors. The family contains
16 entity bindings and 12 registered workers. Evidence currently lives in
`work/bandit_camp_bootstrap`; export logs in `work/bandit_camp_export`.
The x86 DIA tool extracted 31,708 bytes of PDB locals. Typed export applied
1,355 overrides across the 131 functions.

## Worker-name scope

The evidence builder rejected two legitimate WatchForTermination workers:
0x00D01290 registered by Q_BanditCamp Main, and 0x00D04260 registered by
Q_BanditCampBossBattle Main. It had validated unique names across the whole
translation unit before determining which functions a quest reaches.
Name uniqueness is now checked within the reached quest closure. Conflicting
names for one address still fail globally; different bodies with the same
name still fail if both are reached by the same quest.

Five focused tests pass, including two sibling quests retaining their own
worker and a transitive registration collision still being rejected.
An in-memory before/after evidence build across all 17 existing units found
identical results wherever the old builder succeeded. Previously blocked
Guild Training (nine scripts), Wasp and Trader Conflict (two scripts) now
build successfully. Evidence: `work/thread_scope_ab.json`. No existing
unit evidence was rewritten by this comparison.

## Registered draft

Registered `bandit_camp` and promoted the recovery evidence to
`refs/script_recovery/bandit_camp`. The typed export explicitly includes
shared empty OnPersist at 0x00CBD4E0 (132 bodies including this anchor).
The first tracked draft has 19 owners, 85 converted functions, no missing
bodies, 81/85 function syntax passes, 15/19 file syntax passes and 180 TODOs.
All 16 entity bindings and 12 worker bodies are covered by the inventory;
10 inventory/thread-name tests pass.

Remaining syntax failures are CheckAnyBanditsKilled (nested sequence
expressions), BCGameMaster Main (a comma expression in a while condition),
AssassinMarker Main (switch cases cast to CCharString), and BanditKing Main
(unsigned literals). BanditKing also has unresolved x87 truncation operands
(`value`/`value_00`) and needs arithmetic/shift semantics reviewed, not just
syntax repair. Registration remains disabled in the generated package.
The existing live v16 bundle is unchanged.

Generated readable output retains the four syntax failures. Readable smoke:
19 files, 8 problems (four load errors, two free-global reports, plus
Gate1GuardOuter bitwise nil and BanditKingMissionProcess boolean arithmetic).
Evidence: `work/bandit_camp_readable_smoke.json`. This is a recovery baseline,
not a playable package.

## False exception-handler injection

CheckAnyBanditsKilled's typed decompile introduced a stack-space warning
that the untyped export did not have. At 0x004502EB the retail bytes are
`mov eax,ecx; mov ecx,[esp+8]; mov [eax],ecx; ret 8`, but the inherited name
was __EH_epilog3. Ghidra injected exception-stack teardown at its call.
A reviewed helper specification now explicitly clears that call fixup;
the exporter honors this only for helpers carrying `clearCallFixup: true`.

A full read-only typed re-export changed exactly one decompiled body,
CheckAnyBanditsKilled. Both the injected-epilogue and stack-space warnings
disappeared. Regeneration reduces this function's TODOs from 51 to 8, and
the unit total from 180 to 137. Syntax remains 81/85 functions and 15/19
files: its nested conditional stack assignment still needs lowering.
Evidence: `work/bandit_camp_fixup/export.log` and the before/after typed
exports. No other helper receives the opt-in flag.

## Boolean complement arithmetic

BanditKingMissionProcess used a native `1 - bool` expression for its level
wait. Lua rejects arithmetic on booleans. The lifter now converts a known
boolean to 0/1 before the subtraction, preserving a numeric result for
both comparisons and later arithmetic. The runtime regression covers both
boolean values and a numeric subtraction that must remain unchanged.
Focused tests: 98 passed, 35 subtests. The before/after comparison over all
18 units changes only BanditCampBossBattle.lua; all other units are identical.
Evidence: `work/boolean_complement_ab/summary.json` (baseline fab3fbf).

## Conditional stack snapshots

The remaining CheckAnyBanditsKilled syntax error came from a stack-slot
assignment nested in an AND condition. The slot previously held
HUD_QUEST_ICON_BANDIT, so inlining also substituted that string into a
numeric comparison. The sequence-condition pass now accepts direct named
stack assignments, retains the initial numeric value on skipped branches,
and copies known numeric locals through stale casts. The runtime regression
checks skipped and taken branches, both comparison outcomes, and source
mutation after the copy. Pointer-write sequences remain rejected.

After regeneration, syntax is 82/85 functions and 16/19 files, with 137
TODOs. This fixes condition evaluation and slot lifetime; the worker's
killed-thing list and counter argument recovery remain unresolved.

The final 18-unit comparison changes BanditCamp.lua, BS_Teacher.lua and
ChickenMaster.lua only. The latter two have the same conditional stack-write
pattern; their committed sources matched the comparison baseline before
regeneration. Their existing unresolved operations and compile counts are
unchanged. Focused runtime suites: 100 passed, 35 subtests; related Guild,
branch-join and gift regression suites: 28 passed, 32 subtests (Unicorn printed
its existing Windows exception diagnostics but the tests exited successfully).
Evidence: `work/stack_sequence_v2_ab/summary.json`. The earlier incomplete
`work/stack_sequence_ab` run was superseded and stopped.

Readable Bandit Camp smoke is now 19 files / 6 problems: three remaining
load errors, two free-global reports and Gate1GuardOuter's nil bitwise
operand. Evidence: `work/bandit_camp_stack_sequence_smoke.json`.
