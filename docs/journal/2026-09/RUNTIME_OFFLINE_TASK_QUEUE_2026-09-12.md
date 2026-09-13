# Runtime offline task queue — September 12, 2026

## Overnight priority and pause

User stopped for sleep. Resume from
[the current handoff/checklist](NEW_OAKVALE_NIGHT_HANDOFF_2026-09-12.md), not by
restarting the historical queue below. New Oakvale full childhood is played;
remaining audio work now has a confirmed unsupported DirectSound config8 path.
Next offline task is to explain its per-game origin and design a narrow correction.
Do not repeat the already captured V5 startup diagnostic without new justification.
Completed extended-marathon work is recorded in RUNTIME_EXTENDED_MARATHON_2026-09-12.md.


Requested by the user: a marathon work queue that can be completed without visual
confirmation. These are new deliverables, not a relisting of already passing
tests. Work in priority order; take justified implementation fixes through focused
validation. A completed audit may conclude that evidence is insufficient for a
behavior change; it must identify the exact remaining boundary.

## 1. Make the native/resource proofs reproducible

- Promote the Meet Sister C++ integration build preparation out of its dependency
  on existing `work` response files and precompiled Lua object files.
- Provide one offline runner that builds from identified vendor sources in a
  separate output directory, runs the 14 staged entity scenarios and the existing
  ownership/error smoke, and returns a nonzero exit code on any failure.
- Record executable, runtime-header, Lua-package, compiler and fixture provenance.
- Done when a fresh output directory produces the 255-event comparison and
  cleanup checks without relying on previous build products. Engine calls remain
  explicitly mocked; this is not a live playback result.

## 2. Audit Father/Bully cancellation at the native boundaries

- Compare the reviewed native branches with the actual port and runtime helper
  behavior for normal completion, Escape and thread termination.
- Focus on state/reward writes, drawable/removal, fade commands, resource release
  and camera/macro ordering. Distinguish line skip from scene skip.
- Add only missing, discriminating trace cases; do not duplicate the 122 existing
  fixtures or invent a global termination rule from one entity's behavior.
- Done when each reviewed exit has an address-backed event comparison, and any
  confirmed mismatch has a focused fix and regression result. Preserve the
  existing confirmed RUN1 cancellation fix and retail early-skip branch.

## 3. Produce an automatic Escape/fade capture report

- Add an offline reader for the external observer JSONL that correlates Escape
  transitions, foreground PID, native eligibility blockers and raw fade state.
- Handle tick wrap, missing reads, partial lines and repeated key-down events.
- Keep conclusions precise: eligible input observed is not proof that the game
  consumed it; opaque fade state is not proof of opaque pixels.
- Done when deterministic synthetic cases produce expected reports and malformed
  capture data is reported without silently turning it into a successful test.

## 4. Complete speech operand and capture correlation

- Inventory the actual New Oakvale speech call paths and compare their operands
  with the native calls. Use the corrected fifth-emitter-argument route for
  soundIn2D; do not repeat the discarded third-argument interpretation.
- Extend the offline capture report to associate sample lifetimes/IDs/bank
  pointers with listener movement, emitter position and soundIn2D observations.
- Identify overlapping samples and gaps rather than guessing the speaking line
  from timing alone. Keep registered samples distinct from audible voices.
- Done when the source comparison is documented and synthetic camera-turn/sample
  transitions are correctly summarized. Do not claim an audible fix from this.

## 5. Finish the Meet Sister scheduler-entry audit

- Trace native entity creation/entry versus the runtime host's existing pre-entry
  termination guard, complementing the already completed entered-body replay.
- Document the ordering of binding activation, paused flags, Init/Main dispatch
  and termination before entry. Exercise recoverable branches with native-byte
  emulation where feasible; identify external scheduler calls explicitly.
- Done when the bounded entry contract and any proven differences are documented
  with addresses/source references and discriminating tests. No speculative
  change to the common entity host; live streaming remains a separate check.

## 6. Prepare Meet Sister metadata and release materials

- Prepare a separate proposed API metadata change for optional binding flags and
  RetailResources, with examples and ownership/lifetime rules.
- Produce an offline package manifest, prerequisites and rollback/deployment
  instructions that distinguish the source build from installed v23.
- Done when the artifacts are reviewable and hash-validated. Shared manifest
  regeneration still requires coordination with the converter session; neither
  metadata application nor deployment is part of this offline completion gate.

## 7. Select and recover the following supported port

- Compare candidates in the roadmap and existing native inventories for complete
  lifecycle identity, engine API availability and asset dependencies.
- Select one target using that evidence, then recover its binding table, entity
  bodies, resource lifetimes and external dependencies into our own evidence area.
- Done when the selection and native contract are documented with a dependency
  inventory and an explicit list of unsupported operations. Stage an executable
  reference only where the recovered contract supports it; leave activation off.

## Boundaries throughout

- Preserve uncommitted work and converter-owned files. Do not regenerate lifted
  outputs or shared API metadata without ownership coordination.
- Keep the installed v23 build and successful childhood behavior intact. No
  automatic game launches, installations, audio setting changes or restored v24
  hooks are required by these tasks.
- Record progress in NEW_OAKVALE_RUNTIME_HANDOFF_2026-09-12.md and the specific
  port review, with commands, result limits and provenance.
- Actual fade visibility, perceived dialogue loudness, restored-v23 Father Escape
  and in-game Meet Sister playback remain live acceptance checks. Offline work
  must not relabel them as fixed or verified.

Current status: tasks 1, 3, 4, 5, 6 and 7 complete within their offline bounds;
task 2 retains broader cancellation audit work.
Task 7: Maze Research staged disabled, 31 full native body cases plus six
Init/UNLIMBO cases, shared flag/thing support, Release build and offline tests pass.
Actual scheduler, streaming and macro playback remain live acceptance checks.
See maze_research/runtime_evidence/PORT_REVIEW_2026-09-12.md. Task 5 evidence is
`meet_sister/runtime_evidence/SCHEDULER_ENTRY_REVIEW_2026-09-12.md` and four native
entry cases; actual fiber switching remains a live boundary. Task 6 artifacts
are in `meet_sister/release_review_20260912/`, with all seven staged-file hashes,
an unapplied metadata proposal, prerequisites and rollback instructions.
Task 4 added sample/listener correlation and corrected four Guard/Villager
conversation calls whose Lua numeric zeros became true C++ booleans. All 17
conversation-creation sites now pass an actual sol/Lua operand check, and all
122 package fixtures pass. Native speech comparison is now complete within its reviewed static scope:
102 direct Speak calls, 18 native conversation creations, 36 native queued-line
calls and 28 authored speech commands in nine macro assets. The clean six-binary
sol/runtime suite passes. See DIRECT_DIALOGUE_OPERAND_REVIEW,
CONVERSATION_LINE_REVIEW and MACRO_SPEECH_REVIEW in the New Oakvale evidence area.
No audible output or fade-pixel claim follows from matching operands.
Task 3 analyzer:
`tools/script_recovery/analyze_retail_cutscene_observer.py`; six focused tests
and synthetic complete/truncated CLI checks pass. It reports observations and
missing data, not successful visual playback. Task 1 runner:
`tools/script_recovery/run_meet_sister_runtime_checks.py`; clean positive and
negative results: `work/meet-sister-clean-runtime-check-20260912-d/`.
Task 2 added Father pre-card/pre-counter guards and removed the unsupported
explicit camera-unfix call from Bully RUN1 cancellation. Eight Father and six
Bully native comparisons pass; all 122 package fixtures pass. Neither fix is
installed or claimed to resolve the visual/audio reports.

Existing separate wrapper tests, 14-case
C++/Lua integration, native geometry proofs and observer decoder checks are
baselines; rerun them only when a change or unresolved concern warrants it.


## Next marathon batch

8. Audit legacy RunCutsceneWithSetup construction/destruction against native
   actor, boolean and input maps. Finish with reproduced compatibility or
   mismatch evidence, focused fixes/checks where justified, and explicit limits.
9. Audit staged Meet Sister and Maze Research OnPersist fields and lifecycle
   assumptions against native code. Finish with field/type/order evidence and
   an executable round-trip contract check where the runtime supports it.
10. Select the next supported retail script using available native lifecycle,
    entity and asset evidence. Recover a bounded contract and stage disabled
    code only after native comparisons support it.

All three exclude installed changes and visual acceptance. Converter-owned
files remain read-only. Progress to the next task after completing each gate.

Batch result: 8 complete (12 native cleanup cases, narrow runtime guard/fallback
fix, Release build and six checks); 9 complete (26 native persistence cases,
24 actual C++/Lua transfers, seven checks); 10 complete as bounded selection
and recovery (Guardian Trophy Dealer Info root/watcher reference, ten native
watcher cases, three macros and ten placements). Full GTDI_Maze entity port
is the next implementation goal, with resource speech routing still to prove.

Guardian follow-through: 49 native first-encounter/repeat cases now match the
reference; movie CFG identity passes with five negative mutations rejected.
Three macros and all 11 text keys resolve. The quest-card entry exists but its
generic schema decode fails. See ENCOUNTER_RESOURCE_REVIEW_2026-09-12.md.

11. Replay Guardian Init/entry/trigger predicates and outer loops, composing the
    verified branch bodies. Complete with native traces and reference comparison.
12. Prove and expose Speak, task polling and Thing access on explicit resources;
    validate actual sol/C++ dispatch before adapting Guardian's reviewed reference.
13. Resolve Guardian quest-card component/schema evidence, confirm override
    identity, and stage a disabled package with actual adapter integration checks.

These tasks need no visual confirmation. Installed changes and audiovisual
acceptance remain separate; do not modify converter-owned support modules.

Task 11 complete: actual native Init/Main, 28 cases/1,377 events, all matching
the composed reference; existing comparisons still pass (87 cases total).
Task 12 advanced: explicit Speak/task polling implemented and proven by ten
native wrapper cases plus actual sol/C++ dispatch. Seven runtime binaries and
Release build pass. Resource Thing copies/destruction remain. Five native
scheduler-condition cases establish Guardian's bound-IsAlive requirement; add
and prove adapter registration rather than silently ignoring that operation.
Task 13 remains gated on those operations and the quest-card decode/component
review. See ENTITY_AND_DISPATCH_REVIEW_2026-09-12.md; installed v23 unchanged.

Live acceptance update: after the latest FSE_Launcher run, the user confirmed
Father Escape works again and accepted Bully disappearing on scene skip as
matching their retail comparison. These two items no longer await user visual
acceptance on installed v23. Missing fade rendering remains an unresolved retail
observation; quiet/distant dialogue remains separate and unresolved. This does
not validate the newer uninstalled runtime/port changes.

Task 12 complete: explicit scoped Thing copies/lookups/destruction and native
bound-alive-condition registration are implemented. Sixteen native Thing cases,
eight native clone/replacement/teardown cases and actual sol/C++ tests pass.
Guardian's actual entity adapter matches all 28 cases/1,377 native events.
Nine runtime binaries and Release build pass. Required card component checks
also pass: OBJECT 3736 -> CQuestCardDef 12321, correct self-reference, 18 fields,
valid title/summary/category. The generic OBJECT parser limitation is separate.

Next bounded task: root/watcher/PieceOver persistence through the actual adapter,
final override identity/conflict checks, and disabled package assembly. See
THING_CONDITION_ADAPTER_REVIEW_2026-09-12.md. Installed v23 remains unchanged.


Guardian package batch complete: root/watcher/persistence actual-code checks,
registry conflict audit, six-file disabled staging, and documented results.
Confirmed/fixed missing script path in the root adapter using the actual
AddEntityBinding method and sol registration. Nine compiled binaries pass;
28 entity cases/1,377 events, ten watcher cases, eight Guardian persistence
cases, two native root cases, and all 22 package checks. See PACKAGE_REVIEW
and package-validation in Guardian runtime_evidence. Installed v23 unchanged.

Task 13's disabled-package gate is complete. Remaining live gates: Guardian
scheduler/streaming, save/load, first/repeat interactions and Escape playback.
Next independent offline tasks: audit cross-VM quest-state namespacing and
reload behavior for PieceOver, then select another bounded retail root from
the latest native/converter inventory. Keep live deployment separate and
preserve the accepted Father/Bully behavior and outstanding audio investigation.


Extended batch complete: actual cross-VM/reload state checks; confirmed/fixed
VM-reference teardown UAF and borrowed-actor conversation/conversion defects;
all three staged root bindings tested through actual C++; 91 full native audio
update cases; Guardian Sister root/lifecycle foundation (19 native cases) and
macro/text/placement review. ASan, nine-binary regression, Release build, and
18-check final manifest pass. All 25 installed v23 files unchanged.

Evidence: RUNTIME_EXTENDED_MARATHON_2026-09-12.md and
work/runtime-marathon-verified-20260912.json. Next offline task remains full
MazeAtTavern entity recovery: priority3, dynamic call-over distance, resource
animation+48 with seven booleans, exact separate conversation-call ordering,
hit/talk lifetime transitions, macro/completion/cancellation. The helper
7E73D0 dispatches through resource+8/expert+48; do not infer flag defaults.
Two authored fan-exit markers are unresolved; preserve retail assets.
