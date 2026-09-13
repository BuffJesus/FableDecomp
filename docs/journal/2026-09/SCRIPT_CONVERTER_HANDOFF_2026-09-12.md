# Script converter checkpoint — 2026-09-12

## End-of-night resume point

**User requested that work stop for the night. Implementation is stopped; resume only on request.**
The overall objective is unfinished, not achieved or technically blocked. No converter test or
generation process remains pending. The last turn only inspected the proposed resource-lowering
entry points before the stop request; **no experimental Lua resource candidate was created**.

Authoritative checkpoint: `refs/script_recovery/lifted/NewOakValeIntro/CONVERSION_REPORT.json`
reports 47/51 functions and 14/18 files compiling, zero missing bodies, 1,296 TODOs.
`FSE/quests.lua` still contains `Quests = {}`. The remaining syntax failures are AffairWife,
AffairWoman, BarrelMan and Bully. Compiling husband Main is still behaviorally incomplete.

Latest full suite: 870 tests in181.248s, only known Bully2failures/2errors; see
`work/converter_resource_action_ir_tests.log`. It started before the final temporary-Thing
additions; the latest resource/lifetime focused suite passed10 tests in10.230s.
Do not describe the full suite as green or the mocked speech tests as live runtime parity.

Next concrete work: generate a separate disabled resource-aware husband candidate from the
existing generated Lua plus checked native evidence. Connect the single resource local's
constructor, all three preparations, six acquisition sites, action/task calls, ten temporary
Things and destructor together. Do not mix cached `me:AcquireControl` with explicit resource
speech. This candidate still needs cached woman/wife and hit-wrapper lifetimes, movie cleanup,
the dynamic animation global and the resource distance-query API gap resolved.

Read [the resume checklist](../../scripts/SCRIPT_CONVERTER_RESUME_CHECKLIST.md).
Detailed chronological evidence follows; this section supersedes older status counts.

The user has a separate session owning New Oakvale runtime fixes and subsequent reviewed retail
ports. Its prompt is `NEW_OAKVALE_PARALLEL_SESSION_PROMPT_2026-09-12.md` in this directory. Avoid
runtime/game-install changes and changes to reconstructed packages or shared API manifests while
that session is active. Preserve the large existing uncommitted work in both repositories.

## Current converter status

**Active objective:** continue until the entire retail NewOakValeIntro quest/entities/callbacks/helpers
can be converted into usable Lua. The goal is active and NOT complete. Do not stop at compiling
stubs or copying the hand port. Native evidence remains authoritative; parallel Bully/runtime files
stay untouched. Current full-scope work supersedes the old four-entity benchmark as the main target.

`python tools/script_recovery/convert_new_oakvale.py` generates the whole inventoried package under
`refs/script_recovery/lifted/NewOakValeIntro/`, with per-function coverage/syntax/TODO reports.
17 owners, 51 inventoried functions, zero missing source bodies. Initial baseline was 23 compiling
functions / 2 compiling files / 2085 TODOs; current baseline is 32 / 3 / 2015. These are drafts.
RegisterMain/destructor/helper entries are inventoried too; aliases are reported rather than duplicated.
The root goal is nowhere near complete based on these counts.

Latest work: `native_self_wrapper.py` recovers the exact embedded CScriptThing by-value copy passed
to SetIsPushableByHero (vptr 0x01238c8c, entity +0xc data/+0x10 info, refcount increment). It rejects
different layouts/increments, later uses, and matches in comments/strings. Entity param_1+8 receiver
aliases now resolve to me too. Legacy host signatures for EntitySetAsKillable and SetThingHasInformation
are corrected in memory to retain the current runtime's optional flags, like RemoveThing.
Full-package entity fields use a private per-module state table via Lifter.state_receiver; shared
parent fields retain quest access. The legacy single-entity mode keeps its previous storage behavior.

Latest validation: 443/443 shared recovery tests. `test_new_oakvale_conversion.py` verifies all inventory
entries appear, source coverage is complete, draft failures stay visible, registration is disabled,
entity-local state isolation, wrapper-copy rejection cases, optional flags, and actual generated
Bully Init host-effect parity with the unchanged working port. Bully Main is still unsupported.

Immediate next work: helper signature/parameter plumbing (GetVillagerSpeechIndex has a scalar param_1;
do not mistake it for an entity receiver), comma assignments in while predicates, native subregister/
aggregate expressions, resource lifetimes, and structured control flow. The full runner currently
emits generic quest[,me] signatures, so compilation is not proof that helper operands are wired.
Current errors are in CONVERSION_REPORT.json per function. New converter goal must continue through
these issues rather than reverting to small isolated benchmark-only marathons.

- `tools/script_recovery/lift_native_lua.py` remains the entry point.
- New support modules: `native_literal_loops.py` and `native_arguments.py`, each with execution and
  negative-case tests. Beardy Baldy Main and WatchForQuestCardConditions each recover one exact
  literal byte-comparison loop; `LIFT_REPORT.json` records `literalByteComparisons` per function.
- Call operands now preserve nested/quoted commas; template pointer casts no longer leak into
  expressions. Nonterminal returns use `do return end` while native labels/gotos remain visible.
- Earlier work today added Lua 5.4 syntax scoring, parent-state/constant joins, mutable-scalar
  storage, exact-target AL termination results, coherent unnamed scalar field access, and typed ECX
  receiver normalization. Actual Barrel Main has offline execution coverage for instruction and
  termination paths; that is not coverage of the omitted entity callbacks.
- Latest pass adds `native_thing_predicates.py`, its reviewed witness JSON, and tests. Guardian's
  WaitForPieceOver now binds the exact native lookup result to its inlined IsAlive predicate.
  Caller/callee bytes, vtable targets, and raw/annotated export digests must match. This is one
  reviewed case, not general native stack analysis. Application/rejection evidence is recorded
  as `thingPredicateEvidence` in the thread report.
- GSI assignment conditions now also lower in `while`, preserving repeated left evaluation,
  short-circuit calls, final assigned values, and explicit break scope.
- Beardy Main now has a separate verified negative-OR witness for its nearest barber, with the
  native conditional result assignment preserved. Lifecycle functions use the verification path.
  Conditional assignments support direct CScriptThing methods; GSI calls accept whitespace before
  their operands. See the latest LIFTER.md section for native addresses and limits.
- New Oakvale comparison now lives in `test_novi_converter_parity.py`: complete generated
  CreatedBeetle Main vs the actual working port and common modules, under a mocked Lua 5.4 host.
  It covers lifetime expiry, early cancellation, and timer cleanup. It caught dropped RemoveThing
  flags: native/port true,true had become runtime defaults false,true. `converter_signatures`
  corrects only the exact legacy one-argument manifest entry in memory. Shared SDK files stay intact.
  Optional bool operands now receive proper literal/numeric-result conversion too.
- Aeon's Fisherman and MeetSister samples now have Main execution comparisons in
  `test_aeon_converter_parity.py`. Fisherman's nil deactivation delay is fixed by a separately
  byte/source-verified operand witness (`native_call_operands.py` and its JSON registry). The zero
  belongs to DeactivateQuestLater, not the nested GetActiveQuestName call. Main reports operandEvidence.
- Last validation: **435/435 full shared recovery tests**. The actual generated Guardian thread
  executes under Lua 5.4 across both waits, absent/dead Maze, and termination in either wait.
  Benchmark: **9/18 recovered Lua files compile**, excluding stubs/loaders. Nine failures remain.
  Recall is unchanged; Beardy has one additional recognized GetActiveQuestName call (precision
  0.841 -> 0.836). Ghost Fisherman's parser TODO count drops 110 -> 104; its operands still need review.
  Latest Aeon comparison pass leaves those call scores unchanged; Fisherman's TODO count drops 4 -> 3.
- Canonical SDK passed in the preceding checkpoint. No SDK/manifest files changed in this pass;
  the already-recorded external overlay mirror drift is independent.

## Exact continuation point

Read `refs/script_recovery/lifted/LIFT_BENCHMARK.md` / `.json` for current syntax locations.
The New Oakvale port is now useful as a behavioral comparison. Extend the new parity test module
to another supported complete sequence, keeping runtime workarounds distinct from retail evidence.
CreatedBeetle parity is tested against actual modules, not a reimplementation of the port. The
RemoveThing signature correction is specific to the currently verified host's optional flags;
do not infer broader host signatures from native methods alone.
Aeon Main comparisons currently cover Fisherman and MeetSister. They ignore logs and tolerate
extra pure reads through a frame-driven mock world; arguments, frame waits, and transition effects
must agree. Init/OnPersist/entities are not covered. FishermanTasks is Int in Aeon's sample but Bool
in the native persistence recovery; investigate native field evidence before making either an oracle.
Beardy's negative OR condition is recovered with its own verified lookup/receiver witness. Its first
syntax failure is now the refcount comma expression in WatchForNewHairdo, generated line 258:
`(ppVar4 == nil) or (*ppVar4 = *ppVar4 + -1, *ppVar4 ~= 0)`. Establish native ownership/cleanup
semantics before dropping or lowering this expression; it guards a goto and is not an ordinary
scalar assignment. Full Beardy execution remains unsupported. Guardian's quest compiles; GTDI Maze
does not. Ghost Fisherman also needs explicit string/reference and hidden-return operand association:
newly recognized spaced calls expose incorrect question and thing-name/marker argument ordering.

Verified native evidence: CScriptThing's vptr is at +0, implementation +4, info +8; copy constructor
0x004ABE90 and IsAlive 0x004AB130 confirm this layout. IsAlive checks the implementation for null then
dispatches slot 0x12c. GetThingWithScriptName at 0x008A7D60 returns a 12-byte wrapper through a hidden
first stack argument and uses ret 8. The Guardian witness records exact stack offsets and byte/source
digests. New cases need their own receiver association. See the last section of docs/scripts/LIFTER.md.

Other remaining families: nested parent/master-data/vector accesses, string-wrapper operations,
native aggregate/subregister expressions (e.g. SUB41), and comma assignments. The literal loop
recovery deliberately does not emulate arbitrary memory, and template-cast removal does not solve
the ownership/operand association behind the casts.

## Commands and artifacts

```powershell
python -m unittest tools.script_recovery.test_lift_native_lua tools.script_recovery.test_benchmark_lifter tools.script_recovery.test_native_arguments tools.script_recovery.test_native_literal_loops tools.script_recovery.test_native_thing_predicates
python -m unittest discover -s tools/script_recovery -t . -p "test_*.py"
python -m unittest tools.script_recovery.test_novi_converter_parity
python -m unittest tools.script_recovery.test_aeon_converter_parity
python tools/script_recovery/benchmark_lifter.py
```

`benchmark_lifter.py --require-syntax` intentionally returns 1 while those nine recovered files fail.
Compilation is checked with Lua 5.4 without running the source. Generated `quests.lua` registration
remains disabled. Do not enable draft output based on syntax or call-multiset scores.

Latest baseline: `work/lifter_aeon_parity_20260912_baseline/`. Prior artifacts:
`work/lifter_novi_parity_20260912_baseline/`,
`work/lifter_beardy_predicate_20260912_baseline/`,
`work/lifter_thing_predicates_20260912_baseline/`,
`work/lifter_literal_compare_20260912/` and `work/lifter_arguments_20260912/`; drafts/reports are under
`refs/script_recovery/lifted/`. The prior marathon snapshots remain under
`work/lifter_marathon_20260912_*`. Design/evidence notes: `docs/scripts/LIFTER.md`.


### Helper values, retail RNG, and current termination queries

The complete generator now parses inventoried member signatures, removes the native receiver,
and gives explicit scalar parameters distinct Lua names. This prevents helper param_1+8 from being
mistaken for the entity wrapper. Local helper calls preserve assigned return values and check arity.
Native value returns are emitted, and function-scoped locals first assigned inside a Lua block are
hoisted so a loop result remains available after break. Formal parameters are never shadowed.
Comparison-to-integer masks of the form ~-(uint)(a != b) & value return value or zero in Lua.

`native_random.py` folds an adjacent rand-result/modulo pair only when the raw temporary is dead
and exported rand calls target 0x00BFEB16 with matching x86 relative-call bytes. The runtime's
RetailRandModulo calls this same CRT thunk, preserving the game RNG stream. The supported domain
is positive moduli (including the villager's native vector counts). Comments, live temporaries,
unknown targets, mismatching bytes, and extra unmatched calls are rejected. Converter-only host
signatures include this binding; the shared SDK manifest remains untouched.

The full generator enables live termination queries. The current LuaManager.cpp entity-VM binding
captures pEntityHost and calls 0x00F35B30; quest VMs use the quest predicate. Thus reviewed native
termination calls refresh alive at the original check, including before the first frame. The legacy
benchmark retains its older frame-status convention. Unresolved native calls/control flow elsewhere
are still unresolved; this change does not establish whole-quest cancellation correctness.

`test_native_random.py` executes the actual generated GetVillagerSpeechIndex under Lua 5.4 and
compares its draw/frame/write effects to the unchanged working-port helper. It covers immediate
selection, repeat retries, and retry cancellation. Additional native tests check termination before
drawing and before/after the final state write. Retail returns zero on cancellation while the port
returns nil; this intentional difference is asserted explicitly. The generated helper has zero TODOs.

Validation: 452/452 full recovery tests pass. Full conversion remains 51 functions across 17 owners,
zero missing sources, 32/51 function syntax passes and 3/17 file passes, with 2019 TODO diagnostics.
The higher TODO count includes now-visible helper arity mismatches; it is not a completion score.
Legacy benchmark remains 9/18 compiling files. Generated registration stays disabled.

Next work: cross-owner parent helper calls (Bully GivenTeddy calls AddBadDeed at 0xDAEA70;
TeddyGirl GivenTeddy calls AddGoodDeed at 0xDB0660) need a valid shared Lua module/call boundary,
not merely a renamed global across separate entity VMs. TeddyGirl also needs its master-data B
string write. Villager Main still has unresolved vector-count/index/string-resource operations.
All runtime, reconstructed port, installed-game, inventory and shared manifest files were read only
throughout this pass.


### Shared native quest helpers and deed operands

The generator now resolves entity-to-parent helper calls from exact exported call targets, requiring
an explicit parent pointer at this/param_1+0x14 and the reviewed explicit parameter count. It emits
calls through NewOakValeIntro.native_quest_helpers, not cross-VM globals. The generated shared module
contains AddBadDeed and AddGoodDeed, with local forward declarations and a returned table. Its bodies
are lifted again from native source with quest field layout but entity execution context; frame waits
receive me. Root quest copies retain quest execution context. Transitive helper dependencies are
included. Conflicting short call labels are rejected when building the parent-helper map.

The shared artifact has separate per-helper TODO diagnostics and is included in complete-file syntax
checks. The package now has 18 Lua implementation files for the same 17 owners / 51 native functions.
Bully GivenTeddy has no direct TODOs and tests verify gold, teddy removal, both parent flags, and the
shared AddBadDeed call with deed ID 3 in isolated entity VMs. The test substitutes the deed body to
verify the call boundary; it does NOT claim the still-incomplete actual deed body is usable. Loading
the real generated shared module is tested without executing those unresolved bodies.

Argument placement now permits equal values from distinct consumed native temporary slots. This
restores both empty region strings on AddGoodDeed's objective update. Exact-target alias 0x00CBE9EE
recovers AddLogbookTutorialEntry from the misleading CSubtitleRenderer::SetText label. Its one ECX
string operand is resolved before the noise filter (the spurious decompiler argument list contains
unaff_EDI). Tests cover the real string argument, unresolved slots, and an unknown target. Existing
native logbook audit and current runtime verify the tutorial helper's category/title/yield behavior.

Validation: 456/456 shared recovery tests pass. Full generator: 32/51 functions compile, 4/18 files
compile, zero missing native sources, 1994 TODOs in the 51 primary bodies. Shared-helper diagnostics
remain visible separately. Legacy benchmark stays 9/18 files compiling, with lower missing-argument
counts in several scripts after retaining repeated temporary values. All drafts remain disabled.

Exact next recovery target: AddGoodDeed now has only the native morality-load statement and missing
GiveHeroMorality argument as TODOs. AddBadDeed additionally needs its indexed WhichBadDeedsPerformed
array access and phantom register string-wrapper alias. Native amount comes from SCRIPT_DEF
OVI_MoralityChangePerDeed (0.001 in pinned installed script.bin), proven by
`audit_new_oakvale_morality_amount.py`; do not copy a port constant without preserving that data evidence.
The bad-deed decompile misidentifies the indexed enum as unaff_retaddr; inspect instruction evidence
before substituting native_arg_param_1. Working-port deeds.lua is available for execution comparisons.
Runtime and working-port files were read only during this pass.


### Deed bodies recovered and executed against the working port

`native_deeds.py` and `native_deed_witnesses.json` now recover the morality operand for both native
deed helpers. Recovery checks complete native function bytes and both raw/annotated source hashes,
then decodes SCRIPT_DEF entry 597 from the hash-pinned retail script.bin. Its float is
0.0010000000474974513; AddGoodDeed passes it unchanged and AddBadDeed negates it. Native instructions
DB0670..DB067F load global+0xd64 and push its bits into interface slot 0x270. DAEA7A..DAEA8E uses
fld/fchs/fstp before the same slot. Changed source, bytes, or unavailable/changed data reject recovery.
No working-port source or constants feed the generated output.

Bad-deed indexing is also instruction-proven: after push esi/push edi, DAEA98 reads the explicit
stack parameter from [esp+0xc] into EDI. DAEB4B, DAEB67, and DAEBE2 write/read [ESI+EDI+0xfc]. The
pinned recovery replaces Ghidra's erroneous unaff_retaddr index with the actual renamed enum argument
and removes two phantom aliases used only by incorrect string cleanup/logbook signatures. Typed
byte-array loads/stores now lower through quest state, using bool-array metadata from persistence
inventory and the established WhichBadDeedsPerformed_0..4 keys.

Signed 32-bit host int/long parameters now emit -1 for 0xffffffff, preserving the native bit pattern
without passing an out-of-range positive Lua integer. Unsigned slots retain the positive value.
This fixes the deed counter sentinel and the same native animation loop-count idiom.

Both generated AddGoodDeed and AddBadDeed have zero TODOs. `test_native_deeds.py` executes the actual
shared generated module and unchanged reconstructed deeds.lua/common.lua/fields.lua under Lua 5.4.
It compares state writes, host calls, frame waits, tutorial/objective updates, and counter values
across five good-deed scenarios and twenty bad-deed scenarios (all five categories; first, repeat,
new category, and wait cancellation). Morality comparison applies the host's real float32 conversion;
native/port Lua return conventions are not equated. Initial or between-call termination is not
claimed as parity with the port merely because tested wait cancellation agrees.

Validation: 459/459 recovery tests pass. Full conversion remains 32/51 functions and 4/18 files
compiling, zero missing native bodies, 1986 TODOs. The two shared deed bodies also have zero TODOs.
Legacy benchmark remains 9/18 files compiling. Draft registration is disabled. Runtime, installed
application and working-port files remain untouched.

Next: exercise the complete generated Bully GivenTeddy callback through these actual deed bodies,
then recover TeddyGirl's master-state string write. Broader remaining families include native
control-flow joins, aggregate resource wrappers, and the villager/quest speech vectors. Current
source/data pinning deliberately rejects altered retail exports/data instead of guessing operands.


### Teddy callbacks through actual generated deed dependencies

The full generator now loads unambiguous string master-field write mappings from each entity's
native inventory. Exact CCharString assignment through the entity's master pointer at +0x18 maps
to SetMasterGameState; unknown offsets and non-entity/parent-pointer variants stay unresolved.
TeddyGirl GivenTeddy's MASTER+0x54 = B now emits TeddySolution = B, matching the current host's
literal-string assignment binding. Its Main's corresponding C write is also recovered.

An immediately overwritten `local_4 = this` staging assignment is removed only when the very next
statement constructs a literal CCharString into that exact slot. Different slots or an intervening
use retain diagnostics. This recovers the last phantom assignment in TeddyGirl GivenTeddy.

`test_native_teddy_callbacks.py` executes both generated GivenTeddy callbacks through the actual
generated shared deed module, comparing unchanged working-port callbacks and their actual
common/fields/deeds dependencies in isolated Lua 5.4 VMs. Four scenarios per callback cover immediate
completion, two-frame wait, repeat deed, and cancellation during the deed wait. Effects include gold,
item removal, parent flags, morality, logbook, objective/counter, information clearing and master-state
write. The private FoundTeddy state is explicitly checked. Native and port callbacks both continue
their authored post-deed operations after a cancelled deed wait; the converter does not invent an
extra cancellation guard. These tests cover GivenTeddy, not Bully Main's visual skip bug.

Validation: 462/462 recovery tests pass. Full generator remains 32/51 function syntax, 4/18 file
syntax, zero missing native sources, now 1983 TODOs. Both GivenTeddy callbacks and both deed helpers
have zero direct TODOs and execution comparisons. Legacy benchmark remains 9/18 compiling files.
Generated registration remains disabled; runtime, port and installed game files were read only.

Next priority: broad native control-flow/resource expressions in entity Main. Current first failures
include comma-assignment while predicates (AffairWoman, Theresa, Victim, WatchBarrels), native wrapper
pointer tests, and subregister/aggregate values (LiveFather/TeddyGirl). Guard Init is also still invalid.
WatchForGotGold's short body compiles but still has argument staging issues around GetActiveQuestName
and SetQuestCardObjective; use its native source at 0xDBE2E0 and working thread as comparison.


### Comma predicates and counted-loop structure

The lifter now lowers while predicates containing top-level scalar assignments separated by the C
comma operator. Balanced operand splitting preserves nested call commas. Every assignment executes
inside the loop before the final predicate, including the exit check. Prefix targets are mutable
function-scoped locals rather than folded compile-time temporaries. Pointer writes and unsupported
prefix expressions are not discarded or treated as supported scalar assignments.

Canonical for loops with optional scalar assignment initialization and scalar assignment update
now lower to Lua while loops with the update at the corresponding block tail. Pending updates are
tracked by block depth; nested loops retain separate updates and break skips its loop's update.
This fixes WatchBarrels' previous structural failure: unsupported for openings had been left as
comments while their closing braces emitted real Lua end statements. Its pointer-wrapper destructor
calls remain explicit TODOs; balanced blocks do not imply native cleanup has been recovered.

`test_native_comma_conditions.py` executes final false checks, multiple ordered assignments,
call-result predicates, counted-loop final values, breaks, and nested loops under Lua 5.4.
Validation: 468/468 full recovery tests pass. Full generator now compiles 34/51 functions and 5/18
files (Victim Main and WatchBarrels newly compile), zero missing native bodies, 1981 TODOs. Neither
new syntax pass is a complete behavioral parity claim. Legacy benchmark remains 9/18 files compiling.
Draft registration remains disabled, and runtime/port/game files remain untouched.

Next blocking families remain resource wrapper/aggregate expressions, pointer and vector accesses,
and control-flow joins represented as gotos. WatchBarrels still treats GetAllThingsWithScriptName's
aggregate result as a scalar count, has native collection pointer arithmetic, unresolved destruction,
and a branch-to-next-frame goto incorrectly approximated as return (already marked TODO). These must
be recovered before its new syntax pass is usable. Read generated functions using anchored
`^function Name(` matches: unanchored searches can match an earlier CreateThread comment naming the
function and accidentally inspect a different body.


### Scope-checked native jumps

`native_goto_scopes.py` inventories native block ancestry and accepts only jumps to a label in the
same or an enclosing lexical scope. Entering a nested/sibling block remains unresolved. Identical
jump text occurring in multiple scopes is accepted only when every occurrence is safe, because the
current emitter keys on normalized statement text. Duplicate labels or unbalanced block structure
reject the analysis. The full generator enables this mode; legacy benchmark behavior is unchanged.

Accepted jumps emit actual Lua goto/labels instead of early-return approximations. Function locals
are hoisted, and assigned native scalars are kept mutable so forward jumps cannot enter Lua local
scope or incorrectly substitute a skipped assignment. Existing return legalization keeps labels
after native returns parseable. Per-function nativeJumps/nativeLabels expose each recovered edge.

Execution tests cover forward joins skipping assignments, backward jumps, and branch-to-loop-tail
jumps which preserve later iterations. Negative tests reject entering nested scopes and ambiguous
repeated text. An actual WatchBarrels assertion confirms its iVar4 <= count-4 branch goes to
LAB_00dbeabd (the frame wait) instead of returning from the thread. Its sibling-scope cleanup label
LAB_00dbeb07 is explicitly not certified. Resource/count/destructor defects remain in that function.

Validation: 473/473 shared recovery tests pass. Full conversion emits 285 distinct jump statements
and 111 target labels. It remains 34/51 functions and 5/18 files compiling, zero missing bodies,
1632 TODOs. This removes many previously explicit control-flow approximations without asserting
whole-function parity for the still-incomplete entity Main bodies. Generated registration stays
disabled; runtime, working-port and installed game files remain untouched.

Next: native collection and CScriptThing/resource wrapper expressions remain the major syntax and
behavior blockers. Cross-scope C jumps will require restructuring or explicit control-flow lowering;
do not force them into Lua goto or claim cleanup-only based on pointer-looking code alone.


### Guard Init: entity aliases, repeated copies, and hidden vector result

Staging a known embedded-entity alias now retains the actual Lua me value instead of an undefined
native variable. This fixes Guard Init's first three calls, whose operands were pushed via pCVar1.
The exact by-value self-wrapper rule now covers reviewed wander centre/distance and scripting-state
methods in addition to pushability. Repeated passes remove a later dead copy before checking earlier
copies that reuse its locals; a remaining real use still blocks removal. Reviewed decorated signatures
prove float operands for SetWanderMinDistance/MaxDistance (despite Ghidra's long guess), so native
argument words 0 and 0x40c00000 become 0.0 and 6.0. Layout/refcount/unknown-signature guards remain.

By-value C3DVector results from reviewed CScriptThing signatures now retain their hidden return slot
and a Lua result local. SetWanderCentrePoint's pos parameter has converter-only vector metadata,
so it can consume the GetHomePos result and cannot use stale scalar staging as a position. Guard
Init now calls SetWanderCentrePoint(me, homePosition) instead of discarding GetHomePos's result.
This is not general native vector-memory or collection support.

Guard Init has zero TODOs and executes against the unchanged working port in Lua 5.4. Tests compare
all init host effects and a nontrivial position vector. EntitySetAsKillable's omitted third argument
is normalized through the current host's false default in this comparison. Additional tests cover
reused copy locals, live-tail rejection, staged me, and rejecting scalar values for vector parameters.
Native ABI sources: engine_api.tsv at 0x008A23B0, 0x008A2690, 0x008A2950, 0x008A36D0;
CScriptThing GetHomePos vtable slot 0x1c has decorated C3DVector-by-value return. Current runtime
SetWanderCentrePoint/MinDistance/MaxDistance uses retained by-value copies and vector/float operands.

Validation: 477/477 full recovery tests pass. Full converter is now 35/51 functions and 5/18 files
compiling, zero missing bodies, 1624 TODOs. Legacy benchmark remains 9/18 compiling files; Ghost
Fisherman loses one missing-argument TODO. Runtime, port, shared manifests, and installed game remain
untouched. Draft registration remains disabled. Continue with the unresolved NPC Main resource and
collection expressions; Guard Main is still incomplete despite Init now matching the port.


### Conditional declarations, controlled calls, and literal byte slices

Fixed an interaction between hoisted variables and conditional call lowering: an already-hoisted
new variable must not emit a bare Lua expression statement. If no prior value requires assignment,
its function-level declaration is sufficient; it is not reset to nil at each loop/condition check.
This repairs BarrelThug Main syntax without changing the short-circuit call behavior.

Conditional CScriptThing calls now go through the same statement dispatch as standalone calls.
Reviewed CScriptGameResourceObjectScriptedThingBase methods therefore bind to me rather than being
called on a raw stack address. The test executes IsPerformingScriptTask only when the left condition
requires it, verifying both zero calls on the skipped path and the returned boolean on the live path.

`native_subregisters.py` evaluates Ghidra ._byteOffset_byteCount_ expressions only for complete known
integer-literal inputs, after temporary substitution. Zero, nonzero byte positions, and negative
integer bit patterns are tested. Unknown values, floating inputs, invalid widths and quoted strings
remain untouched. This removes malformed constant slices in LiveFather/TeddyGirl but does not recover
their remaining aggregate resource state or justify folding unknown subregisters.

Validation: 481/481 full recovery tests pass. Full conversion is 36/51 function syntax, 6/18 file
syntax, zero missing sources and 1622 TODOs. BarrelThug Main now compiles but remains behaviorally
incomplete. Legacy benchmark remains 9/18 compiling files. No runtime, working-port, shared manifest
or installed-game changes; generated registration remains disabled.


### Parent byte arrays and decisive constant conditions

Entity parent-state loading now expands explicitly declared bool arrays when the owner's reviewed
persistence layout proves their exact byte extent. Guard's WhichBadDeedsPerformed[0..4] declaration
maps parent+0xfc..0x100 to the same WhichBadDeedsPerformed_0..4 keys used by generated AddBadDeed.
Ownership, declaration, element type and inclusive extent must agree; conflicting offsets reject.
Tests execute all five reads before/after shared-state changes and reject mismatched owners/ranges.

`native_constant_conditions.py` folds only a decisive literal equality/inequality on the left of
and/or, with one balanced parenthesized RHS. For example, a known-null equality in an OR makes the
reference-count decrement RHS unreachable. Unknown pointers, nondecisive tests, mixed nil/numeric
comparisons and outer operator combinations are left unchanged. Tests verify short-circuit effects
and folding after native literal staging. This is not a live reference-count/cleanup implementation.

Validation: 486/486 full recovery tests pass. Full converter remains 36/51 functions and 6/18 files
compiling, zero missing native bodies, 1612 TODOs. The existing scalar-parent test was updated to
include its now-supported, explicitly declared bool-array fixture. Legacy benchmark remains 9/18.
Runtime, working port, shared manifests and installed game remain untouched; drafts stay disabled.

Guard Main now reaches a live piStack_13c decrement/cleanup condition before its first syntax failure
(around generated function-relative line 622). That case needs native ownership/resource evidence;
do not remove it using the constant-null rule. Other remaining families are native collection counts,
resource wrappers, aggregate subregisters, and cross-scope control-flow joins.


### Native speech vectors and collection return contract

`native_speech_vectors.py` recovers eight speech tables / 42 ordered keys from the native Init
construction export, checking its full source hash, 3306 native bytes, and agreement with reviewed
inventory offsets/keys. No port source supplies generated literals. The full Villager module emits
these immutable tables and records staticVectors evidence in its report. Known parent vector
(end-begin)>>2 expressions become exact lengths; vector-base reads become local table references;
CCharString assignment from base+index*4 becomes a 1-based Lua lookup whose result is tracked as a
string operand. This recovers all eight native selector call/count/base pairs and their shared read.
It does not claim the entire native Init body or all Villager Main logic has been converted.

`test_native_speech_vectors.py` executes every index through the actual eight native selector-call
pairs plus indexed string-copy expression, verifies all 42 selected lines, and compares table order
to the unchanged working-port speech module. Changed native bytes reject table recovery. Full source
hashes likewise reject changed construction evidence. Full recovery suite: 488/488 tests pass.
Conversion remains 36/51 functions and 6/18 files compiling, zero missing sources, 1587 TODOs.
Draft registration remains disabled. Runtime, working port, installed game and shared manifests
were read only during this pass.

Collection contract finding for the next WatchBarrels work: native GetAllThingsWithScriptName at
0x008A8570 returns a signed count and fills vector<CScriptThing>&. The host's LuaQuestState.cpp:836
returns only a dense Lua table, and skips failed heap allocations or rejected WrapScriptThingOutput
results. Its length is therefore not an unconditional proof of the raw native count. The existing
working port uses table length; record the compatibility limitation or expose raw count before
claiming exact equivalence for failure/filtered-wrapper cases. Current generated WatchBarrels still
incorrectly treats a Lua table as the native scalar result and also has native vector pointer/count
and cleanup operations. This finding is not an external blocker while other converter work remains.


### Villager conditional RNG, parent aliases, and Theresa wait conditions

The Villager Main conditional rand/modulo check is now recovered with exact raw-source,
annotated-source and native-body SHA256 witnesses. Native 0xDAE635 calls the retail rand thunk;
0xDAE63A..0xDAE642 computes/tests signed remainder 100. Both outgoing paths overwrite the raw
random result before reuse. The converter preserves the short-circuit draw inside the condition,
using RetailRandModulo(100). Tests execute draw/no-draw paths and reject changed source or bytes.

Entity parent-pointer aliases now resolve reviewed parent scalar fields and speech-vector bases.
Branch entry snapshots restore aliases for sibling alternatives; invalidations merge conservatively
at joins, including nested branches. Conditional aliases do not escape their defining scope, and
literal, unresolved-call, address and skipped-noise assignments invalidate stale aliases. This is
structured-flow tracking, not a proof for arbitrary native gotos or complete Main behavior.

native_conditions.py replaces the parenthesis-sensitive conditional-call regex with balanced
recognition of one outer short-circuit operator and a two-part call-assignment comma expression.
It preserves nested/quoted call arguments and rejects ambiguous multiple outer operators. The
actual Theresa Main animation-wait header now uses the existing controlled-resource dispatch.
Lua execution tests cover initially near, animation completion, repeated frame checks and arrival
after a frame. Theresa's next syntax failure is function-relative line 108 (native :: expression);
the whole Main is not yet usable. Native task-query call target is 0x007E7450, first at 0x00DB9954.

Validation: 507/507 full recovery tests pass; legacy benchmark remains 9/18 compiling files.
Regenerated full intro: 17 owners, 51 functions, zero missing bodies, 38/51 functions and 8/18
files compiling, 1583 TODOs. Compilation still does not establish behavior. Live resource cleanup,
collection count/vector semantics, string comparisons and unsupported control flow remain work.
Runtime, working port and installed game were not edited; generated registration remains disabled.


### Embedded termination queries in native conditions

native_termination_expressions.py recovers a reviewed member termination call immediately followed
by its extraout_AL zero/nonzero test inside a comma expression. Only this/param_1 receivers and
known termination names are accepted; quoted/commented text, unknown callees, other receivers and
intervening effects remain unchanged. Live mode emits a small Lua expression which updates alive
and returns the tested result, retaining the original short-circuit position and saved result.
Legacy mode uses the saved frame status as before.

The actual Theresa Main OR condition is covered by Lua execution tests: an already-terminated
left side skips the second query; a live left side queries once; the saved status reflects its
result. All matching native metadata calls resolve to 0x00F35B30. known_callee_aliases now rejects
labels associated with any unknown, missing, invalid or differently reviewed target, independent
of call order. This prevents trusting a similarity label merely because one occurrence is known.

Validation: 512/512 full recovery tests pass. Full conversion remains 38/51 functions and 8/18
files compiling, zero missing native bodies, 1583 TODOs; legacy benchmark remains 9/18. Theresa's
first syntax failure moves from function-relative line 108 to line 212, an inline literal byte
comparison. The existing literal-loop rule declines it because the pointer locals are reused
elsewhere; do not remove their updates without proving subsequent reads cannot observe them.
Generated drafts remain disabled; no runtime, installed-game or working-port files were edited.


### Literal comparison pointer lifetimes across native control flow

native_local_liveness.py supplies a conservative read-before-overwrite proof over normalized native
statements. It follows conditional/unconditional native jumps, branch alternatives, loop back edges,
break exits and returns. It rejects escaping local addresses, unknown control structures, missing
jump targets and unbalanced scopes. Extra branch paths may reject safe cases; they cannot justify
removing a potentially observed pointer value. Re-entering the comparison marker kills its old
pointer values because the recognized loop initializes both pointers before reading them.

The literal-loop folder can now use this proof when locals are reused elsewhere, while retaining
the original no-outside-use fallback. It still requires the exact ASCII byte-loop shape, distinct
locals, signed count and reads within each literal's terminating NUL. The actual Theresa comparison
of OBJECT_CHOCOLATE_BOX_UNGIVEABLE against the empty string folds to count 31 / equality false.
Adding a subsequent read of either pointer rejects the transformation. Tests also cover branch
bypasses, loops, back edges, conditional breaks, address escape and unknown control flow.

Validation: 519/519 recovery tests pass; legacy benchmark remains 9/18. Full output records
literalByteComparisons per function: one each in TeddyGirl Main and Theresa Main. Overall 38/51
functions and 8/18 files compile, zero missing bodies, 1580 TODOs. Theresa's next syntax failure is
function-relative line 258, a conditional CCharString__NotEqual assignment. Its string operand
recovery needs native evidence before lowering. Draft registration remains disabled; runtime,
installed game and working port were not edited.


### Presented-item comparison operands recovered from native instructions

native_presented_items.py recovers Theresa and TeddyGirl's argumentless CCharString__NotEqual
calls using pinned raw/annotated source, complete native function bytes, comparison callees and
retail string literals. At 0xDBA4D1 / 0xDAF729 slot 0x8c receives [esp+0x10] as the output string.
The following push supplies 0x12D8F24 (chocolate) / 0x12D8958 (teddy); ECX=[esp+0x14] then points to
the same output after the push. Calls at 0xDBA4E8 / 0xDAF740 target 0x99E960, which negates the exact
byte equality routine at 0x4115A0. Both callee byte ranges are included in the witnesses.

The recovered explicit CCharString comparison lowers inside the existing conditional-assignment
emitter. It keeps the saved boolean and native short-circuit path. Only this reviewed global-name
operand plus a literal is supported; arbitrary CCharString pointers remain unresolved. The host
contract inspected was D:/Code/ForgeFSE-retail-shadow/FableScriptExtender/LuaEntityAPI.cpp:648:
it returns bool and publishes g_PresentedItemName before returning true. The function text hash
(normalized Python read_text newlines) was 3a12cad01557628d8865fdf451b3396a510481d9b311d400d5419c4e3ce832b3.
There is no intervening call/yield before the generated comparison reads the global. This is the
inspected host contract; no second Lua return value was assumed or runtime binding changed.

Tests execute both actual recovered conditional headers for absent/present messages, matching,
other, case-different, empty and nil names. They verify saved results, exactly one message query,
and short-circuit protection from stale globals. Changed source, bytes or item literals reject.
Full recovery suite: 521/521 pass; legacy benchmark remains 9/18. Full conversion stays 38/51
functions and 8/18 files compiling, 1580 TODOs, zero missing bodies. presentedItemEvidence is now
recorded per function. TeddyGirl next fails at function-relative line 328 (repeat/end structure),
Theresa at 291 (aggregate/subregister expression). Neither Main is complete. Drafts remain disabled;
runtime, working port and installed game were read only.


### Preserve structural conditions during native-noise filtering

TeddyGirl's repeat/end failure came from dropping `if (unaff_EDI != (undefined4 *)0x0) {`
while retaining its closing brace. Generic substring noise filtering now preserves structural
statements and records unresolved native control operands. Guessed register names and destructor
operands alone do not prove that an enclosing branch can be erased. Existing explicitly recognized
reference-count block handling remains separate. Tests execute both alternatives and a conditional
loop, and require unknown native function conditions to remain visible diagnostics.

The new execution tests also exposed whitespace-sensitive null conversion: removing a pointer cast
left extra spaces before 0x0, bypassing the existing comparison normalization. It now accepts those
spaces, preserving nil-pointer branch behavior. Full recovery suite: 524/524 pass. Full intro still
38/51 functions and 8/18 files compile, zero missing bodies; TODOs rise to 1584 because four formerly
silently dropped conditions are reported. TeddyGirl's next syntax failure is function-relative
line 331, an unresolved distance-call argument. Legacy syntax remains 9/18; previously hidden
conditions also increase diagnostics in StatueMaster, BeardyBaldy and DragonBossFight. No runtime,
working-port or installed-game edits; generated registration remains disabled.


### TeddyGirl cached Bully distance operands

native_teddy_distance.py reconnects the initial NOVI_Bully lookup to TeddyGirl's 10-unit distance
check. At 0xDAF106..0xDAF110, slot 0x120 receives the script-name string at original ESP+0x10 and
an output wrapper at original ESP+0x28. At 0xDAFAB1 the distance helper receives that wrapper as
ESP+0x2c after pushing float 0x41200000; ECX is entity+8. Ghidra's uStack_14c operand association
was misleading and the old output lost both the entity and distance placement. A converter-owned
hidden output slot now connects the lookup result to the reviewed use. It does not replace the
later AffairWife lookup or infer other wrapper uses.

Recovery pins raw source, annotated source after presented-item recovery, full 5498 native bytes,
and the retail script-name literal. Tests execute both distance outcomes, checking exact entity
handles, one NOVI_Bully lookup and threshold 10. Changed source, bytes or strings reject recovery.
The report records teddyDistanceEvidence. All 526 recovery tests pass; legacy benchmark remains
9/18. Regenerated full intro now compiles 39/51 functions and 9/18 files, zero missing bodies,
1583 TODOs. TeddyGirl Main compiles but still contains unresolved behavior; this is not full Main
parity or authorization to enable it. Runtime, working port and installed game remain untouched;
generated registration stays disabled.


### Theresa unsigned three-byte condition

native_subregisters.py now lowers explicit Ghidra `(uint3)` casts on scalar atoms to a 24-bit
mask, preserving truncation before later shifts. It does not just erase the cast. Tests exercise
the actual `(char)((uint3)uVar30 >> 0x10) == 0` condition with zero, bit 16 set, high-only bits,
negative and full-width values. Calls, compound expressions, floats, indexed/member expressions,
postincrements and quoted/commented text are excluded. The native source assigns uVar30 as 0 or
0x10000 in this routine; its remaining byte-slice test now compiles.

Validation: 528/528 recovery tests pass; full intro stays 39/51 functions and 9/18 files compiling,
1583 TODOs, zero missing bodies. Theresa's next failure is function-relative line 599, a cached
marker operand in a distance call. Native evidence for the next pass: GetThingWithScriptName at
0xDB9844 fills original ESP+0x2c (LEA ESP+0x30 after the name push) for M_TriggerOutro. At 0xDBA3E8
retail pushes 0x40000000 (2.0), obtains the hero at 0xDBA3ED, and at 0xDBA3F3 addresses that same
wrapper as ESP+0x30; ECX=hero and EDX=wrapper feed 0xCBE2FF at 0xDBA3F9. This is the same missing
cached-output association family as TeddyGirl, suitable for a shared reviewed recovery rule.
No runtime, installed-game or working-port changes; generated registration remains disabled.


### Theresa cached outro marker recovered

The cached-distance recovery now shares its validation/replacement implementation between the
TeddyGirl and Theresa witnesses. Theresa's initial M_TriggerOutro lookup gets an explicit hidden
output slot; the reviewed distance use consumes that same result alongside the current hero and
fVar26=2.0. Native sites 0xDB9844 and 0xDBA3F9, complete 7010 function bytes, raw/annotated sources
and the string at 0x12D9930 are pinned. The source witness is after presented-item recovery, as for
TeddyGirl. The unrelated entity lookups and other distance checks are not inferred from this rule.

Execution tests check exact lookup/hero/distance order, handles, 2.0 threshold and both boolean
results; changed threshold source or unavailable native bytes rejects recovery. The report records
theresaDistanceEvidence. Full recovery suite: 530/530 tests pass; legacy benchmark remains 9/18.
Full intro now compiles 40/51 functions and 10/18 files, zero missing bodies, 1582 TODOs. Theresa
Main now compiles but remains behaviorally incomplete (including reused-temporary/aggregate and
resource cleanup issues). Compiling Main bodies must not be treated as gameplay-parity evidence.
Runtime, working port and installed game remain untouched; generated registration stays disabled.


### Invalidate stale operand values after unresolved writes

Lifter.forget_value centralizes invalidation of temporary values, operand ordering, receiver/parent
aliases, result kinds and staging facts. Unresolved stack writes, skipped noise writes, unknown
local-call assignments and unresolved address assignments now discard the previous known value.
Known address copies capture their referent before invalidation. This prevents a previously staged
chocolate string from surviving a later unlifted bitmask assignment and entering a bitwise test.
It does not claim to recover the missing assignment; subsequent operands remain unresolved.

Tests cover stale stack strings, unknown calls, ignored bitmask writes, receiver/kind invalidation
and preservation of a valid address copy. Full recovery suite: 534/534 pass. Regenerated intro stays
40/51 functions and 10/18 files compiling, zero missing bodies, with 1600 TODOs (formerly 1582):
invalidating stale values exposes additional missing operands rather than silently substituting old
ones. Legacy syntax remains 9/18. No runtime, working-port or installed-game edits; drafts disabled.

Viability assessment shared with user: native-to-Lua conversion is supported by real recoveries and
execution tests, but unattended whole-script parity is unproven. Remaining work includes replacing
function-specific recovery patches with stronger general control-flow/type/value tracking, native
resource lifetime handling, and readable output beyond decompiler locals and raw labels.


### Shared control-flow IR and reaching definitions

native_flow_ir.py extracts the existing conservative control-flow construction into immutable
NativeStatement/NativeFlowGraph records with source text, normalized code, statement identity and
successor edges. Native literal-pointer liveness now consumes this shared graph instead of keeping
a private graph implementation. Unsupported structural syntax rejects graph construction. The graph
explicitly reports exact=False: alternate-branch overapproximation remains and must not be mistaken
for a fully reconstructed executable CFG.

native_reaching_definitions.py adds fixed-point analysis of which scalar-local assignments can
reach each statement. It joins definition sets across branches and loop back edges, ignores
unreachable definitions and tracks conditional/compound/subregister writes. Unknown entry values
and address-escaped locals remain unknown. This analysis is not yet used to rewrite arbitrary
operands: expression typing, alias handling and precise control edges still need development.
Tests cover joins, loops, unreachable assignments, conditional definitions, escape and compound
writes. Full recovery suite: 540/540 pass; full output unchanged at 40/51 functions, 10/18 files,
zero missing bodies, 1600 TODOs.

Actual-native analysis artifact: work/script_converter_native_flow_analysis.json. Graphs accepted
CreatedBeetle Main (26 statements), TeddyGirl (821), Theresa (906) and Villager (431). Results record
ambiguous incoming facts and structural reachability, not behavior validation or completion. This
is the first shared analysis foundation for replacing source-order temporary guessing; existing
native-specific witnesses remain active. No runtime/port/game edits; drafts remain disabled.


### Typed scalar facts from reaching definitions

native_value_ir.py adds immutable typed scalar values and resolves literals/copies through all
incoming definitions. Copies consult their defining program point. Agreement is required across
all paths; cycles, unknown writes, escaped locals and differing values remain unknown. Bool,
integer and string types remain distinct. Native narrowing/casts and general arithmetic are not
yet folded here. Reaching definitions now conservatively treats locals mentioned in calls as
potential outputs until API effect signatures are supplied, and recognizes indexed/compound/
subregister mutations rather than trusting an earlier constant.

Tests cover agreeing/disagreeing branches, copy timing, types, calls, partial writes, escapes and
loop mutations. Full recovery suite: 544/544 pass. Work artifact:
work/script_converter_native_value_analysis.json. Actual raw-native analysis finds 21 known
incoming values in TeddyGirl, 18 in Theresa and 9 in Villager, zero in CreatedBeetle. No additional
literal copy substitutions were proven in this first pass. Those are incoming facts, not a count
of corrected instructions or behavioral coverage. No new Lua rewriting is enabled from these facts;
call read/write effects and more precise branch modeling are the next prerequisites. Full conversion
remains incomplete at 40/51 compiling functions and 1600 TODOs. Runtime, port and game unchanged;
drafts remain disabled.


### Correct alternative edges and scalar API input effects

Native flow construction now sends a completed if/else-if arm past all sibling alternatives.
The previous fall-through into else could erase a real true-path definition; it was not a sound
conservative approximation. Nested chains and unmatched alternatives are covered by tests, as is
the corresponding pointer-liveness failure where an else-only overwrite hid a true-path read.
The global exact flag remains false because other structural approximations still exist.

native_call_effects.py recognizes standalone annotated GSI calls with required scalar-only Quest
parameters/returns, matching arity and atomic operands. Their input locals are passed by value;
world effects and returned values remain unknown. NativeValues/reaching_definitions optionally
consume this classifier. Hidden outputs, optional parameters, pointers, nested expressions and
unknown calls fall back to conservative invalidation. Escaped locals stay unknown even with a
known scalar API. Tests verify timer-ID preservation independently from its unknown return value.

Actual annotated-native measurement: work/script_converter_api_effect_analysis.json. All four
sample Main graphs construct; incoming known facts are 0 CreatedBeetle, 21 TeddyGirl, 19 Theresa,
7 Villager with corrected branches. The initial scalar API classifier does not increase those
counts yet; richer entity/result/reference effects remain necessary. These facts are not newly
emitted Lua or behavioral parity. Full generated intro remains 40/51 functions, 10/18 files,
zero missing bodies, 1600 TODOs. Drafts remain disabled; runtime, port and installed game untouched.


### Symbolic API results and first IR-driven readable locals

ProducedValue records a call's result kind, producing statement and callee without inventing its
runtime value or replaying the call. NativeValues carries these facts through copies at their
original definition points. Distinct producers do not merge. Producers on graph cycles are rejected
because one static statement could represent multiple runtime instances. The initial result profile
covers scalar API results plus the reviewed direct-pointer, zero-argument GetHero binding.

native_readable_names.py is the first emitter consumer of the new analysis. It names RegisterTimer
and GetHero locals only when all uses retain that producer, with no reassignment or repeated producer.
The full converter enables it; legacy defaults remain unchanged. It avoids native/generated name
collisions and preserves quoted strings and comments. The conversion report records readableLocalNames.
Current actual output: CreatedBeetle Main uVar1 -> timerId; Bully Init uVar4 -> hero; Villager Init
uVar3 -> hero. Uncertain locals retain their native names. The symbolic-results work artifact records
four proven uses of CreatedBeetle's registered timer and no new symbolic facts in the three large
Main routines tested. Richer reference/call effects remain necessary there.

Validation: full suite 558/558 passed before adding the final focused emitter test; all four readable
name tests then passed, including Lua execution verifying timer registration result, set/remove calls
and return value after renaming. A no-candidate early exit avoids unnecessary analysis; regenerated
full output remains 40/51 functions, 10/18 files, zero missing bodies, 1600 TODOs. Full generation now
takes about 7 seconds in this run versus roughly 1-2 before analysis. No runtime, port or installed-game
edits; generated registration stays disabled. Completion and whole-script behavioral parity remain
unproven.


### Shared entity output-slot and const-reference effects

The call-effect profile now treats a supported explicit GetThingWithScriptName hidden return slot
and literal key as an entity definition. Its output address alone is not classified as an escape;
any other unrecognized address use still is. NativeValues resolves such definitions into ProducedValue
facts without requiring a textual assignment. Unsupported arity, missing contracts, variable keys
and unknown functions remain conservative.

A second profile recognizes the const-reference distance helper only when every occurrence of its
native label resolves to 0x00CBE2FF and the reviewed parameter contract agrees. Its input bindings
survive the call. The checked native_support/0x00CBE2FF.c shows validity/position queries and a squared
distance comparison over two const CScriptThing references. Conflicting targets or unknown names do
not receive this effect. This profile feeds both analysis and proven readable-local naming.

Tests exercise both entire actual recovered TeddyGirl/Theresa native bodies: the shared analysis now
connects local_native_cached_bully and local_native_outro_marker from their original hidden-output
producer to the distance use inside the loop. Unknown effects correctly lose the loop fact. Additional
tests cover escape and unsupported output forms. Full suite: 564/564 pass. Regenerated intro remains
40/51 functions and 10/18 files compiling, zero missing bodies, 1600 TODOs. This improves general
provenance tracking after operand recovery, not yet elimination of the native operand witnesses.
No runtime, working-port or installed-game changes; generated registration remains disabled.


### Native call-setup IR and automatic instruction windows

native_call_setup_ir.py symbolically evaluates straight-line x86 MOV/LEA/PUSH argument setup,
recording ECX, EDX, call target and stack arguments. Values include constants, memory/address
expressions, window-relative stack addresses and explicitly profiled intervening-call results.
Unknown calls/control transfers, unsupported stack mutations and memory writes reject recovery;
unmodeled partial-register writes invalidate the whole register fact. Stack offsets are relative
to window entry, not automatically globally comparable native frame locations.

Automatic window discovery decodes from the aligned function entry, stops at prior calls/branches,
and respects incoming branch targets so it cannot use skipped setup instructions. Both cached-distance
rules now consume this shared decoder: lookup output address must agree with distance EDX, the target
must be 0xCBE2FF, and the pushed float must match the reviewed threshold. The report includes decoded
lookupSetup/distanceSetup snapshots. Manually supplied window boundaries were removed from witnesses.
Theresa's intervening GetHero call still has an explicit reviewed zero-cleanup/result profile; call
identity and relationships between window stack baselines are not yet inferred generally.

Metadata finding: TU size 5498 for TeddyGirl stops inside the final ADD ESP; actual stack-restore/RET
ends at 0xDB0600. Theresa's 7010 likewise ends inside ADD ESP; the tail ends at 0xDBB305. Earlier
'complete function bytes' wording for these pinned ranges was too strong: they omit 6 and 3 tail
bytes respectively. Decoder extension accepts only observed ADD ESP immediate plus RET, rejecting
arbitrary undecoded tails that could hide incoming branches. Existing source/call-region evidence
remains unchanged. Capstone is now used by production cached-argument validation as well as audits.

Validation: 570/570 full recovery tests pass, including actual native windows, automatic boundaries,
incoming-jump bypasses, partial registers and rejection cases. Full generation remains 40/51 functions
and 10/18 files compiling, zero missing bodies, 1600 TODOs. This moves operand verification into reusable
instruction analysis but does not prove whole-script parity or eliminate all per-function witnesses.
Runtime, working port and installed game remain untouched; generated registration remains disabled.

### Conservative instruction effects during argument recovery

The call-setup interpreter now rejects unmodeled instructions unless they are
reviewed register-only scalar operations or read-only CMP/TEST comparisons.
Register-write metadata and first-operand checks alone cannot establish absence
of implicit memory effects or exceptional control transfers. Byte-level regressions
cover string stores/copies (including REP), interrupts, UD2, SYSENTER, memory
exchange, and preservation through a read-only stack comparison. Actual cached
TeddyGirl/Theresa argument recovery remains covered and passes.

Validation: full recovery suite 578 tests passed; regenerated intro remains
40/51 functions and 10/18 files compiling, zero missing bodies, 1600 TODOs.
Generated quests.lua still contains Quests = {}. This closes a decoder soundness
gap; it does not establish new function coverage or whole-intro behavioral parity.
No runtime or working-port files were edited by this change.

### Reviewed argument counts distinguish call operands from native frames

The shared call-setup decoder accepts an optional explicit native stack-argument
count, including hidden arguments. In this mode it models aligned immediate SUB
ESP frame allocation and reads only the requested top argument words. Reserved
frame contents remain absent; reading them as arguments rejects. Without a
reviewed count, the prior strict behavior remains. The count must come from an
ABI contract, not an incomplete decompiler parameter list.

This recovers the actual first StartBarrelTimer GetTimer call at 0xDB4F85 from
aligned full-function bytes (0xDB4F70, size 562): its sole argument is memory at
entry ECX + 0x108, after tracking MOV ESI,ECX through the frame prologue. The
indirect target is global GSI's vtable slot 0x168. Persistence evidence identifies
0x108 as WatchTimer. work/script_converter_barrel_timer_arguments.json records
the symbolic result. This is analysis-only pending native-call/source correlation;
the later loop timer call, cached guard point and branch-selected colours still
need recovery. No emitted operand was guessed from the working port.

Validation: 583 full recovery tests passed, including actual native argument
recovery, saved-register/frame separation, invalid counts and uninitialized frame
rejection. Full generation remains 40/51 compiling functions, 10/18 files, zero
missing bodies, 1600 TODOs. Generated draft registration remains disabled. No
runtime or working-port edits were made by this change.

### Native entry timer evidence now feeds Lua emission

native_entry_timer.py correlates a missing first GetTimer source statement with
the first native call in an uninterrupted quest-entry prefix. It checks the raw
source's global-GSI slot 0x168, the Quest/int timer contract, the decoded receiver
and target, a single argument proven to read entry ECX plus an integer state
field, and absence of branches into/reentering the prefix. It does not equate an
arbitrary later analysis-window ECX with function-entry this. Unsupported source,
ABI, control flow, field mappings and native targets reject recovery.

The full converter now emits the first StartBarrelTimer query using
quest:GetTimer(quest:GetStateInt("WatchTimer")); entryTimerEvidence records the
actual decoded setup and source/native hashes. This rule has no function-address
allowlist and obtains the field offset from instructions. It currently covers
only the first entry call; the loop query still has a missing operand. Native
call/source correlation across loops and branch-selected colours remain needed.

Validation: full suite 586 tests passed before the final header-comment span
regression; all four focused entry-timer tests subsequently passed with that fix.
Lua execution verifies dynamic state reads for several timer IDs. Negative cases
cover altered targets, native-source disagreement, reentry, missing state and API
contracts. Regenerated output: 40/51 compiling functions, 10/18 files, zero missing
bodies, 1599 TODOs (one required operand resolved). Registration remains Quests = {}.
No runtime or working-port files were edited by this work.

### Register origins across native loops

native_register_origins.py adds fixed-point register-copy provenance over aligned
native instructions. Branch joins retain a function-entry origin only when all
reachable paths agree. Partial writes, loads, computations and unprofiled calls
lose the affected facts; stack saves are not reconstructed. Calls preserve only
registers explicitly supplied per call site by the consumer's ABI profile.
Indirect/misaligned branches, interrupts and unsupported system transfers reject
analysis. The consumer must supply the complete function instruction extent.

Analysis of actual StartBarrelTimer bytes shows the loop's second timer query at
0xDB4FB6 reads [ESI+0x108], with ESI tracing to entry ECX if the four relevant timer,
frame and termination calls preserve ESI. The shared call decoder independently
resolves this operand and the global GSI slot 0x168. Evidence is saved in
work/script_converter_barrel_loop_register_origins.json. Native slot inventory
maps GetTimer to 0x6E7710 and NewScriptFrame to 0x6E7410. The new loop result is
analysis-only: consumers must validate profile targets and native/source loop
correspondence before it can replace the second missing Lua operand.

Validation: 593 full recovery tests passed, including actual-loop provenance,
branch disagreement, partial writes, unknown calls, unreachable assignments and
unsupported control-flow rejection. No emitter changes in this checkpoint;
last generated coverage remains 40/51 functions, 10/18 files, 1599 TODOs, zero
missing bodies. Draft registration remains disabled. Runtime and port files
were not edited by this work.

### Second barrel wait query now uses native register-origin evidence

native_loop_timer.py consumes register_origins and read_call_window to recover
the second GetTimer operand in StartBarrelTimer. The reviewed witness pins the
native function and annotated source after entry-query recovery, all return paths
of GetTimer (0x6E7710, 46 bytes), NewScriptFrame (0x6E7410, 66 bytes), and quest
IsActiveThreadTerminating (0xCB7940, 14 bytes), plus the two GSI vtable entries.
GetTimer restores saved ESI before either return; NewScriptFrame restores ESI on
its modifying path and leaves it unchanged on early exit; termination does not
modify it. Nested calls retain the native MSVC ABI assumption. The caller's four
reviewed sites receive an explicit ESI-preservation profile. Altered bodies or
dispatch pointers reject recovery.

The shared analysis must prove ESI originates at entry ECX on all reaching loop
paths, remains unchanged during argument setup, and supplies the decoded integer
field operand. Source/native hashes provide the current per-function loop/source
correspondence; this is not yet generalized loop matching. Generated Lua now reads
WatchTimer in both initial-wait queries. loopTimerEvidence records the proof.

Validation: 596 full recovery tests passed. Lua execution of the entire recovered
initial wait covers already-positive timers, repeated frames with changing timer
IDs, and termination without a subsequent timer query. Changed callee bodies,
vtable pointers, source and field mappings reject. Full generation: 40/51
compiling functions, 10/18 files, zero missing bodies, 1598 TODOs. Quests = {} stays
in generated registration. Runtime and working port were not edited.

Read-only port comparison: reconstructed StartBarrelTimer uses the same positive
timer wait but caches WatchTimer and adds logging queries. Native reloads the field
on every query; generated tests preserve that evidence rather than copying the
port's cache/logging. Whole-function behavior remains unproven: guard-point lookup,
distance and information-bar arguments/colours still require recovery.

### Barrel guard-point lookup and distance recovery

The shared cached-distance recovery now accepts an explicit reviewed lookup text,
covering StartBarrelTimer's decompiled GetThingWithScriptName(ppVar9,&stack...) as
well as the previously argument-less TeddyGirl/Theresa calls. Native barrel evidence
pins source after both timer recoveries, full native bytes and marker string. Aligned
function-entry decoding shows the lookup at 0xDB5075 writes the wrapper at window
ESP+0x20; the distance at 0xDB50BD takes that slot in EDX after pushing float 2.0,
with the intervening GetHero result in ECX. Relationships between these window
baselines remain reviewed per-function evidence, not a general frame proof.

Generated Lua now looks up M_WHouse_GuardPoint once and passes the cached entity,
current hero and 2.0 to the distance helper. StartBarrelTimer compiles: full output
is now 41/51 functions, 10/18 files, zero missing bodies, 1598 TODOs. Its information
bar arguments and branch colours remain incorrect/incomplete; compilation does
not establish behavior parity. Generated registration remains disabled.

Validation: seven focused barrel/Teddy/Theresa tests passed, including cached entity
reuse across two frames, current-hero queries, actual whole-native-function value
provenance and changed evidence rejection. The full 600-test run completed with
2 failures and 2 errors, all in test_audit_bully_run_off_bytes subtests. An isolated
run of that five-test module reproduces them without the converter: current working
port/fixture traces omit FixMovieSequenceCamera(false) where the termination tests
expect it. Those tests directly load reconstructed NOVI_Bully.lua and trace fixtures;
they do not use this new converter recovery. The parallel session's files were left
untouched. No other full-suite failures were reported.

### Native timer-bar colour branches now emit Forge colour tables

native_colours.py introduces a typed converter literal __native_colour_bgra32,
which lowers a validated 32-bit packed word into explicit r/g/b/a bytes. It is an
IR marker, not a claimed retail API. The native colour slice is pinned after
barrel timer/distance recovery. Its two branch-selected packed colours become
one ChangeQuestInfoBarColour call in each branch, preserving exactly one call
and the original distance condition while removing dead pointer/stack temporaries.
Native BL is zeroed at 0xDB5008; the true branch begins 0xDB50C9, false branch
0xDB50F8, joining before the slot-0x534 call at 0xDB512C. The pinned source and
bytes establish that no intervening observable effects are moved.

Read-only Forge inspection confirms ParseColorTable accepts lowercase r/g/b/a
and returns retail BGRA bytes. Lua tests compare both emitted branch colours
against the actual working port's common.lua COLOUR_GREEN/COLOUR_RED values,
assert one API call and check a packed word with four distinct channels.
Changed native/source/contract evidence rejects recovery.

Validation: six focused colour/distance tests passed. Full suite ran 609 tests;
only the already isolated Bully run-off termination tests failed (2 failures,
2 errors), still expecting FixMovieSequenceCamera(false) absent from the working
port trace. No parallel runtime/port files were edited. Generation remains 41/51
compiling functions, 10/18 files, zero missing bodies; TODOs rise to 1599 because
removing stale colour temporaries exposes the two genuinely missing UpdateQuestInfoBar
arguments instead of silently filling them with colour words. Registration stays
disabled. StartBarrelTimer is still behaviorally incomplete.

Next native evidence: 0xDB5145 and 0xDB514A push 0xBF800000 (-1.0) for the later
bar update before GetTimer's own argument/call. GetTimer returns at 0xDB5156;
FILD at 0xDB515A and FSTP at 0xDB5162 convert the signed timer integer to float32,
then slot 0x530 is called at 0xDB5169. This is the next missing operand sequence.

### Timer/bar update argument boundaries recovered

native_barrel_update.py pins the actual update source/native slice after colour
recovery and consumes shared call-setup evidence at 0xDB5150. That snapshot has
three staged words: GetTimer's WatchTimer operand and two older 0xBF800000 words
belonging to UpdateQuestInfoBar. The reviewed native sequence pops the one timer
argument, converts the signed int32 result via FILD/FSTP, and calls slot 0x530
with the bar ID, current value, -1.0 and -1.0. The converter now emits those calls
explicitly, using native_bar_timer_value as a local rather than the misleading
CCharString-cast stack temporary.

Read-only Forge signature inspection confirms UpdateQuestInfoBar(int,float,float,float)
forwards typed float parameters. Lua retains the exact integer timer until that
binding performs the float32 conversion; no redundant Lua packing helper is emitted.
Tests model this binding boundary and cover negative/zero/ordinary timer values,
16777217 -> 16777216 and INT_MAX -> 2147483648 float32 rounding. The bar state is
read after GetTimer, including when the query changes the bar ID. Changed source,
bytes or float contract rejects recovery.

Validation: five focused update/colour tests passed; full suite ran 616 tests with
only the already isolated Bully run-off termination failures (2 failures, 2 errors).
Generation: 41/51 compiling functions, 10/18 files, zero missing bodies, 1598 TODOs.
The UpdateQuestInfoBar missing defaults are resolved. Initial AddQuestInfoBar
arguments remain incorrect and whole StartBarrelTimer parity is still unproven.
Generated registration remains Quests = {}; runtime and working-port files were
not edited by this change.

### Whole generated StartBarrelTimer passes scenario comparisons

native_barrel_creation.py recovers all seven AddQuestInfoBar arguments from the
reviewed native source/byte witness: 45.0, 0.0, green, green, HUD_CLOCK_ICON, empty
text, 1.0. It validates both retail string addresses; the empty literal is checked
as a NUL byte because RData.string_at intentionally returns None for empty strings.
The typed colour IR supplies the two tables. The returned ID is written to the
original GUIBarrelCounter field. This remains a reviewed per-function native
construction rule, not a general C++ aggregate reconstruction pass.

The whole function is now generated with zero TODOs and executes in Lua tests
against the actual working port. Tests compare observable bar/state/frame effects
for immediate positive timer completion, an initial wait, termination in that wait,
and termination in the active loop. They verify exact creation arguments, green
then red updates with current values 44/43, one cached lookup, and normal-only bar
removal. Port diagnostic GetTimer/Log calls and symmetric distance operand order
are not treated as effect differences. Separate previous tests retain native query
order and dynamic state reloads. A native-only test verifies termination immediately
before creation prevents the bar; the port wait helper lacks that extra check.
These are mock-host scenario comparisons, not an in-game parity claim.

Validation: 618 full-suite tests ran; only the already isolated Bully run-off tests
failed (2 failures, 2 errors). Generated StartBarrelTimer creation evidence reports
recovered, syntax passes, TODO list is empty, and its emitted creation call was
inspected. Whole-intro coverage remains 41/51 functions, 10/18 files, zero missing
bodies, 1598 TODOs. Registration stays Quests = {}. Runtime and working-port files
were not edited; the complete intro objective remains unfinished.

### Reviewed player-control query fills an SDK-manifest gap

converter_signatures now supplies IsHeroControlledByPlayer only when absent from
the shared SDK manifest. Read-only evidence: LuaManager binds the method; the
LuaQuestState bool/no-argument implementation forwards the native query (and
returns false if unavailable); GameInterface typedef and slot inventory agree on
vtable[394]/0x628 -> 0x0088E980. No shared manifest or runtime edits were needed.
An existing/newer contract remains untouched.

The actual ManageQuestCoreMarkers player-control wait is executed in Lua tests:
already-controlled, repeated false queries followed by true, and cancellation after
a frame before another query. Bool typing correctly turns the native char-zero
condition into a Lua boolean wait. Seven focused query/previous-parity tests passed.
The full suite ran 620 tests with only the existing isolated Bully run-off failures
(2 failures, 2 errors). Full conversion now emits two supported calls in Guard Main
and two in ManageQuestCoreMarkers, dropping TODOs from 1598 to 1594. Coverage remains
41/51 compiling functions, 10/18 files, zero missing bodies. Both larger routines
still have other unresolved operands/cleanup. Draft registration remains disabled;
no runtime or working-port files were edited.

### Quest-marker entity operands connected to native lookup outputs

native_quest_markers.py now recovers ManageQuestCoreMarkers' three cached lookups
and seven marker add/remove calls. Each call consumes shared read_call_window
facts: lookup outputs at window-relative offsets 0x0C (father), 0x24 (book trader),
0x18 (Theresa), then marker operations over those same reviewed frame slots.
Targets must match the quest GSI slots 0x120/0x578/0x580. Source/native hashes and
four string addresses pin source correspondence and the reviewed relationship
between window baselines. This is not a general cross-window stack-frame proof.

The emitted marker sequence is father -> book trader -> Theresa -> father, using
HUD_ORB_QUEST_CORE throughout. Previously the first marker was guessed onto Theresa,
the next texture could become NOVI_BookTrader, and later entity arguments were nil.
The focused Lua test executes all ten recovered call statements and checks each
cached entity, add/remove order and texture; waits/cleanup are outside that test.
Changed evidence rejects recovery. Four focused marker/player-query tests passed.

Full suite: 622 tests, with only the existing Bully termination subtest failures
(2 failures, 2 errors). Generation: 41/51 compiling functions, 10/18 files, zero
missing bodies, 1590 TODOs. ManageQuestCoreMarkers still fails syntax in its
reference-count cleanup and retains two TODOs for local_24 bookkeeping. Next
native evidence: ordinary cleanup calls 0x4AA840 on ESP+0x18/+0x24/+0x0C; the early
exit at 0xDBE7D5 inlines reference decrements for those same wrappers and calls
0x99A2E0 after clearing each. Cleanup recovery must distinguish wrapper ownership
from script effects and preserve early-return paths. No runtime/port edits;
generated draft registration remains disabled.

### Marker helper compiles; script-effect scenarios pass with ownership caveat

native_marker_cleanup.py separates the reviewed temporary-wrapper cleanup from
the thread's early return. It pins the full source/native body after marker-operand
recovery and helpers 0x4AA840 (66 bytes) / 0x99A2E0 (7 bytes). The former restores
the CScriptThing vtable, decrements the counted pointer and calls its destructor at
zero, clears its members and tails into the base-vtable restoration. Early native
cleanup inlines the same operations on the three cached wrappers. The recovery
removes only those pinned fragments and obsolete staging, preserving the gold-wait
termination branch and the common exit label.

Important verified host limitation: current LuaQuestState::GetThingWithScriptName
returns an adopted shared_ptr whose deleter decrements RefCount and frees the heap
wrapper, but does not invoke the native zero-reference callback. Lua GC also does
not establish native destruction timing. The converter therefore retains an explicit
OWNERSHIP_LIMITATION TODO and markerCleanupEvidence; this is not claimed as exact
native lifetime parity. No runtime changes were made.

The complete generated ManageQuestCoreMarkers now compiles and passes observable
script-effect comparisons against the actual working port: normal progress through
all five waits, cancellation at each wait, tutorial declined, and already-satisfied
conditions. An extra native-only test checks preexisting termination after the gold
query. Tests do not simulate native reference counts/destructor callbacks.

Validation: four focused cleanup/marker tests passed. Full suite ran 624 tests with
only the known Bully run-off failures (2 failures, 2 errors). Generated coverage is
42/51 functions, 10/18 files, zero missing bodies, 1589 TODOs. The marker helper's
sole remaining diagnostic is the ownership limitation above. Quests = {} remains
in generated registration; runtime and working-port files remain untouched by this
change. Complete intro conversion is still unfinished.

### Nil-safe native IsAlive queries on typed lookup results

The first PostAttackStuff availability loop exposed a latent runtime error: Forge
returns nil for an absent GetThingWithScriptName result, while retail returns an
empty CScriptThing wrapper and calls IsAlive on it. Native slot 0x12C at 0x4AB130
checks the implementation pointer and returns false if absent. Read-only Forge
LuaEntityAPI::IsAlive likewise returns false for null/missing implementation data.

The shared thing-call emitter now guards assigned, zero-argument IsAlive calls on
known thing-typed local identifiers: (thing ~= nil and thing:IsAlive()). The bound
entity-thread me remains direct. Restricting the guard to local identifiers avoids
replaying receiver expressions; the native bool signature establishes result type.
The actual initial post-attack lookup/IsAlive sequence is exercised for missing,
non-alive and alive results, checking one lookup and at most one predicate call.
An existing hidden-output alias expectation was updated for the intentional guard.

Validation: 81 focused lifter tests passed. Full suite ran 625 tests with only the
known Bully run-off termination failures (2 failures, 2 errors). Generated
PostAttackStuff now contains the guard, but still has untranslated cleanup and
other operands. Intro coverage remains 42/51 functions, 10/18 files, zero missing
bodies, 1589 TODOs. Drafts remain disabled; runtime and working-port files were
not edited by this change.


## PostAttackStuff availability-loop wrapper cleanup

Added native_post_attack_cleanup.py and a source/native-byte-pinned witness. Review
used aligned disassembly from DBEB20: lookup DBEB4F writes wrapper ESP+10, IsAlive
DBEB59 preserves the branch result, cleanup DBEB61..DBEBA5 decrements info ESP+18,
invokes its callback/free at zero, clears implementation/info, and restores base
vtable via 99A2E0. Only that inline wrapper cleanup and its decompiler staging
assignment are removed. String cleanup DBEBAE and all retry/termination control stay.
The generated opening loop now compiles and retries missing/dead entities correctly.
An ownership TODO remains because runtime shared_ptr/GC cannot prove zero-ref
callback parity. New report field: postAttackCleanupEvidence.

Three new tests cover actual-loop execution (live immediately, missing then dead
then live, cancellation on missing/dead), source/byte mutations, and full-lifter
ownership evidence. Four focused tests passed with nullable-alive coverage. Full
suite: 630 tests, only the previously documented Bully-port 2 failures + 2 errors.
Generation: 42/51 functions compiling, 10/18 files, zero missing, 1589 TODOs.
Draft registration remains disabled; working-port/runtime files were not edited.

Next useful PostAttackStuff evidence: aligned native calls DBEBEB slot AE8 pushes
45; DBECE4 slot 684 pushes 0; DBECFC slot AE8 pushes 57. These are still missing
arguments in output; decode with shared call-window IR and check manifest contracts.
DBECC1 direct CBE87F receives ECX=20. Existing readiness/address-map work identifies
that misnamed CSubtitleRenderer::SetText as AddLogbookStoryEntry; see
analyze_native_conversion_readiness.py:87 and logbook_helpers_native.c. Verify the
runtime address/contract and lower from actual register evidence, not its incorrect
BSim multi-argument prototype. Next syntax errors include stale iVar1=*piVar5 and
SUB41 stack expression before teleport; both need bounded evidence before removal.


## PostAttackStuff typed music/camera operand recovery

Added native_post_attack_scalars.py and witness, after availability cleanup in
Lifter. Shared call-window IR verifies interface receiver ESI+40, indirect vtable
target, and exactly one pushed argument for DBEBEB/DBECE4/DBECFC. Manifest contracts
and vtable entries are pinned too. Generated calls now use music 45, camera float
0.0, music 57; report has postAttackScalarEvidence with decoded setups/typed values.
Reviewed working port has the same values, but production recovery uses native
instructions, not port text. All replacements are atomic on evidence disagreement.

Tests execute the recovered calls in Lua and reject changed native/source bytes,
vtable entries, contracts, or decoder receiver/target/argument results. Six focused
tests passed. Full suite ran 634 tests in 83.097s; only the known Bully run-off
termination 2 failures + 2 errors remain. Full generation: 42/51 functions, 10/18
files compiling, zero missing bodies, 1586 TODOs (three fewer). Drafts stay disabled;
no edits to runtime/working-port files.

Read-only runtime verification for next logbook lowering: FableAPI.h:241 declares
void __fastcall(int storyId), FableAPI.cpp:482 binds CBE87F, LuaManager.cpp:288
exposes numeric/string overload, LuaQuestState.cpp:2334 forwards int to native.
PostAttack call DBECC1 has constant ECX=20 and zero stack args under shared decoder.
Still needs safe source correspondence recovery for misnamed SetText call and
callee evidence; do not trust its BSim multi-argument declaration.


## Numeric logbook recovery in PostAttackStuff

Added native_post_attack_logbook.py/witness after scalar recovery. Pins raw and
annotated caller source, 1095 caller bytes, full 225-byte CBE87F callee through RET
CBE95F. Shared call-window IR verifies direct call DBECC1 to CBE87F with ECX=20
and no ABI stack arguments. Actual body copies ECX into EDX for GFIntToCharString,
builds TEXT_QST_LOG_STORY_ strings and invokes interface slot 4D0, consistent with
reviewed runtime numeric AddLogbookStoryEntry binding. Replaces the incorrect BSim
CSubtitleRenderer::SetText multi-argument call with GSI->AddLogbookStoryEntry(20).
Report field postAttackLogbookEvidence records native setup and prototype evidence.

Focused testing exposed int|string placement classifying 20 as a missing string.
Fixed one-parameter explicit integer overload placement before string inference;
it does not consume stale strings and handles signed int32 hex. String overload
still tested. Four new tests cover numeric Lua dispatch, string/numeric placement,
and native/source/contract/decoder mutation rejection. 90 focused tests pass.
Full 638-test suite: only known Bully termination 2 failures + 2 errors, 74.722s.
Generation: 42/51 functions, 10/18 files compile; zero missing, 1585 TODOs. Draft
registration is disabled and no runtime/working-port edits made.

Next teleport evidence (aligned DBEB20 disassembly): DBEC01 loads interface from
ESI+40, DBEC04 caches vtable in EBX; DBEC06 pushes false for later teleport, then
lookup args staged DBEC08..13 and slot120 call DBEC16. Lookup result EAX pushed
DBEC1C; slot118 GetHero DBEC21 result pushed DBEC2A; teleport slot760 DBEC2B.
Raw decompiler iVar1=*piVar5 and SUB41 stack expression are staging artifacts that
still break output syntax, but remove only with source/native operand proof.
Runtime LuaQuestState.cpp:2240 accepts optional third bool and forwards default
false; shared manifest currently lists only two thing params. Current two-argument
output uses that default but should record native false evidence when recovering.


## PostAttackStuff teleport staging recovered

Added native_post_attack_teleport.py/witness after logbook recovery. Native caller,
source and full wrappers pinned: GetThingWithScriptName 8A7D60 size151 (RET8 on both
paths), GetHero 891CA0 size173 (RET0, returns persistent wrapper), teleport88E540
size86 (RET12). Preserved EBX/ESI/EDI plus explicit lookup8/hero0 call profiles let
shared call-window IR prove hero/lookup/false stack order at DBEC2B. Lookup DBEC16
and hero DBEC21 targets/receivers checked separately, along with vtable entries and
M_PostAttackStart string. Removed only reviewed pointer-staging block, retaining
following cleanup. Generated code now has a readable hero variable and explicit
EntityTeleportToThing(hero,CVar4,false). Runtime optional third bool is added in
converter signature overlay only; shared manifest unchanged.

83 focused tests passed; full suite ran646 tests in66.338s, only known Bully
termination 2 failures+2 errors. Generation remains42/51 functions,10/18 files,
zero missing,1584TODOs. Drafts disabled; runtime/working-port files untouched.
New tests execute actual lookup/hero/teleport block, check evaluation/argument order,
and reject changed caller/callee/source/dependency targets or swapped operands.

Next cached-distance recovery: initial lookup DBED2C produces window stack+28 and
name wrapper+12. Actual distance calls are DBED51 and DBED8D (DBED56/DBED92 in
Ghidra source are RETURN addresses, not call sites). Actual GetHero sites DBED45
and DBED81 (not DBED4B/DBED87). Both push40A00000 (5.0), load EDX from ESP+20 after
push (baseline+28), ECX from hero EAX, direct target CBE2FF. Current output incorrectly
uses stack expression as third operand and piVar5 as repeated target. Need support
two reviewed distance occurrences sharing cached output, preserving loop termination.


## Repeated father-trigger distance recovered; PostAttackStuff compiles

Extended shared native_teddy_distance.py with optional distanceChecks list, checking
every native setup before replacing all corresponding source occurrences. Existing
single-check witnesses preserved. New PostAttack witness after teleport recovery:
lookup DBED2C cached output baselineESP+28, checks DBED51/DBED8D, profiled hero calls
DBED45/DBED81, EDX=stack28, ECX=hero result, pushed40A00000=5.0. Both source calls
now consume one cached MK_OVI_DADTRIGGER output rather than stale piVar5/stack text.
Report postAttackDistanceEvidence includes all distanceSetups (old distanceSetup
field retained for existing consumers). Stack baseline relationships remain reviewed
per-function rather than a claimed general global frame proof.

Tests execute actual wait fragment with native goto cancellation label: immediate
near, two retries with a fresh hero each time, and cancellation before recheck.
They assert one lookup, target identity, current hero, threshold, and exact events.
Mutation of either check's EDX, ECX, or threshold rejects recovery. Nine focused
distance tests passed (an initially requested nonexistent Theresa module was removed
from command; Theresa coverage resides with existing distance tests). Full suite:
648 tests in69.096s; only known Bully run-off2failures+2errors remain.

Generation now43/51 functions compiling,10/18 files,zero missing,1583TODOs.
PostAttackStuff syntax now passes, but cutscene/resources/control behavior is still
incomplete. In particular generated me:AcquireControl(4) is invalid in this quest
function; lift_native_lua.py StartScriptingEntity special case always assumes me,
ignores actual resource/target, defaults priority4. Next work should recover the
hero resource and cutscene map/control rather than treating syntax as parity.
Later movie camera flag, village limbo false and cutscene macro also unresolved.
Draft registration remains disabled; runtime and working-port files untouched.


## Movie flags recovered; control ABI investigation

Refactored native_post_attack_scalars into shared _recover_scalars with original
music/camera witness and new movie-flags witness after distance recovery. Verified
DBEE47 pause(true), DBEE54 fixcamera(true), DBEE8F fixcamera(false), native receivers,
vtable slots, words, manifestbool. Final DBEE9B pause(false) is not yet independently
covered (incoming EDI receiver needs provenance). New postAttackMovieFlagEvidence.
Two new tests execute flags with stale scalar0 and verify real Lua booleans, reject
each mutated native push before any replacement. Five focused tests pass; full650
tests in66.068s only known Bully2failures+2errors. Generation43/51functions,10/18files,
zero missing,1581TODOs. Drafts disabled; no runtime/port edits.

Control investigation establishes important limitation rather than assuming equivalence:
DBEDAD constructor7E72A0 at ESP+0x28; GetHeroDBEDBE; StartScriptingEntityDBEDC8.
Shared setup with GetHero profile0 decodes [hero result, stack40decimal, constant4]
and interface slot0x20. Runtime slot target89B5B0. It yields internally at89B684
while resource byte+18 busy, checks entity alive and global termination13D2838+5,
but can also returnfalse at89B6E8 (priority/failure). Caller ignores return and
continues map/cutscene. LuaEntityAPI.cpp:198 loops after every false with another
NewScriptFrame, stopping only IsThreadTerminating; also reuses/borrows handles.
Thus merely changing me to hero and using AcquireControl does not prove native
failure/scheduling parity. Native map ctorCDBF70 at DBEDCF, HERO string1255174,
operator[]CD3D2E at DBEDF2, resource assignment8ABD10 at DBEDF9. Movie resource
ctor6E7B60 DBEE0B at ESP+0x38; StartMovieSequenceDBEE2F has output resource and
empty string. RunCutsceneMacroCBFB7D DBEE7A receives stack words0,0,0,1 and ECX/EDX
string/map pointers; destructor6E7B80 DBEEA5, mapdestroyCDBFB0 DBEEAE, hero resource
7E74D0 DBEEB7. Full resource lifetime/cutscene lowering remains next substantial work.


## Native village limbo entry/exit recovered

Added native_post_attack_limbo.py/witness after movie flags. Caller/source plus
lookup8A7D60 size151 pinned; vtable120/6E0 and string12D9DF8 V_OakVale checked.
Native shared windows prove DBEC6E consumes DBEC64 lookup result and true;
DBEEE7 consumes DBEEDD fresh lookup result and false. Each lookup receiver/target
is verified before its result profile is used. Annotated calls now explicitly use
ppVar7,true and ppVar9,false, preserving surrounding constructors/cleanup.
New postAttackLimboEvidence contains both setup snapshots. Tests execute both
actual lookup/call sequences with distinct village results to catch accidental
reuse, and reject changed lookup-result/flag/source evidence atomically.

Four focused tests pass. Full suite658tests in65.694s, only known Bully2failures
and2errors. Generation43/51functions,10/18files,zero missing,1581TODOs aggregate.
Verified output SetVillageLimbo(ppVar7,true) and SetVillageLimbo(ppVar9,false).
Draft registration remains disabled. No runtime or working-port edits.

Resource/control/map/cutscene work still remains as documented above; this step
resolves missing exit flag but does not claim whole PostAttackStuff parity.


## Stop inventing implicit entity receivers in quest functions

Lifter.interface_call now checks execution_entity before StartScriptingEntity or
Entity-scope interface dispatch. A quest without an explicit execution entity gets
an unresolved receiver/resource TODO instead of undeclared me:AcquireControl or
another me method. Such calls are not counted in emitted API coverage. Assigned
results are declared nil with an unresolved-native-result marker, not fabricated
bool success. Entity threads and shared helpers with execution_entity=True retain
existing behavior; explicit thing_call path remains separate.

This removes the known phantom actor from PostAttackStuff while keeping control
ownership/scheduling incomplete and visible. It deliberately does not infer an
actor/resource relationship from a nearby GetHero temporary. Four tests cover
recent hero in quest, assigned entity predicate, shared entity-context helper, and
actual PostAttackStuff.84focused tests passed; full662tests in67.439s only known
Bully2failures+2errors. Generation43/51functions,10/18files,zero missing,1581TODOs.
Drafts remain disabled and runtime/working-port files untouched. Remaining work
is still native resource/control/map/cutscene lowering, not resolved by this guard.


## Verified post-attack resource graph in conversion report

Added native_post_attack_resources.py/witness and report postAttackResourceEvidence.
Status mapped, loweringStatus unresolved. Ten decoded events connect acquired
hero resource(windowoffset40decimal), actors map16, movie56, and native destructors.
GetHero891CA0 size173 and mapentryCD3D2E size118 (RET4 atCD3DA1) pinned for profiles.
Actor key HERO1255174 and macro12D9DC8 checked. Events are acquireDBEDC8,
mapctorDBEDCF,mapentryDBEDF2,assignDBEDF9,moviectorDBEE0B,startDBEE2F,macroDBEE7A,
moviedestroyDBEEA5,mapdestroyDBEEAE,actordestroyDBEEB7. Macro has flags/input null,
setupfalse,skippabletrue. Cross-window offsets still per-function reviewed equality.
Actor constructorDBEDAD not included in decoded event list because preceding
DadFound store is currently rejected by straight-line setup IR; do not silently
weaken memory-write rejection to include it.

Tests assert resource-to-map-to-macro links, cleanup receivers and flags; mutate
all10event targets, native/callee bytes and strings to ensure rejection.7focused
checks pass; full665tests in76.663s only knownBully2failures+2errors. Generation
unchanged43/51functions,10/18files,zero missing,1581TODOs. No Lua equivalence claim
or ownership TODO removal; this is checked input for future resource lowering.
Drafts disabled; runtime/working-port files untouched.

Read runtime RunCutsceneWithSetup1738..1980: pretermination check; for each actor
constructhandle, reuse existing FindControlHandle else retry priorityVERY_HIGH;
assign mapentry; reuses caller-owned movie by Lua-state handle, otherwise creates
movie/pause/camera; macro forwards flags,input,setup,skippable; cleanup actor handles
andmap afterwards. So existing outermovie avoids duplicate acquisition, but control
failure/ordering and cleanup lifetime differences still require treatment before
substituting whole native region. Current adapter returnvoid cannot expose all
native acquisition outcomes. Resource graph now makes these dependencies explicit.


## Call-setup memory-store invalidation and actor constructor

Shared native_call_setup_ir.py now supports explicit scalar MOV stores only when
argument_count is supplied. It clears all staged stack contents and recursively
invalidates memory-derived register facts, retaining independent addresses and
values. No value/alias inference for writes. Explicit requested args missing after
invalidation reject; fresh pushes can establish new args. Default unbounded ABI
mode still rejects writes. Unsupported/implicit memory effects still reject.
This recovers actor ctorDBEDAD ECX=stack40 across DadFound byte store; added it to
resource graph (now11events) and checked ctor/acquire/destructor share actor slot.

Three new tests cover actual constructor, nested memory provenance loss, byte/word/
dword argument invalidation and fresh pushes.18focused tests passed; full668tests
in64.434s only knownBully2failures+2errors. Generation43/51functions,10/18files,
zero missing,1581TODOs. Resource graph still mapped/unresolved, not Lua parity.

Next concrete lowering opportunity: movie resource constructor6E7B60 and destructor
6E7B80. Runtime LuaQuestState.cpp3583 StartMovieSequence allocates/constructs per-Lua
state movie, defaultemptyclass; EndMovieSequence3625 calls exact derived destructor
(MovieResource_Destroy_API) at3648 then frees/removes handle. Can consume verified
movie graph56 to replace exact native movie ctor/destructor with Start/End lifecycle,
without pretending to resolve hero resource/map/macro. Need verify native helpers,
source correspondence and cleanup order. Actor-control native failure mismatch
continues unresolved, but movie cleanup can make meaningful independent progress.


## Movie lifetime lowered from verified resource graph

Added native_post_attack_movie_lifetime.py/witness after limbo recovery. Consumes
mapped resource graph and requires movie ctor/start/destroy allslot56decimal,
ordered ctor<start<destroy<mapdestroy<actordestroy. Pins constructor6E7B60 size26
and derived destructor6E7B80 size66. Runtime bindings verified: baseconstruct99A380,
vtable1260EF4, EndMovieSequence invokes MovieResource_Destroy_API6E7B80 beforefree.
Checks emptyclass122D70E NUL and Start/End manifests. Removes exact decompiler movie
ctor atstack0xffffff9c; replaces exact movie destructor atstack0xffffff74 with
GSI->EndMovieSequence. Actor ctor/destructor/map remain unresolved and untouched.

Report postAttackMovieLifetimeEvidence recovered movie only; general graph remains
mapped/unresolved. Generated EndMovieSequence now immediately precedes mapdestroy
TODO. Tests check actual source cleanup order, one emitted start/end pair inLua,
preservedactor ctor, and reject changed helpers/source/wrongresource/reorderedcleanup.
6focused tests pass; full671tests in73.458s onlyknownBully2failures+2errors.
Generation43/51functions,10/18files,zero missing,1580TODOs. Drafts disabled; runtime
and working-port files not edited. This is movie lifecycle recovery, not whole
cutscene parity; actor acquisition/map/macro lowering remains incomplete.


## Remaining syntax survey and shared inlined GetPos evidence

Reviewed all8current syntax failures: AffairMan distance has stack-address threshold;
AffairWife/AffairWoman/BarrelMan fail on inlined implementation GetPos slot18; BookTrader
rawparam_1+4 interface load; Bully resource slot6C; Guard reference-count goto condition;
questInit vector bookkeeping. Three failing owners therefore share position recovery.
Created work/converter_position_readiness.json with6inline fallback patterns across
4functions, raw source fragments, implementation locals and preceding string hints.
Hints are explicitly NOT actor proof. Includes full18-byte native CScriptThing::GetPos
4AA980 instruction/hash evidence: wrapper+4 implementation, null returns143E8E0,
otherwise tailcalls implementation slot18. This is candidate-pattern evidence, not
Lua lowering and not a reason to replace arbitrary pointer calls.

Important runtime mismatch: LuaEntityAPI.cpp540 GetPos returns emptytable for missing
wrapper/impl (lines544,547), otherwise x/y/z from implementation. Native fallback
vector143E8E0 cannot be read by RData.bytes_at (None). Do not assume zero without
proving fallback initialization/writes. Needed next: connect hidden lookup wrapper
outputs to impl locals from assembly, normalize position fallback and copied vector
channels, then distance/movement consumers. Barrel source explicitly passes hidden
outputaCStack_15c and implpiStack_158, useful starting relationship but still verify
native stack layout. Wife lookupNOVI_AffairMan, WomanAffairWomanRunOffPoint,
BarrelM_BarrelManWalkOff. Three common consumers can share recovery once proved.

No converter/runtime/port code changes this turn; new evidence changes the next
implementation target. Last verified coverage43/51functions,10/18files,1580TODOs,
last full671tests onlyknownBully2failures+2errors. Drafts remain disabled.


## Literal reader raw-section bounds fixed

RData.string_at previously used max(virtualsize,rawsize), treating virtualpadding
as file offsets and allowing terminators beyond rawsectionend. Now uses raw_secs
and limits search by section rawend,fileend,512byte cap. Existing printableASCII
and emptystring semantics preserved. Six tests cover fake literal invirtualpadding,
terminatoroutside/rawend, secondsectionproperRVA, truncatedfile/nonprintable/empty,
scanlimit, and actualretailfallback location.86focused checks passed; full677tests
in79.015s onlyknownBully2failures+2errors. Generation43/51functions,10/18files,
zero missing,1580TODOs. Drafts disabled; runtime/working-port files untouched.

Fallback143E8E0 is RVA103E8E0, within data RVA F74000 virtualsizeCA9A4 but outside
its rawsize44000. Bytes_atNone is genuine non-file-backeddata, not rdata-onlyreader
limitation. Does not prove runtime vectorzero: need initializer/write-reference
analysis. Existing scriptvm_decomp.c references shown are reads, no global write
proof obtained. Common position-inline recovery remains next after fallback and
hidden-wrapper/implementation alias proof. This bounds fix prevents fictional
literal evidence while that investigation proceeds.


## Fallback references and barrel wrapper layout evidence

New work/converter_position_fallback_references.json audits raw literal references
to143E8E0/E4/E8 using aligned disassembly from coverage.tsv function starts.290byte
hits:6directreads,141addressmaterializations,143unresolved. No resolveddirectwrites,
but unresolved/discontiguouscoverage and computed/escapedaddresses prevent claiming
immutability. These counts are evidence limits, not a zero-vector proof.

New work/converter_barrel_position_layout.json records three position regions from
aligned whole6128byte BarrelMan functionDB5330. First marker lookupDB55E2 reads
M_BarrelManWalkOff12D91F4, outputwrapper at baselineESP+AC (172decimal), then
implementation loadedESP+B0 (176decimal) atDB55F4, exactly wrapper+4; position
implementation slot18 callDB5608. This is a verified first actor relationship.
Second preceding alternate lookupDB57F8 outputs atbaselineDC but position loadsD4
atDB580A. Thus nearest-string heuristic would associate the wrong wrapper. Third
position DB5926 loads implESP+28; preceding alternate-marker string is notproof.
Need trace these cached wrappers separately. These findings change recovery design:
only first lookup can be tied directly; generalized proximity matching is invalid.
Fallback global contents still unresolved, so no position rewrite introduced.

No converter/runtime/port edits or new tests this research turn. Prior validated
43/51functions,10/18files,1580TODOs;677tests onlyknownBully2failures+2errors. Drafts
stay disabled. This is progress in native operand proof, not a completion claim.


## Checked jump-table support unblocks BarrelMan argument analysis

Attempting shared lookup recovery returnedNone despite valid first wrapper layout.
Cause was the blanket rejection of any indirect jump in read_call_window, not the
known truncated epilogue. Barrel dispatch DB551D cmp eax,3; DB5520 jaDB5DEE;
DB5526 jmp[eax*4+DB6B24] has4bounded cases. Added generic recognition of contiguous
CMP32bitregister,nonnegative max<1024 / JAimmediate / JMP[index*4+absolute table].
Reads full table, requires all targets at decoded instruction boundaries and table
outside decodedcode; rejects any direct/table edge into JA/JMP bypassing guard.
All table targets trim windows just like direct branch targets. Other indirect
branches still reject. This does not broadly ignore indirect control flow.

Actual firstlookupDB55E2 now yields wrapperstack172 and stringstack160 using shared
analysis. Four new tests check bypassed setup, bypassed bound, signed condition,
missing/out-of-range/misalignedtargets and realBarrel.19focused tests pass; full681
tests in67.030s onlyknownBully2failures+2errors. Generationunchanged43/51functions,
10/18files,zero missing,1580TODOs. Drafts disabled; runtime/port untouched.

Next: use now-working lookup setup as part of inlined-position recovery. First
wrapperAC/implB0 relationship remains valid; laterD4/28 impls need cachedwrapper
tracing. Fallback contents still unresolved. Barrel nominal6128endsDB6B20 inside
ADD ESP; accepted standardepilogue endsDB6B23, table beginsDB6B24. Existing narrow
ADDESP/RETtail extension handles this correctly. No separate position mapping
rule was added yet; shared jump-table support was prerequisite discovered here.


## Position ownership mapper and overlapping-branch rejection

Call-window analysis now rejects direct branch targets inside decoded instructions,
including extendedepilogue, rather than ignoring targets missing from positions.
This closes guard-bypass/overlappinginstructionstream gap. New test enters CMP,
JA,JMP andargumentsetup interiors. Added native_barrel_position.py/witness and
report barrelPositionEvidence for firstlookup only. Checks caller/source, lookup
8A7D60size151,stringcleanup99EAE0size12,GetPos4AA980size18, string andvtablebinding;
sharedlookupsetup mustbe wrapper172,string160,interfaceESI+4,slot120. Reviewed
impl176=wrapper+4 remains tied to that exactbody. statusmapped/loweringunresolved,
fallback143E8E0 andvectorconsumers explicitlyunresolved; laterqueriesnotincluded.

22focusedtests pass; full684tests in65.531s onlyknownBully2failures+2errors.
Generation43/51functions,10/18files,zero missing,1580TODOs. Verified report contains
firstmarkerM_BarrelManWalkOff withslots172/176. Drafts disabled; no runtime/port edits.
Nextstillneeds fallbackcontents plusnativevectorcopy/distanceconsumer conversion;
mapper now provides checkedfirstactor instead of relying on nearby strings.

## Reviewed position-distance binding

Added converter-only signature for Entity:IsDistanceFromPositionOver(position,
distance), verified against LuaManager.cpp:90 and LuaEntityAPI.cpp:562 in the
retail-shadow runtime. The wrapper forwards to native 0x00CBE45C; native actor
liveness, squared threshold, and comparison behavior remain in that helper.
Direct IsDistanceFromThingToPositionOver lowering requires a known thing receiver,
known vector, and numeric threshold. Quest context cannot invent me. Unknown
stack vectors and raw hexadecimal threshold bits remain unresolved; assigned
unresolved results explicitly become nil, with no converted-call credit.

Four new tests cover forwarding a signed threshold and vector, nil receiver,
discarded result syntax, unproven operands, and signature overlay preservation.
Full suite: 688 tests in 67.406s, only known Bully 2 failures + 2 errors.
Regeneration: 43/51 functions and 10/18 files compiling, zero missing bodies,
1591 TODOs (previously 1580; explicit unresolved result propagation exposes more
missing value evidence). No actual Barrel stack-vector consumers newly lowered.
Drafts remain disabled; runtime and reconstructed ports untouched.

Next: establish vector copies and cached actor ownership at Barrel distance sites
DB5647/DB56F2/DB5965/DB5A12/DB622B/DB6301. First threshold is 2.0. Existing
first-position mapper proves marker wrapper but fallback 0x143E8E0 contents and
later cached wrappers remain unresolved. Do not replace GetPos fallback with zero
or infer actor identity from nearest marker string.

## Barrel position consumers mapped

Extended first-position evidence with reviewed XYZ copy offsets and checked repeat
consumer setup. Native DB560B..DB5639 copies EAX vector channels 0/4/8 to baseline
ESP+B8/BC/C0: apparent destinations B8/C0/C8 account for intervening pushes.
First DB5647 distance uses that vector; repeat DB56F2 independently decodes to
EDX stack184 and threshold bits40000000 (2.0). Actor comes from resource
GetScriptThing DB56E4, resource stack20 and hidden output stack232. Profiled
GetScriptThing7E7490 full57bytes (both RET4 paths) and distanceCBE45C91bytes.
Shared call-window checks require precise resource/output/result/vector/threshold
relationships. Full caller hash pins the reviewed first-copy sequence.

27 focused tests pass, including five independently perturbed consumer facts.
Regeneration unchanged43/51 functions,10/18files,zero missing,1591TODOs. Last full
suite remains previous688tests with knownBully2failures+2errors; not rerun for this
evidence-only addition. No new Lua lowering and no runtime/working-port changes.
Next needs actor resource ownership plus fallback handling before emitting these
queries; later position regions still require separate cached-wrapper tracing.

## First Barrel controlled-actor queries recovered

Native initial acquisition DB5577 and retry DB55A4 both pass EDI, resource
baselineESP+14, priority4. EDI is initialized to entity this+8 at DB533C and is
unchanged except self-MOV until epilogue POPs. Full caller bytes pin this reviewed
callee-saved register relationship. Added shared argument checks for both calls,
vtable20 binding89B5B0 and full454byte callee hash. Success/retry/termination edges
lead into the first position section with that same resource.

Added annotated-source-hash-gated recovery of first two pCVar7 GetScriptThing
queries to me. Other resource queries are untouched. Native wrapper cleanup stays
unresolved; position fallback and vector consumers still prevent complete lowering.
Tests reject changed resource/actor operands and annotated input. Eight focused
tests pass. Regeneration43/51functions,10/18files,zero missing,1589TODOs (two fewer).
Generated drafts remain disabled; runtime and reconstructed ports untouched.
Full suite completed: 691 tests in 93.429s; only known Bully 2 failures + 2 errors.

## Guard refcount exit recovered

Fallback investigation found existing Affair Wife/Woman constant audits check the
port's ZERO_POSITION declaration, not native global initialization or immutability.
They are not evidence for substituting zero at143E8E0. Fallback remains open.

Switched to Guard's remaining syntax failure: an inline pointer decrement/comma
expression guarding gotoDADD6C. Aligned native DADD47..DADD8F proves all outcomes
of this branch return: null/nonzero reference count skips callback, zero invokes
callback and delete, then common resource clear/destruction/epilogue. No outcome
falls through into the following hero conversation. Added native_guard_cleanup
with full caller, rawsource and annotatedsource hashes, exact fragment matching,
and report guardCleanupEvidence. Recovers the unconditional function exit only.
Explicit TODO retains unresolved native ownership callback and control-resource
destruction; not a claim of GC/control-release parity.

Two focused tests verify actual Guard Lua5.4 compilation and rejection of changed
source/bytes. Regenerated44/51functions and11/18files compiling,zero missing,
1588TODOs. Drafts disabled; runtime and working port untouched.
Full suite completed: 694 tests in 97.068s, only known Bully 2 failures + 2 errors.

## AffairMan cached wife distance recovery

Extended shared native_teddy_distance recovery to select a reviewed lookup
occurrence and accept different source expressions for multiple checks of the
same native cached wrapper. Existing witnesses retain first-lookup defaults.
New AffairMan witness pins caller/source/annotated hashes plus lookup/distance
callees. DB0B07 is second lookup NOVI_AffairWife at baselineESP+2C; first DB0AD9
is NOVI_AffairWoman at20. DB0EE7 and DB162D both use cached2C and float5.0 despite
decompiler naming the same storage stack0xfffffeb8 and aCStack_130. EBP=this+8 is
reviewed from entryDB09EC through unchanged callee-saved register to epilogue.
Each check requires ECX=EBP and shared call-window proves EDX/threshold.

Generated both checks as quest:IsDistanceBetweenThingsUnder(me,r2,5.0), preserving
one wife lookup. Six focused tests pass, including Lua trace true/false results,
one cached lookup, unchanged firstlookup and rejection of changed receiver/slot.
Generation44/51functions,11/18files,zero missing,1586TODOs. AffairMan still fails
syntax later in a short-circuit conditional goto containing rand and a comma
assignment distance check (generated line476). This is next useful shared control
expression target; current change does not claim full AffairMan conversion.
Runtime/port untouched; generated drafts disabled.
Full suite completed: 696 tests in 98.557s, only known Bully 2 failures + 2 errors.

## Shared nested conditional-goto sequences

Added native_sequence_conditions expansion after statement splitting and before
jump/label planning. Nested &&/|| retain evaluation order; comma-prefix local
assignments execute only inside the applicable guard. Generated boolean condition
temporaries are collision-checked and excluded from native operand staging so
later inferred API arguments cannot consume converter bookkeeping. Pointer stores,
ternary expressions and unsupported effects stay untouched. Numeric simple if
leaves now compare against0, avoiding Lua's truthy numericzero.

Three new tests exercise nested path matrix, ordered reads, retained values when
assignments are skipped, numericzero, and unsupported syntax. Actual AffairMan
nested rand/distance goto now expands into structured short-circuit blocks.
Remaining random binding, cached woman distance operand and other resource
operands remain unresolved; no full behavioral or compilation claim.

Full suite699tests in83.545s retained onlyknownBully2failures+2errors. Final change
excluding generated controltemps from operand staging was checked afterward with
93 focused lifter/condition tests, allpass. Regenerated44/51functions,11/18files,
zero missing,1587TODOs. Drafts disabled; runtime and working ports untouched.
Next: recover AffairMan's woman cached wrapper for DB1841 two-unit check, and
native rand atDB1821 using existing RetailRandModulo host binding and exact native
random evidence. Full AffairMan currently fails earlier at another resource stack
operand, independently of the now-expanded short-circuit branch.

## AffairMan cached woman and conditional random gate

Added second shared distance witness after wife recovery: DB0AD9 firstlookup
NOVI_AffairWoman writes baselineESP+20; DB1841 takes that wrapper viaESP+24 after
pushing2.0. ReceiverEBP remains self. Both wife and woman lookups now have explicit
separate cached outputs; woman check stays inside existing short-circuit gate.

Added AffairMan conditional random witness to shared native_random. DB1821 CRT
rand then CDQ/IDIV100 tests remainder. Nonzero successorDB1C71 overwrites EAX and
calls NewScriptFrame; zero successor stages entity/vector before distanceDB1841.
Neither uses rawrand afterward, so replace only that expression with native
RetailRandModulo100 and compare remainder!=0. Other two rand sites untouched.
Rawcaller, annotatedsource(after both distance recoveries), nativebody hashes and
all original rand call targets remain checked.

Nine focused tests pass. Actual recovered gate trace covers conversation short
circuit, first-comment bypass of rand, nonzero remainder bypass of distance, and
both distance outcomes. Generated gate uses r1/woman2.0; wife checks remainr2/5.0.
Generation44/51functions,11/18files,zero missing,1586TODOs. Remaining AffairMan
resource operands and two random gates still unresolved. Drafts disabled; no
runtime or reconstructed-port edits.
Full suite completed: 700 tests in 80.404s, only known Bully 2 failures + 2 errors.

## All AffairMan random gates recovered

Extended reviewed native_random witness edits atomically to three AffairMan sites.
DB165C uses signedremainder50; nonzero goes to frameDB1C71 and zero to termination
DB1673, discarding rawrand. DB1BAA uses signedremainder2 implemented by AND80000001,
JNS,DEC,OR,INC and branch flagsDB1BBD. For native CRT rand's nonnegative result,
uVar14 masked value is exactly remainder0/1. Replaced complete decompiler mask/sign
sequence with RetailRandModulo2 plus bVar4 zero comparison. Existing100gate stays
inside its short-circuit condition. Source/native hashes and all3rand calltargets
remain required; edits validate before any replacement.

Seven focused random/distance tests pass. Added exhaustive remainder0..49 and0..1
branch tests, exactly one native RNG call per gate, and changedmodulus rejection.
Generated AffairMan has RetailRandModulo50/100/2 and no rawrand() calls. Generation
unchanged44/51functions,11/18files,zero missing,1586TODOs. Resource operands still
block complete AffairMan conversion. Drafts disabled; runtime/port untouched.
Full suite completed: 701 tests in 71.874s, only known Bully 2 failures + 2 errors.

## AffairMan cached IsAlive queries; function now compiles

Added native_cached_thing_queries using full source/annotated/native caller hashes
and pinned IsAlive4AB13028bytes. Checked DB1597 ECXstack20 matches woman lookup
DB0AD9 hiddenoutput20; DB15BB ECXstack2C matches wife lookupDB0B07 output2C. Requires
both named cached lookup definitions already recovered before editing queries.
Applies after distance/random recoveries; leaves subsequent facing selection and
movie cleanup untouched. Source-shaped pointer queries become nil-safe r1/r2
IsAlive through existing thing emitter.

Two focused tests cover live/dead/missing receivers, priority of woman over wife,
no secondquery when first succeeds, and fail-closed changed receiver/source.
Generation now45/51functions and12/18files compiling,zero missing,1586TODOs.
This is a syntax milestone only; AffairMan still has many unresolved behaviors.
Generated drafts disabled; runtime/reconstructed port untouched.

Next nearby issue: puVar11=&stack0xfffffea8/woman or &stack0xfffffeb4/wife converges
atDB15D9 to EntitySetFacingAngleTowardsThing(selected,self,false). Generated call
currently misplaces receiver and misses selected actor. Need preserve branch value
across join, not a static alias overwritten by whichever branch is emitted last.
Full suite completed: 704 tests in 77.682s, only known Bully 2 failures + 2 errors.

## AffairMan facing selection recovered

Added native_affair_facing after cached liveness recovery. Native DB15A9/DB15CD
pushfalse, womanDB15B1 takes baseline20, wifeDB15D5 takes baseline2C; commonDB15DE
pushes selfEBP then selectedEAX into slot76C atDB15E0. Pinned caller/source/annotated
hashes, facingcallee88E620193bytes and vtable76C binding. Runtime optional snap
parameter defaultsfalse (LuaQuestState.cpp2361), so two-argument Lua call is exact.

Restructured the reviewed source join: explicit facing call on each incoming
selected-actor edge; wife's successful edge jumps forward to new source label at
native common cleanupDB15E6. This avoids an illegal goto into woman's sibling Lua
scope while bypassing the wife's termination-only teardown. Original cleanup
statements remain intact and unresolved; no destructor parity claim.

Two tests run the actual recovered branch with selected actors, missing actors,
termination on each live-actor path, and correct success/termination exit labels.
Changed source rejects restructuring. Generation45/51functions,12/18files,zero
missing,1583TODOs. Drafts disabled; runtime and working ports untouched.
Full suite completed: 706 tests in 72.859s, only known Bully 2 failures + 2 errors.

## AffairMan saved-interface unpause recovered

Five raw (*ppuStack_140+5EC)(0) calls now recover as PauseAllNonScriptedEntities(false).
Native DB0E4F loads interface fromESI+4, DB0E54 push1 then DB0E58 savesEDI atESP+3C
=baseline38. NormalDB15EE and cancellationsDB1D12/DB1D2B shared call-window setup
proves receiver memory(stack56), vtable5EC and stackfalse. DB1D42/DB1D59 have flag
pushed before conditional edges atDB15A9/DB15CD; these incoming relationships are
reviewed under full caller hash rather than falsely treated as straight-line.
Pinned target890AB0size80, vtable5EC, raw/annotatedsource and caller, bool host API.

Three focused tests pass, including actual facing-region normal/termination paths
with exactly one unpause each and changedreceiver/flag rejection. All original
movie destructor calls remain unchanged/unresolved. Generation45/51functions,
12/18files,zero missing,1578TODOs. Drafts disabled; runtime/port untouched.
Next movie lifetime: AffairMan has starts/constructors for resources atbaseline48
and other scopes, with12 destructor sites. Need map each start/end pair and preserve
termination exits before replacing native destructor with EndMovieSequence.
Full suite completed: 707 tests in 70.368s, only known Bully 2 failures + 2 errors.

## AffairMan movie lifetimes recovered

Added shared native_resource_lifetime checker for aligned native CFGs: at most one
active resource, each end matches it, no active resource at any return, every
reviewed event reachable; reject unknown branches, interior/outside targets,
unmatched ends, nested starts, traps and falloff. Actual AffairMan14events (2starts,
12ends) pass across1444 reachable instruction/state pairs. Complete extent5058
includes final ADDESP140/RET; nominal5052 cuts that epilogue.

native_affair_movies pins source after facing/unpause, complete caller, ctor6E7B60,
dtor6E7B80, base99A380, start89B110 and vtable5C8 plus emptyclass122D70E. Every
start/end call setup checked by shared window analysis. Firstmovie stack96 and
secondmovie72 have no overlapping lifetime, so existing single host movie handle
is sufficient. Replace 5 ppuStack_fc and7 aCStack_114 native destructor expressions
with EndMovieSequence; remove second explicit ctor staging. Unrelated misnamed
MovieBase destructor on stack0xfffffeb4 remains untouched.

Four focused tests pass: lifetime invalid paths and actual facing/termination
branch trace now require exactly unpause then movie_end. Generation45/51functions,
12/18files,zero missing,1577TODOs. Drafts disabled; runtime and port untouched.
This covers movie resource ends, not actor-control resource lifetime or full
AffairMan behavior. Other entities still need position fallback/vector recovery.
Full suite completed: 709 tests in 74.611s, only known Bully 2 failures + 2 errors.

## Acquisition boolean typing and retry-loop preservation

Inspection of all six AffairMan native slot20 calls shows actorEBP=self, resource
baseline16, priority4 for initial/retry pairsDB0A76/DB0AA4,DB0CB5/DB0CE4,DB0E85/DB0EB4.
Native resets and final resource destruction still need host-lifetime treatment;
no ReleaseControl substitution made. Current runtime also reuses same-priority
handles, so native resource reset cannot simply be inferred from actor identity.

Fixed generic StartScriptingEntity result typing: emitted AcquireControl returns
Lua bool and now records bool kind, producing while not result instead of bool==0.
Restricted collapse_acquire_loops to exactly one frame and retry, with result dead
afterward. Earlier broad pattern could discard arbitrary loop effects or remove a
result used later. Explicit termination checks/other effects remain in place.
82 focused tests pass, including false-then-true retry with frame/timer side effect
and retained post-loop result. No native scheduling parity claim beyond preserving
these emitted operations; runtime owns the blocking acquire implementation.
Full suite711tests in74.713s retains knownBully2failures+2errors only. Generation45/51functions,12/18files,zero missing,1577TODOs. Drafts disabled; runtime/working port untouched.

## BookTrader saved-interface pause recovery

Extended shared pause recovery with reviewed per-site setup snapshots and atomic
source edits. BookTrader DB45F6 loads EBX=interfaceESI+4, push1 then DB45FF stores
EBX atESP+2C=baseline28, DB4603 pausestrue. Shared setup conservatively rejects
that store-after-push window; trueflag is separately reviewed under full caller
hash, not inferred from source uVar9. DB46E0 EBXfalse and DB4BCD/DB4CD7 saved28false
have checked call snapshots. Native5EC binding and80bytecallee pinned.

Removed only four exact interface/vtable staging assignments (ppuVar2, savedcopy,
two puVar15 loads), replaced four raw calls with true/false/false/false API calls.
Other ppuStack_124 resource uses remain. Five focused tests pass including prior
Affair recovery, BookTrader staging preservation and changed nativefalseflag
rejection. Generation45/51functions,12/18files,zero missing,1573TODOs. BookTrader
still fails later at condition assignment (functionline395); initial invalid
interface-pointer assignment is gone. Drafts disabled; runtime/port untouched.
Full suite completed: 713 tests in 72.479s, only known Bully 2 failures + 2 errors.

## Ordinary if comma sequences; BookTrader now compiles

Extended native_sequence_conditions from conditionalgoto to ordinary if blocks,
renamed entrypoint expand_sequence_conditions. Uses the same short-circuit tree
and guarded assignments, preserving result values and original else/body scope.
Existing dedicated conditional calls and repeated termination predicates retain
their original lowering (full suite caught the latter missing exemption; fixed,
94 focused tests then pass). New test covers if/else nested path matrix with
skipped assignments, zero results, branch body effects and surviving result.

Regeneration46/51functions and13/18files compiling,zero missing,1572TODOs.
BookTrader's iVar11=rand() inside timerzero&&randommod200 now emits syntactically
valid guarded assignment, but rawrand binding and timer operand are stillunresolved.
Do not claim behavioral readiness from the syntax milestone. Drafts disabled;
runtime/reconstructed port untouched.

## Wife hero-distance thresholds checkpoint

Added native_hero_distance.py after route and cached-husband recovery. Native
DB32F1/DB35DA GetHero calls consume no arguments; the preceding pushed floats
belong to distance calls DB32FB/DB35E4: 5.0 and 15.0 respectively. The second
float had been attached to GetHero as 0x41700000 and distance used stale fVar33.
Recovery pins raw/stage source, caller/callee/vtable bytes and decoded setups.
Report field affairWifeHeroDistanceEvidence records recovery. Both getters keep
their original placement and distinct evaluations. Focused tests verify Lua
actor/threshold traces and atomic rejection for altered threshold/cached actor.

Generation: 47/51 functions, 14/18 files, zero missing bodies, 1381 TODOs.
Six focused Wife tests pass. Full suite: 758 tests in 128.599s, two failures and
two errors in the known Bully termination camera cleanup tests. Drafts disabled.
Hero-distance changes do not resolve Wife's position/movement syntax failure.

Next recovery evidence: Wife hit classification DB2C35 self slot54, true jumps
DB2C99(result1); otherwise DB2C5C self slotA8, false jumps DB2C92(result0).
Otherwise DB2C88 self slotA4 has ability14 pushed DB2C80 plus HERO string; false
jumps result1, true result0. Same boolean structure as BookTrader, but result
cStack_9d and temporary masks 8/18/38. Preserve query order, string lifetimes
and subsequent mask cleanup. Source sibling goto LAB_00db2c99 remains to recover.

Full suite completed:756 tests in93.452s; only known Bully2failures+2errors.
Next: verify BookTrader's native rand200 call and recover it with shared CRT modulo
binding after its saved-interface pause recovery (or deliberately update stage
hashes if inserting earlier). Remaining syntaxfailures: AffairWife,AffairWoman,
BarrelMan,Bully,questInit; position/vector and resource operands remain substantial.
Final full suite: 714 tests in 80.033s; only known Bully 2 failures + 2 errors. The new termination-lowering regression is resolved.

## BookTrader retail random gate recovered

Added fold_book_trader_random after saved-interface pause recovery, sharing
fold_retail_random's reviewed witness machinery. DB4CFF/DB4D01 skip RNG for nonzero
timer; DB4D07 calls retailCRT BFEB16 then signed IDIV200 and tests remainder. Zero
path overwrites return registers through GetHero/distance; nonzero DB4E64 loads
interfacevtable/NewScriptFrame, so rawrandom is dead on both successors. Replace
only (iVar11=rand(),iVar11%200==0) with native RetailRandModulo200 comparison inside
the original short-circuit condition. All native/source/annotated hashes and rand
call targets remain checked. Earlier general random pass leaves BookTrader alone;
explicit later pass avoids changing its pause witness input.

Eight focused tests pass including all200remainder outcomes, exactlyone RNGcall
for timerzero, none for nonzero, and changedmodulus rejection. Generation remains
46/51functions,13/18files,zero missing,1572TODOs. BookTrader timer operand and other
behaviors still unresolved; compiledbody is not retail-ready. Drafts disabled;
runtime and reconstructed port untouched. Next useful target: actual timerID and
reset value around DB4CF9 timer read and conditional speech/reset path.
Full suite completed: 715 tests in 82.233s, only known Bully 2 failures + 2 errors.

## BookTrader shared timer operands recovered

Added native_book_trader_timer after the random pass. Native DB4CF9 reads the
timer ID at parent quest +0x104; DB4D60 resets that same ID to 3. The parent
inventory maps it to TalkIntermittentTimer (Int). Source, annotated-stage and
caller hashes, both decoded call setups, GSI receiver/vtable slots and API
contracts gate atomic replacements. Generated Lua now reads the named parent
state for both GetTimer and SetTimer rather than passing zero.

Shared call-setup IR now models 32-bit immediate ADD/SUB on non-ESP registers,
normalizes nested address offsets, wraps constants and preserves unknowns.
This recovers the read's ADD EAX,0x104 followed by MOV EAX,[EAX], matching the
reset's direct parent-field load. Tests cover pointer arithmetic, subtraction,
constant overflow/underflow and partial-register invalidation. Timer tests run
generated Lua with a nonzero timer ID and verify read/reset ID plus delay 3;
changed source or native operands reject both edits.

Full suite: 720 tests in 86.342s, only the known Bully 2 failures + 2 errors
around missing FixMovieSequenceCamera on run1 termination. Regeneration remains
46/51 functions, 13/18 files compiling, zero missing bodies, 1572 TODOs. Benchmark
also ran; its broader corpus still contains unresolved syntax. Disabled quests
registry verified. No runtime/reconstructed port edits made.

Next BookTrader targets: intermittent conversation AddLineToConversation still
has an unresolved argument, actor resource cleanup remains unknown, and other
branches retain TODO gotos. Timer recovery alone does not establish gameplay
parity. Remaining five NewOakvale syntax failures and position fallback evidence
remain as described above.

## BookTrader intermittent conversation line recovered

Added native_book_trader_line after timer recovery. Native DB4D70 creates the
conversation and DB4D7B saves its ID in EBX, which is not overwritten before the
line. EBP is self (LEA EBP,[ESI+8] at DB3FAC, no subsequent writes). DB4E38 constructs
TEXT_QST_048_TRADER_ROLL_UP at stack112; DB4E42 gets hero; DB4E55 passes ID, text,
false, self, hero to slot5B8/890750. Read-only inspection of LuaQuestState.cpp3744
and GameInterface.h1162 confirms host ordering is ID, text, speaker, listener,
flag. The previous draft incorrectly used hero as speaker and omitted listener.

New source uses a named native_arg_book_listener getter and explicit host-order
arguments. Full caller/source/stage hashes, decoded setup with known GetHero,
vtable target and manifest contract gate the rewrite. Nine focused BookTrader
tests pass; Lua trace checks one getter then exact conversation ID/key/participants
and false flag. Changed speaker and contract reject recovery. Generated full
BookTrader body inspected at line448: correct explicit participants. Generation
46/51 functions,13/18 files,zero missing bodies,1571 TODOs; drafts stay disabled.

Next useful BookTrader work: seven GetHealth calls still lack actors, ally calls
also have missing actors, and termination/cleanup gotos remain unresolved. This
line fix does not establish complete BookTrader behavior.

Full suite completed: 725 tests in 81.517s; only the known Bully 2 failures and
2 errors concerning run1 termination camera cleanup. No new regression found.

## BookTrader seven health actors recovered

Added native_book_trader_health after the line pass. All eight native acquisition
calls use EBP=self and resource stack20 (initial priority3, later priority4).
Seven getters at DB4426,DB463A,DB471F,DB47E2,DB49A3,DB4B2D,DB4BF1 read that resource;
their hidden output wrappers at stack240,276,252,216,228,180,192 feed GetHealth
immediately. Native 7E7490 getter bytes and interface bindings are pinned with
the caller. Shared call-window checks every acquisition, getter and consumer;
source/stage hashes and host contract gate all seven edits atomically.

Generated calls now GetHealth(me); the prior wrapper scaffolding and cleanup
remain unresolved. Eleven focused BookTrader tests pass, including Lua actor and
health-return checks and changed getter/consumer rejection. Regeneration:
46/51 functions,13/18 files compiling,zero missing bodies,1564 TODOs. Inspected
all seven generated calls and verified empty disabled quests registry.
Next targets remain ally operands, cleanup control flow and resource lifetime.

Full suite: 727 tests in90.350s; only known Bully 2 failures+2 errors.
Next ally evidence already decoded: DB4336 passes (EBP=self, GetHero result from
DB432B); DB434C passes (GetHero result from DB4342, EBP=self). Both slot95C.
Shared read_call_window with known getter (cleanup0) and argument_count2 recovers
both, including self pushed before the second getter. Source calls currently
EntitySetThingAsAllyOfThing() and EntitySetThingAsAllyOfThing(uVar9). Recovery not
implemented yet; preserve both getter evaluations and opposite argument orders.

## BookTrader directed ally operands recovered

Implemented native_book_trader_allies after health recovery using the two sites
above. Source/stage/caller hashes, slot95C target8A7EC0, host contract and both
decoded call setups gate the replacement. First getter now has explicit local
native_arg_book_ally; second remains assigned to uVar9. Removed the erroneous
self argument attached to the second GetHero by the decompiler. Read-only host
LuaQuestState.cpp8966 confirms actor order is unchanged by the wrapper.

Thirteen focused BookTrader tests pass. New Lua trace uses distinct values for
successive getters to verify two evaluations, directed actor order and retained
uVar9 result; changed native argument direction rejects both edits. Regeneration
46/51 functions,13/18 files,zero missing bodies,1562 TODOs. Inspected full generated
ally calls. Remaining major issue is cleanup gotos into nested source scopes:
LAB_DB46DA unpauses then jumps DB4F4D; DB4F39 loads saved vtable then DB4F45
unpauses; DB4F4D selects movie resource before DB4F51 destructor. These likely
need movie lifetime evidence plus structured cleanup recovery, similar to AffairMan.

Full suite completed:729 tests in85.084s; only known Bully 2 failures+2 errors.
Disabled registry verified; runtime/reconstructed port files untouched.

## BookTrader shared movie destructors recovered

Extended check_single_resource_lifetime with optional ECX receiver selections.
CFG state carries the selected resource separately from the active lifetime;
other ECX/full or partial writes and calls clear it. Caller-proven LEA sites
select resource identities; ('register','ecx') end events resolve per path.
Synthetic tests cover two resources converging on one end, wrong/missing
selection, partial/full clobbers and calls.

BookTrader full native extent is4042bytes, RET DB4F69 (nominal4039 cuts ADD ESP).
Starts DB43F4 stack152 and DB45E7 stack44. Ends DB4CE1 and DB4F51 use selected ECX.
Selections: DB4510/DB4F1B/DB4F30 ->152; DB4CDD/DB4F4D ->44. These LEAs use baseline
ESP. Full native CFG validates both lifetimes; changing terminal selection DB4F30
to44 rejects. Shared movie recovery now has a BookTrader wrapper/witness after
the ally pass. Hashes pin caller/callees/source/stage, checked call setups and
native binding. Remove two CCarriedReadable constructors; replace exactly two
movie destructors(pCVar13) with EndMovieSequence. Misnamed stackfed0 destructors
actually call7E74D0 actor cleanup and remain untouched.

Eight focused lifetime/BookTrader/AffairMan tests pass. Regeneration46/51functions,
13/18files,zero missing,1560TODOs. Full generated start/end calls inspected,
disabled registry verified. Cleanup gotos into nested scopes still unresolved:
this change recovers movie events, not all paths reaching them. Next step is
source control-flow recovery for the shared terminal cleanup regions, preserving
unpause/movie-end/actor-release order and normal continuation at DB4CE6.

Full suite completed:733 tests in78.900s; only known Bully 2 failures+2 errors.

## BookTrader terminal cleanup jumps expanded

Added native_book_trader_cleanup after movie recovery. Traverse native suffixes
from DB46DA,DB4F39,DB4F3F,DB4F45,DB4F4D,DB4F51 to DB4F56; only reviewed MOV/LEA/PUSH,
direct jumps and exact unpause/movie-end call sites are allowed. The first four
suffixes unpause then end; last two only end. Full caller/source/stage hashes and
per-target occurrence counts gate all21 source jumps. Conditional jumps expand
to guarded effects then goto existing outer LAB_DB4F5A; unconditional jumps expand
directly. This reaches the existing unresolved actor destructor corresponding to
native DB4F5A; native DB4F56 is only its receiver LEA. No actor release is invented.

Tests extract each actual source expansion, generate Lua and verify false guards
have no effects and taken paths reach terminal cleanup with exact order. Changed
source rejects. Regeneration46/51functions,13/18files,zero missing,1541TODOs. Normal
movie continuation jumps (DB4CE1 and DB4BC5) remain unresolved and are deliberately
outside the terminal-suffix proof. Runtime/reconstructed port files untouched.

Full generated tail inspection confirms supported gotos now reach LAB_DB4F5A.
The generic lifter currently omits its native actor destructor, so reaching this
label does not yet release control. The recovery evidence explicitly keeps
actorCleanup=unresolved. Do not treat these paths as gameplay-complete.

Full suite completed:735 tests in83.747s, only known Bully2failures+2errors.
Disabled registry remains empty.

## BookTrader remaining cleanup continuations recovered

Correction to prior handoff: DB4BC5 is terminal, not normal continuation. Native
DB4BCD unpauses then DB4BD3 jumps DB4F4D, ending the movie before actor cleanup.
DB4CE1 alone is normal movie-end and falls into DB4CE6 timer check. Extended the
existing suffix recovery with per-label native/source destinations: two jumps to
DB4BC5 expand to unpause/end/goto final actor label; one jump to DB4CE1 expands to
end/goto DB4CE6. All24 expansions are exercised by the Lua test, with explicit
destination assertions separating normal from terminal paths.

Regeneration46/51functions,13/18files,zero missing,1538TODOs. Native/runtime actor
resource cleanup remains unresolved. Generated normal cleanup now reaches the
existing timer label; drafts remain disabled. No runtime/reconstructed port edits.

Full suite:736 tests in85.503s; only known Bully2failures+2errors. The remaining
unsupported BookTrader goto is DB42C4 in hit classification. Native DB4258 self
slot54 takes SCRIPT_NAME_HERO and true->DB42C4 sets resulttrue. Otherwise DB4280
slotA8(any special fromhero), false->resultfalse. True leads DB42B3 slotA4 with
arguments14 (0xE), SCRIPT_NAME_HERO; false->resulttrue, true->resultfalse. Native
byte result atstack13 is consumed DB430B. Source uses cVar5 and bVar4; the special
ability operand is currently missing. All three string temporaries have separate
construction/cleanup mask bits1,2,4. Recover this branch without skipping cleanup
or changing short-circuit query order. No edit to hit classification yet.

## BookTrader hit classification recovered

Added native_book_trader_hits after cleanup recovery. Restores explicit ability14
on MsgIsHitBySpecialAbilityFrom and restructures its sibling-label jump into a
local bVar4 assignment with an else for the no-special case. All three conditional
string constructors, mask assignments, cVar5 results and subsequent cleanup stay
in place. The native truth table is hit OR(anySpecial AND NOT special14).
Native caller/source/stage hashes, SCRIPT_NAME_HERO bytes and three host contracts
gate the rewrite. Read-only LuaEntityAPI683/695/3400 confirms host hero predicates
use the same native methods and hero string; special wrapper forwards ability ID.

Lua tests cover all8predicate combinations, exact short-circuit query sequence,
ability14, last query value and construction-mask1/3/7. Altered native immediate
rejects recovery. Generation46/51functions,13/18files,zero missing,1535TODOs.
BookTrader now has no TODO goto or missing-argument markers, but still has other
unresolved native operations and actor cleanup, so it is not behavior-complete.
Full generated branch inspected; drafts remain disabled; runtime/port untouched.

Full suite completed:738 tests in90.901s, only known Bully2failures+2errors.

## BookTrader loop termination result recovery

Found a major semantic issue despite no missing-argument markers: generated Main
assigned cVar5=extraout_AL_00 and later extraout_AL_40. Undefined nil~=0 made the
entry loop exit immediately. The general termination fold only handles immediate
result copies; both native source copies had intervening pure uVar16 assignments.
Added native_book_trader_termination after hit recovery, capturing each known
termination query directly into cVar5 then preserving the uVar16 assignment.
Native sites DB4015 and DB4E6E call F35B30 then testAL; wrapper12bytes and full
caller/source/stage hashes pin the correspondence. Each query is evaluated once.

Lua traces cover initial termination, one-frame and two-frame runs with exact
check/frame order. Regeneration stays46/51functions,13/18files,zero missing,
1535TODOs: this fixes a previously uncounted semantic error. Full generated loop
entry and tail now use cVar5=not alive, with boolean condition. Drafts disabled.

Read-only runtime review confirms resource issue remains: AcquireControl treats
a different priority on an existing handle as nesting; native BookTrader reuses
one resource stack20 and resets it before reacquisition. First two acquisitions
are priority3 but draft currently uses4 because source argument recovery is still
missing there. Need paired reset/acquisition/lifetime recovery rather than simply
assuming every native resource reset can release a nested host handle. No runtime
or reconstructed port changes made.

Full suite completed:740 tests in112.998s, only known Bully2failures+2errors.

## BookTrader control resource evidence mapped

Generalized the existing PostAttack resource mapper behind a BookTrader wrapper.
New witness records18 native call setups:8acquire,4has_resource,4reset,2destroy;
single resource stack20, initial priorities3/3 then six4s. Full4042caller/source
and profiled helpers are hashed. PresenceCD23B9 is pImp.Data!=null. ResetCD2770
calls7E70E0 on +8, decrements/callback/frees the reference metadata and zeros both
fields. Destruction7E74D0 performs equivalent reference cleanup then base teardown.
Two additional inline exit cleanups DB4E7B/DB4EC6 remain separately noted.

Important native semantics: StartScriptingEntity89B5B0 has a false-return fallback
path DB89B715..89B773 which constructs903810 and copies via8ABD10 into the caller
resource before returningfalse. Factory allocates903710 object and shares its
pointer/refcount; copier writes +8/+C and increments metadata. Thus has_resource
cannot be replaced with acquisition-success boolean. Runtime AcquireControl
retries internally and destroys failed handle on termination, so this mismatch
must be addressed in the eventual resource/scheduling lowering. No speculative
ReleaseControl substitution made.

Linear setup reports incoming ECX for DB455F and shared destructorDB4F5A.
DB4555 LEA ECX,[ESP+14] precedes branch todestructor orfallthroughpresencecheck;
other destructor selectors DB41A1 andDB4F56 are sameLEA. Witness documents these
reviewed paths rather than falsifying the linear symbol. Five focused resource
tests pass including wrong-reset-slot rejection and native selectorbytes.
Report adds bookTraderResourceEvidence with loweringStatus unresolved. Generated
behavior unchanged; drafts disabled. No runtime/reconstructed port edits.

Full suite completed:742 tests in99.268s, only known Bully2failures+2errors.
Verified generated report has mapped18events for DB3FA0 and registry stays empty.

## BookTrader initial acquisition priorities recovered

Added native_book_trader_acquisition after termination recovery. Uses the checked
resource map's DB404D/DB4078 events (self,stack20,priority3) plus source/stage hashes
to restore explicit native arguments for the first acquisition/retry pair.
Preserves intervening frame, termination query and early cleanup source. Generated
first two AcquireControl calls now3, remaining six4. No resource-lifetime or
per-attempt scheduling equivalence claimed; evidence records both as unresolved.

Four focused acquisition/resource tests pass. Lua trace covers two failed
attempts then success with priority3 at each attempt and intervening frames;
missing map or altered retry priority rejects. Existing retry-collapse warnings
are retained and asserted. This is operand recovery, not a resource reset fix.
Disabled registry verified; runtime and reconstructed port files untouched.

Full suite:744 tests in97.268s; only known Bully2failures+2errors. Generation
46/51functions,13/18files,zero missing,1535TODOs. Remaining syntax errors currently
AffairWife68,AffairWoman245,BarrelMan96,Bully101,questInit88 (native pointer syntax).

## Quest Init speech-vector construction materialized

Quest Init DAADD0's syntax failure was its42C++ vector append/copy blocks, not
position arithmetic. Existing native_speech_vectors already recovers8immutable
tables and42ordered text keys under native/canonical-source hashes, reviewed
inventory checks, and working-port order comparisons. Added
recover_init_vector_construction plus source/stage witness for the TU version.
When the caller supplies matching static vectors, replace only the reviewed
construction tail (first speech string through final return) with return; emit
the same vector_prelude in the quest package as in Villager. All preceding quest
state/API initialization remains byte-for-byte unchanged at the annotated stage.

Four focused tests pass: preserved prefix, absent/reordered table rejection,
native/port order agreement and all42actual Villager selection outputs. This is
the existing immutable one-time speech-data contract; repeated Init append or
mutable-vector semantics are not recovered and must not be inferred from it.
Generation improves to47/51functions and14/18files compiling,zero missing bodies,
1389TODOs. Init still has unresolved timer/state operands despite now compiling.
Remaining syntax failures:AffairWife,AffairWoman,BarrelMan,Bully (position/pointer
expressions). Generated table prelude/report inspected; drafts disabled; no
runtime/reconstructed port files changed.

Full suite completed:746 tests in91.540s; only known Bully2failures+2errors.

## Init timer and signed GUI counter recovered

Shared native call setup now models XOR r32,r32 as constantzero, retaining unknown
for partial writes and rejecting ESP mutation. This proves Init's zero EBX from
DAADD4 through state writes to SetTimerDAAE2A. Receiver is GSI; first argument is
entry quest field104, secondzero. Added native_init_scalars immediately after
speech construction materialization; source/stage/caller hashes, typed state and
API contracts gate replacing the dead mistyped ppVar4 load/call with explicit
GetStateInt(TalkIntermittentTimer). Also normalize decimal offset100 to64 and
signed bitpatternFFFFFC19 to -999 for Int GUIBullyHealthCounter, nativeDAAE30.

Twenty-one focused Init/call-IR tests pass (plus earlier28 resource/lifetime checks
after the shared XOR change). Lua trace uses timerID57 and verifies timer reset0
then GUI counter-999; changed timer argument or missing typed mapping rejects.
Generation47/51functions,14/18files,zero missing,1387TODOs. Generated calls and
empty disabled registry inspected. Runtime/reconstructed port untouched.

Full suite completed:749 tests in98.328s; only known Bully2failures+2errors.

## Init packed bad-deed flags recovered

OffsetsFC..100 are WhichBadDeedsPerformed bool[5], not unrelated scalar fields.
Native DAAE5B zerosEAX; DAAE5F writesdwordFC and DAAE6E writesbyte100. Added
recover_init_bad_deeds after Init scalar recovery, expanding these two stores
into five typed indexed writes through existing state-array lowering. Full
source/stage/native hashes and the named Bool array mapping gate the rewrite.
AddBadDeed already reads/writes the indexed keys; Init now clears those same keys
instead of self_0xfc/self_0x100, fixing stale flags after initialization.

Six focused Init tests pass. New Lua regression seeds six flags true, clears five,
reads those five through the actual array-load lowering, and confirms adjacent
flag5 plus timer state remain unchanged. Missing array mapping rejects. Generated
Init has zero TODOs under the immutable speech-table contract, but whole-game
behavior and Init reentry are not proven by that count. Overall47/51functions,
14/18files,zero missing,1385TODOs. Generated five writes/consumer keys and disabled
registry inspected. Runtime/reconstructed port untouched.

Full suite completed:751 tests in117.617s; only known Bully2failures+2errors.

## Whole Init scalar-effect audit

Added audit_generated_init, which decodes native DAADD0..DAAE74 independently of
the source rewrites, tracks zero-register values and typed state stores, and
records SetTimer from checked call setup. It then freshly lifts the complete Init
through all recovery passes and runs it with nondefault state in Lua. Both traces
match:30state writes plus1timer reset in exact order, zeroTODOs. Two different
existing timer IDs (57,913) remain unchanged. Packed bad-deed clears expand to the
same five indexed keys used by consumers.42speech keys remain checked separately
by the existing native/inventory/port-order and selection tests.

Nine focused Init/speech tests pass. Saved actual trace to
work/converter_init_effect_audit.json (oktrue,31native/31Luaeffects). No converter
output changed this pass, so the full suite was not repeated; last full751tests
had only known Bully2failures+2errors. Scope is scalar initialization under the
current immutable speech-data model, not game execution or repeated Init appends.
Next substantive targets remain the four pointer/position syntax failures and
runtime resource/scheduling gaps. Goal remains active; drafts remain disabled.

## AffairWife route self operands recovered

First husband route begins nativeDB3417 (source reordered near function start).
LookupDB3445 creates wrapper atstack48, implementationpointer at52. The position
branch DB3462..3476 uses fallback143E8E0 fornull or implslot18 forlive. MoveDB3486
uses resource16 and vectorEAX,radius2,movetype1,false,true. Existing runtime null
GetPos behavior and fallback value remain unresolved, so no position rewrite yet.

Found two surrounding actor errors: DB345C slot844 passes EDI=self,true, whereas
draft selected the staged husband; DB3491 slot5A4 passes EDI=self, whereas draft
passed nil. EDI defined DB2B1C=ESI+8 and not overwritten. Added
native_affair_wife_route early after self-wrapper folding with native/source/stage
and binding hashes plus checked call setups. Emits explicit me,true and me.
Final movement-disable call is not part of this recovery.

Two focused tests pass, including a staged unrelated actor and changed native
self rejection. Generated calls inspected.47/51functions,14/18files,zero missing,
1383TODOs; Wife still fails syntax at unresolved position expression. Report
affairWifeRouteEvidence explicitly leaves positionLowering unresolved. Drafts
disabled; runtime/reconstructed port untouched.

Full suite completed:754 tests in90.135s; only known Bully2failures+2errors.
Next route operand issue: distanceDB34A2 uses selfEDI,cachedhusband stack48,3.0;
draft now retains r1husband but threshold remains &ppuStack_a4. Recover exact
cached distance operands independently of unresolved GetPos fallback.

## AffairWife cached husband distances recovered

Added recover_affair_wife_distance to shared native_teddy_distance machinery,
immediately after Wife route-actor pass. Witness pins Main4887bytes, raw/stage
source and CBE2FF114bytecallee. LookupDB3445 writes stack48; both DB34A2 and
DB3586 consume that wrapper inEDX with EDI=self and float3.0. Rewrite one lookup
to explicit local_native_cached_husband output and exactlytwo matching distance
expressions. Other two native distance sites remain unchanged.

Eight focused Wife/Man distance/route tests pass. Lua trace covers first-check
success, first-fail/second-success and both-fail with exactlyone cached lookup,
proper wife/husband operands and threshold3; altered repeat-cache slot rejects.
Full generated Wife now has correct calls at lines73/339. Generation remains
47/51functions,14/18files,zero missing,1381TODOs. Position fallback and movement
still cause Wife syntax failure; no behavior-complete claim. Drafts disabled;
runtime/reconstructed port untouched.


## Wife hit classification and branch-local retention

Shared native_book_trader_hits now has recover_affair_wife_hits, immediately
following Wife hero-distance recovery. Own source/stage/native4887byte witness
pins the reviewed DB2C35/DB2C5C/DB2C88 branches and ability14. Rewrites sibling
goto into short-circuit structured classification and renames cStack_9d uses to
native_arg_wife_hit, with boolean result assignments. Conditional constructors,
masks8/18/38 and downstream cleanup preserved. Report affairWifeHitEvidence.

Tests initially exposed a shared bug: definitions Counter excluded native_arg_
locals, allowing branch literal assignments to collapse to the last literal.
Counter now accepts the same native_arg_ pattern as RE_LOCAL_ASSIGN. Wife Lua
truth-table tests exercise all8 combinations, query order, masks and last query;
changed native ability rejects recovery. Six focused Wife/BookTrader/hero tests
pass. Generated reaction uses if native_arg_wife_hit. Generation47/51functions,
14/18files,zero missing,1375TODOs. Drafts disabled. Position and movie/resource
lifetime remain unresolved; no behavior-complete claim.

Next investigation artifact work/converter_wife_pause_sites.json records17
slot5EC sites. Pause true at DB2DD9/DB2F1F/DB389C/DB3A65; other sites unpause.
DB2F1F setup reader returnsNone due post-push memory store; saved interface EBP
written at DB2F11 and stack36 storedDB2F1B. DB3E07 has incoming branch receiver
selection, requiring CFG review. Do not treat artifact as a validated lowering.

Full suite completed:760 tests in124.337s; only known Bully termination
camera cleanup2failures+2errors. Benchmark rerun saved to
work/converter_wife_hits_benchmark.log. No runtime/reconstructed files edited.


## Wife pause/unpause operands recovered

Added recover_affair_wife_pause wrapper in shared native_affair_pause; own
witness follows Wife hits. Pins4887caller,80bytecallee890AB0,slot5EC binding,
raw/stage source,16 decoded setups. DB2F1F post-push store is separately reviewed
under full caller hash. Four true sites and13false;8 source edits cover9calls,
existing8explicitcalls retained. Generated all17call operands verified.

EBP GSI definitions DB2DCF/2F11/3892/3A5B. Second saves stack36 atDB2F1B,
then reuses EBP DB30D6. SharedDB3E07 has EBP path viaDB3DD9 fromDB2F61/3007
(before reuse), or savedstack36 viaDB3DFF. Source unaff_EDI is savedinterface,
not actorEDI. No broad replacement of resource/interface scaffolding.

Five focused Wife/Man/BookTrader pause tests pass. Generation47/51functions,
14/18files,zero missing,1372TODOs. Report affairWifePauseEvidence. Drafts disabled;
no reconstructed/runtime files edited. Movie resource lifetime and cleanup goto
recovery remain necessary before Wife can be considered behaviorally correct.

Full suite:762 tests in96.535s, only known Bully2failures+2errors.
Next movie investigation work/converter_wife_movie_sites.json records4starts
(DB2D99/2EDE/385F/3A28) and7ends(DB32A5/396B/3B37/3CF0/3D23/3D65/3E11).
Artifact4896bytes includes trailingINT3; trim to actualRET before lifetime proof.
Shared ends DB32A5/3E11 need predecessor receiver selections. No lowering yet.


## Wife movie resources recovered

Added recover_affair_wife_movies to native_affair_movies, after Wife pause pass.
Shared helper supports witness startCount(default2); Wife4. Full4890bytebody
endsRET DB3E29. Starts DB2DC0 resource164, DB2F02 resource64, DB3883 resource64,
DB3A4C resource116; seven ends DB32A5/396B/3B37/3CF0/3D23/3D65/3E11.
Shared DB32A5 and DB3E11 receiverECX selection proved by LEAs DB2E90=164,
DB32A1=64, DB3DBA=164, DB3DD0=164, DB3E0D=64. Full CFG checker passes
single active movie, matching ends and no active movie at returns.

Four misnamed6E7B60 constructors removed; exactlyseven6E7B80 destructors mapped
to EndMovieSequence. Other misnamed MovieBase destructors are actorcleanup,
retained. Four focused Wife/BookTrader movie tests pass, including wrong shared
receiver selection and rejected lifetime proof. Generation47/51functions,
14/18files,zero missing,1368TODOs. Drafts disabled. Report affairWifeMovieEvidence.

Next: recover source cleanup jumps, which still lose control flow. Normal
DB2E83 andDB3293 unpause/end thenjoinDB32AA; terminal movie routes end then
actorcleanupDB3E16. Later DB3D61 closes resource116 and continuesactorcleanup
DB3D6A. Many DB3D90/3D8B jumps are actor-resourcecleanup and must not be treated
as movie ends. Position expressions and actor ownership remain unresolved.

Full suite764tests in130.917s, only known Bully termination camera cleanup
2failures+2errors. No runtime/reconstructed files edited.


## Wife movie cleanup jumps expanded

Shared native_book_trader_cleanup now has parameterized witness/regex/body/effect
map and recover_affair_wife_cleanup, after Wife movie pass. WIFE_JUMP covers
2E83/3293/32A5/3DD9/3DE0/3DEF/3DFF/3E05/3E0D/3E11/3D61,19sourcejumps.
Witness pins raw/stage source and4890body; decoded suffix traversal accepts only
MOV/LEA/PUSH/directJMP and listedunpause/movieend calls, checkingorder/destination.
Normal pathsjoin32AA,terminalpathsactorcleanup3E16,latermovieendactorcleanup3D6A.
Every condition preserved. Report affairWifeCleanupEvidence withactorCleanup
unresolved. Existing BookTrader behavior retains its defaults.

Four focused Wife/BookTrader cleanup tests pass. Tests extract every actual
expansion, executeLua with true/false guards, checknoeffects onfalse and exact
ordered effects/destination ontrue. Generated edits inspected.47/51functions,
14/18files,zero missing,1368TODOs unchanged. The19expansions include previously
accepted gotos and newlyexplicitcalls, so TODOcount does not measure this fix.
Wife position syntax and actorownership remain unresolved. Drafts disabled.
Remaining3D90/3D8B/3D6A jumps areactorcleanup; do not treat asmovieends.

Full suite766tests in116.972s, only known Bullytermination camera cleanup
2failures+2errors. Actor suffix investigation savedwork/converter_wife_actor_cleanup_paths.json:
3D6A and3D90 destroy wrapperstack48 via4AA840, then actorresourcestack16 via
7E74D0 andreturn. 3D8B first destroysstring99EAE0. 3E16 destroysactorresource16
only. These are not automatically equivalent to hostReleaseControl; native
resourcepresence vs acquisitionresult and scheduler semantics still need proof.


## Wife later argument hit branch recovered

Added recover_affair_wife_argument_hits in sharednative_book_trader_hits after
Wife cleanup. Own witness raw/stage/native4887 hash. DB36FE ordinaryhittrue
->DB3762; elseDB3725 anyspecialfalse->DB375B; elseDB3751 ability14false->true,
true->false. Operand14 alreadycorrect, only siblinggoto3762 structured away.
Preserves constructors/masks1/3/7, cVar5 and unaff_EBP aliases. Tests all8truth
combinations, queryorder/masks/lastquery,changednativeabilityrejection. Explicitly
expect two retainedunaff_EBP assignmentTODOs in snippet; do not hide them.
ReportaffairWifeArgumentHitEvidence. Generation47/51functions,14/18files,
zero missing,1366TODOs.22focused/integrationtests pass in30.774s. Fullsuite was
not repeated for this isolated recovery; latestfullbaseline766tests onlyknown
Bully2failures+2errors. Draftsdisabled, no runtime/reconstructed edits.

Next useful issue: saved-registermask aliases in laterloop. Source unaff_EBP
assigneduVar14 onpredicate/bit4cleanupbranches, uVar21=unaff_EBP beforebit2cleanup,
and unaff_EBP=uVar21 atloopbottom. Native actuallyusesEBX masks atDB36F5/371B/3746,
then clears4 atDB376C,2atDB3781,1atDB3796 with stringdestructorsDB3777/378C/37A1.
No EBP use in that cleanup. Need recover mask dataflow fromnative ratherthan
merelydiscardaliasassignments; they feeduVar21 and subsequentloopiterations.


## Wife argument cleanup mask alias recovery

Added native_affair_wife_mask after argument-hit pass. Pins raw/stage/body4890
hashes and exact3+1+1sourcecorrespondence. Native EBX OR1/2/4 andclear4/2/1 are
singlemask; noEBP use. Remove3unaff_EBP=uVar14 assignments and loopbottom
unaff_EBP=uVar21, replaceuVar21=unaff_EBP withuVar21=uVar14. Conditional native
string cleanup bodies remain. Report affairWifeMaskEvidence.

Correction to earlier suspicion: first query alwayssetsbit1, whosecleanup writes
uVar21 fromuVar14 regardless of alias. No proven final-mask corruption; this
removes spurious register plumbing, not a claimed observed gameplay bug.
Repeated Lua tests spanall8predicates with uppermasks0/8/38/A8/FFFFFFF8, preserving
upperbits and clearinglow3. Alterednativeclear rejects. Generation47/51functions,
14/18files,zero missing,1362TODOs. Draftsdisabled. Actorownership/position unresolved.

18focused/integrationtests pass in50.795s. No fullsuite repeat for isolated
mask pass; latestfullbaseline766tests onlyknownBully2failures+2errors. Generated
Wife containsno unaff_EBP reference. No runtime/reconstructed files edited.


## Wife facing operands and optional snap flag recovered

Added native_affair_wife_facing aftermaskpass; raw/stage/body4890,callee88E620193,
vtable76C hashes,exacthostcontract and3decodedsetups. DB36D8/3954/3B20 all pass
selfEDI,cachedhusbandstack48,flags0/1/1. Cachedwrapper previouslyrecovered from
DB3445lookup. Two sourcepatterns(count1/2) nowexplicit cachedhusband,false/true.
ReportaffairWifeFacingEvidence. Generated calls me,r1,false/true/true inspected.

Runtime read-only LuaQuestState.h156/cpp2361 forwards optionalboolsnap defaultfalse
into native4argumenttypedef GameInterface.h1658. Sharedmanifest only2params;
converter_signatures nowaddsoptionalsnap only exactoldQuestvoidpThing/pTarget
shape. SharedSDKmanifest untouched; newerentrypreserved. Fivefocused Wife/Man
facingtests pass, includingcachedtarget vsstagedhero,flags,alteredstackreject,
overlaysignatureidempotence/inputimmutability. Generation47/51functions,
14/18files,zero missing,1359TODOs. Draftsdisabled; no runtime/reconstructed edits.

Native ally calls justafterhitDB37D1/37E7 appear similar toBookTrader's bidirectional
self/hero pair; source currentlydrops operands and attaches self tosecondGetHero.
Candidate next recovery using shared allyoperand machinery, preservingtwo getters.

Fullsuite774tests in188.657s, onlyknownBully2failures+2errors. Benchmark rerun
work/converter_wife_facing_benchmark.log. All converterfocusedtests passed.


## Wife directed ally calls recovered

Shared native_book_trader_allies now accepts witnessbody/calllist/selfregister/
edits, preservingBookTraderdefaults. recover_affair_wife_allies followsfacing.
Initialhit gettersDB2CFD/2D14 -> allycallsDB2D08/2D1E; argumenthit gettersDB37C6/
37DD ->DB37D1/37E7. Each pairselfEDI,freshhero thenfreshhero,selfEDI. Reversepath
pushself beforezeroargGetHero. Ownsource/stage/body4887/binding95C/contractchecks
andfourdecodedsetups. Twoexactsourceblocks replaced, branchplacementunchanged.
Removedonlydead iVar10vtableloads. LatersecondresultuVar11 retained.

Fourfocused Wife/BookTraderallytests pass, includingfourdistinctheroevaluations,
orderedarguments andalterednative directionrejection. Generatedfourcallsinspected.
ReportaffairWifeAllyEvidence. Generation47/51functions,14/18files,zero missing,
1356TODOs. Draftsdisabled. No runtime/reconstructed edits. Actorresourceownership
andpositionhandlingstillunresolved; optionalfacingflag alreadycoveredpreviouspass.

17focused/integrationtests pass in31.550s. Fullsuite not repeated for scoped
ally pass; latestfullbaseline774tests onlyknownBully2failures+2errors.


## Wife actor-resource map and all acquisition operands

Added map_affair_wife_resources to sharednative_post_attack_resources; own
witness4890body/rawsource/profiles,26decodedsetups:10acquires,5presence,5reset,
6destructors. Allslot16; selfEDI; first2priority3,remaining8priority4. Acquires
DB2BC3/2BF4,DB2D51/2D7C,DB2F48/2F74,DB381A/3845,DB39D1/39FC.
CD23B9 testsresource+8; CD2770 resets;7E74D0 destroys; profile89B5B0fallback
maypopulatehandle despitefalse, so do notlowerpresence using successboolean.

native_affair_wife_acquisition afterallies checksstagehash,resourceproof and
exact10sourcecallsequence, thenemitme,&local_native_wife_control,priority.
Other source byte-for-byte unchanged after maskingcalltext. Native/sourceorder
pairs initialloop,initialhit,firsttalk,argumenthit,argumenttalk reviewed.
ReportaffairWifeAcquisitionEvidence nests resourceEvidence and unresolvedownership.
Fourfocused Wife/BookTrader acquisitiontests pass. Allfivepairs fail/fail/succeed
withcorrectpriorities/frameorder; alteredresetresource rejects entire recovery.
Generation47/51functions,14/18files,zero missing,1356TODOs unchanged. Native/host
retry scheduling andhandle lifetime unresolved. Draftsdisabled; no runtime edits.

19focused/resource/integrationtests pass in31.589s. Fullsuite not repeated;
latestfullbaseline774tests onlyknownBully2failures+2errors. Generatedall10priorities
inspected. Remaining firstloopgoto33EB/33F3 may hide nonterminationpaths; inspect
native beforetryingtoerase those unsupportedgotos.


## Wife saved termination results recovered

Whilechecking33EB/33F3 foundlooplevelcVar5=extraoutAL00/22 separated from
F35B30byuVar21 assignments. Addedrecover_affair_wife_termination inshared
native_book_trader_termination afteracquisition. Ownraw/stage/body4890/callee12
hashes. DB2B8C TESTAL JNE33AB andDB339E TESTAL JE2B9B captureboolcVar5 once,
retainuVar21=0/uStack8C. EarlierDB2B5B entryguard untouched. Draftpreviously
leftALalias unresolved, so nil~=0 couldexitlive thread. ReportWifeTermination.

FourfocusedWife/BookTraderterminationtests pass:immediatetrue,false/true,
false/false/true withcorrectquery/frameorder. Generation47/51functions,14/18files,
zero missing,1356TODOs unchanged; behavioralfixnotcountedbyTODOmetric.
Native inlineinitial-retrycleanup33EB/33F3 stillunresolved; resourcepresence and
hostlifetime cannotbeassumed equivalent. Draftsdisabled; runtimeuntouched.

16focused/integrationtests pass in34.784s. GeneratedentrycVar5=notalive and
ifcVar5 verified. Fullsuite not repeated; latest774testbaselineonlyknown
Bully2failures+2errors. Correct reportfieldaffairWifeTerminationEvidence.


## Wife task-query operand audit

Inspected all14native calls7E7450 and matched ECXresource16 viaCallSetupIR.
Helper15bytes:resource+8,nullfalse,elseimplvtable68. RuntimeLuaEntityAPI.cpp731
selectsactorhandle thennullcheck/implquery. ExistinggeneratedLuaalreadyuses
me:IsPerformingScriptTask atall14sites, so no operandrewrite needed.

Extended Wife resource witness with14task_query events andhelperhash;40events
nowtotal. Acquisitiontestcountupdated. New test_native_affair_wife_task_queries
liftsall14actualsourcecalls, stagesunrelatedhero, checks true/false resultand
onewifequery; changedlastnativequeryresource rejectsfullmapping. Fourfocused
query/acquisitiontests pass in5.709s. Generationrerunforupdatedresourceevidence.
No fullsuite repeat for evidence-only audit. Lastfullbaseline774tests onlyknown
Bully2failures+2errors. No resource lifetime or scheduler parityclaim.


## Wife intermittent speech timer and retail random gate

Shared native_book_trader_timer now has Wife wrapper and witnessget/setsite
parameters(defaultBookTraderunchanged). AfterWifetermination, readDB32BD and
resetDB332B useparentquest104 TalkIntermittentTimer, reset3. Ownraw/stage/body4890
hashes anddecodedGSI/args; sameAPIcontractchecks. Sixfocused Wife/BookTradertimer
tests pass includingtimerID73.18timer/termination/integrationtests passedbefore
randomintegration. ReportaffairWifeTimerEvidence.

Added fold_affair_wife_random aftertimer. NativeDB32D2 BFEB16, CDQ IDIV500,
TESTEDX; originalrandvalue overwrittenonbothsuccessors. Preservetimernonzero
skip andForceFirstTimeSpeak byte1D bypass. Witnessraw/stage/body4890 andexact
randsiteverification. All500residues tested across timer0/1 andforcedtrue/false;
queryorder verifies timerfirst,forcedonlytimer0, RNGonlytimer0andnotforced.
ReportaffairWifeRandomEvidence. Generation47/51functions,14/18files,zero missing,
1355TODOs. Generatedsharedtimerread/reset3 andRetailRandModulo500 inspected.
Draftsdisabled. No runtime/reconstructed edits.

Next conversation: DB333F NewConversation(self,false,false) suffers post-push
byteflagstore; IDsavedEBP atDB334A. DB3357 AddPerson(IDEBP, freshheroDB334C).
DB3385 AddLine(IDEBP,keyTEXT_QST_048_AFFAIR_WIFE_WHERES_HUSBAND,false,selfEDI,
freshheroDB3372). Native setupartifact work/converter_wife_wheres_husband_conversation.json
is investigationonly, not reviewedlowering. Needpreserveseparateheroqueries.

Fullsuite787tests in134.692s, onlyknownBully2failures+2errors. Resource,
termination,timer,random additions allcovered. Goalstillactive/incomplete.


## Wife Where's Husband conversation recovered

Addednative_affair_wife_conversation afterrandom. Ownraw/stage/body4890 and
vtable118/5B0/5B4/5B8hashes,4APIcontracts,text12D8DAC and2decodedsetups.
DB333F NewConversation(self,false,false) reviewedunderbodyhash(postpushstore
preventsIRsetup). DB334A savesID EBP; DB3357 adds freshhero334C; DB3385 line
usesIDEBP,textstack44,false,selfEDI,freshhero3372. Sourceblockpreservestextctor,
uVar11ID/uVar12listener, removesdeadvtblreads andnamesfirsthero result.
ReportaffairWifeConversationEvidence. TestID173 anddistinctparticipant/listener
heroresults verifiesnew/person/lineorder,native-to-hostargumentorder,flags and
speaker; alteredlistenerrejectswholeblock. Twofocusedtests pass.
Generation47/51functions,14/18files,zero missing,1354TODOs. Actualgenerated
new(me,false,false),participant andline inspected. Draftsdisabled. Runningto
husband andlaterargument conversations stillwrong/unresolved, asarepositions
andnativehandlelifetimes. No runtime/reconstructed edits.

17focused/line/random/integrationtests pass in31.402s. Fullsuite not repeated
forisolatedconversation; latestfullbaseline787tests onlyknownBully2failures+2errors.


## Wife running-to-husband line and typed null actor operands

Sharednative_affair_wife_conversation nowparameterizedwitness/textaddress/event
profiles, withrecover_affair_wife_running_line afterfirstconversation. DB3510
NewConversation(self,false,false), DB3521 savesIDEBP; DB355F lineID,textstack44,
false,selfEDI,emptywrapperstack116. Nullwrapper base352C,vtable1238C8C at3543,
pImp/refcountzero354E/3555. Callerhashpinslinepostpushstores (IR cannotreadsetup).
Constructorcallsetupchecked; text12D8D80 andbindingspinned. Originalnullwrapper
scaffolding/destructor retained. Newexplicitlistener(CScriptThing*)0x0.

Read-only runtimeLuaQuestState.cpp3743 accepts nil listener by constructing
actualemptyCScriptThing withg_pCScriptThingVTable; avoidsnullC++pointervirtualcall.
Lifter.interface_call nowrecognizesonlyexplicittypednullCScriptThing zero and
passes per-call explicit_things toplace_args, emittingnil actoroperand. Doesnot
markglobalnilkind orreclassifyplainnumericzero. Fourfocusedrunning/firstconversation
tests pass; stagedherodoesnotdisplacenull, IDs91 andfalseflagsverified.
ReportaffairWifeRunningLineEvidence. Generation47/51functions,14/18files,
zero missing,1353TODOs. Actualrunningline(me,nil,false) inspected. Draftsdisabled.
No runtime/reconstructed edits. Laterargumentconversation remainsunresolved.

Fullsuite791tests in145.492s, onlyknownBully2failures+2errors.
Nextargumentconversation: DB3B49 IsConversationActive slot5C0 readsstack132,
initializedzeroDB2B85, newIDEBX fromDB3B70 storedstack132 atDB3B83(after2pushes
ESP+8C). AddPersonDB3B8A ID,selfcachedhusbandstack48. NewConversation selfEDI,
false,false. IDstack132 hasmisidentifiedfloat uStack30 source; recoverread/write
identitybeforeline-building. Nextstringcounterstack60 adds10atDB3B94; textsequence
semanticsrequirecarefulreview, not blindlyincrementinglinecount.


## Wife argument conversation ID and participant recovered

Addednative_affair_wife_argument_conversation afterrunningline. Raw/stage/body4890,
vtable5B0/5B4/5C0 and3contracts pinned; IsActiveDB3B49 andNewDB3B70 setupschecked.
IDstack132 initializedzeroDB2B85, read3B3F, newIDEBX3B79 stored3B83(aftertwo
pushesESP8C). Participant3B8A cachedhusbandstack48,IDEBX; postpushstore reviewed
underfullbodyhash. ReplaceuStack30=0 withnative_arg_wife_argument_id=0, queryexplicit
ID, createexplicitself,false,false, copyresultppVar15todediID, addcachedhusband.
OtherppVar15/ppVar25/ppStack5Caliasesretained; numberedtextsequenceunresolved.
ReportaffairWifeArgumentConversationEvidence, textSequenceunresolved.

Twofocusedtests pass: initialquery0 creates91, activequery91 skipsnew, later
inactivequery91 creates92, bothaddcachedhusband. AlterednativeIDslotrejectswhole
pass. GeneratedsavedID/read/write/participantinspected.47/51functions,14/18files,
zero missing,1352TODOs. Draftsdisabled; no runtime/reconstructed edits.

16focused/conversation/integrationtests pass in32.981s. Fullsuite not repeated;
latest791testbaselineonlyknownBully2failures+2errors. Nexttextcounterstack60
isdistinctfromcleanupmaskEBX: decompilerusesuVar21forboth, needingnativeproof.


## Wife numbered argument sequence specialized from installed text bank

Addednative_affair_wife_text_sequence afterargumentconversation. Verifiedinstalled
Englishtext.big withTextBank: exactprefixkeys10/20/30/40,50absent. WitnesspinsbankSHA,
raw/stagesource,body4890,99F830116bytes and%dstring1265DE8. Counterstack60 initializes
DB35B3(aftertwo pushesESP44),increments3B90/94/9E,resets10 at3C04(afterpushESP40).
Decompiler incorrectlysharedcounterwithuVar21cleanupmask; dedicatednative_arg_wife_line_counter
andkey nowused. Counter0init retainedlocation, prefix..tostring(counter), missing
conditioncounter>40 validforreachable10..50,reset10/key10 afteroriginaltermination
check; goto3C11 preserved. NativeGFIntToCharString confirmed%d, no LuaRNG introduced.

Removedonlyprefixint/stringtempconstruction+destructors, no actorcleanup. Wife
line3C24 nowexplicitID,textkey,false,self,cachedhusbandstack48. ReportaffairWifeTextSequenceEvidence
includespinnedbank scope; changed/unavailablebanks reject. This specialization
isnotgeneralTextEntryExists implementation or claimaboutmodified/localizedbanks.

Twofocusedtests pass:9linesverifytwo wraparounds,key/ID/speaker/listener/flag,
terminationqueryonlyatmissingkey; cancellationthere queuesnothinganddoesnotreset.
Testextractretainslabel3D8Bmarkerforlaterreplyedgeoutside snippet. Changedbankrejects.
Generation47/51functions,14/18files,zero missing,1348TODOs. Actualgeneratedcounter/key/
line inspected. Draftsdisabled; runtime/reconstructed filesuntouched. Husbandreply
randmod2 anddialogueoperands remainunresolved. Adjacentmovementdisablecurrentlyuses
(counter~=0)aftercounter0; explicitfalsecouldberestoredfromnative35BBlater.

## Full test checkpoint and optional husband reply

The preceding full suite completed: 798 tests in 159.337s, with only the known
Bully run1-termination cleanup failures (two failures, two errors). These tests
load the reconstructed port; that port and its tests remain untouched.

Added native_affair_wife_reply after text-sequence recovery. Its witness pins raw
and staged source, the full 4890-byte body, reply text bytes, and the dialogue
contract. DB3C2A calls BFEB16; AND/sign adjustment through DB3C3B implements signed
remainder modulo 2. Both successors discard raw EAX via calls before further use.
The zero edge alone checks F35B30 termination, then queues the line. DB3C6F uses
EBX conversation ID, stack84 reply text, false, stack48 husband, EDI wife.
String destruction at DB3C79/82 is removed with its temporaries; actor cleanup
and cancellation destination LAB_00db3d8b remain. Generic random call-count guards
were not weakened: this is a separate exact-source, full-body-reviewed pass.

Four reply/text-sequence tests pass in 6.092s, including emitted Lua dialogue
roles, random-call count/modulus, conditional cancellation, and altered native
listener rejection. Generated draft: 47/51 functions compile, 14/18 files compile,
zero missing bodies, 1347 TODOs. Draft registration remains disabled. Whole-entity
behavior is still not established; resource ownership and position gaps remain.

Inspected emitted reply: RetailRandModulo(2), saved termination predicate, and
AddLineToConversation(ID,key,r1,me,false), where r1 is the cached husband. The
whole draft still lowers the unresolved LAB_00db3d8b edge to a return/TODO;
the isolated regression supplies that destination and proves branch selection,
not completion of native actor cleanup. Cross-script benchmark also completed;
its existing unsupported pointer/expression syntax cases remain unresolved.

## Wife health operands and threshold

Extended native_book_trader_health with witness-controlled body address/size,
self register and resource offset, retaining BookTrader defaults. Added wrapper
recover_affair_wife_health after optional reply; report affairWifeHealthEvidence.
Wife witness verifies all ten acquisitions use EDI/resource16, getter7E7490
hidden outputs, and immediate slot420 consumers at DB2DF6, DB2FA1, DB3149,
DB3207, DB38BC, DB3A85. Six GetHealth calls now explicitly use me. The six
comparisons use pinned float32 zero at 0122DEDC instead of an unresolved global.
Native wrapper scaffolding/ownership is retained as unresolved.

Six focused Wife-health/BookTrader-health/Wife-reply tests pass in 9.050s.
Health tests execute all six recovered statements with a staged unrelated hero,
covering health -1/0/0.25/1/100 and the zero threshold; altered first/last getter
and consumer operands reject. Regenerated and inspected all six actual Lua calls
and threshold assignments. Current generation: 47/51 functions compile, 14/18
files compile, zero missing bodies, 1342 TODOs. Registration remains disabled.
Full suite not repeated for this parameterized recovery; latest full checkpoint
is 798 tests with only the known Bully two failures and two errors.

Investigation correction: Wife contains THREE native rand sites, DB32D2,
DB3600, DB3C2A. The remaining DB3600 signed-modulo2 chooses POINT_AWAY versus
POINT_AT animation and still emits raw rand(). Earlier notes calling the original
count two were incorrect; native call metadata and current source confirm three.
Position and actor-cleanup paths remain required work, not validated behavior.

## Wife animation selector

Added native_affair_wife_animation_random after health recovery, with raw/stage
source and full-body byte pins. DB3600 calls BFEB16; AND/sign fix at3605..3610
computes signed modulo2. MOV ECX at3611 preserves flags, JNE3613 takes POINT_AT
for nonzero, fallthrough takes POINT_AWAY for zero. Both successors immediately
call termination helper F35B30 (3615/365A), making the raw random value dead.
Only the selector is rewritten; original suffix with both termination branches,
animation calls and cleanup is preserved exactly. No raw rand() remains in Wife.

Six animation-selector/health/reply tests pass in10.908s. Selector test compares
emitted Lua against independent native instruction arithmetic for -100..100 and
signed32 extrema, checks one retail call with modulus2, verifies unchanged source
suffix, and rejects an altered native branch opcode. Regenerated draft remains
47/51 functions,14/18 files,zero missing,1342 TODOs, disabled registration.

Animation operands still require work: both native calls7E73D0 push eight values,
including mutable byte01375748 and an explicit true flag. Current generated
PlayAnimation has only three false flags; do not claim full animation parity.
Native sites364F/3695 both use resource16. Position and actor cleanup remain open.

## Correct entity PlayAnimation host signature

Read current shadow runtime: LuaManager.cpp153 binds me:PlayAnimation to
PlayAnimation_NonBlocking, whose LuaEntityAPI.cpp1998..2046 forwards seven flags
through the existing control handle and returns immediately. The SDK manifest
instead describes the older blocking three-flag wrapper. Added a narrow deep-copy
converter_signatures overlay for that exact old parameter shape, replacing flags
with b1..b7 and blocking=false. Shared manifest and runtime are untouched.

Two focused signature tests pass (0.054s), including all128 flag combinations in
emitted Lua, input immutability, idempotence, and preservation of custom shapes.
Regenerated draft retains false,false,false,true,DAT_01375748,false for Wife's
two animation calls. The source still omits the seventh native flag; the mutable
global remains unresolved. Do not claim full native argument recovery.
Generation remains47/51 functions,14/18 files,zero missing,1342 TODOs; disabled.

Full suite completed806 tests in197.613s: three failures,two errors. Four are
the known reconstructed Bully cleanup cases. The fifth was this new signature
test's initial fixture, loaded before its correction during the running suite:
it passed the un-overlaid manifest and an explicit pMe despite the lifter's
implicit entity receiver convention. Corrected fixture uses converter_signatures
and native operands without pMe. Focused rerun of both signature tests passes,
including after full-suite completion. No remaining new failure observed, but
the recorded full-suite result is not green and predates that fixture fix.

## Wife full animation operands, dynamic global retained

Native wrapper7E73D0 is15bytes: loads resource+8, null RET20, otherwise forwards
unchanged to implslot48. DB364F/3695 each pass key plus seven flags:
false,false,false,true,byte[01375748],false,false. Added exact-source/full-body/
wrapper-pinned native_affair_wife_animation_operands after animationrandom,
recovering two explicit calls and the previously omitted seventh false flag.
Report affairWifeAnimationOperandsEvidence lists runtime global access and resource
ownership/cleanup as unresolved. No runtime or reconstructed files changed.

Read installed global byte=1, in writable .data (characteristics C0000040).
work/inspect_animation_global.py found156 raw address references, with candidate
decodes saved in work/animation_global_references.txt. This is a candidate scan,
not validated instruction boundaries or a proof of no indirect writes. Therefore
the global is NOT folded totrue. Working port supplies true for this flag, matching
the installed initial value but not proving immutability.

Six operand/random/signature tests pass in6.227s. Both recovered calls execute in
Lua with dynamic false andtrue, checking exact seven-flag order; changed forwarding
wrapper rejects. This proves supplied-flag preservation, not live global access.

## Wife position fallback and host mismatch mapped

Added map_affair_wife_position and full-body/wrapper-pinned witness, integrated
as affairWifePositionEvidence. DB3462 reads cached husband implstack52, tests it;
present branch3473 calls implslot18, absent branch346A sets EAX=0143E8E0.
Both join3476. Pushes at3476..3481 provide position EAX,radiusbits40000000(2),
moveType1,false,true; resource16 derived at3482;3486 calls7E72F0. Wrapper forwards
to implslot10 or RET14 when resourceabsent. Mapping is explicitly unresolved.

Read host LuaEntityAPI.cpp540..560: GetPos returns empty table for absent actor/
implementation. MoveToPosition_NonBlocking cpp2110..2157 defaults missing x/y/z
tozero. Therefore simply lowering husband:GetPos() followed by MoveToPosition
would introduce an unproven zero fallback. Native0143E8E0 has no file-backed bytes
in RData; current runtime contents remain required evidence or bridge support.
No assumption of zero and no narrower MoveToThing substitution was introduced.
The new evidence records exact runtime requirements for completing this path.

## Missing ClearAllActions binding recovered

Current LuaManager.cpp180 exposes ClearAllActions_NonBlocking; LuaEntityAPI.cpp2550
selects the actor's existing handle and calls its ClearAllActions slot. Added
setdefault converter signature Entity/void/nonblocking/pMe only. Existing custom
contracts are preserved; shared manifest/runtime untouched. The existing mangled
resource-call emitter now emits the real action clear instead of a TODO.

Wife DB3A16 calls7E7400 on resource16, then DB3A1F calls7E7360 on resource16.
Regression decodes both call receivers and checks complete13-byte wrappers:
actions forwards slot54, commands slot28. Extracts actual consecutive source
calls (including bogus extra decompiler arguments) and executes emitted Lua,
requiring actions then commands. Six clear-actions/signature/animation-operand
tests pass in4.137s. Generation47/51 functions,14/18 files,zero missing,1340 TODOs.
The current host binding exists; no runtime changes needed for this operation.
Control ownership/lifetime and position fallback remain unresolved. Drafts disabled.

## Husband health actor recovery

Verified newly enabled husband ClearAllActions nativeDB0ECE uses resource16;
source immediately follows with ClearCommands, matching the generated order.
Found eight husband health operands still missing or guessed from unrelated
temporaries. Added recover_affair_man_health using the shared health recovery,
after recover_affair_movies. ReportaffairManHealthEvidence. Witness pins complete
5058-byte body, raw/stage source, getter helper and contract; checks six acquisitions
EBP=self/resource16/priority4 and eight getter/health-consumer pairs. Sites:
DB0D0B/0D14,0F2E/0F37,0FE9/0FF2,10BB/10C4,11A9/11B2,1360/1369,142D/1436,14FA/1503.
Hidden outputs312,288,228,300,252,264,276,240. Distance getter calls unchanged.

Eight husband/wife/BookTrader-health and action-clear tests pass in6.071s.
All eight recovered husband statements execute with a staged unrelated actor,
for health0/0.25/100, and query husband every time. Changed stage source rejects.
Regenerated output inspected: eight quest:GetHealth(me) calls. Generation47/51
functions,14/18 files,zero missing,1333 TODOs. Registration stays disabled;
shared runtime/reconstructed files untouched. Native wrapper cleanup and the
husband's unresolved threshold-global expressions remain required work.

## Husband health threshold and branch boundaries completed

Extended the health witness with eight replacements of _DAT_0122dedc by0.0.
The inherited region list already pins its four zero bytes; generator asserts
both the bytes and presence of that region. Native FCOMP sites use this read-only
float32 zero. Threshold expressions are now recovered, superseding the preceding
note that they remain unresolved.

Strengthened the health regression to extract actual query/threshold pairs and
their predicates across the native wrapper destructor. Seven test positive health,
one tests nonpositive health. Execute all eight with -1,0,0.25,100 and an unrelated
actor staged first. Changed native threshold tofloat1 rejects the entire recovery.
Native destructor semantics are not included in this isolated assertion.
Seven husband/wife/BookTrader-health tests pass in5.953s. Regenerated and inspected
eight zero thresholds and the nonpositive branch. Generation remains47/51 functions,
14/18 files,zero missing,1333 TODOs. Runtime/reconstructed files untouched; drafts
remain disabled. Position, runtime global access, and ownership remain unfinished.

## Husband hit predicate recovered

Added recover_affair_man_hits using shared native_book_trader_hits, immediately
after husband health. Witnesspins raw/stage source and5058-byte body. DB0B57
ordinary hit true reachesresult1;0B85 anyspecial false reachesresult0;0BB8 tests
ability14 pushed0BB0, false reachesresult1. Result storedstack3F. Restored explicit
14 and boolean native_arg_man_hit, preserving conditional string constructions,
uVar17 masks1/3/7, last query cVar5, and surrounding cleanup. LAB_00db0bc9 is
replaced by structured boolean assignment rather than an early return/TODO.

Six husband/wife/BookTrader-hit tests pass in2.728s. Husband test covers all eight
predicate combinations, query order, ability14, high maskbit80 preservation and
last-query result. Changed native ability rejects. Generated output inspected;
resource cleanup/position/global-access gaps remain, and registration is disabled.

## Husband mask spill separated from dialogue stack expression

Added recover_affair_man_mask via parameterized native_affair_wife_mask (Wife
defaults retained), immediately after husband hits; reportaffairManMaskEvidence.
Native EBX spills to baseline stack44 during hit construction and cleanup. Final
clears at0BD3/0BEB/0C03 store0BDD/0BF5/0C0D. Backedge1C82 jumps0B2B, confirmed
MOV EBX,[ESP+44]. Recovered five scalar copies plus finalbit1clear to dedicated
native_arg_man_cleanup_mask; loopload now reads that local. The address-taken
pCVar13=&uStack_118 dialogue expression remains untouched and unresolved.

Four husband/wife mask tests pass in2.376s, covering all hit predicates and masks
0/8/38/A8/FFFFFFF8, restoring the saved value after working-variable overwrite.
Changed source mask rejects. Full suite started into work/converter_man_mask_tests.log;
result pending at this checkpoint (unified exec session66700). Preserve/poll that
handle before starting any replacement run.

## Full mask checkpoint and husband conversation creation

Session66700 completed817 tests in201.388s: only known Bully two failures andtwo
errors. Full resultwork/converter_man_mask_tests.log. No pending process remains
from that run. It predates the conversation tests added below.

Checked husband loop termination after maskreload: actual emitted alive query,
mask assignment, terminationgoto andbackedge already preserve the predicate.
No termination rewrite needed. Added native_affair_man_conversation after mask,
with raw/stage/native5058 and hostcontract pins. DB1897/1A35 NewConversation
slot5B0 receives EBP=self,false,false. EDI receivesID; DB18AE/1A4C AddPerson
slot5B4 receivesEDI,cachedwomanstack32. Bothsourcecalls nowexplicit; savedIDstores,
conditions,counter,termination unchanged. ReportaffairManConversationEvidence
lists persistentID query andnumberedlineconstruction asunresolved.

Four conversation/mask tests pass in2.818s. Execute both actualsourcecreation
fragments, stageunrelatedhero, requireID173,self,false,false andwomanparticipant.
Changedstage rejects. GeneratedLua inspected: both correctNew andAddPerson pairs.
Generation47/51functions,14/18files,zero missing,1322TODOs. Draftsdisabled;
runtime/reconstructed filesuntouched. Ownership/globalaccess/position stillopen.

## Husband persistent conversation ID recovered

Extended the existing conversation pass: native ID is baseline stack64, zeroed
DB0B18 from EDI (XOR0A41), read1805 then passed180C toslot5C0. Both returned IDs
store18AA/1A48 atESP48 aftertwo pushes, again baseline64. Dedicated
native_arg_man_conversation_id now initializeszero, receives uVar8 after both
creations, and supplies IsConversationActive. Retain old saved-ID alias assignments
and address-taken/string uses of stack names; no broad rename. Witness contract
now includes activequery and unresolved list no longer includes persistentID.

Five conversation/mask tests pass in3.534s. New emittedLua regression initializes
then queries0, creates91 through firstpath andqueries91, creates92 through second
path andqueries92. This isolates ID dataflow; it does not prove surrounding gate
or complete dialogue behavior. Regenerated draft query and both saves inspected;
registration remainsdisabled. Numbered dialogue, positions, globals and ownership
remain required work. No runtime/reconstructed files changed.

## Husband close-range numbered dialogue

Added recover_affair_man_near_lines after conversation via parameterized existing
conversation recovery. New witness pins raw/stage/body5058, intformat99F830116,
%d bytes1265DE8 and both prefix literals. Native18E4/192A reads currentfield20;
prefixesman12D8AA0/woman12D8A74 generate stack60/5C keys. Atindex10 call198D
queueswoman then19D8 man. Otherindices19BF man then19D8 woman. ID EDI, selfEBP,
cachedwomanstack32, false subtitle. Recover explicit keys, speaker/listener/order.
Branch-local termination196D/199F retained beforeeitherline. Removed only local
stringconstruction/temporary cleanup; native1D6A string-only cancellation suffix
joins actorcleanup1D7C. Exact source verifies all1D6A references were insideedit.
Counterincrement/wrap and subsequentHeroDiscoveredInfidelitywrite untouched.

Five near-line/conversation tests pass in3.333s. Execute10/20/30/40/50 with both
cancellation states; require one termination query, no line on cancellation,
otherwisecorrecttwo keys/order/ID/roles/flags. Alteredprefixbytes reject. Tests
supply currentindex and actorcleanup sentinel; they do not prove wrap/lifetime.
Ranged dialogue remains unresolved; sharedruntime/reconstructed filesuntouched.

## Husband ranged numbered dialogue and shared string cleanup

Added recover_affair_man_ranged_lines afternear via existing conversation helper.
Native1A82/1AC8 formatsfield20; prefixes12D8A48 manSCRMSG and12D8A20 woman produce
baseline74/8C strings. DB1B1D queuesman first (EDI ID,selfEBP,listenerwoman32,false);
1B39 womansecond (sameID,speakerwoman32,listenerself,false). There is no intervening
termination query. Removed native string-only suffix1B46/1B4F and near19E9/shared
1B4F suffix. Fullsource confirms only one incomingjump, within replacedrangeblock;
both paths now fallthrough toexistingTalkingToWoman check, preservingstatewrites.

Seven ranged/near/conversation tests pass in5.090s: indices10..50 exactkeys/roles/
ID/order/flags, no new termination query, successful fallthrough, changedprefix
rejects. Currentline pass does not establish surroundingdistance/counter/lifetime
parity. Corrected nearwitness explanatory offsets: manbaseline5C,woman58, since
constructor-outputLEA includes one pendingpush; prior60/5C notes missedthatpush.
Neargeneratedcode andactualtextrole ordering were alreadycorrect, unchanged.
Runtime/reconstructed files untouched; draftsdisabled.

## Dialogue sequence integration and working-port comparison

Added test_native_affair_man_dialogue_sequence. Extract actual recovered fragments
from each NewConversation through counterincrement/wrap and completedlinepair,
using actual NOVI_AffairMan field inventory and live termination helpers. Twelve
alternating near/ranged calls yield10/20/30/40/50 repeatedly with two wraps, correct
IDs/participants, keys and falseflags. Termination count is one pernear exchange
plus one atwrap. Wrap cancellation happens after NewConversation/AddPerson and
incrementto60, before reset/lines; both modes preserve that native ordering.

Also read working port read-only, extract its literal constants, next_badger_index,
near_conversation andranged_conversation verbatim, and execute them againstmatching
stubs. The generated fragments match all line IDs/keys/speakers/listeners across
the12 mixedexchanges. Parent F.set is stubbed and its effects not compared; flags
are checked separately sinceport omits optionalfalse. Distancegate, cancellation
insideport (not present inthosehelpers), andresourcecleanup are outsidecomparison.
Seven sequence/near/ranged tests pass in5.162s. No productioncodechange thisturn;
the new regression strengthens behavioral evidence. Drafts remain disabled.

## Husband affection pause and facing operands

Added recover_affair_man_affection_setup afterrangedlines using witness-gated
conversation helper. Native DB1B7E slot5E0 receives floatbits3ECCCCCD (0.4);
DB1B91 slot76C receives EBPself,womanstack32,true;1BA4 reverseactors,true.
Wholeconsecutive sourceblock is matched, since an unrelated noargfacingcall
exists elsewhere and must remain untouched. Fullbody, facingcallee193 and native
vtablebinding pinned; contracts checked. SurroundingTalkingToWoman/termination
guards and subsequentretailrandom/branchchecks unchanged.

Five affection/dialoguesequence tests pass in3.568s, checking pausefloatbits,
bothroles/order/snap despite stagedhero, and changednative snap rejection.
Generatedcallblock inspected; runtime/reconstructed filesuntouched, draftsdisabled.
This restores operands only; control lifetime, remaininganimationflags/global,
otherfacingcall anddistancegates remain incomplete.

## Husband cowering-facing target recovered

Extended the setup witness with separate contextual edit for unresolved facing
immediately before iVar10=RetailRandModulo50. DB1656 receivesEBPself,stack44cachedwife,
false snap (push0 at164E,LEAESP30 afterpush at1650). New explicit call facesman
towife; preserve completeadjacent randomassignment and surroundingdistance/
terminationguards. This resolves the othermissingfacingoperand mentionedabove.

Six setup/dialogue tests pass in4.209s. Added nativecallsetup assertion andLua
execution requiring man/wife/false before modulus50 query, with remainders0/1/49
andunrelatedhero staged. Regenerateddraft inspected; no runtime/reconstructed
changes, registration remainsdisabled. Remaining movement,global,ownership and
animationflag gaps still prevent complete conversion.

## Husband conversation woman-to-hero facing corrected

Extended setup witness for DB1192: snapfalse pushed1176 beforeterminationbranch,
fresh GetHero1183 resultpushed1189, womanwrapperstack32 addressed118A viaESP28
aftertwo pushes, then slot76C. Decompiled resource-looking stackargument was
dropped and generatedactororder becamehero/wife. Explicit cachedwoman,uVar9,false
now preserves actualwoman-to-freshhero behavior and leaves sourceGetHero unchanged.
Fullbody and existingfacingcontract/callee pins remain inforce.

Seven setup/dialoguesequence tests pass in5.489s. New regression executes the
originalhero lookup and recoveredcall, requiring one lookup,woman/freshhero/false.
Regeneratedcall inspected; runtime/reconstructed filesuntouched anddraftsdisabled.
Other native resource/position/global/animation gaps remain unresolved.

## Husband complete animation operand lists

Added recover_affair_man_animation_operands aftersetup, through shared witness
helper. Body5058, forwardingwrapper7E73D0, all three literalstrings, and current
PlayAnimation contract pinned. DB16B4 cower receives0,0,0,1,dynamic,0,0;
DB1C0A kiss/1C60 hug receive0,1,0,1,dynamic,0,0; resource16. Dynamic byte01375748
retained; do not freeze from fileinitialvalue. Restore omittedseventhflagfalse.
Original branchguards/statewrites/stringtemporaries preserved.

Six husband/wife animationoperand/signature tests pass in8.832s. All three calls
execute with dynamicfalse/true and exactsevenflags; alteredfinalnativeflag rejects.
RegeneratedLua inspected, draftsdisabled, runtime/reconstructed filesuntouched.
Full suite started in work/converter_man_animation_tests.log, unifiedexecsession
83888, stillpending at this checkpoint. Poll that handle before any replacement.

## Husband hero-distance operands

Parameterized native_hero_distance for functionaddress/body/selfregister while
retaining Wife defaults. Added recover_affair_man_hero_distances after animation
operands; reportaffairManHeroDistanceEvidence. Native1867GetHero->1871CBE2FF uses
EBPself, freshheroEDX, float32threshold5 pushedbeforegetter. Ranged1A01->1A0B
similarly uses13. Explicitconstants replace fVar25 distancearguments; separate
GetHero calls and sourcebranch order unchanged. Fullbody/callee regions and
symbolic setupreader independently gate recovery.

Four husband/wife hero-distance tests pass in2.060s. Husband stub casesverify
nearpriority, secondqueryonlywhennearfalse, distincthero1/hero2 and5/13thresholds;
changednative rangedargument rejects. This checks operand/selection lowering,
not geometricdistance implementation. Regenerateddraft inspected. Fullsuite
session83888 stillpending; do not restart it. Drafts remain disabled.

## Full animation checkpoint and husband yes/no prompt

Session83888 completed838 tests in207.169s, onlyknownBully2failures+2errors.
Verified both husband ClearThingHasInformation native1159/14CD passselfEBP;
generatedcallsalreadycorrect. No marker rewrite needed.

Found question prompt stringarguments shuffled. Current LuaQuestState.cpp2368
returnsvoid and forwards fourstrings plus optionalb5 (defaulttrue), while SDK
manifest stilldeclaresint/fourstrings. Added narrow converter_signatures overlay,
preserving input/newercontracts. Added recover_affair_man_question afterhero
distances with body/source/string/contract pins: DB12BA slot1C8 receivesquestion
stack70,yes7C,no94,empty84,true. Exactlabels nowexplicit; no generatedresultcapture.
Original stringconstructors/destructors andanswerpolling leftintact.

Four question/hero-distance tests pass in2.339s: exactUIargumentorder andflag,
voidcall emission, signatureimmutability/idempotence. Regeneratedprompt inspected,
draftsdisabled, sharedruntime/reconstructed filesuntouched. Fullsuite forsignature
change started into work/converter_question_signature_tests.log; pending.

## Husband question cancellation movie cleanup

Added recover_affair_man_question_cleanup afterquestion. Fullbody/source/contracts
pin nine conditional jumps (five1D23,four1D0A). Native suffixes1D0A..1D21 and
1D23..1D3A both read savedinterfaceESP38, PauseAllfalse slot5EC, destroy movie
resourceESP48 via6E7B80, thenJMP1D7C. Inline these two runtime calls at eachsource
jump while retaining originalpredicate andterminal actorcleanup target. Existing
inline suffix definitions remain. No actor release inferred.

Four cleanup/question tests pass in2.171s. Allnine expansions tested false/true;
actualanswerwait fragment tested pending(-1)->frame->terminationquery->answer1
and cancellation afterframe, which mustunpause/end beforeanyrepoll. This fixes
the generated earlyreturn that previously skipped moviecleanup in that loop.
Actorcleanup1D7C remains unresolved. Generatedwait block inspected; draftsdisabled.
Fullsignature suite session5393 stillrunning; logwork/converter_question_signature_tests.log.
Poll exacthandle; no replacement run started. Runtime/reconstructed filesuntouched.

Signature suite subsequentlycompleted842 tests in183.694s, onlyknownBully two
failures/twoerrors. Session5393 terminal, no pendingrun. This fullcheckpoint
predates the cleanup tests; those pass separately as recordedabove.

## Additional husband movie cleanup exits

Extended cleanup witness to six sourceLAB1085 conditional jumps. Native1CF3
pushesfalse, uses saved interfaceEDI, destroys movie48, joins1D7C. Native1085
same suffix consumes false already pushed1176 beforeJNE1178. Native1CF3 incoming
sites0EC4/0F17/1061/10A4/114D/14E3 reviewed alongside1178->1085 and existing
sourceinline suffix. Do not extend to1D3C by textual similarity; its decompiled
source differs. Original predicates retained, actorcleanup stillunresolved.

All15 expansions tested bothfalse/true; actualanswerwait cleanup testretained.
Additional mutatedpreparedflag test verifies bodypin rejects push1 instead of0.
Regeneratedoutput no longer has gotoLAB1085 markers. Generation47/51functions,
14/18files,zero missing,1293TODOs. Runtime/reconstructed filesuntouched;
registrationdisabled. Focusedcleanup/question suitepasses; broadergoalunfinished.

## Husband return-home position and movement arguments

Added recover_affair_man_home after question cleanup, with four edits pinned to
the complete DB09E0 body, pipeline source, GetHomePos/distance/movement contracts,
GetScriptThing wrapper 7E7490 (57 bytes), movement wrapper 7E72F0 (15 bytes), and
distance helper CBE45C (91 bytes). Generator: work/build_man_home_witness.py.

DB1707 writes the second self GetHomePos result into baseline ESP+0xCC. The first
DB16D7 query and 0.1 distance gate remain separate. Calls DB171B and DB17C5 take
resource baseline 16 and a hidden wrapper output; wrapper RET4 consumes that
output pointer. With the remaining radius push, DB1720/DB17CA ESP+0xD0 therefore
selects baseline 0xCC. Both distance helper calls use 2.0. DB176F uses the same
vector (ESP+0xDC after four pushes), radius 0, move type 0, false, true, and self
resource 16 (ESP+0x24 after five pushes). Preserve frame/termination/task loops.
Existing wrapper destructors stay in source; resource ownership is not solved.

Shared converter_signatures now narrowly updates the legacy MoveToPosition
entry to nonblocking, typed vector, optional moveType and two optional booleans.
Read-only runtime evidence: LuaManager.cpp 159-161 and
LuaEntityAPI::MoveToPosition_NonBlocking at 2110 select the existing handle and
forward all five arguments. Native defaults false/true differ from host defaults
true/false, so dropping these flags changes behavior. No runtime changes.

Seven focused tests pass (3.569 seconds): immutable/idempotent/custom signature,
four flag combinations, omitted optional arguments, two movement retries using
one snapshot, cancellation before move/during task wait/after task completion,
already within retry radius, and mutated native movement flag rejection.
Generated report confirms recovered four edits, no rejected recovery evidence.
Generation: 47/51 functions and 14/18 files compile, zero missing bodies, 1294
TODOs. Net +1 TODO: the stricter vector contract exposes five other missing
positions while the husband recovery removes four. Husband Main has 34 TODOs.
Registration remains disabled. Full suite running in session12408, log
work/converter_home_movement_tests.log; poll the existing session.

The full suite subsequently completed: 852 tests in 229.720 seconds, only the
known Bully two failures and two errors concerning camera cleanup after run1
termination in the read-only reconstructed port. Session12408 is terminal.
The final focused movement/home suite has eight passing tests (4.202 seconds),
including a later test of both initial gate outcomes with different first/second
home positions. It proves the movement snapshot is queried only after the gate
and uses the second position for the move and both retry checks. No new converter
failures were reported. The broader benchmark completed with existing syntax
gaps still reported; do not describe those scripts as fully converted.

## Husband question entry after intro speech

Previous turn was concrete progress (home recovery/signature/tests). This batch
extends question cleanup from 15 to 19 conditional jumps, adding all four source
1CF3 jumps. Native incoming edges to the equivalent suffix were rechecked at
0EC4/0F17/1061/10A4/114D/14E3. Regenerated the downstream home witness because
its exact pipeline-stage source changed.

Added recover_affair_man_question_entry after home, with full native body,
raw/stage source and question/cleanup contract pins. Generator:
work/build_man_question_entry_witness.py. This addresses the actual generated
early return at goto1243 after normal INTRO speech completion. Native health
query11B2, FCOMP zero11B8, TEST AH,41 at11C0 and JE11D9 select question entry for
nonpositive/unordered health. Positive health speaks INTRO and waits; termination
query1236/JNE123D exits through1D0A, otherwise falls through1243 to parent71 write
and the question. Emit an optional positive-health speech block, then the original
question/answer body. Do not invert <=: NaN must bypass speech, so use zero<health.
Cancellation edge becomes unpause/end/goto1D7C. Other question-body text remains
unchanged. Actor ownership is still unresolved.

Ten focused entry/cleanup/home tests pass in8.165s. New entry regression tests
negative/zero/positive/NaN health, normal completion, cancellation during task wait,
cancellation after task completion, state-before-question ordering and prompt
arguments. Changed native123D branch rejects recovery. Generation remains
47/51 functions and14/18 files, zero missing bodies, now1291 TODOs; husband Main31.
Report confirms question entry recovered and19 cleanup edits. Draftsdisabled.
All husband tests running in session83437; poll that existing handle.
No full-suite restart required for these function-specific passes; previous
852-test shared-signature checkpoint remains recorded above.

Session83437 completed: all41 husband tests passed in27.310s. No running test
or generation processes remain from this batch. Goal remains active/incomplete.

## Husband shared talk continuations

Previous turn was progress (question-entry/control-flow fixes). Added
recover_affair_man_talk_continuation after question entry. Generator:
work/build_man_talk_continuation_witness.py. Full5058 body, raw/stage source,
clear-information/pause/movie contracts pinned. Two source goto1153 expansions
emit ClearThingHasInformation(me), goto1593. One goto1144 expansion queries
termination first, unpauses/ends/goto1D7C if cancelling, otherwise same1153 suffix.
Native1158 PUSH EBP,1159 slot5A4,115F JMP1593;1146 F35B30 and114D JNE1CF3.
Original inline suffix definitions retained. This fixes SOME_NERVE and
HOW_FIND_OUT success paths that previously fell through to cancellation cleanup.

Focused tests pass2 in1.464s: both suffix forms, both termination outcomes,
correct clear receiver, no extra termination query on direct1153 paths, and
mutated1158 actor push rejection. These execute recovered suffixes with terminal
sentinels, not the entire native resource lifetime. IsActiveThreadTerminating
uses the lifter's reviewed special operation/live_termination, not a manifest
entry (the shared SDK has none); it is deliberately not contract-indexed.

Generation47/51functions,14/18files,zero missing,1288TODOs; husband Main28.
Report confirms new pass recovered; generated1153/1144 TODO gotos removed.
Registration remains disabled; runtime/reconstructed files untouched.
All husband tests running session49448; generation session41261 completed.

Session49448 completed: all43 husband tests passed in30.683s. No live processes
remain from this batch. Full converter objective remains incomplete and active.

## Native speech scheduling mismatch and explicit resource API discovery

Previous turn was progress (three talk continuation fixes). Current source
inspection found a material limitation hidden by mocked speech tests:
LuaManager.cpp190-196 binds me:Speak to Speak_Blocking, which issues speech then
runs its own task/frame/termination loop in LuaEntityAPI.cpp. Native7E7390 is
only pImp dispatch through slot34, or RET18 for empty resource. It never waits.
Thus existing generated speech + caller task loops are not runtime-parity proof.

Added native_affair_man_speech.py mapping and full-body/wrapper/string/host-contract
witness. Eight native sites0D5A/0F81/103C/110A/11F8/13AF/147C/1549 use resource16
(ESP+28 after six pushes), fresh hero query, selection0, false/true/false. Native
key order is ON_HIT,HAD_A_DEAL,SOME_NERVE,HOW_FIND_OUT,INTRO,GOOD_LAD,DONT_TELL,SHOO.
Generator work/build_man_speech_mapping_witness.py asserts the aligned setup at
all eight sites. Report adds affairManSpeechEvidence mapped plus eight explicit
TODOs; this is honest coverage, not a completed speech lowering. Mutated wrapper
or host contract rejects mapping. Generation47/51,14/18,zero missing,1296TODOs.
All45 husband tests passed25.529s, session84179 terminal; generation71130 terminal.

New authoritative source capability: LuaRetailResources.h already supplies
NewResource/PrepareResource/TryAcquire/ReleaseResource/Frame and nonblocking
Speak/IsPerformingScriptTask, plus explicit Thing creation/health/destruction,
movie locals, actor maps. LuaManager registers it and Quest:WithRetailResources.
Do not continue assuming all resource ownership requires the cached Entity API.
However, resource MoveToPosition/PlayAnimation/ClearCommands/ClearAllActions are
absent, so the full husband cannot yet be switched without more bridge work.
Read-only snapshot generator work/review_man_resource_bridge.py writes
work/man_resource_bridge_review.json with exact source hashes and missing methods.
No DLL build/deployment claim. Runtime files remain read-only; do not alter the
parallel runtime session. Lower constructors/preparation/acquisition attempts/uses/
deterministic destruction together; do not mix resource speech with me:AcquireControl.
Dynamic animation global and actor/position semantics are still outstanding.
Goal stays active; no live processes from this batch.

## Reviewable resource action extension

Previous turn was progress: authoritative speech mismatch plus discovery of
explicit resource API changed the next implementation path. Prepared the missing
MoveToPosition/PlayAnimation/ClearCommands/ClearAllActions as a candidate patch,
not applied to the read-only parallel runtime. Files:
tools/script_recovery/retail_resource_actions.inc,
prepare_man_resource_extension.py, retail_resource_actions_harness.cpp,
test_retail_resource_actions.py. Generated artifacts in work/man_resource_extension:
LuaRetailResources.h, resource-actions.patch, proposal.json.

Preparation checks exact native wrapper bytes before writing: 7E72F0 slot10
RET14;7E73D0 slot48 RET20;7E7360 slot28 RET;7E7400 slot54 RET. All dispatch through
the selected resource's pImp, empty resource no-op. Candidate mirrors that with
Get(id,Kind::Resource), all explicit arguments, no cached control/wait/termination.
Movement requires xyz fields rather than inventing zero coordinates; animation
uses scoped FableString and forwards all seven booleans. All four methods are
registered on RetailResources. Existing extension or changed anchors reject the
proposal. Source header hash pinned in metadata; newline/BOM preserved so patch
applies exactly. git apply --check against read-only shadow succeeded after
correcting patch generation to preserve CRLF. No patch was applied.

Three tests passed0.848s. C++17 g++ -Wall -Wextra -Werror harness compiles the
actual proposed method fragment, tests two distinct resource IDs, all128 animation
flag combinations for each, all4 movement-flag combinations, xyz/radius/type,
clear operation identity, empty-resource no-ops (including absent coordinates),
invalid position, missing vtable operations and closed/released resources.
String wrapper destruction checked. Other tests verify read-only preparation,
existing-extension rejection and mutated native dispatch rejection.
These stub-type tests are not engine ABI/complete x86 runtime validation.
No DLL built/deployed; no runtime/reconstructed files modified. Converter's eight
speech warnings remain required until lifecycle lowering and runtime integration
are completed. Goal remains active; no live processes from this batch.

## Husband native resource lifetime map

Previous turn was progress (reviewable extension plus compiled forwarding tests).
Added map_affair_man_resources via shared native_post_attack_resources mapper;
new witness generated by work/build_man_resources_witness.py. Full5058 native
body/raw source and helper bytes pinned. Fourteen events with symbolic call setup:
constructor0A3C (baseline16), has_resource/reset at0A57/0A64,0C96/0CA3,0E66/0E73,
six acquisitions0A76/0AA4/0CB5/0CE4/0E85/0EB4 (selfEBP,resource16,priority4),
destructor1D92 (resource16). Initializer0A47 writes vtable127094C,0A4F/53 empty
pImp fields. Reacquisition uses the original local, not a new local each time.

Shared native_resource_lifetime checker gains a non-mutating use event: active
identity must match, with no changes to start/end semantics. Full aligned native
CFG verifies these14 events. Termination0A32 branches past construction to1D97;
every path that constructs reaches1D92 destructor once. All preparatory/acquire
uses require live resource16. This proof does not yet cover action dispatch or
temporary Thing/movie lifetimes. It remains mapped/loweringStatus unresolved.
Converter report exposes affairManResourceEvidence beside speech diagnostics.

Seven focused resource/lifetime tests passed2.724s. Tests assert all event/operand
counts and reject missing constructor/destructor, a wrong local on final acquire,
and changed native actor push0A75. All husband tests session10896 and generation
session52630 running; poll existing handles. Runtime/reconstructed files unchanged.

Both sessions completed. All48 husband tests passed29.749s; generation47/51,
14/18,zero missing,1296TODOs. Goal active/incomplete; no live batch processes.

## Resource action and returned-Thing lifetime proofs

Previous turn was progress (14-event resource lifetime map). Expanded the same
map to57 events: prior14 plus8 speech,19 task queries,3 animations,1 move,1
ClearAllActions,1 ClearCommands,10 GetScriptThing calls. Every receiver proven
baseline16. Speech windows profile their own fresh GetHero calls (zero cleanup),
so six pushed args correctly recover resource16 and listener/key/flags. No
constructor profile needed for animations; their setup starts after construction.

Shared native_call_setup_ir now understands XOR parent,parent; MOV low-byte,[mem]
as unsigned_byte(memory), only for AL/BL/CL/DL with proven zero upper bytes.
Unknown upper bits/high-byte/wider loads remain rejected. Any memory store
invalidates the new fact recursively. Animation byte1375748 stays dynamic.
27 focused IR/resource/lifetime tests initially passed5.666s.

Also mapped ten temporary Things returned by7E7490. Hidden output slots from IR:
312,288,228,300,252,264,276,240,216,216 (decimal). Native query consumes that returned
wrapper (health stack operand, or distance ECX), followed by destructor4AA840 at
0D34/0F57/1012/10E4/11D2/1389/1456/1523/1737/17E1. Separate30-event CFG proof
checks create/use/end without overlap and all paths balanced. Query and destructor
setups are recomputed against witness. Destructor4AA840 pinned66 bytes (refcount
decrement/conditional destruction, zero fields, tail jump99A2E0). This does not
cover cached woman/wife or hit-condition wrappers or movie locals.

Ten latest resource/lifetime tests passed10.230s, including missing final temporary
destructor and wrong-actor distance query rejection. Full shared-IR suite completed
870 tests181.248s with only known Bully2failures/2errors. It began before the final
temporary-Thing additions; those have focused verification above. Log:
work/converter_resource_action_ir_tests.log. Session2691 terminal, generation18233
terminal, earliergeneration94052 terminal. Generated report confirms57events and
ten temporary Thing lifetimes; still47/51functions,14/18files,zero missing,1296TODOs.
Proof mapping is not lowering; eight speech scheduling diagnostics stay active.
No runtime/reconstructed modifications; registrationdisabled. Goal active.
